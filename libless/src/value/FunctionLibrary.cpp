#include "less/value/FunctionLibrary.h"

using namespace std;

const FuncInfo* FunctionLibrary::getFunction(const char* functionName) const {
  auto i = map.find(functionName);

  if (i != map.end())
    return i->second;
  else
    return nullptr;
}

void FunctionLibrary::push(
    string name,
    const char* parameterTypes,
    Value* (*func)(const vector<const Value*>& arguments)) {
  auto* fi = new FuncInfo();
  fi->parameterTypes = parameterTypes;
  fi->func = func;
  map[name] = fi;
}

bool FunctionLibrary::checkArguments(
    const FuncInfo* fi, const vector<const Value*>& arguments) const {
  const char* types = fi->parameterTypes;
  auto it = arguments.begin();
  size_t len = strlen(types);

  for (size_t i = 0; i < len; i++) {
    if (it == arguments.end()) {
      if (i + 1 < len && (types[i + 1] == '?' || types[i + 1] == '+')) {
        i++;
        continue;
      } else
        return false;
    }

    if (types[i] != '.' && (*it)->type != Value::codeToType(types[i]))
      return false;

    it++;

    if (i + 1 < len) {
      if (types[i + 1] == '+') {
        while (
            it != arguments.end() &&
            (types[i] == '.' || (*it)->type == Value::codeToType(types[i]))) {
          it++;
        }
        i++;
      } else if (types[i + 1] == '?') {
        i++;
      }
    }
  }

  return it == arguments.end();
}

const char* FunctionLibrary::functionDefToString(const char* functionName,
                                                 const FuncInfo* fi) {
  if (fi == nullptr)
    fi = getFunction(functionName);
  if (fi == nullptr)
    return "";

  string str(functionName);
  const char* types = fi->parameterTypes;
  size_t len = strlen(types);
  char* retstr;

  str.append("(");
  for (size_t i = 0; i < len; i++) {
    if (types[i] == '.')
      str.append("Any");
    else
      str.append(Value::typeToString(Value::codeToType(types[i])));

    if (i + 1 < len) {
      if (types[i + 1] == '?') {
        str.append(" (optional)");
        i++;
      } else if (types[i + 1] == '+') {
        str.append("...");
        i++;
      }
    }

    if (i != len - 1)
      str.append(", ");
  }
  str.append(")");

  retstr = new char[str.length() + 1];
  strcpy(retstr, str.c_str());

  return retstr;
}
