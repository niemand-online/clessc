#include "less/value/UnitValue.h"
#include "less/value/BooleanValue.h"

using namespace std;

UnitValue::UnitValue(Token &token) {
  tokens.push_back(token);
  type = UNIT;
}

const char *UnitValue::getUnit() const {
  return tokens.front().c_str();
}

Value *UnitValue::add(const Value &v) const {
  Token t;
  const StringValue *s;
  StringValue *ret;

  if (v.type == STRING) {
    s = dynamic_cast<const StringValue *>(&v);
    t = this->tokens.front();
    t.type = Token::STRING;
    ret = new StringValue(t, s->getQuotes());
    ret->add(v);
    return ret;
  }
  throw ValueException("Can't do math on unit types.", *this->getTokens());
}
Value *UnitValue::substract(const Value &v) const {
  (void)v;
  throw ValueException("Can't do math on unit types.", *this->getTokens());
}
Value *UnitValue::multiply(const Value &v) const {
  (void)v;
  throw ValueException("Can't do math on unit types.", *this->getTokens());
}
Value *UnitValue::divide(const Value &v) const {
  (void)v;
  throw ValueException("Can't do math on unit types.", *this->getTokens());
}

BooleanValue *UnitValue::lessThan(const Value &v) const {
  const UnitValue *u;

  if (v.type == UNIT) {
    u = dynamic_cast<const UnitValue *>(&v);
    return new BooleanValue(getUnit() < u->getUnit());
  } else {
    throw ValueException("You can only compare a unit with a *unit*.",
                         *this->getTokens());
  }
}
BooleanValue *UnitValue::equals(const Value &v) const {
  const UnitValue *u;

  if (v.type == UNIT) {
    u = dynamic_cast<const UnitValue *>(&v);
    return new BooleanValue(getUnit() == u->getUnit());
  } else {
    throw ValueException("You can only compare a unit with a *unit*.",
                         *this->getTokens());
  }
}

UnitValue::UnitGroup UnitValue::getUnitGroup(const string &unit) {
  if (unit == "m" || unit == "cm" || unit == "mm" || unit == "in" ||
      unit == "pt" || unit == "pc" || unit == "px") {
    return LENGTH;
  }
  if (unit == "s" || unit == "ms") {
    return TIME;
  }
  if (unit == "rad" || unit == "deg" || unit == "grad" || unit == "turn") {
    return ANGLE;
  }
  return NO_GROUP;
}

double UnitValue::lengthToPx(const double length, const string &unit) {
  if (unit == "m")
    return length * (96 / .0254);

  if (unit == "cm")
    return length * (96 / 2.54);

  if (unit == "mm")
    return length * (96 / 25.4);

  if (unit == "in")
    return length * 96;

  if (unit == "pt")
    return length * (4 / 3);

  if (unit == "pc")
    return length * 16;

  if (unit == "px")
    return length;

  return -1;
}
double UnitValue::pxToLength(const double px, const string &unit) {
  if (unit == "m")
    return px / (96 / .254);

  if (unit == "cm")
    return px / (96 / 2.54);

  if (unit == "mm")
    return px / (96 / 25.4);

  if (unit == "in")
    return px / 96;

  if (unit == "pt")
    return px / (4 / 3);

  if (unit == "pc")
    return px / 16;

  if (unit == "px")
    return px;

  return -1;
}
double UnitValue::timeToMs(const double time, const string &unit) {
  if (unit == "s")
    return time * 1000;
  if (unit == "ms")
    return time;
  return -1;
}
double UnitValue::msToTime(const double ms, const string &unit) {
  if (unit == "s")
    return ms / 1000;
  if (unit == "ms")
    return ms;
  return -1;
}
double UnitValue::angleToRad(const double angle, const string &unit) {
  const double pi = 3.141592653589793;

  if (unit == "rad")
    return angle;

  if (unit == "deg")
    return angle / 180 * pi;

  if (unit == "grad")
    return angle / 200 * pi;

  if (unit == "turn")
    return angle * 2 * pi;

  return -1;
}
double UnitValue::radToAngle(const double rad, const string &unit) {
  const double pi = 3.141592653589793;

  if (unit == "rad")
    return rad;

  if (unit == "deg")
    return rad / pi * 180;

  if (unit == "grad")
    return rad / pi * 200;

  if (unit == "turn")
    return rad / (2 * pi);

  return -1;
}
