#ifndef __less_value_Color_h__
#define __less_value_Color_h__

#include <algorithm>
#include <cmath>
#include "less/value/FunctionLibrary.h"
#include "less/value/NumberValue.h"
#include "less/value/StringValue.h"
#include "less/value/Value.h"

#define RGB_RED 0
#define RGB_GREEN 1
#define RGB_BLUE 2
//#define ABS(x) (x < 0 ? 0 - x : x)

class Color : public Value {
private:
  unsigned int color[3];
  double alpha;

  double maxArray(double* array, const size_t len) const;
  double minArray(double* array, const size_t len) const;

  void updateTokens();

public:
  Color();
  Color(Token& token);
  Color(unsigned int red, unsigned int green, unsigned int blue);
  Color(unsigned int red, unsigned int green, unsigned int blue, double alpha);
  Color(const Color& color);

  /**
   * The HSL to RGB conversion on
   * http://en.wikipedia.org/wiki/HSL_and_HSV did not work at all so
   * the calculations are from
   * http://130.113.54.154/~monger/hsl-rgb.html, which does not list a
   * source.
   */
  static Color* fromHSL(double hue, double saturation, double lightness);

  ~Color() override = default;

  Value* add(const Value& v) const override;
  Value* substract(const Value& v) const override;
  Value* multiply(const Value& v) const override;
  Value* divide(const Value& v) const override;

  BooleanValue* equals(const Value& v) const override;
  BooleanValue* lessThan(const Value& v) const override;

  /**
   * Converts the internal RGB value to HSL. The source of the
   * calculations is http://en.wikipedia.org/wiki/HSL_and_HSV except
   * for the saturation value, which did not work.
   */
  double* getHSL() const;

  /**
   * Change the color to a new rgb value.
   */
  void setRGB(unsigned int red, unsigned int green, unsigned int blue);

  /**
   * Returns the the amount of red in the color.
   *
   * @return an int value between 0-255
   */
  unsigned int getRed() const;
  /**
   * Returns the the amount of green in the color.
   *
   * @return an int value between 0-255
   */
  unsigned int getGreen() const;
  /**
   * Returns the the amount of blue in the color.
   *
   * @return an int value between 0-255
   */
  unsigned int getBlue() const;

  void setAlpha(double alpha);
  double getAlpha() const;

  static void loadFunctions(FunctionLibrary& lib);
  static Value* rgb(const std::vector<const Value*>& arguments);
  static Value* rgba(const std::vector<const Value*>& arguments);
  static Value* lighten(const std::vector<const Value*>& arguments);
  static Value* darken(const std::vector<const Value*>& arguments);
  static Value* saturate(const std::vector<const Value*>& arguments);
  static Value* desaturate(const std::vector<const Value*>& arguments);
  static Value* fadein(const std::vector<const Value*>& arguments);
  static Value* fadeout(const std::vector<const Value*>& arguments);
  static Value* spin(const std::vector<const Value*>& arguments);
  static Value* hsl(const std::vector<const Value*>& arguments);
  static Value* hue(const std::vector<const Value*>& arguments);
  static Value* saturation(const std::vector<const Value*>& arguments);
  static Value* lightness(const std::vector<const Value*>& arguments);
  static Value* argb(const std::vector<const Value*>& arguments);
  static Value* red(const std::vector<const Value*>& arguments);
  static Value* green(const std::vector<const Value*>& arguments);
  static Value* blue(const std::vector<const Value*>& arguments);
  static Value* _alpha(const std::vector<const Value*>& arguments);

  static Value* hsla(const std::vector<const Value*>& arguments);
  static Value* hsv(const std::vector<const Value*>& arguments);
  static Value* hsva(const std::vector<const Value*>& arguments);
  static Value* hsvhue(const std::vector<const Value*>& arguments);
  static Value* hsvsaturation(const std::vector<const Value*>& arguments);
  static Value* hsvvalue(const std::vector<const Value*>& arguments);
  static Value* luma(const std::vector<const Value*>& arguments);
  static Value* fade(const std::vector<const Value*>& arguments);
  static Value* mix(const std::vector<const Value*>& arguments);
  static Value* greyscale(const std::vector<const Value*>& arguments);
  static Value* contrast(const std::vector<const Value*>& arguments);
  static Value* screen(const std::vector<const Value*>& arguments);
  static Value* overlay(const std::vector<const Value*>& arguments);
  static Value* softlight(const std::vector<const Value*>& arguments);
  static Value* hardlight(const std::vector<const Value*>& arguments);
  static Value* difference(const std::vector<const Value*>& arguments);
  static Value* exclusion(const std::vector<const Value*>& arguments);
  static Value* avarage(const std::vector<const Value*>& arguments);
  static Value* negation(const std::vector<const Value*>& arguments);
};

#endif  // __less_value_Color_h__
