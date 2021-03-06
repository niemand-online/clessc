#include "less/VariableMap.h"

using namespace std;

const TokenList *VariableMap::getVariable(const string &key) const {
  VariableMap::const_iterator mit;

  if ((mit = this->find(key)) != this->end()) {
    return &mit->second;
  }

  return nullptr;
}

void VariableMap::merge(const VariableMap &map) {
  this->insert(map.begin(), map.end());
}

void VariableMap::overwrite(const VariableMap &map) {
  VariableMap::const_iterator it;

  for (it = map.begin(); it != map.end(); ++it) {
    (*this)[it->first] = it->second;
  }
}

string VariableMap::toString() const {
  string str;
  VariableMap::const_iterator it;

  for (it = this->begin(); it != this->end(); ++it) {
    str.append(it->first);
    str.append(": ");
    str.append(it->second.toString());
    str.append("\n");
  }
  return str;
}
