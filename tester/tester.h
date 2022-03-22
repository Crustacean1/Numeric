#ifndef TESTER
#define TESTER

#include <functional>
#include <chrono>
#include <map>
#include <random>

#include "./normalizedtest.h"
#include "timer.h"

template<typename T>
class Tester{
    std::map<std::string,NormalizedTest<T>> _tests;
    std::default_random_engine _engine;

    Timer _timer;

    template<typename... Q>
    using TestFunc = bool(*)(Q...);

    public:

    Tester();
    template<typename... Q>

    void addTest(const std::string & name,TestFunc<Q...> func);
    size_t getArgumentCount(const std::string & name);
    double assert(const std::string & name,std::vector<T> & arguments,bool expected);
};

template<typename T>
Tester<T>::Tester() : _engine(2137)
{}

template<typename T>
template<typename... Q>
void Tester<T>::addTest(const std::string & name,TestFunc<Q...> func)
{
    constexpr size_t paramCount = sizeof...(Q);

    auto testFunc = [func,paramCount](std::vector<T> params) -> bool{
        return executeFunction<paramCount>(func,params);
    };
    _tests.emplace(name,NormalizedTest<T>(testFunc,paramCount));
}

template<typename T>
double Tester<T>::assert(const std::string & testName,std::vector<T> & arguments, bool expected){
    if(_tests.find(testName) == _tests.end()){
        throw std::runtime_error("Invalid test name: "+testName);
    }
    _timer.start();
    bool passed = _tests[testName].execute(arguments);
    auto time = _timer.read();
    return passed  == expected ? time : -1;
}

template<typename T>
size_t Tester<T>::getArgumentCount(const std::string & testName){
    auto test = _tests.find(testName);
    if(test == _tests.end()){
        throw std::runtime_error("Invalid test name: "+testName);
    }
    return test->second.getArgumentCount();
}

#endif /*TESTER*/
