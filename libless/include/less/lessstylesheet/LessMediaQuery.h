#ifndef __less_lessstylesheet_LessMediaQuery_h__
#define __less_lessstylesheet_LessMediaQuery_h__

#include <list>

#include "less/stylesheet/Selector.h"
#include "less/stylesheet/Stylesheet.h"
#include "less/stylesheet/StylesheetStatement.h"

#include "less/css/CssWriter.h"
#include "less/lessstylesheet/Function.h"
#include "less/lessstylesheet/LessRuleset.h"
#include "less/lessstylesheet/LessStylesheet.h"
#include "less/lessstylesheet/Mixin.h"
#include "less/lessstylesheet/ProcessingContext.h"

class LessMediaQuery : public LessStylesheet, public StylesheetStatement {
private:
  Selector selector;
  LessStylesheet *parent;

public:
  LessMediaQuery();
  ~LessMediaQuery() override;

  Selector *getSelector();
  void setSelector(const Selector &s);

  virtual void setLessStylesheet(LessStylesheet &parent);
  LessStylesheet *getLessStylesheet() const;

  void getFunctions(std::list<const Function *> &functionList,
                    const Mixin &mixin) const override;
  const TokenList *getVariable(const std::string &key) const override;

  ProcessingContext *getContext() override;
  void process(Stylesheet &s) override;
  void write(CssWriter &writer) override;
};

#endif  // __less_lessstylesheet_LessMediaQuery_h__
