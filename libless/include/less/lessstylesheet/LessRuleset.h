#ifndef __less_lessstylesheet_LessRuleset_h__
#define __less_lessstylesheet_LessRuleset_h__

#include <list>
#include <map>
#include <string>

#include "less/stylesheet/Ruleset.h"
#include "less/stylesheet/Selector.h"
#include "less/stylesheet/Stylesheet.h"

#include "less/css/ParseException.h"
#include "less/value/ValueProcessor.h"

#include "less/Token.h"
#include "less/TokenList.h"
#include "less/VariableMap.h"

#include "less/lessstylesheet/Function.h"
#include "less/lessstylesheet/LessSelector.h"
#include "less/lessstylesheet/Mixin.h"
#include "less/lessstylesheet/ProcessingContext.h"
#include "less/lessstylesheet/UnprocessedStatement.h"

class LessStylesheet;
class MediaQueryRuleset;
class Closure;

class LessRuleset : public Ruleset, Function {
protected:
  VariableMap variables;
  std::list<LessRuleset *> nestedRules;
  std::list<Closure *> closures;

  std::list<UnprocessedStatement *> unprocessedStatements;

  LessRuleset *parent;
  LessStylesheet *lessStylesheet;
  LessSelector *selector;

  ProcessingContext *context;

  void processVariables();
  void insertNestedRules(Stylesheet &s,
                         Selector *prefix,
                         ProcessingContext &context) const;

  void addClosures(ProcessingContext &context) const;

public:
  LessRuleset();
  explicit LessRuleset(const Selector &selector);
  ~LessRuleset() override;

  void setSelector(const Selector &selector) override;
  LessSelector *getLessSelector() const override;

  UnprocessedStatement *createUnprocessedStatement();
  LessRuleset *createNestedRule();
  MediaQueryRuleset *createMediaQuery();

  void deleteNestedRule(LessRuleset &ruleset);
  void deleteUnprocessedStatement(UnprocessedStatement &statement);

  const std::list<UnprocessedStatement *> &getUnprocessedStatements() const;
  const std::list<LessRuleset *> &getNestedRules() const;

  void putVariable(const std::string &key, const TokenList &value);
  VariableMap &getVariables();

  const TokenList *getVariable(const std::string &key) const override;
  const TokenList *getInheritedVariable(const std::string &key,
                                        const MixinCall &stack) const override;

  const std::list<Closure *> &getClosures() const;

  void setParent(LessRuleset *r);
  LessRuleset *getParent() const;

  void setLessStylesheet(LessStylesheet &stylesheet);
  LessStylesheet *getLessStylesheet() const;

  ProcessingContext *getContext();

  void processExtensions(ProcessingContext &context, Selector *prefix);

  bool call(Mixin &mixin,
            Ruleset &target,
            ProcessingContext &context) const override;
  bool call(Mixin &mixin,
            Stylesheet &s,
            ProcessingContext &context) const override;

  virtual void processStatements(Ruleset &target,
                                 ProcessingContext &context) const;
  void processStatements(Stylesheet &target, ProcessingContext &context) const;
  void process(Stylesheet &s) override;
  virtual void process(Stylesheet &s,
                       Selector *prefix,
                       ProcessingContext &context);

  void getFunctions(std::list<const Function *> &functionList,
                    const Mixin &mixin,
                    TokenList::const_iterator selector_offset) const override;

  void saveReturnValues(ProcessingContext &context);
  /**
   * Look for a ruleset inside this ruleset and scope up to
   * getParent(), or getLessStylesheet() if getParent() is nullptr.
   */
  void getLocalFunctions(std::list<const Function *> &functionList,
                         const Mixin &mixin) const override;
  void getLocalFunctions(std::list<const Function *> &functionList,
                         const Mixin &mixin,
                         const LessRuleset *exclude = nullptr) const;

  bool matchConditions(ProcessingContext &context) const;
  bool putArguments(const Mixin &mixin, VariableMap &scope) const;
};

#endif  // __less_lessstylesheet_LessRuleset_h__
