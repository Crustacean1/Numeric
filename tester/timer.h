#ifndef TIMER
#define TIMER

class Timer{
    std::chrono::time_point<std::chrono::system_clock> _prevPoint;
    int a;
    public:
    void start();
    double read();
};

void Timer::start(){
    _prevPoint = std::chrono::system_clock::now();
}

double Timer::read(){
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now-_prevPoint).count();
}

#endif /*TIMER*/