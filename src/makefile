IDIR=../include
CC=g++
CFLAGS= -g -I$(IDIR)

ODIR=obj
LDIR=../lib

_DEPS = Lumber.h Parser/HTMLStateMachine.h Parser/HTMLDocumentParser.h Parser/Lexer.h Parser/HTMLToken.h Parser/CSSParser.h Parser/Stylesheet.h Parser/StyleRule.h DOM/Document.h DOM/HTMLElement.h DOM/HTMLHeadElement.h DOM/Element.h DOM/Node.h DOM/CharacterData.h DOM/Comment.h DOM/Attribute.h DOM/NamedNodeMap.h DOM/DocumentType.h DOM/Text.h Browser/HTMLView.h Browser/Window.h Browser/Render.h Browser/BoxModel.h Browser/RenderNode.h Browser/Layout.h Browser/Dimension.h Terminal/Terminal.h
DEPS = $(patsubst, %,$(IDIR)/%,$(_DEPS))

_OBJ = Main.o Lumber.o Parser/HTMLStateMachine.o Parser/HTMLDocumentParser.o Parser/Lexer.o Parser/HTMLToken.o Parser/CSSParser.o Parser/Stylesheet.o Parser/StyleRule.o DOM/Document.o DOM/HTMLElement.o DOM/HTMLHeadElement.o DOM/Element.o DOM/Node.o DOM/CharacterData.o DOM/Comment.o DOM/Attribute.o DOM/NamedNodeMap.o DOM/DocumentType.o DOM/Text.o Browser/HTMLView.o Browser/Window.o Browser/Render.o Browser/BoxModel.o Browser/RenderNode.o Browser/Layout.o Browser/Dimension.o Terminal/Terminal.o
OBJ = $(patsubst %, $(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) `pkg-config --libs --cflags gtk+-3.0 gtkmm-3.0 cairo libcurl`

lumber: $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS) `pkg-config --libs --cflags gtk+-3.0 gtkmm-3.0 cairo libcurl`

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
