#include <JS/JS.h>
#include <iostream>
#include <Parser/Lexer.h>
#include <JS/Scanner.h>
#include <JS/Token.h>
#include <JS/Parser.h>
#include <vector>
#include <memory>
#include <JS/Interpreter.h>
static Lexer lexer;
bool hadError = false;

JS::JS(std::string fileName)
{
    if (!fileName.empty())
    {
        runFile(fileName);
    }
    else
    {
        runPrompt();
    }
}

void JS::runPrompt()
{
    printf("lumber js\n");
    for (;;)
    {
        std::string line;
        printf("> ");
        std::getline(std::cin, line);
        run(line);
    }

}

void JS::runFile(std::string path)
{
    run(lexer.getFileContent(path));

    if (hadError)
        exit(0);
}

void JS::run(std::string source)
{
    std::unique_ptr<Scanner> scanner = std::make_unique<Scanner>(source);
    std::vector<std::shared_ptr<Token>> tokens = scanner->scanTokens();

#ifdef TOKEN_DEBUG
    for (auto token: tokens)
    {
       // printf("%s\n", token->toString().c_str());
    }
#endif

    std::unique_ptr<Parser> parser = std::make_unique<Parser>(tokens);

    std::shared_ptr<Expression> expression = parser->parse();

    // Fix ASTPrinter not working because of different template types

    // ASTPrinter astPrinter;
    // printf("AST:\n  %s\n", astPrinter.print(expression).c_str());

    std::unique_ptr<Interpreter> interpreter = std::make_unique<Interpreter>();
    interpreter->intereprt(expression);
}

void JS::error(int line, std::string message)
{
    report(line, "", message);
}

void JS::error(std::shared_ptr<Token> token, std::string message)
{
    if (token->type == TokenType::ENDOFFILE)
    {
        report(token->line, " at end", message);
    }
    else
    {
        report(token->line, " at '" + token->lexeme + "'", message);
    }
}

void JS::report(int line, std::string where, std::string message)
{
    printf("[line %d] Error%s: %s\n", line, where.c_str(), message.c_str());
}
