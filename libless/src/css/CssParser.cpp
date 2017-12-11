#include "less/css/CssParser.h"
#include <iostream>
#include "less/LogStream.h"
#include "less/stylesheet/AtRule.h"
#include "less/stylesheet/MediaQuery.h"

CssParser::CssParser(CssTokenizer& tokenizer) {
  this->tokenizer = &tokenizer;
}

void CssParser::parseStylesheet(Stylesheet& stylesheet) {
  tokenizer->readNextToken();

  skipWhitespace();
  while (parseStatement(stylesheet) || parseEmptyStatement()) {
    skipWhitespace();
  }

  // stream should end here
  if (tokenizer->getTokenType() != Token::EOS) {
    throw ParseException(tokenizer->getToken(), "end of input");
  }
}

void CssParser::skipWhitespace() {
  while (tokenizer->getTokenType() == Token::WHITESPACE ||
         tokenizer->getTokenType() == Token::COMMENT) {
    tokenizer->readNextToken();
  }
}
bool CssParser::parseWhitespace(TokenList& tokens) {
  while (tokenizer->getTokenType() == Token::WHITESPACE ||
         tokenizer->getTokenType() == Token::COMMENT) {
    if (tokenizer->getTokenType() == Token::WHITESPACE) {
      tokens.push_back(Token::BUILTIN_SPACE);
    }

    tokenizer->readNextToken();
  }
  return true;
}

bool CssParser::parseEmptyStatement() {
  Token* t;

  // empty statement
  if (tokenizer->getTokenType() == Token::DELIMITER) {
    t = &tokenizer->getToken();

    LogStream().warning() << t->source << ": Line " << t->line << ", Column"
                          << t->column
                          << " Warning: Semicolon without statement.";

    tokenizer->readNextToken();

    return true;
  } else
    return false;
}

bool CssParser::parseStatement(Stylesheet& stylesheet) {
  Ruleset* ruleset = parseRuleset(stylesheet);
  if (ruleset != nullptr)
    return true;

  MediaQuery* query = parseMediaQuery(stylesheet);
  if (query != nullptr)
    return true;

  AtRule* atrule = parseAtRule(stylesheet);
  return atrule != nullptr;
}

MediaQuery* CssParser::parseMediaQuery(Stylesheet& stylesheet) {
  MediaQuery* query;

  if (tokenizer->getTokenType() != Token::ATKEYWORD ||
      "@media" != tokenizer->getToken())
    return nullptr;

  query = stylesheet.createMediaQuery();

  query->getSelector().push_back(tokenizer->getToken());

  tokenizer->readNextToken();
  skipWhitespace();

  parseSelector(query->getSelector());

  if (tokenizer->getTokenType() != Token::BRACKET_OPEN) {
    throw ParseException(tokenizer->getToken(), "{");
  }
  tokenizer->readNextToken();

  skipWhitespace();
  while (parseStatement(*query)) {
    skipWhitespace();
  }

  if (tokenizer->getTokenType() != Token::BRACKET_CLOSED) {
    throw ParseException(tokenizer->getToken(),
                         "end of media query block ('}')");
  }
  tokenizer->readNextToken();
  skipWhitespace();
  return query;
}

AtRule* CssParser::parseAtRule(Stylesheet& stylesheet) {
  AtRule* atrule;

  if (tokenizer->getTokenType() != Token::ATKEYWORD)
    return nullptr;

  atrule = stylesheet.createAtRule(tokenizer->getToken());
  tokenizer->readNextToken();
  skipWhitespace();

  while (parseAny(atrule->getRule())) {
  };

  if (!parseBlock(atrule->getRule())) {
    if (tokenizer->getTokenType() != Token::DELIMITER) {
      throw ParseException(tokenizer->getToken(),
                           "delimiter (';') at end of @-rule");
    }
    tokenizer->readNextToken();
    skipWhitespace();
  }

  return atrule;
}

bool CssParser::parseBlock(TokenList& tokens) {
  if (tokenizer->getTokenType() != Token::BRACKET_OPEN)
    return false;

  tokens.push_back(tokenizer->getToken());
  tokenizer->readNextToken();
  skipWhitespace();

  while (true) {
    if (!(parseAny(tokens) || parseBlock(tokens))) {
      if (tokenizer->getTokenType() == Token::ATKEYWORD) {
        tokens.push_back(tokenizer->getToken());
        tokenizer->readNextToken();
        parseWhitespace(tokens);

      } else if (tokenizer->getTokenType() == Token::DELIMITER) {
        tokens.push_back(tokenizer->getToken());
        tokenizer->readNextToken();
        skipWhitespace();

      } else
        break;
    }
  }

  if (tokenizer->getTokenType() != Token::BRACKET_CLOSED) {
    throw ParseException(tokenizer->getToken(), "end of block ('}')");
  }
  tokens.push_back(tokenizer->getToken());
  tokenizer->readNextToken();
  skipWhitespace();
  return true;
}

Ruleset* CssParser::parseRuleset(Stylesheet& stylesheet) {
  Ruleset* ruleset;
  Selector selector;

  if (!parseSelector(selector)) {
    if (tokenizer->getTokenType() != Token::BRACKET_OPEN) {
      return nullptr;
    }
  } else if (tokenizer->getTokenType() != Token::BRACKET_OPEN) {
    throw ParseException(tokenizer->getToken(),
                         "a declaration block ('{...}')");
  }
  tokenizer->readNextToken();

  ruleset = stylesheet.createRuleset(selector);

  skipWhitespace();
  parseDeclaration(*ruleset);

  while (tokenizer->getTokenType() == Token::DELIMITER) {
    tokenizer->readNextToken();
    skipWhitespace();
    parseDeclaration(*ruleset);
  }

  if (tokenizer->getTokenType() != Token::BRACKET_CLOSED) {
    throw ParseException(tokenizer->getToken(),
                         "end of declaration block ('}')");
  }
  tokenizer->readNextToken();
  skipWhitespace();

  return ruleset;
}

bool CssParser::parseSelector(Selector& selector) {
  if (!parseAny(selector))
    return false;

  while (parseAny(selector)) {
  };

  // delete trailing whitespace
  selector.rtrim();
  return true;
}

Declaration* CssParser::parseDeclaration(Ruleset& ruleset) {
  Declaration* declaration = nullptr;
  TokenList property;
  Token keyword;

  if (!parseProperty(property))
    return nullptr;

  skipWhitespace();

  keyword = property.front();
  keyword.assign(property.toString());

  declaration = ruleset.createDeclaration(keyword);

  if (tokenizer->getTokenType() != Token::COLON) {
    throw ParseException(tokenizer->getToken(),
                         "colon following property(':')");
  }
  tokenizer->readNextToken();
  skipWhitespace();

  if (!parseValue(declaration->getValue())) {
    throw ParseException(tokenizer->getToken(), "value for property");
  }
  return declaration;
}

bool CssParser::parseProperty(TokenList& tokens) {
  if ("*" == tokenizer->getToken()) {
    // suppor for an IE Hack
    tokens.push_back(tokenizer->getToken());
    tokenizer->readNextToken();

    if (tokenizer->getTokenType() == Token::IDENTIFIER) {
      tokens.push_back(tokenizer->getToken());
      tokenizer->readNextToken();
    }
    return true;

  } else if (tokenizer->getTokenType() != Token::IDENTIFIER)
    return false;

  tokens.push_back(tokenizer->getToken());
  tokenizer->readNextToken();
  return true;
}

bool CssParser::parseValue(TokenList& value) {
  if (parseAny(value) || parseBlock(value)) {
  } else if (tokenizer->getTokenType() == Token::ATKEYWORD) {
    value.push_back(tokenizer->getToken());
    tokenizer->readNextToken();
    parseWhitespace(value);

  } else {
    return false;
  }

  while (true) {
    if (parseAny(value) || parseBlock(value)) {
    } else if (tokenizer->getTokenType() == Token::ATKEYWORD) {
      value.push_back(tokenizer->getToken());
      tokenizer->readNextToken();
      parseWhitespace(value);

    } else
      return true;
  }
}

bool CssParser::parseAny(TokenList& tokens) {
  switch (tokenizer->getTokenType()) {
    case Token::NUMBER:
    case Token::PERCENTAGE:
    case Token::DIMENSION:
    case Token::STRING:
    case Token::URL:
    case Token::HASH:
    case Token::UNICODE_RANGE:
    case Token::INCLUDES:
    case Token::DASHMATCH:
    case Token::COLON:
    case Token::OTHER:
      tokens.push_back(tokenizer->getToken());
      tokenizer->readNextToken();
      break;

    case Token::PAREN_OPEN:
      tokens.push_back(tokenizer->getToken());
      tokenizer->readNextToken();
      skipWhitespace();

      while (parseAny(tokens) || parseUnused(tokens)) {
      }
      if (tokenizer->getTokenType() != Token::PAREN_CLOSED) {
        throw ParseException(tokenizer->getToken(),
                             "closing parenthesis (')')");
      }
      tokens.push_back(tokenizer->getToken());
      tokenizer->readNextToken();
      break;

    case Token::IDENTIFIER:
      tokens.push_back(tokenizer->getToken());
      tokenizer->readNextToken();
      break;

    case Token::BRACE_OPEN:
      tokens.push_back(tokenizer->getToken());
      tokenizer->readNextToken();
      skipWhitespace();
      while (parseAny(tokens) || parseUnused(tokens)) {
      }
      if (tokenizer->getTokenType() != Token::BRACE_CLOSED) {
        throw ParseException(tokenizer->getToken(), "closing brace (']')");
      }
      tokens.push_back(tokenizer->getToken());
      tokenizer->readNextToken();
      break;

    default:
      return false;
  }
  parseWhitespace(tokens);
  return true;
}

bool CssParser::parseUnused(TokenList& tokens) {
  if (parseBlock(tokens)) {
  } else if (tokenizer->getTokenType() == Token::ATKEYWORD) {
    tokens.push_back(tokenizer->getToken());
    tokenizer->readNextToken();
    parseWhitespace(tokens);
  } else if (tokenizer->getTokenType() == Token::DELIMITER) {
    tokens.push_back(tokenizer->getToken());
    tokenizer->readNextToken();
    skipWhitespace();
  } else
    return false;
  return true;
}
