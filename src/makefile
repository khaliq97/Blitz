IDIR= ${CURDIR}
CC=g++
CFLAGS= -g -I$(IDIR)

ODIR=obj
LDIR=../lib

_OBJ = Main.o Blitz.o LexerUtils.o \
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
		Layout/LayoutBox.o

OBJ = $(patsubst %, $(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.cpp
	@ mkdir -p $(ODIR)/Browser $(ODIR)/CSS $(ODIR)/CSS/Selectors $(ODIR)/DOM $(ODIR)/JS $(ODIR)/Layout $(ODIR)/Parser $(ODIR)/Terminal
	$(CC) -c -o $@ $< $(CFLAGS) `pkg-config --libs --cflags gtkmm-3.0 fmt curlpp`

blitz: $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS) `pkg-config --libs --cflags gtkmm-3.0 fmt curlpp`

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(ODIR)/Browser/*.o $(ODIR)/CSS/*.o $(ODIR)/CSS/Selectors/*.o $(ODIR)/Layout/*.o $(ODIR)/DOM/*.o $(ODIR)/JS/*.o $(ODIR)/Parser/*.o $(ODIR)/Terminal/*.o *~ core $(INCDIR)/*~
