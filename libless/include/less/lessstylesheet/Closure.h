#ifndef __less_lessstylesheet_Closure_h__
#define __less_lessstylesheet_Closure_h__

#include <list>
#include "less/lessstylesheet/Function.h"
#include "less/lessstylesheet/Mixin.h"
#include "less/stylesheet/Ruleset.h"

class ProcessingContext;
class MixinCall;
class LessRuleset;

class Closure : public Function {
public:
  const LessRuleset *ruleset;
  const MixinCall *stack;

  Closure(const LessRuleset &ruleset, const MixinCall &stack);

  bool call(Mixin &mixin,
            Ruleset &target,
            ProcessingContext &context) const override;
  bool call(Mixin &mixin,
            Stylesheet &s,
            ProcessingContext &context) const override;
  void getFunctions(std::list<const Function *> &functionList,
                    const Mixin &mixin,
                    TokenList::const_iterator selector_offset) const override;
  void getLocalFunctions(std::list<const Function *> &functionList,
                         const Mixin &mixin) const override;

  LessSelector *getLessSelector() const override;

  const TokenList *getVariable(const std::string &key) const override;
  const TokenList *getInheritedVariable(const std::string &key,
                                        const MixinCall &stack) const override;

  bool isInStack(const LessRuleset &ruleset);
};

#endif  // __less_lessstylesheet_Closure_h__
