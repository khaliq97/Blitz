#include <Parser/HTMLStateMachine.h>
#include <Parser/Lexer.h>
#include <Parser/HTMLToken.h>
#include <stack>
#include <memory>
#include <Parser/Lexer.h>
#include <fmt/core.h>

enum HTMLState
{
    Data,
    TagOpen,
    MarkupDeclarationOpenState,
    CommentStartState,
    CommentStartDashState,
    CommentEndState,
    CommentEndDashState,
    CommentState,
    DOCTYPE,
    BeforeDOCTYPENameState,
    DOCTYPENameState,
    EndTagOpenState,
    TagNameState,
    SelfClosingStartTagState,
    BeforeAttributeNameState,
    AttributeNameState,
    BeforeAttributeValueState,
    AttributeValueDoubleQuotedState,
    AfterAttributeValueDoubleQuotedState,
    AttributeValueSingleQuotedState,
    AfterAttributeValueQuotedState
};


std::string HTMLStateTypes[] =
{
    "Data",
    "TagOpen",
    "MarkupDeclarationOpenState",
    "CommentStartState",
    "CommentStartDashState",
    "CommentEndState",
    "CommentEndDashState",
    "CommentState",
    "Doctype",
    "BeforeDOCTYPENameState",
    "DOCTYPENameState",
    "EndTagOpenState",
    "TagNameState",
    "SelfClosingStartTagState",
    "BeforeAttributeNameState",
    "AttributeNameState",
    "BeforeAttributeValueState",
    "AttributeValueDoubleQuotedState",
    "AfterAttributeValueDoubleQuotedState",
    "AttributeValueSingleQuotedState",
    "AfterAttributeValueQuotedState"
};

std::string TokenTypes[] =
{
    "Doctype",
    "StartTag",
    "EndTag",
    "Comment",
    "Character",
    "EndOfFile",
};

HTMLState htmlTokenizerState;

HTMLStateMachine::HTMLStateMachine(std::string source)
{
    lexer = std::make_unique<Lexer>(source);
    htmlTokenizerState = HTMLState::Data;
}

void switchToState(HTMLState newState)
{
#ifdef PRINT_STATE_SWITCHING_DEBUG
    printf("Switching state from [%s] to [%s]\n", HTMLStateTypes[htmlTokenizerState].c_str(), HTMLStateTypes[newState].c_str());
#endif
    htmlTokenizerState = newState;
}

std::string boolToString(bool value)
{
    return value ? "true" : "false";
}

void HTMLStateMachine::printAllTokens()
{
    printf("\n");

    for (auto const& token: m_tokens)
    {
        if (token.getType() == HTMLToken::Type::StartTag || token.getType() == HTMLToken::Type::EndTag)
        {
             printf("%s:\n   Name:%s\n   Self Closing: %s\n", TokenTypes[token.getType()].c_str(), token.getTagName().c_str(), boolToString(token.getTagIsSelfClosing()).c_str());

             printf("   Attributes: \n");
             for (auto &attribute: token.tag.attributes)
             {
                 printf("       %s=%s\n", attribute.name.c_str(), attribute.value.c_str());
             }
        }

        if (token.getType() == HTMLToken::Type::Doctype)
        {
             printf("%s:\n  Data: %s\n", TokenTypes[token.getType()].c_str(), token.getDoctypeName().c_str());
        }

        if (token.getType() == HTMLToken::Type::Comment || token.getType() == HTMLToken::Type::Character)
        {
             printf("%s:\n  Data: '%s'\n", TokenTypes[token.getType()].c_str(), token.getCommentOrCharacterData().c_str());
        }
    }
}

HTMLToken::Attribute HTMLStateMachine::createAttribute(std::string name, std::string value)
{
    HTMLToken::Attribute returnAttr;
    returnAttr.name = name;
    returnAttr.value = value;

    return returnAttr;
}

void HTMLStateMachine::consumeEscapeCodes(bool tab, bool newLine, bool space)
{
    if (tab)
        lexer->consumeToken('\t');
    if (newLine)
        lexer->consumeToken('\n');
    if (space)
        lexer->consumeToken(' ');
}

void HTMLStateMachine::run()
{
    HTMLToken currentToken;

    std::string currentAttributeKey;
    std::string currentAttributeValue;

    while (lexer->getTokenIndex() < lexer->getTokensLength() - 1)
    {
      // printf("Current token: '%c\n", getTokens()[getTokenIndex()]);

        switch (htmlTokenizerState) {
            case HTMLState::Data:
                if (lexer->peek() == '<')
                {
                    lexer->consume();
                    switchToState(HTMLState::TagOpen);
                    break;
                }
                else
                {
                    currentToken = {};
                    currentToken.setType(HTMLToken::Type::Character);

                    if (lexer->peek() == '\n')
                    {
                        currentToken.appendCommentOrCharacterData('\n');
                        lexer->consume();
                    }
                    else if (lexer->peek() == '\t')
                    {
                        currentToken.appendCommentOrCharacterData('\t');
                        lexer->consume();
                    }
                    else
                    {
                        currentToken.appendCommentOrCharacterData(lexer->consume());
                    }

                    m_tokens.push_back(currentToken);
                    break;
                }

                // NULL OR EOF?

            case HTMLState::TagOpen:
                if (lexer->peek() == '!')
                {

                    lexer->consume();
                    switchToState(HTMLState::MarkupDeclarationOpenState);
                    break;
                }
                else if (lexer->peek() == '/')
                {
                    lexer->consume();
                    switchToState(HTMLState::EndTagOpenState);
                    break;
                    // TODO: end tag open state
                }
                else if(std::isalpha(lexer->peek()))
                {
                    currentToken = {};
                    currentToken.setType(HTMLToken::Type::StartTag);
                    // TODO: Set name to empty string.
                    switchToState(HTMLState::TagNameState);
                    break;
                }
                else if(lexer->peek() == '?')
                {
                    // unexpected-question-mark-instead-of-tag-name parse error.
                }
                break;
                //0020 SPACE
                //U+002F SOLIDUS (/)
                //U+003E GREATER-THAN SIGN (>)
                //EOF
           case HTMLState::TagNameState:
                consumeEscapeCodes(true, true, false);

                if (lexer->peek() == ' ')
                {
                    switchToState(HTMLState::BeforeAttributeNameState);
                    break;
                    // Before Attribute State.
                }
                else if (lexer->peek() == '/')
                {
                    lexer->consume();
                    switchToState(HTMLState::SelfClosingStartTagState);
                    break;
                }
                else if (lexer->peek() == '>')
                {
                    lexer->consume();
                    m_tokens.push_back(currentToken);
                    switchToState(HTMLState::Data);
                    break;
                }
                else if (std::isupper(lexer->peek()))
                {
                    currentToken.appendTagName(std::tolower(lexer->consume()));
                    break;
                }
                else
                {
                    currentToken.appendTagName(lexer->consume());
                    break;
                }

                // EOF OR NULL??
                break;

           case HTMLState::BeforeAttributeNameState:
                consumeEscapeCodes(true, true, false);
                if (lexer->peek() == ' ')
                {
                    lexer->consume();
                    break;
                }
                else if (lexer->peek() == '/')
                {
                    lexer->consume();
                    break;
                }
                else if (lexer->peek() == '>')
                {
                    switchToState(HTMLState::AfterAttributeValueDoubleQuotedState);
                    break;
                }
                else if (lexer->peek() == '=')
                {
                    // unexpected-equals-sign-before-attribute-name
                }
                else
                {
                    switchToState(HTMLState::AttributeNameState);
                    break;
                }

                // EOF CHECK??

            case HTMLState::AttributeNameState:
                if (lexer->peek() == ' ')
                {
                    lexer->consume();
                    break;
                }
                else if (lexer->peek() == '\t')
                {
                    lexer->consume();
                    break;
                }
                else if (lexer->peek() == '/')
                {
                    lexer->consume();
                    break;
                }
                else if (lexer->peek() == '>')
                {
                    lexer->consume();
                    break;
                }
                else if (lexer->peek() == '=')
                {
                    lexer->consume();
                    switchToState(HTMLState::BeforeAttributeValueState);
                    break;
                }
                else if (std::isalpha(lexer->peek()) && std::isupper(lexer->peek()))
                {
                    currentAttributeKey.push_back(std::tolower(lexer->consume()));
                    break;
                }
                else
                {
                    currentAttributeKey.push_back(lexer->consume());
                    break;
                }

                //U+0022 QUOTATION MARK (")
                //U+0027 APOSTROPHE (')
                //U+003C LESS-THAN SIGN (<)
                // This is an unexpected-character-in-attribute-name parse error. Treat it as per the "anything else" entry below.
                // EOF AND NULL??

            case HTMLState::BeforeAttributeValueState:
                //consumeEscapeCodes(true, true, false);

                if (lexer->peek() == ' ')
                {
                    lexer->consume();
                    break;
                }
                else if (lexer->peek() == '\'')
                {
                    lexer->consume();
                    switchToState(HTMLState::AttributeValueSingleQuotedState);
                }
                else if (lexer->peek() == '\"')
                {
                    lexer->consume();
                    switchToState(HTMLState::AttributeValueDoubleQuotedState);
                    break;
                }

                break;

            case HTMLState::AttributeValueDoubleQuotedState:
                if (lexer->peek() == '\"')
                {
                    lexer->consume();
                    switchToState(HTMLState::AfterAttributeValueDoubleQuotedState);
                    break;
                }
                else
                {
                    currentAttributeValue.push_back(lexer->consume());
                    break;
                }

            case HTMLState::AfterAttributeValueDoubleQuotedState:
                consumeEscapeCodes(true, true, false);

                if (lexer->peek() == ' ')
                {
                    lexer->consume();
                    currentToken.tag.attributes.push_back(createAttribute(currentAttributeKey, currentAttributeValue));

                    currentAttributeKey.clear();
                    currentAttributeValue.clear();

                    switchToState(HTMLState::BeforeAttributeNameState);
                    break;
                }
                else if (lexer->peek() == '/')
                {
                    lexer->consume();
                    switchToState(HTMLState::SelfClosingStartTagState);
                    break;
                }
                else if (lexer->peek() == '>')
                {
                    lexer->consume();
                    currentToken.tag.attributes.push_back(createAttribute(currentAttributeKey, currentAttributeValue));
                    m_tokens.push_back(currentToken);

                    currentAttributeKey.clear();
                    currentAttributeValue.clear();

                    switchToState(HTMLState::Data);
                    break;
                }

            break;

            case HTMLState::AttributeValueSingleQuotedState:
                if (lexer->peek() == '\'')
                {
                    lexer->consume();
                    switchToState(HTMLState::AfterAttributeValueQuotedState);
                    break;
                }
                else
                {
                    currentAttributeValue.push_back(lexer->consume());
                    break;
                }

            case HTMLState::AfterAttributeValueQuotedState:
                consumeEscapeCodes(true, true, false);

                if (lexer->peek() == ' ')
                {
                    lexer->consume();
                    currentToken.tag.attributes.push_back(createAttribute(currentAttributeKey, currentAttributeValue));
                    currentAttributeKey.clear();
                    currentAttributeValue.clear();

                    switchToState(HTMLState::BeforeAttributeNameState);
                    break;
                }
                else if (lexer->peek() == '/')
                {
                    lexer->consume();
                    switchToState(HTMLState::SelfClosingStartTagState);
                    break;
                }
                else if (lexer->peek() == '>')
                {
                    lexer->consume();
                    currentToken.tag.attributes.push_back(createAttribute(currentAttributeKey, currentAttributeValue));
                    m_tokens.push_back(currentToken);
                    currentAttributeKey.clear();
                    currentAttributeValue.clear();

                    switchToState(HTMLState::Data);
                    break;
                }

                break;

            case HTMLState::EndTagOpenState:
                if (std::isalpha(lexer->peek()))
                {
                    currentToken = {};
                    currentToken.setType(HTMLToken::Type::EndTag);
                    // TODO: Set name to empty string.
                    switchToState(HTMLState::TagNameState);
                    break;
                }
                else if (lexer->peek() == '>')
                {
                    //  missing-end-tag-name parse error.

                }
                else
                {
                    //  invalid-first-character-of-tag-name parse error
                }

            break;
           case HTMLState::SelfClosingStartTagState:
                if (lexer->peek() == '>')
                {
                    lexer->consume();
                    currentToken.setTagIsSelfClosing(true);
                    m_tokens.push_back(currentToken);
                    switchToState(HTMLState::Data);
                    break;
                }

            break;

           case HTMLState::DOCTYPE:
                consumeEscapeCodes(true, true, false);

                if (lexer->peek() == ' ')
                {
                    lexer->consume();
                    switchToState(HTMLState::BeforeDOCTYPENameState);
                    break;
                }
                break;
           case HTMLState::BeforeDOCTYPENameState:
                consumeEscapeCodes(true, true, true);

                currentToken = {};
                currentToken.setType(HTMLToken::Type::Doctype);

                if(std::isupper(lexer->peek()))
                {
                    currentToken.appendDoctypeName(std::tolower(lexer->consume()));
                    switchToState(HTMLState::DOCTYPENameState);
                    break;
                }
                else
                {
                    currentToken.appendDoctypeName(lexer->consume());
                    switchToState(HTMLState::DOCTYPENameState);
                    break;
                }
           case HTMLState::DOCTYPENameState:
                consumeEscapeCodes(true, true, false);

                if(lexer->peek() == ' ')
                {
                    lexer->consume();
                    break;
                }
                else if (lexer->peek() == '>')
                {
                    lexer->consume();
                    m_tokens.push_back(currentToken);
                    switchToState(HTMLState::Data);
                    break;
                }
                else if (std::isupper(lexer->peek()))
                {
                    currentToken.appendDoctypeName(std::tolower(lexer->consume()));
                    break;
                }
                else
                {
                    currentToken.appendDoctypeName(lexer->consume());
                    break;
                }

                // NULL OR EOF??
           case HTMLState::MarkupDeclarationOpenState:
                if (lexer->peek() == '-' && lexer->peekAtOffset(1) == '-')
                {
                    lexer->consume();
                    lexer->consume();

                    currentToken = {};
                    currentToken.setType(HTMLToken::Type::Comment);

                    switchToState(HTMLState::CommentStartState);
                    break;
                }
                else if (lexer->caseInsensitiveStringCompare(lexer->peekWhileCaseInsesitive("DOCTYPE"), "DOCTYPE"))
                {
                    lexer->consumeTokenWhileCaseInsesitive("DOCTYPE");
                    switchToState(HTMLState::DOCTYPE);
                    break;
                }

                break;

            case HTMLState::CommentStartState:
                if (lexer->peek() == '-')
                {
                    lexer->consume();
                    switchToState(HTMLState::CommentStartDashState);
                    break;
                }
                else
                {
                    switchToState(HTMLState::CommentState);
                    break;
                }

            case HTMLState::CommentState:
                if(lexer->peek() == '<')
                {
                    lexer->consume();
                    break;
                }
                else if (lexer->peek() == '-')
                {
                    lexer->consume();
                    switchToState(HTMLState::CommentEndDashState);
                    break;
                }
                // else if (lexer->peek())
                // {
                //     // Do I need this?
                //     break;
                // }
                else if (lexer->getTokenIndex() == lexer->getTokensLength() - 1)
                {
                    // End of file!
                    break;
                }
                else
                {
                    currentToken.appendCommentOrCharacterData(lexer->consume());
                    break;
                }

           case HTMLState::CommentStartDashState:
                if(lexer->peek() == '-')
                {
                    lexer->consume();
                    switchToState(HTMLState::CommentEndState);
                    break;
                }
                break;

          case HTMLState::CommentEndState:
                if(lexer->peek() == '>')
                {
                    lexer->consume();
                    switchToState(HTMLState::Data);
                    m_tokens.push_back(currentToken);
                    break;
                }
                break;

          case HTMLState::CommentEndDashState:
                if(lexer->peek() == '-')
                {
                    lexer->consume();
                    switchToState(HTMLState::CommentEndState);
                    break;
                }
                break;
        }
    }
}
