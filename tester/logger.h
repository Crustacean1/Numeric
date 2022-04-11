#ifndef LOGGER
#define LOGGER

#include <iostream>

class Logger {
  std::ostream &_infoStream;
  std::ostream &_errorStream;
  template <typename... Q>
  void log(std::ostream &stream, unsigned char verbosity, Q... data);
  unsigned char _verbosity;

  std::string colorString(const std::string &str, unsigned char color);

public:
  Logger(std::ostream &infoStream, std::ostream &errorStream,
         unsigned char verbosity)
      : _infoStream(infoStream), _errorStream(errorStream),
        _verbosity(verbosity) {}

  template <typename... Q> void logInfo(unsigned char verbosity, Q... q);
  template <typename... Q> void logError(unsigned char verbosity, Q... q);
  template <typename... Q> void success(unsigned char verbosity, Q... q);
  template <typename... Q> void fail(unsigned char verbosity, Q... q);
};

template <typename... Q>
void Logger::log(std::ostream &stream, unsigned char verbosity, Q... q) {
  if (verbosity > _verbosity) {
    return;
  }
  ((stream << std::forward<Q>(q) << "\t"), ...);
  stream << std::endl;
}

template <typename... Q>
void Logger::logError(unsigned char verbosity, Q... q) {
  log(_errorStream, verbosity, "[ERROR]: ", std::forward<Q>(q)...);
}
template <typename... Q> void Logger::logInfo(unsigned char verbosity, Q... q) {
  log(_infoStream, verbosity, "[INFO]: ", std::forward<Q>(q)...);
}
template <typename... Q> void Logger::success(unsigned char verbosity, Q... q) {
  log(_infoStream, verbosity, colorString("passed",102),
      std::forward<Q>(q)...);
}
template <typename... Q> void Logger::fail(unsigned char verbosity, Q... q) {
  log(_infoStream, verbosity, colorString("fail", 101), std::forward<Q>(q)...);
}

#endif /*LOGGER*/
