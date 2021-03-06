#ifndef __less_stylesheet_Ruleset_h__
#define __less_stylesheet_Ruleset_h__

#include "less/stylesheet/CssComment.h"
#include "less/stylesheet/Selector.h"
#include "less/stylesheet/Stylesheet.h"
#include "less/stylesheet/StylesheetStatement.h"

#include "less/Token.h"
#include "less/TokenList.h"

#include "less/css/CssWriter.h"

class RulesetStatement;
class Declaration;

class Ruleset : public StylesheetStatement {
private:
  std::list<RulesetStatement *> statements;
  std::list<Declaration *> declarations;

protected:
  Selector selector;
  virtual void addStatement(RulesetStatement &statement);
  virtual void deleteStatement(RulesetStatement &statement);

public:
  Ruleset() = default;
  explicit Ruleset(const Selector &selector);
  ~Ruleset() override;
  virtual void setSelector(const Selector &selector);

  Declaration *createDeclaration();
  Declaration *createDeclaration(const Token &property);
  CssComment *createComment();

  void deleteDeclaration(Declaration &declaration);

  void addDeclarations(std::list<Declaration> &declarations);

  Selector &getSelector();
  const Selector &getSelector() const;
  const std::list<RulesetStatement *> &getStatements() const;
  std::list<Declaration *> &getDeclarations();

  void clearStatements();

  virtual void processStatements(Ruleset &target) const;
  void process(Stylesheet &s) override;
  void write(CssWriter &writer) override;
};

#endif  // __less_stylesheet_Ruleset_h__
