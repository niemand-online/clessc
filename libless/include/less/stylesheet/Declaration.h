#ifndef __less_stylesheet_Declaration_h__
#define __less_stylesheet_Declaration_h__

#include "less/css/CssWriter.h"
#include "less/stylesheet/Ruleset.h"
#include "less/stylesheet/RulesetStatement.h"

#include "less/Token.h"
#include "less/TokenList.h"

class Declaration : public RulesetStatement {
private:
  Token property;
  TokenList value;

public:
  Declaration() = default;
  Declaration(const Token &property);
  ~Declaration() override = default;
  void setProperty(const Token &property);
  void setValue(const TokenList &value);

  Token &getProperty();
  TokenList &getValue();

  void process(Ruleset &r) override;
  void write(CssWriter &writer) override;
};

#endif  // __less_stylesheet_Declaration_h__
