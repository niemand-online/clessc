/*
 * Copyright 2012 Bram van der Kroef
 *
 * This file is part of LESS CSS Compiler.
 *
 * LESS CSS Compiler is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LESS CSS Compiler is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LESS CSS Compiler.  If not, see <http://www.gnu.org/licenses/>. 
 *
 * Author: Bram van der Kroef <bram@vanderkroef.net>
 */

#ifndef __String_h__
#define __String_h__

#include "Value.h"

class String: public Value {
private:
  const char* str;
  bool quotes;

public:
  String(Token* token);
  String(Token* token, bool quotes);
  virtual ~String();
  
  virtual TokenList* getTokens();

  void setQuotes(bool quotes);
  bool getQuotes();
  
  void add(Value* v);
  void substract(Value* v);
  void multiply(Value* v);
  void divide(Value* v);

  void escape();
  void e();
  void format(vector<Value*> args);
  Color* color();
  void data_uri();
  
}
