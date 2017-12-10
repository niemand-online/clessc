#include "less/TokenList.h"

using namespace std;

TokenList::~TokenList() {
}

void TokenList::ltrim() {
  while (!empty() && front().type == Token::WHITESPACE) {
    pop_front();
  }
}
void TokenList::rtrim() {
  while (!empty() && back().type == Token::WHITESPACE) {
    pop_back();
  }
}
void TokenList::trim() {
  ltrim();
  rtrim();
}

string TokenList::toString() const {
  string str;
  list<Token>::const_iterator it;

  for (it = begin(); it != end(); it++) {
    str.append(*it);
  }
  return str;
}

bool TokenList::contains(const Token &t) const {
  list<Token>::const_iterator it;

  for (it = begin(); it != end(); it++) {
    if (*it == t)
      return true;
  }
  return false;
}

bool TokenList::contains(Token::Type type, const string &str) const {
  list<Token>::const_iterator it;

  for (it = begin(); it != end(); it++) {
    if ((*it).type == type && *it == str)
      return true;
  }
  return false;
}

bool TokenList::containsType(Token::Type type) const {
  list<Token>::const_iterator it;

  for (it = begin(); it != end(); it++) {
    if ((*it).type == type)
      return true;
  }
  return false;
}
