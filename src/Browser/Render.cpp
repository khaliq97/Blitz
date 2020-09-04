#include <Browser/Render.h>
#include <DOM/Text.h>
#include <iostream>
#include <algorithm>
#include <stdio.h>


Render::Render(std::shared_ptr<Document> document, std::shared_ptr<Lexer> lexer)
{
    this->document = document;
    this->lexer = lexer;
}

std::string remove_excessive_ws(std::string str)
{
    bool seen_space = false;
    auto end{ std::remove_if(str.begin(), str.end(),
                             [&seen_space](unsigned ch) {
                                 bool is_space = std::isspace(ch);
                                 std::swap(seen_space, is_space);
                                 return seen_space && is_space;
                             }
              )
    };

    if (end != str.begin() && std::isspace(static_cast<unsigned>(end[-1])))
        --end;
    str.erase(end, str.end());
    return str;
}

