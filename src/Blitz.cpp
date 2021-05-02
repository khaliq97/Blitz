#include <Blitz.h>
#include <fstream>
#include <gtkmm.h>
#include <iostream>
#include <CSS/StyleResolver.h>
#include <fmt/core.h>

Blitz::Blitz(std::string htmlFilePath) : m_htmlFilePath(htmlFilePath)
{

}

void Blitz::loadHTML(std::string html)
{
    m_htmlTokenizer = std::make_unique<HTMLStateMachine>(html);
    m_htmlTokenizer->run();

    m_documentParser = std::make_unique<HTMLDocumentParser>(m_htmlTokenizer->tokens());
    m_documentParser->run();

    m_cssTokenizer = std::make_unique<Tokenizer>(Tools::getFileContent("../res/default.css"));
    m_cssParser = std::make_unique<CSS::Parser>(m_cssTokenizer->tokens());

    m_document = m_documentParser->document();

    m_blitzDefaultStyleSheet = m_cssParser->styleSheet();
    m_selectorEngine = std::make_unique<SelectorEngine>(m_blitzDefaultStyleSheet, m_document);

    m_documentWithStyles = m_selectorEngine->documentWithStyles();
    m_styleResolver = std::make_unique<StyleResolver>(m_documentWithStyles);

    m_documentWithResolvedStyles = m_styleResolver->documentWithResolvedStyles();

    m_documentWithResolvedStyles->printDocument(m_documentWithResolvedStyles, "");
}
