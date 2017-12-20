#include "less/lessstylesheet/MixinCall.h"
#include "less/lessstylesheet/Function.h"
#include "less/lessstylesheet/Mixin.h"

using namespace std;

MixinCall::MixinCall(MixinCall* parent,
                     const Function& function,
                     bool savepoint) {
  this->parent = parent;
  this->function = &function;
  this->savepoint = savepoint;
}

const TokenList* MixinCall::getVariable(const string& key) const {
  VariableMap::const_iterator mit;
  const TokenList* t;

  if ((t = function->getVariable(key)) != nullptr)
    return t;

  if ((t = arguments.getVariable(key)) != nullptr)
    return t;

  if ((t = function->getInheritedVariable(key, *this)) != nullptr)
    return t;

  if (parent != nullptr)
    return parent->getVariable(key);
  return nullptr;
}

void MixinCall::getFunctions(list<const Function*>& functionList,
                             const Mixin& mixin) const {
  function->getLocalFunctions(functionList, mixin);
  if (!functionList.empty())
    return;

  if (parent != nullptr)
    parent->getFunctions(functionList, mixin);
}

bool MixinCall::isInStack(const Function& function) const {
  return (this->function == &function) ||
         (parent != nullptr && parent->isInStack(function));
}

const VariableMap* MixinCall::getArguments(const Function& function) const {
  if (this->function == &function)
    return &arguments;

  if (parent != nullptr)
    return parent->getArguments(function);

  return nullptr;
}
