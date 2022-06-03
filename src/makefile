IDIR= ${CURDIR}
CC=g++
CFLAGS= -g -I$(IDIR) -std=c++20

ODIR=obj
LDIR=../lib

_DEPS = Blitz.h \
		LexerUtils.h \
		HTML/Tokenizer.h \
		HTML/HTMLDocumentParser.h \
		HTML/Lexer.h \
		HTML/HTMLToken.h \
		DOM/Document.h \
		DOM/HTMLElement.h \
		DOM/HTMLHeadElement.h \
		DOM/Element.h \
		DOM/Node.h \
		DOM/CharacterData.h \
		DOM/Comment.h \
		DOM/Attribute.h \
		DOM/NamedNodeMap.h \
		DOM/DocumentType.h \
		DOM/Text.h \
		Browser/Core.h \
		Terminal/Terminal.h \
		JS/JS.h \
		JS/Interpreter.h \
		JS/Scanner.h \
		JS/Token.h \
		JS/TokenType.h \
		JS/ASTNode.h \
		JS/Expression.h \
		JS/ExpressionStatement.h \
		JS/BinaryExpression.h \
		JS/LiteralExpression.h \
		JS/VariableDeclaration.h \
		JS/Enviroment/DeclarativeEnviromentRecord.h \
		JS/Enviroment/FunctionEnvriomentRecord.h \
		JS/Enviroment/VariableEnviroment.h \
		JS/Enviroment/VariableRecord.h \
		CSS/Tokenizer.h \
		CSS/CSSToken.h \
		CSS/Parser.h \
		CSS/QualifiedRule.h \
		CSS/SimpleBlock.h \
		CSS/Stylesheet.h \
		CSS/Selectors/ComplexSelector.h \
		CSS/Selectors/CompoundSelector.h \
		CSS/Selectors/SimpleSelector.h \
		CSS/Declaration.h \
		CSS/StyleRule.h \
		CSS/SelectorEngine.h \
		CSS/StyleProperty.h \
		CSS/StyleResolver.h \
		Layout/WebView.h \
		Layout/HTMLView.h \
		Layout/LayoutBox.h \
		Layout/BlockFormattingContext.h \
		Layout/LineBox.h \
		Layout/LayoutText.h

DEPS = $(patsubst, %,$(IDIR)/%,$(_DEPS))

_OBJ = Main.o Blitz.o LexerUtils.o \
		HTML/Tokenizer.o \
		HTML/HTMLDocumentParser.o \
		HTML/Lexer.o \
		HTML/HTMLToken.o \
		DOM/Document.o \
		DOM/HTMLElement.o \
		DOM/HTMLHeadElement.o \
		DOM/Element.o \
		DOM/Node.o \
		DOM/CharacterData.o \
		DOM/Comment.o \
		DOM/Attribute.o \
		DOM/NamedNodeMap.o \
		DOM/DocumentType.o \
		DOM/Text.o \
		Browser/Core.o \
		Terminal/Terminal.o \
		JS/JS.o \
		JS/Interpreter.o \
		JS/Scanner.o \
		JS/Parser.o \
		JS/Token.o \
		JS/TokenType.o \
		JS/ASTNode.o \
		JS/Expression.o \
		JS/ExpressionStatement.o \
		JS/BinaryExpression.o \
		JS/LiteralExpression.o \
		JS/UnaryExpression.o \
		JS/Value.o \
		JS/Error.o \
		JS/VariableDeclaration.o \
		JS/Enviroment/DeclarativeEnviromentRecord.o \
		JS/Enviroment/FunctionEnvriomentRecord.o \
		JS/Enviroment/VariableEnviroment.o \
		JS/Enviroment/VariableRecord.o \
		CSS/Tokenizer.o \
		CSS/CSSToken.o \
		CSS/Parser.o \
		CSS/QualifiedRule.o \
		CSS/SimpleBlock.o \
		CSS/Stylesheet.o \
		CSS/Selectors/ComplexSelector.o \
		CSS/Selectors/CompoundSelector.o \
		CSS/Selectors/SimpleSelector.o \
		CSS/Declaration.o \
		CSS/StyleRule.o \
		CSS/SelectorEngine.o \
		CSS/StyleProperty.o \
		CSS/StyleResolver.o \
		Layout/WebView.o \
		Layout/HTMLView.o \
		Layout/LayoutBox.o \
		Layout/BlockFormattingContext.o \
		Layout/LineBox.o \
		Layout/LayoutText.o

OBJ = $(patsubst %, $(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.cpp
	@ mkdir -p $(ODIR)/Browser $(ODIR)/CSS $(ODIR)/CSS/Selectors $(ODIR)/DOM $(ODIR)/JS $(ODIR)/Layout $(ODIR)/HTML $(ODIR)/Terminal
	$(CC) -c -o $@ $< $(CFLAGS) `pkg-config --libs --cflags gtkmm-3.0 fmt curlpp`

blitz: $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS) `pkg-config --libs --cflags gtkmm-3.0 fmt curlpp`

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(ODIR)/Browser/*.o $(ODIR)/CSS/*.o $(ODIR)/CSS/Selectors/*.o $(ODIR)/Layout/*.o $(ODIR)/DOM/*.o $(ODIR)/JS/*.o $(ODIR)/HTML/*.o $(ODIR)/Terminal/*.o *~ core $(INCDIR)/*~
