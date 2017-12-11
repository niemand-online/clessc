#include "less/lessstylesheet/LessStylesheet.h"
#include "less/LogStream.h"
#include "less/lessstylesheet/LessMediaQuery.h"

using namespace std;

LessStylesheet::LessStylesheet() {
}

LessStylesheet::~LessStylesheet() {
}

LessRuleset* LessStylesheet::createLessRuleset() {
  auto* r = new LessRuleset();

  LogStream().notice(3) << "Creating LessRuleset";

  addRuleset(*r);
  lessrulesets.push_back(r);
  r->setLessStylesheet(*this);
  return r;
}

Mixin* LessStylesheet::createMixin() {
  auto* m = new Mixin();

  LogStream().notice(3) << "Creating mixin";

  addStatement(*m);
  m->setLessStylesheet(*this);
  return m;
}

LessAtRule* LessStylesheet::createLessAtRule(const Token& keyword) {
  auto* atrule = new LessAtRule(keyword);
  addAtRule(*atrule);
  atrule->setLessStylesheet(*this);
  return atrule;
}

LessMediaQuery* LessStylesheet::createLessMediaQuery() {
  auto* q = new LessMediaQuery();

  LogStream().notice(3) << "Adding Media Query";

  addStatement(*q);
  q->setLessStylesheet(*this);
  return q;
}

void LessStylesheet::deleteLessRuleset(LessRuleset& ruleset) {
  lessrulesets.remove(&ruleset);
  deleteStatement(ruleset);
}

void LessStylesheet::deleteMixin(Mixin& mixin) {
  deleteStatement(mixin);
}

void LessStylesheet::getFunctions(list<const Function*>& rulesetList,
                                  const Mixin& mixin) const {
  list<LessRuleset*>::const_iterator i;

  for (i = lessrulesets.begin(); i != lessrulesets.end(); i++) {
    (*i)->getFunctions(rulesetList, mixin, mixin.name.begin());
  }
}

void LessStylesheet::setContext(ProcessingContext* context) {
  this->context = context;
}
ProcessingContext* LessStylesheet::getContext() {
  return context;
}

void LessStylesheet::putVariable(const string& key, const TokenList& value) {
  variables[key] = value;
}
const TokenList* LessStylesheet::getVariable(const string& key) const {
  return variables.getVariable(key);
}

void LessStylesheet::process(Stylesheet& s, ProcessingContext& context) {
  list<Extension>* extensions;

  list<Ruleset*>::iterator r_it;
  list<Extension>::iterator e_it;
  list<Closure*> closureScope;

  this->context = &context;

  context.setLessStylesheet(*this);
  Stylesheet::process(s);

  saveReturnValues(context);

  // post processing
  extensions = &context.getExtensions();

  for (r_it = s.getRulesets().begin(); r_it != s.getRulesets().end(); r_it++) {
    for (e_it = extensions->begin(); e_it != extensions->end(); e_it++) {
      (*e_it).updateSelector((*r_it)->getSelector());
    }
  }
}

void LessStylesheet::saveReturnValues(ProcessingContext& context) {
  // move closures from context to this->closures
  context.saveClosures(this->closures);

  // move variables from context to this->variables
  context.saveVariables(this->variables);
}
