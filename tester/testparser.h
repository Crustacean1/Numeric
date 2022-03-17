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
        _logger.logInfo("Trying to case");
        executeLiteral(stream);
    }else if(command == "rand"){
        _logger.logInfo("Trying to randomize");
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
        arguments.push_back(T(buffer));
    }
    int expected;
    stream >> expected;
    float testResult = _tester.assert(_currentTest,arguments,expected == 1);
    if(testResult<0){
        _logger.logError("Test case failed for:");
        for(const auto & arg: arguments){
            _logger.logError("Arg: ",arg);
        }
    }
}

template<typename T>
void TestParser<T>::executeRandom(std::istream & stream){
    size_t argCount,argSize;
    stream>>argCount>>argSize;
    std::vector<T> arguments;
    while(argCount--){
        generateRandomArguments(arguments,_tester.getArgumentCount(_currentTest),argSize);
        _tester.assert(_currentTest,arguments,true);
        _logger.logInfo("First arg: ",arguments[0]);
    }
}

template<typename T>
void TestParser<T>::generateRandomArguments(std::vector<T> & args,size_t count, size_t size){
    args.clear();
    while(count--){
        args.push_back(T::createRandom(size));
        _logger.logInfo("created argument with size: ",args.back().size());
    }
}
#endif /*TESTPARSER*/
