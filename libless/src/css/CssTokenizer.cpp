#include "less/css/CssTokenizer.h"
#include "less/LogStream.h"

using namespace std;

CssTokenizer::CssTokenizer(istream& in, const char* source)
    : in(&in), line(0), source(source) {
  currentToken.source = source;
  lastRead = 0;
  readChar();
  column = 0;
}

const char* CssTokenizer::getSource() {
  return source;
}

void CssTokenizer::readChar() {
  if (in == nullptr)
    return;

  // Last char was a newline. Increment the line counter.
  if (lastReadEq('\n')) {
    line++;
    column = 0;
  } else
    column++;

  in->get(lastRead);

  // check for end of file or escape key
  if (in->eof() || lastRead == 27)
    in = nullptr;
  else if (in->fail() || in->bad())
    throw IOException("Error reading input");

  if (lastReadEq('\n') && column > 0)  // don't count newlines as chars
    column--;
}

Token::Type CssTokenizer::readNextToken() {
  if (in == nullptr) {
    currentToken.type = Token::EOS;
    return Token::EOS;
  }

  currentToken.clear();
  currentToken.line = line;
  currentToken.column = column;

  switch (lastRead) {
    case '@':
      currentToken.type = Token::ATKEYWORD;
      currentToken.append(lastRead);
      readChar();
      if (!readIdent()) {
        currentToken.type = Token::OTHER;
      }
      break;

    case '#':
      currentToken.type = Token::HASH;
      currentToken.append(lastRead);
      readChar();
      if (!readName()) {
        throw ParseException(
            &lastRead, "name following '#'", line, column, source);
      }
      break;

    case '-':
      currentToken.append(lastRead);
      readChar();
      if (readNum(true)) {
        currentToken.type = Token::NUMBER;
        readNumSuffix();
      } else if (readIdent()) {
        currentToken.type = Token::IDENTIFIER;
      } else
        currentToken.type = Token::OTHER;
      break;

    case '~':
      currentToken.append(lastRead);
      readChar();
      if (lastRead == '=') {
        currentToken.append(lastRead);
        readChar();
        currentToken.type = Token::INCLUDES;
      } else
        currentToken.type = Token::OTHER;
      break;

    case '|':
      currentToken.append(lastRead);
      readChar();
      if (lastRead == '=') {
        currentToken.append(lastRead);
        readChar();
        currentToken.type = Token::DASHMATCH;
      } else
        currentToken.type = Token::OTHER;
      break;

    case '/':
      currentToken.append(lastRead);
      readChar();
      if (readComment())
        currentToken.type = Token::COMMENT;
      else
        currentToken.type = Token::OTHER;
      break;

    case ';':
      currentToken.type = Token::DELIMITER;
      currentToken.append(lastRead);
      readChar();
      break;
    case ':':
      currentToken.type = Token::COLON;
      currentToken.append(lastRead);
      readChar();
      break;
    case '{':
      currentToken.type = Token::BRACKET_OPEN;
      currentToken.append(lastRead);
      readChar();
      break;
    case '}':
      currentToken.type = Token::BRACKET_CLOSED;
      currentToken.append(lastRead);
      readChar();
      break;
    case '(':
      currentToken.type = Token::PAREN_OPEN;
      currentToken.append(lastRead);
      readChar();
      break;
    case ')':
      currentToken.type = Token::PAREN_CLOSED;
      currentToken.append(lastRead);
      readChar();
      break;
    case '[':
      currentToken.type = Token::BRACE_OPEN;
      currentToken.append(lastRead);
      readChar();
      break;
    case ']':
      currentToken.type = Token::BRACE_CLOSED;
      currentToken.append(lastRead);
      readChar();
      break;

    case '.':
      currentToken.append(lastRead);
      readChar();
      if (readNum(false)) {
        currentToken.type = Token::NUMBER;
        readNumSuffix();
      }
      break;

    default:
      if (readString())
        currentToken.type = Token::STRING;
      else if (readNum(true)) {
        currentToken.type = Token::NUMBER;
        readNumSuffix();

      } else if (readIdent()) {
        currentToken.type = Token::IDENTIFIER;

        if (currentToken == "url" && readUrl())
          currentToken.type = Token::URL;
        else if (currentToken == "u" && lastReadEq('+')) {
          currentToken.append(lastRead);
          readChar();
          currentToken.type = Token::UNICODE_RANGE;
          readUnicodeRange();
        }
      } else if (readWhitespace()) {
        currentToken.type = Token::WHITESPACE;
        while (readWhitespace()) {
        };
      } else {
        currentToken.append(lastRead);
        readChar();
      }
      break;
  }

  LogStream().notice(4) << "Token: " << currentToken << '[' << currentToken.type
                        << ']';

  return currentToken.type;
}

bool CssTokenizer::readIdent() {
  if (lastReadEq('-')) {
    currentToken.append(lastRead);
    readChar();
  }
  if (!readNMStart())
    return false;
  else
    while (readNMChar()) {
    }
  return true;
}

bool CssTokenizer::readName() {
  if (!readNMChar())
    return false;
  while (readNMChar()) {
  }
  return true;
}

bool CssTokenizer::readNMStart() {
  if (in == nullptr)
    return false;

  if (lastReadEq('_') || lastReadInRange('a', 'z') ||
      lastReadInRange('A', 'Z')) {
    currentToken.append(lastRead);
    readChar();
    return true;
  } else
    return (readNonAscii() || readEscape());
}
bool CssTokenizer::readNonAscii() {
  if (in == nullptr || lastRead >= 0)
    return false;

  currentToken.append(lastRead);
  readChar();
  return true;
}

bool CssTokenizer::readEscape() {
  if (!lastReadEq('\\'))
    return false;
  currentToken.append(lastRead);
  readChar();

  if (readUnicode())
    return true;
  else if (!lastReadEq('\n') && !lastReadEq('\r') && !lastReadEq('\f')) {
    currentToken.append(lastRead);
    readChar();
    return true;
  } else
    return false;
}

bool CssTokenizer::readUnicode() {
  if (!lastReadIsHex())
    return false;

  // [0-9a-f]{1,6}(\r\n|[ \n\r\t\f])?
  for (int i = 0; i < 6; i++) {
    currentToken.append(lastRead);
    readChar();
    if (readWhitespace() || !lastReadIsHex())
      break;
  }
  return true;
}

bool CssTokenizer::readNMChar() {
  if (in == nullptr)
    return false;

  if (lastReadEq('_') || lastReadInRange('a', 'z') ||
      lastReadInRange('A', 'Z') || lastReadIsDigit() || lastReadEq('-')) {
    currentToken.append(lastRead);
    readChar();
    return true;
  } else
    return (readNonAscii() || readEscape());
}

bool CssTokenizer::readNum(bool readDecimals) {
  if (!lastReadIsDigit())
    return false;
  while (lastReadIsDigit()) {
    currentToken.append(lastRead);
    readChar();
  }

  if (readDecimals && lastReadEq('.')) {
    currentToken.append(lastRead);
    readChar();

    while (lastReadIsDigit()) {
      currentToken.append(lastRead);
      readChar();
    }
  }
  return true;
}

bool CssTokenizer::readNumSuffix() {
  if (lastRead == '%') {
    currentToken.type = Token::PERCENTAGE;
    currentToken.append(lastRead);
    readChar();
    return true;
  } else if (readIdent()) {
    currentToken.type = Token::DIMENSION;
    return true;
  }
  // TODO: Support identifiers starting with '-'
  return false;
}

bool CssTokenizer::readString() {
  if (!lastReadEq('"') && !lastReadEq('\''))
    return false;
  char delim = lastRead;

  currentToken.append(lastRead);
  readChar();
  while (in != nullptr) {
    if (lastReadEq(delim)) {
      currentToken.append(lastRead);
      readChar();
      return true;
    } else if (lastReadEq('\n') || lastReadEq('\r') || lastReadEq('\f')) {
      throw ParseException(
          "end of line", "end of string", line, column, source);
    } else if (lastReadEq('\\')) {
      // note that even though readEscape() returns false it still
      // eats the '\'.
      if (!readEscape()) {
        readNewline();
      }
    } else {
      currentToken.append(lastRead);
      readChar();
    }
  }
  throw ParseException("end of input", "end of string", line, column, source);
}

bool CssTokenizer::readNewline() {
  if (lastReadEq('\r')) {
    currentToken.append(lastRead);
    readChar();
    if (lastReadEq('\n')) {
      currentToken.append(lastRead);
      readChar();
    }
    return true;
  } else if (lastReadEq('\n') || lastReadEq('\f')) {
    currentToken.append(lastRead);
    readChar();
    return true;
  } else
    return false;
}

bool CssTokenizer::readWhitespace() {
  if (lastReadEq(' ') || lastReadEq('\t') || lastReadEq('\r') ||
      lastReadEq('\n') || lastReadEq('\f')) {
    currentToken.append(lastRead);
    readChar();
    return true;
  } else
    return false;
}

bool CssTokenizer::readUrl() {
  string urlchars = "!#$%&*-[]-~";

  if (!lastReadEq('('))
    return false;
  currentToken.append(lastRead);
  readChar();
  while (readWhitespace()) {
  };

  if (readString()) {
    if (lastReadEq(')')) {
      currentToken.append(lastRead);
      readChar();
      return true;
    } else {
      throw ParseException(&lastRead, "end of url (')')", line, column, source);
    }
  }

  while (in != nullptr) {
    if (readWhitespace() || lastReadEq(')')) {
      while (readWhitespace()) {
      };
      if (lastReadEq(')')) {
        currentToken.append(lastRead);
        readChar();
        return true;
      } else {
        throw ParseException(
            &lastRead, "end of url (')')", line, column, source);
      }
    } else if (in != nullptr && urlchars.find(lastRead)) {
      currentToken.append(lastRead);
      readChar();
    } else if (!readNonAscii() && !readEscape()) {
      throw ParseException(&lastRead, "end of url (')')", line, column, source);
    }
  }
  throw ParseException(&lastRead, "end of url (')')", line, column, source);
}

bool CssTokenizer::readComment() {
  if (!lastReadEq('*'))
    return false;
  currentToken.append(lastRead);
  readChar();
  while (in != nullptr) {
    if (lastReadEq('*')) {
      currentToken.append(lastRead);
      readChar();

      if (lastReadEq('/')) {
        currentToken.append(lastRead);
        readChar();
        return true;
      }
      continue;
    }
    currentToken.append(lastRead);
    readChar();
  }
  throw ParseException(&lastRead, "end of comment (*/)", line, column, source);
}

bool CssTokenizer::readUnicodeRange() {
  if (in == nullptr)
    return false;
  for (int i = 0; i < 6; i++) {
    if (!lastReadIsHex())
      break;
    currentToken.append(lastRead);
    readChar();
  }
  if (!lastReadEq('-'))
    return true;

  for (int i = 0; i < 6; i++) {
    if (!lastReadIsHex())
      break;
    currentToken.append(lastRead);
    readChar();
  }
  return true;
}

Token& CssTokenizer::getToken() {
  return currentToken;
}
Token::Type CssTokenizer::getTokenType() {
  return currentToken.type;
}

bool CssTokenizer::lastReadEq(char c) {
  return (in != nullptr && lastRead == c);
}

bool CssTokenizer::lastReadInRange(char c1, char c2) {
  return (in != nullptr && lastRead >= c1 && lastRead <= c2);
}
bool CssTokenizer::lastReadIsDigit() {
  return (in != nullptr && lastReadInRange('0', '9'));
}
bool CssTokenizer::lastReadIsHex() {
  return (in != nullptr && (lastReadIsDigit() || lastReadInRange('a', 'f') ||
                            lastReadInRange('A', 'F')));
}
