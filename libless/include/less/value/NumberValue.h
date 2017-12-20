#ifndef __less_value_NumberValue_h__
#define __less_value_NumberValue_h__

#include <cmath>
#include <vector>
#include "less/value/StringValue.h"
#include "less/value/UnitValue.h"
#include "less/value/Value.h"

class FunctionLibrary;

class NumberValue : public Value {
  static bool isNumber(const Value &val);

  void verifyUnits(const NumberValue &n);
  double convert(const std::string &unit) const;

public:
  explicit NumberValue(const Token &token);
  explicit NumberValue(double value);
  NumberValue(double value, Token::Type type, const std::string *unit);
  NumberValue(const NumberValue &n);
  ~NumberValue() override = default;

  Value *add(const Value &v) const override;
  Value *substract(const Value &v) const override;
  Value *multiply(const Value &v) const override;
  Value *divide(const Value &v) const override;

  BooleanValue *equals(const Value &v) const override;
  BooleanValue *lessThan(const Value &v) const override;

  void setType(const NumberValue &n);

  std::string getUnit() const;
  void setUnit(std::string unit);
  double getValue() const;
  void setValue(double d);

  static void loadFunctions(FunctionLibrary &lib);
  static Value *unit(const std::vector<const Value *> &args);
  static Value *get_unit(const std::vector<const Value *> &args);
  static Value *ceil(const std::vector<const Value *> &args);
  static Value *floor(const std::vector<const Value *> &args);
  static Value *percentage(const std::vector<const Value *> &args);
  static Value *round(const std::vector<const Value *> &args);
  static Value *sqrt(const std::vector<const Value *> &args);
  static Value *abs(const std::vector<const Value *> &args);
  static Value *sin(const std::vector<const Value *> &args);
  static Value *asin(const std::vector<const Value *> &args);
  static Value *cos(const std::vector<const Value *> &args);
  static Value *acos(const std::vector<const Value *> &args);
  static Value *tan(const std::vector<const Value *> &args);
  static Value *atan(const std::vector<const Value *> &args);
  static Value *pi(const std::vector<const Value *> &args);
  static Value *pow(const std::vector<const Value *> &args);
  static Value *mod(const std::vector<const Value *> &args);
  static Value *convert(const std::vector<const Value *> &args);
};

#endif  // __less_value_NumberValue_h__
