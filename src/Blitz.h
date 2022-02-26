#ifndef BLITZ_H
#define BLITZ_H
#include <string>
#include <memory>
#include <HTML/HTMLDocumentParser.h>
#include <HTML/Tokenizer.h>
#include <CSS/Tokenizer.h>
#include <CSS/Parser.h>
#include <CSS/SelectorEngine.h>
#include <Layout/WebView.h>
#include <CSS/StyleResolver.h>
class Blitz {
public:
    Blitz(std::string htmlFilePath);
    void loadHTML(std::string html);

    std::shared_ptr<Document> documentWithResolvedStyles() { return m_documentWithResolvedStyles; }
    const std::string htmlFilePath() { return m_htmlFilePath; }
private:

    std::string m_htmlFilePath;

    std::unique_ptr<HTML::Tokenizer> m_htmlTokenizer;
    std::unique_ptr<HTMLDocumentParser> m_documentParser;
    std::unique_ptr<Tokenizer> m_cssTokenizer;
    std::unique_ptr<CSS::Parser> m_cssParser;
    std::unique_ptr<SelectorEngine> m_selectorEngine;
    std::unique_ptr<StyleResolver> m_styleResolver;

    std::shared_ptr<Document> m_document;
    std::shared_ptr<Document> m_documentWithStyles;
    std::shared_ptr<Document> m_documentWithResolvedStyles;
    std::shared_ptr<Stylesheet> m_blitzDefaultStyleSheet;
};

#endif // BLITZ_H
