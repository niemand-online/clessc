#include "less/css/CssWriter.h"

using namespace std;

CssWriter::CssWriter() {
  out = nullptr;
  column = 0;
  sourcemap = nullptr;
}

CssWriter::CssWriter(ostream &out) : out(&out), column(0) {
  sourcemap = nullptr;
}
CssWriter::CssWriter(ostream &out, SourceMapWriter &sourcemap)
    : out(&out), column(0), sourcemap(&sourcemap) {
}

unsigned int CssWriter::getColumn() {
  return column;
}

void CssWriter::writeStr(const char *str, size_t len) {
  out->write(str, len);
  column += len;
}
void CssWriter::writeToken(const Token &token) {
  string url;

  if (rootpath != nullptr && token.type == Token::URL) {
    url = token.getUrlString();
    if (url.find(':') == string::npos) {
      writeStr("url(\"", 5);
      writeStr(rootpath, strlen(rootpath));

      writeStr(url.c_str(), url.size());
      writeStr("\")", 2);
    } else {
      writeStr(token.c_str(), token.size());
    }
  } else {
    writeStr(token.c_str(), token.size());
  }
}
void CssWriter::writeTokenList(const TokenList &tokens) {
  auto i = tokens.begin();

  for (; i != tokens.end(); i++) {
    writeToken(*i);
  }
}

void CssWriter::writeSelector(const TokenList &selector) {
  TokenList::const_iterator it;
  bool newselector = true;

  for (it = selector.begin(); it != selector.end(); it++) {
    if (newselector && sourcemap != nullptr) {
      sourcemap->writeMapping(column, *it);
      newselector = false;
    }

    writeToken(*it);

    if ((*it) == ",")
      newselector = true;
  }
}

void CssWriter::writeValue(const TokenList &value) {
  auto it = value.begin();
  const Token *t;

  while (it != value.end() && (*it).type == Token::WHITESPACE) {
    it++;
  }

  if (sourcemap != nullptr)
    sourcemap->writeMapping(column, *it);
  t = &(*it);

  for (; it != value.end(); it++) {
    if ((*it).source != t->source || (*it).line != t->line) {
      if (sourcemap != nullptr)
        sourcemap->writeMapping(column, (*it));
      t = &(*it);
    }

    writeToken(*it);
  }
}

void CssWriter::writeAtRule(const Token &keyword, const TokenList &rule) {
  if (sourcemap != nullptr)
    sourcemap->writeMapping(column, keyword);

  writeToken(keyword);
  writeStr(" ", 1);

  if (sourcemap != nullptr)
    sourcemap->writeMapping(column, rule.front());

  writeTokenList(rule);
  if (rule.back() != "}")
    writeStr(";", 1);
}

void CssWriter::writeRulesetStart(const TokenList &selector) {
  writeSelector(selector);

  writeStr("{", 1);
}

void CssWriter::writeRulesetEnd() {
  writeStr("}", 1);
}

void CssWriter::writeDeclaration(const Token &property,
                                 const TokenList &value) {
  if (sourcemap != nullptr)
    sourcemap->writeMapping(column, property);

  writeToken(property);
  writeStr(":", 1);

  writeValue(value);
}

void CssWriter::writeDeclarationDeliminator() {
  writeStr(";", 1);
}

void CssWriter::writeComment(const Token &comment) {
  writeToken(comment);
}

void CssWriter::writeMediaQueryStart(const TokenList &selector) {
  writeSelector(selector);
  writeStr("{", 1);
}

void CssWriter::writeMediaQueryEnd() {
  writeStr("}", 1);
}

void CssWriter::writeSourceMapUrl(const char *sourcemap_url) {
  *out << endl << "/*# sourceMappingURL=" << sourcemap_url << " */" << endl;
}
