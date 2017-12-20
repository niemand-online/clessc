#include "less/LogStream.h"

using namespace std;

#ifdef WITH_LIBGLOG

#include <glog/logging.h>

ostream& LogStream::notice() {
  return LOG(INFO);
}

ostream& LogStream::notice(int verbosityLevel) {
  return VLOG(verbosityLevel);
}

ostream& LogStream::warning() {
  return LOG(WARNING);
}

ostream& LogStream::error() {
  return LOG(ERROR);
}

#else  // WITH_LIBGLOG

#include <iostream>

ostream& LogStream::notice() {
  return clog << '\n';
}

ostream& LogStream::notice(int verbosityLevel) {
  return notice() << "VERBOSE " << verbosityLevel << ": ";
}

ostream& LogStream::warning() {
  return cerr << '\n';
}

ostream& LogStream::error() {
  return cerr << '\n';
}

#endif  // WITH_LIBGLOG
