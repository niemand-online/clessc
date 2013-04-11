Number::Number(Token* token) {
  tokens.push(token);
  
  switch(token->type) {
  case Token::NUMBER:
    type = NUMBER;
    break;
  case Token::PERCENTAGE:
    type = PERCENTAGE;
    break;
  case Token::DIMENSION:
    type = DIMENSION;
    break;
  default:
    throw new ValueException("Token used for Value that isn't a number, percentage or dimension");
  }
}

Number::~Number() {
}

void Number::add(Value* v) {
  if (type == NUMBER) 
    setType(v);
  setValue(getValue() + v->getValue());
}
void Number::substract(Value* v) {
  if (type == NUMBER) 
    setType(v);
  setValue(getValue() - v->getValue());
}
void Number::multiply(Value* v) {
  if (type == NUMBER) 
    setType(v);
  setValue(getValue() * v->getValue());
}

void Number::divide(Value* v) {
  if (type == NUMBER) 
    setType(v);
  setValue(getValue() / v->getValue());
}

void Number::setType(Value* v) {
  type = v->type;
  if (v->type == DIMENSION)
    setUnit(v->getUnit());
  else if (v->type == PERCENTAGE) 
    tokens.front()->type = Token::PERCENTAGE;
  else if (v->type == NUMBER) {
    setUnit("");
    tokens.front()->type = Token::NUMBER;
  }
}

double Number::getValue() {
  string number;
  istringstream stm;
  double ret;
  char c;

  for (unsigned int i = 0; i < tokens.front()->str.size(); i++) {
    c = tokens.front()->str[i];
    if (!isdigit(c) && c != '.' && c != '-') {
      number = tokens.front()->str.substr(0, i);
      break;
    }
  }
  if (number == "")
    number = tokens.front()->str;
  stm.str(number);
  stm >>ret;
  return ret;
}
string Number::getUnit () {
  char c;
  unsigned int i;
  
  for (i = 0; i < tokens.front()->str.size(); i++) {
    c = tokens.front()->str[i];
    if (!isdigit(c) && c != '.' && c != '-')
      return tokens.front()->str.substr(i);
  }
  return string("");
}

void Number::setUnit(string unit) {
  ostringstream stm;
  stm << getValue();
  stm << unit;
  type = DIMENSION;
  tokens.front()->type = Token::DIMENSION;
  tokens.front()->str = stm.str();
}

void Number::setValue(double d) {
  ostringstream stm;
  stm << d;
  if (type == DIMENSION)
    stm << getUnit();
  else if (type == PERCENTAGE)
    stm << "%";
  tokens.front()->str = stm.str();
}

