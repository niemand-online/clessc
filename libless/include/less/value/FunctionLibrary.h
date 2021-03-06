#ifndef __less_value_FunctionLibrary_h__
#define __less_value_FunctionLibrary_h__

#include <cstring>
#include <map>
#include <vector>
#include "less/value/Value.h"

typedef struct FuncInfo {
  const char* parameterTypes;
  Value* (*func)(const std::vector<const Value*>& arguments);
} FuncInfo;

class FunctionLibrary {
private:
  std::map<std::string, FuncInfo*> map;

public:
  const FuncInfo* getFunction(const char* functionName) const;

  void push(std::string name,
            const char* parameterTypes,
            Value* (*func)(const std::vector<const Value*>& arguments));

  bool checkArguments(const FuncInfo* fi,
                      const std::vector<const Value*>& arguments) const;
  const char* functionDefToString(const char* functionName,
                                  const FuncInfo* fi = nullptr);
};

#endif  // __less_value_FunctionLibrary_h__
