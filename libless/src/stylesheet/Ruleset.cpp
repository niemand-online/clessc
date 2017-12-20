#include "less/stylesheet/Ruleset.h"
#include "less/LogStream.h"
#include "less/stylesheet/Declaration.h"

using namespace std;

Ruleset::Ruleset(const Selector& selector) {
  this->selector = selector;
}

Ruleset::~Ruleset() {
  clearStatements();
}

void Ruleset::setSelector(const Selector& selector) {
  this->selector = selector;
}
void Ruleset::addStatement(RulesetStatement& statement) {
  statements.push_back(&statement);
  statement.setRuleset(this);
}
Declaration* Ruleset::createDeclaration() {
  auto* d = new Declaration();
  declarations.push_back(d);
  addStatement(*d);
  return d;
}
Declaration* Ruleset::createDeclaration(const Token& property) {
  auto* d = new Declaration(property);
  declarations.push_back(d);
  addStatement(*d);
  return d;
}

CssComment* Ruleset::createComment() {
  auto* c = new CssComment();
  addStatement(*c);
  return c;
}

void Ruleset::deleteStatement(RulesetStatement& statement) {
  statements.remove(&statement);
  delete &statement;
}

void Ruleset::deleteDeclaration(Declaration& declaration) {
  declarations.remove(&declaration);
  deleteStatement(declaration);
}

void Ruleset::addDeclarations(list<Declaration>& declarations) {
  auto i = declarations.begin();
  for (; i != declarations.end(); i++) {
    this->declarations.push_back(&(*i));
    addStatement(*i);
  }
}

Selector& Ruleset::getSelector() {
  return selector;
}
const Selector& Ruleset::getSelector() const {
  return selector;
}
const list<RulesetStatement*>& Ruleset::getStatements() const {
  return statements;
}

list<Declaration*>& Ruleset::getDeclarations() {
  return declarations;
}

void Ruleset::clearStatements() {
  declarations.clear();
  while (!statements.empty()) {
    delete statements.back();
    statements.pop_back();
  }
}

void Ruleset::processStatements(Ruleset& target) const {
  list<RulesetStatement*> statements = getStatements();
  list<RulesetStatement*>::iterator i;
  for (i = statements.begin(); i != statements.end(); i++) {
    (*i)->process(target);
  }
}

void Ruleset::process(Stylesheet& s) {
  Ruleset* target = s.createRuleset();

  LogStream().notice(2) << "Processing Ruleset: " << getSelector().toString();

  target->setSelector(getSelector());
  processStatements(*target);
}

void Ruleset::write(CssWriter& writer) {
  list<RulesetStatement*> statements = getStatements();
  list<RulesetStatement*>::iterator i;

  if (getStatements().empty())
    return;

  writer.writeRulesetStart(getSelector());

  for (i = statements.begin(); i != statements.end(); i++) {
    if (i != statements.begin())
      writer.writeDeclarationDeliminator();

    (*i)->write(writer);
  }
  writer.writeRulesetEnd();
}
