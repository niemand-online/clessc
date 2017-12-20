#ifndef __less_css_CssPrettyWriter_h__
#define __less_css_CssPrettyWriter_h__

#include <iostream>
#include "less//Token.h"
#include "less/TokenList.h"
#include "less/css/CssWriter.h"

class CssPrettyWriter : public CssWriter {
private:
  int indent_size;

protected:
  void indent();
  void newline();

  void writeSelector(const TokenList &selector) override;

public:
  explicit CssPrettyWriter(std::ostream &out) : CssWriter(out) {
    indent_size = 0;
  };
  CssPrettyWriter(std::ostream &out, SourceMapWriter &sourcemap)
      : CssWriter(out, sourcemap) {
    indent_size = 0;
  }

  void writeAtRule(const Token &keyword, const TokenList &rule) override;
  void writeRulesetStart(const TokenList &selector) override;
  void writeRulesetEnd() override;
  void writeDeclaration(const Token &property, const TokenList &value) override;
  void writeDeclarationDeliminator() override;
  void writeComment(const Token &comment) override;
  void writeMediaQueryStart(const TokenList &selector) override;
  void writeMediaQueryEnd() override;
};

#endif  // __less_css_CssPrettyWriter_h__
