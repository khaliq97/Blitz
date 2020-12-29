IDIR= ${CURDIR}
CC=g++
CFLAGS= -g -I$(IDIR)

ODIR=obj
LDIR=../lib

_DEPS = Blitz.h \
		Parser/HTMLStateMachine.h \
		Parser/HTMLDocumentParser.h \
		Parser/Lexer.h \
		Parser/HTMLToken.h \
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
		Browser/Window.h \
		Browser/Render.h \
		Browser/BoxModel.h \
		Browser/Layout.h \
		Browser/Dimension.h \
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
		CSS/SelectorEngine.h

DEPS = $(patsubst, %,$(IDIR)/%,$(_DEPS))

_OBJ = Main.o Blitz.o \
		Parser/HTMLStateMachine.o \
		Parser/HTMLDocumentParser.o \
		Parser/Lexer.o \
		Parser/HTMLToken.o \
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
		Browser/Window.o \
		Browser/Render.o \
		Browser/BoxModel.o \
		Browser/Layout.o \
		Browser/Dimension.o \
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
		CSS/SelectorEngine.o

OBJ = $(patsubst %, $(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) `pkg-config --libs --cflags gtk+-3.0 gtkmm-3.0 cairo libcurl`

blitz: $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS) `pkg-config --libs --cflags gtk+-3.0 gtkmm-3.0 cairo libcurl`

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(ODIR)/Browser/*.o $(ODIR)/CSS/*.o $(ODIR)/CSS/Selectors/*.o $(ODIR)/DOM/*.o $(ODIR)/JS/*.o $(ODIR)/Parser/*.o $(ODIR)/Terminal/*.o *~ core $(INCDIR)/*~
