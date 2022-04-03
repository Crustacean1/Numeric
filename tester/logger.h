#ifndef LOGGER
#define LOGGER

#include <iostream>

class Logger {
  std::ostream &_infoStream;
  std::ostream &_errorStream;
  template <typename... Q> void log(std::ostream &stream, Q... data);

public:
  Logger(std::ostream &infoStream, std::ostream &errorStream)
      : _infoStream(infoStream), _errorStream(errorStream) {}
  template <typename... Q> void logInfo(Q... q);
  template <typename... Q> void logError(Q... q);
};

template <typename... Q> void Logger::log(std::ostream &stream, Q... q) {
  stream << "[LOGGER]: ";
  ((stream << std::forward<Q>(q) << "\t"), ...);
  stream << std::endl;
}
template <typename... Q> void Logger::logError(Q... q) {
  log(_errorStream, std::forward<Q>(q)...);
}
template <typename... Q> void Logger::logInfo(Q... q) {
  log(_infoStream, std::forward<Q>(q)...);
}

#endif /*LOGGER*/
