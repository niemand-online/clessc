#ifndef __less_value_BooleanValue_h__
#define __less_value_BooleanValue_h__

#include "less/value/StringValue.h"
#include "less/value/Value.h"
#include "less/value/ValueException.h"

/**
 * True or false.
 */
class BooleanValue : public Value {
private:
  bool value;

public:
  explicit BooleanValue(bool value);
  BooleanValue(const Token &t, bool value);
  ~BooleanValue() override = default;

  bool getValue() const;
  void setValue(bool value);

  Value *add(const Value &v) const override;
  Value *substract(const Value &v) const override;
  Value *multiply(const Value &v) const override;
  Value *divide(const Value &v) const override;

  BooleanValue *equals(const Value &v) const override;
  BooleanValue *lessThan(const Value &v) const override;
};

#endif  // __less_value_BooleanValue_h__
