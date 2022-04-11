#include "logger.h"

std::string Logger::colorString(const std::string &str, unsigned char color) {
  return "\x1B[30;" + std::to_string(color) + "m" + str + "\x1B[0m";
}
