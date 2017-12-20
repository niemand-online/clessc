#include "less/Token.h"

using namespace std;

char Token::BUILTIN_SOURCE[8] = "builtin";

const Token Token::BUILTIN_SPACE(" ", Token::WHITESPACE, 0, 0, BUILTIN_SOURCE);
const Token Token::BUILTIN_COMMA(",", Token::OTHER, 0, 0, BUILTIN_SOURCE);
const Token Token::BUILTIN_PAREN_OPEN(
    "(", Token::PAREN_OPEN, 0, 0, BUILTIN_SOURCE);
const Token Token::BUILTIN_PAREN_CLOSED(
    ")", Token::PAREN_CLOSED, 0, 0, BUILTIN_SOURCE);

Token::Token() : line(0), column(0), source(BUILTIN_SOURCE), type(OTHER) {
}

Token::Token(unsigned int line, unsigned int column, const char *source)
    : line(line), column(column), source(source), type(OTHER) {
}

Token::Token(const string &s,
             Type t,
             unsigned int line,
             unsigned int column,
             const char *source)
    : line(line), column(column), source(source) {
  type = t;
  append(s);
}

void Token::setLocation(const Token &ref) {
  line = ref.line;
  column = ref.column;
  source = ref.source;
}

void Token::clear() {
  string::clear();
  type = OTHER;
}

bool Token::stringHasQuotes() const {
  return (at(0) == '"' || at(0) == '\'');
}

void Token::removeQuotes() {
  removeQuotes(*this);
}

void Token::removeQuotes(string &str) const {
  char quote;
  string::iterator i;

  if (str.empty() || !(str[0] == '"' || str[0] == '\''))
    return;

  quote = str[0];
  str.erase(str.begin());
  str.erase(str.end() - 1);

  for (i = str.begin(); i != str.end(); i++) {
    if (*i == '\\' && *(i + 1) == quote)
      str.erase(i);
  }
}

string Token::getUrlString() const {
  string ret;
  if (type == URL) {
    ret = substr(4, length() - 5);
    removeQuotes(ret);
    return ret;
  } else
    return "";
}
