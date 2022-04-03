#ifndef TIMER
#define TIMER

#include <chrono>

class Timer {
  std::chrono::time_point<std::chrono::system_clock> _prevPoint;
  int a;

public:
  void start() { _prevPoint = std::chrono::system_clock::now(); }
  double read() {
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now -
                                                                 _prevPoint)
        .count();
  }
};

#endif /*TIMER*/
