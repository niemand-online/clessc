#ifndef __less_lessstylesheet_Mixin_h__
#define __less_lessstylesheet_Mixin_h__

#include <map>
#include <string>
#include <vector>

#include "less/stylesheet/Ruleset.h"
#include "less/stylesheet/Selector.h"
#include "less/stylesheet/Stylesheet.h"
#include "less/stylesheet/StylesheetStatement.h"

#include "less/css/CssWriter.h"

#include "less/TokenList.h"

#include "less/lessstylesheet/ProcessingContext.h"

class LessStylesheet;
class LessRuleset;
class ProcessingContext;

class Mixin : public StylesheetStatement {
private:
  std::vector<TokenList> arguments;
  std::map<std::string, TokenList> namedArguments;

  LessStylesheet *lessStylesheet;
  void parseArguments(TokenList::const_iterator i, const Selector &s);

public:
  Selector name;

  Mixin() = default;
  explicit Mixin(const Selector &name);
  ~Mixin() override = default;

  const TokenList *getArgument(const std::size_t i) const;
  std::size_t getArgumentCount() const;

  const TokenList *getArgument(const std::string &name) const;

  bool call(Stylesheet &s,
            ProcessingContext &context,
            Ruleset *ruleset,
            LessRuleset *parent);
  bool parse(const Selector &selector);

  virtual void setLessStylesheet(LessStylesheet &stylesheet);
  LessStylesheet *getLessStylesheet();

  void process(Stylesheet &s) override;
  void write(CssWriter &writer) override{};
};

#endif  // __less_lessstylesheet_Mixin_h__
