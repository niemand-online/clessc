#ifndef __less_value_UrlValue_h__
#define __less_value_UrlValue_h__

#include <string>
#include "less/value/Color.h"
#include "less/value/Value.h"

class UrlValue_Img {
public:
  unsigned int width;
  unsigned int height;
  Color background;
};

class UrlValue : public Value {
private:
  std::string path;

  bool loadImg(UrlValue_Img &img) const;
  bool loadPng(UrlValue_Img &img) const;
  bool loadJpeg(UrlValue_Img &img) const;

public:
  UrlValue(Token &token, std::string &path);

  ~UrlValue() override = default;

  std::string getPath() const;

  std::string getRelativePath() const;

  Value *add(const Value &v) const override;
  Value *substract(const Value &v) const override;
  Value *multiply(const Value &v) const override;
  Value *divide(const Value &v) const override;

  BooleanValue *lessThan(const Value &v) const override;
  BooleanValue *equals(const Value &v) const override;

  unsigned int getImageWidth() const;
  unsigned int getImageHeight() const;
  Color getImageBackground() const;

  static void loadFunctions(FunctionLibrary &lib);

  static Value *imgheight(const std::vector<const Value *> &arguments);
  static Value *imgwidth(const std::vector<const Value *> &arguments);
  static Value *imgbackground(const std::vector<const Value *> &arguments);
};

#endif  // __less_value_UrlValue_h__
