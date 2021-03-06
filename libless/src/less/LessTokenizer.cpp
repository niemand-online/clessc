#include "less/less/LessTokenizer.h"

bool LessTokenizer::readComment() {
  if (!lastReadEq('/'))
    return CssTokenizer::readComment();

  currentToken.append(lastRead);
  readChar();
  while (in != nullptr && !lastReadEq('\n')) {
    currentToken.append(lastRead);
    readChar();
  }
  return true;
}
