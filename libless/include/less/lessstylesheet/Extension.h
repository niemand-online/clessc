#ifndef __less_lessstylesheet_Extension_h__
#define __less_lessstylesheet_Extension_h__

#include "less/stylesheet/Selector.h"

/**
 *
 */
class Extension {
private:
  Selector target;
  Selector extension;
  bool all;

public:
  Extension();
  explicit Extension(bool all);
  virtual ~Extension() = default;

  Selector& getTarget();
  Selector& getExtension();

  void setExtension(Selector& selector);

  void updateSelector(Selector& s) const;
  void replaceInSelector(Selector& s) const;
};

#endif  // __less_lessstylesheet_Extension_h__
