#include "less/lessstylesheet/LessMediaQuery.h"
#include "less/stylesheet/MediaQuery.h"

using namespace std;

LessMediaQuery::LessMediaQuery() {
  parent = nullptr;
}
LessMediaQuery::~LessMediaQuery() {
}

Selector *LessMediaQuery::getSelector() {
  return &selector;
}
void LessMediaQuery::setSelector(const Selector &s) {
  selector = s;
}

void LessMediaQuery::setLessStylesheet(LessStylesheet &parent) {
  this->parent = &parent;
}
LessStylesheet *LessMediaQuery::getLessStylesheet() const {
  return parent;
}

void LessMediaQuery::getFunctions(list<const Function *> &functionList,
                                  const Mixin &mixin) const {
  LessStylesheet::getFunctions(functionList, mixin);
  getLessStylesheet()->getFunctions(functionList, mixin);
}

const TokenList *LessMediaQuery::getVariable(const string &key) const {
  const TokenList *t = LessStylesheet::getVariable(key);
  if (t == nullptr)
    t = getLessStylesheet()->getVariable(key);
  return t;
}

ProcessingContext *LessMediaQuery::getContext() {
  return getLessStylesheet()->getContext();
}
void LessMediaQuery::process(Stylesheet &s) {
  MediaQuery *query = s.createMediaQuery();

  query->setSelector(*getSelector());
  getContext()->processValue(query->getSelector());

  LessStylesheet::process(*query, *parent->getContext());
  getContext()->setLessStylesheet(*parent);
}

void LessMediaQuery::write(CssWriter &writer) {
  LessStylesheet::write(writer);
}
