#ifndef TESTPARSER
#define TESTPARSER

#include <iostream>
#include <random>
#include "tester.h"
#include "logger.h"

template<typename T>
class TestParser{
    std::default_random_engine _engine;
    std::string _currentTest;
    std::vector<T> _arguments;

    Tester<T> & _tester;
    Logger _logger;

    template<typename... Q>
    void logInfo(Q... q);

    template<typename... Q>
    void logError(Q... q);

    void parseCommand(const std::string & buffer,std::istream & stream);
    void startNewTest(std::istream & stream);
    void executeRandom(std::istream & stream);
    void executeLiteral(std::istream & stream);

    void generateRandomArguments(std::vector<T> & args,size_t count, size_t size);

    void assess(float result,std::vector<T> & arguments);
    public:

    TestParser(Tester<T> & tester,Logger logger);
    void executeTests(std::istream & stream);
};

template<typename T>
TestParser<T>::TestParser(Tester<T> & tester,Logger logger) :
 _engine(2137), _tester(tester), _logger(logger)
{}

template<typename T>
void TestParser<T>::executeTests(std::istream & stream){
    std::string buffer;
    stream>>buffer;
    while(stream){
        parseCommand(buffer,stream);
        stream>>buffer;
    }
}

template<typename T>
void TestParser<T>::parseCommand(const std::string& command,std::istream & stream){
    if(command == "test"){
        _logger.logInfo("Starting new test: ",command);
        startNewTest(stream);
    }else if(command == "case"){
        _logger.logInfo("Executing case test:\n");
        executeLiteral(stream);
    }else if(command == "rand"){
        _logger.logInfo("Executing randomized test:\n");
        executeRandom(stream);
    }else{
        throw std::runtime_error("Invalid syntax, keyword: '"+command+ "' not recognized");
    }
}

template<typename T>
void TestParser<T>::startNewTest(std::istream & stream)
{
    stream>>_currentTest;
}

template<typename T>
void TestParser<T>::executeLiteral(std::istream & stream){
    auto size = _tester.getArgumentCount(_currentTest);
    std::vector<T> arguments;
    std::string buffer;
    while(size--){
        stream>>buffer;
        T arg(buffer);
        arguments.push_back(std::move(arg));
    }
    int expected;
    stream >> expected;
    float testResult = _tester.assert(_currentTest,arguments,expected == 1);
    assess(testResult,arguments);
}

template<typename T>
void TestParser<T>::executeRandom(std::istream & stream){
    size_t argCount,argSize;
    stream>>argCount>>argSize;
    std::vector<T> arguments;
    while(argCount--){
        generateRandomArguments(arguments,_tester.getArgumentCount(_currentTest),argSize);
        _logger.logInfo("dec: ",arguments[0]);
        _logger.logInfo("bin: ",arguments[0].toBin());
        //float result =_tester.assert(_currentTest,arguments,true);
        //assess(result,arguments);
    }
}

template<typename T>
void TestParser<T>::generateRandomArguments(std::vector<T> & args,size_t count, size_t size){
    args.clear();
    while(count--){
        args.push_back(T::createRandom(size));
    }
}

template<typename T>
void TestParser<T>::assess(float result,std::vector<T> & arguments){
  if(result<0){
    _logger.logError("\x1B[31mTEST FAILED\x1B[39;49m");
    for(size_t i =0;i<arguments.size();++i){
      _logger.logError(arguments[i]);
    }
  }
  else{
    _logger.logInfo("\x1B[92mTEST PASSED\x1B[39;49m");
    for(size_t i =0;i<arguments.size();++i){
      _logger.logInfo(arguments[i]);
      _logger.logInfo(arguments[i].toBin());
    }
  }
}

#endif /*TESTPARSER*/
