#include "less/value/StringValue.h"
#include <regex>
#include "less/value/BooleanValue.h"

using namespace std;

StringValue::StringValue(const Token& token, bool quotes) {
  type = Value::STRING;

  tokens.push_back(token);
  this->quotes = quotes;
  setString(token);
}

StringValue::StringValue(const string& str, bool quotes) {
  Token token(str, Token::STRING, 0, 0, "generated");

  type = Value::STRING;
  tokens.push_back(token);
  this->quotes = quotes;
  setString(token);
}

StringValue::StringValue(const StringValue& s) {
  Token token(s.getString(), Token::STRING, 0, 0, "generated");

  type = Value::STRING;
  tokens.push_back(token);
  this->quotes = s.getQuotes();
  setString(token);
}

StringValue::StringValue(const Value& val, bool quotes) {
  const StringValue* sval;
  Token token = val.getTokens()->front();
  token.type = Token::STRING;

  if (val.type == STRING) {
    sval = dynamic_cast<const StringValue*>(&val);
    token = sval->getString();
  } else {
    token = val.getTokens()->toString();
  }

  type = Value::STRING;
  tokens.push_back(token);
  this->quotes = quotes;
  setString(token);
}

void StringValue::updateTokens() {
  string::iterator i;
  string newstr;

  if (quotes) {
    // add quotes
    newstr.push_back('"');
    for (i = strvalue.begin(); i != strvalue.end(); i++) {
      if (*i == '"')
        newstr.push_back('\\');
      newstr.push_back(*i);
    }
    newstr.push_back('"');
    tokens.front() = newstr;
  } else
    tokens.front() = strvalue;
}

string StringValue::getString() const {
  return strvalue;
}
void StringValue::setString(const string& newValue) {
  this->strvalue = newValue;
  updateTokens();
}

void StringValue::setQuotes(bool quotes) {
  this->quotes = quotes;
  updateTokens();
}
bool StringValue::getQuotes() const {
  return quotes;
}

void StringValue::append(const Value& v) {
  const StringValue* s;

  if (v.type == STRING) {
    s = dynamic_cast<const StringValue*>(&v);
    strvalue.append(s->getString());
  } else {
    strvalue.append(v.getTokens()->toString());
  }

  updateTokens();
}

Value* StringValue::add(const Value& v) const {
  auto* sv = new StringValue(*this);
  sv->append(v);
  return sv;
}

Value* StringValue::substract(const Value& v) const {
  (void)v;
  throw ValueException("Can't substract from strings.", *this->getTokens());
}
Value* StringValue::multiply(const Value& v) const {
  string newstr;
  double i;
  const NumberValue* n;

  if (v.type != Value::NUMBER) {
    throw ValueException("Strings can only be multiplied by a number.",
                         *this->getTokens());
  }

  n = dynamic_cast<const NumberValue*>(&v);

  for (i = 0; i < n->getValue(); i++) {
    newstr.append(getString());
  }
  return new StringValue(newstr, getQuotes());
}

Value* StringValue::divide(const Value& v) const {
  (void)v;
  throw ValueException("Can't divide strings.", *this->getTokens());
}

BooleanValue* StringValue::equals(const Value& v) const {
  const StringValue* s;

  if (v.type == STRING) {
    s = dynamic_cast<const StringValue*>(&v);
    return new BooleanValue(getString() == s->getString());
  } else {
    throw ValueException("You can only compare a string with a *string*.",
                         *this->getTokens());
  }
}
BooleanValue* StringValue::lessThan(const Value& v) const {
  const StringValue* s;

  if (v.type == STRING) {
    s = dynamic_cast<const StringValue*>(&v);
    return new BooleanValue(getString() < s->getString());
  } else {
    throw ValueException("You can only compare a string with a *string*.",
                         *this->getTokens());
  }
}

string StringValue::escape(string rawstr, string extraUnreserved) {
  string unreservedChars(
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~");

  ostringstream newstr;
  unsigned int i;

  for (i = 0; i < rawstr.size(); i++) {
    if (unreservedChars.find(rawstr[i]) == string::npos &&
        extraUnreserved.find(rawstr[i]) == string::npos) {
      newstr << '%' << setfill('0') << setw(2) << hex << (int)rawstr[i];
    } else
      newstr << rawstr[i];
  }
  return newstr.str();
}

void StringValue::loadFunctions(FunctionLibrary& lib) {
  lib.push("escape", "S", &StringValue::escape);
  lib.push("e", "S", &StringValue::e);
  lib.push("%", "S.+", &StringValue::format);
  lib.push("replace", "SSSS?", &StringValue::replace);
  lib.push("color", "S", &StringValue::color);
  lib.push("data-uri", "SS?", &StringValue::data_uri);
}

Value* StringValue::escape(const vector<const Value*>& arguments) {
  string unreservedChars(
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~,/"
      "?@&+'!$");

  auto* s = new StringValue(*(const StringValue*)arguments[0]);
  s->setString(StringValue::escape(s->getString(), ",/?@&+'!$"));
  return s;
}

Value* StringValue::e(const vector<const Value*>& arguments) {
  auto* s = new StringValue(*(const StringValue*)arguments[0]);
  s->setQuotes(false);
  return s;
}

Value* StringValue::format(const vector<const Value*>& arguments) {
  string escapeChars("adsADS");

  auto* s = new StringValue(*(const StringValue*)arguments[0]);

  string oldstr = s->getString();
  ostringstream newstr;
  unsigned int i, argc = 1;
  string argStr;

  for (i = 0; i < oldstr.size(); i++) {
    if (oldstr[i] == '%') {
      i++;

      if (escapeChars.find(oldstr[i]) != string::npos) {
        if (argc == arguments.size())
          throw ValueException(
              "Format template expects more arguments than provided.",
              *arguments[0]->getTokens());

        if ((oldstr[i] == 's' || oldstr[i] == 'S') &&
            arguments[argc]->type == STRING) {
          argStr = ((const StringValue*)arguments[argc])->getString();
        } else
          argStr = arguments[argc]->getTokens()->toString();

        if (oldstr[i] == 'A' || oldstr[i] == 'D' || oldstr[i] == 'S') {
          argStr = StringValue::escape(argStr);
        }
        newstr << argStr;
        argc++;
      } else
        newstr << oldstr[i];
    } else
      newstr << oldstr[i];
  }

  if (argc != arguments.size()) {
    throw ValueException(
        "Format template does not supply placeholders for all given arguments.",
        *arguments[0]->getTokens());
  }

  s->setString(newstr.str());
  return s;
}

Value* StringValue::replace(const vector<const Value*>& arguments) {
  string out;
  regex regex;
  regex_constants::syntax_option_type regex_flags = regex_constants::ECMAScript;
  regex_constants::match_flag_type match_flags =
      regex_constants::match_default | regex_constants::format_first_only;

  const auto* in = (const StringValue*)arguments[0];
  const auto* pattern = (const StringValue*)arguments[1];
  const auto* replacement = (const StringValue*)arguments[2];
  string options;

  if (arguments.size() > 3) {
    options = ((const StringValue*)arguments[3])->getString();

    if (options.find('i') != string::npos)
      regex_flags |= regex::icase;
    if (options.find('g') != string::npos)
      match_flags &= ~regex_constants::format_first_only;
  }

  regex = std::regex(pattern->getString(), regex_flags);
  out = regex_replace(
      in->getString(), regex, replacement->getString(), match_flags);
  return new StringValue(out, in->getQuotes());
}

Value* StringValue::color(const vector<const Value*>& arguments) {
  const StringValue* s;
  Token t;

  s = dynamic_cast<const StringValue*>(arguments[0]);

  t = Token(s->getString(), Token::HASH, 0, 0, "generated");
  return new Color(t);
}
Value* StringValue::data_uri(const vector<const Value*>& arguments) {
  return new StringValue(*(const StringValue*)arguments[0]);
}
