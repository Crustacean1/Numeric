#ifndef TESTPARSER
#define TESTPARSER

#include <iostream>
#include <random>
#include "tester.h"

template<typename T>
class TestParser{
    Tester<T> & _tester;
    std::default_random_engine _engine;
    std::string _currentTest; 
    std::vector<T> _arguments;

    std::ostream & _logStream;
    std::ostream & _errStream;

    template<typename... Q>
    void logInfo(Q... q);

    template<typename... Q>
    void logError(Q... q);

    void parseCommand(std::istream & stream);
    void startNewTest(std::istream & stream);
    void executeRandom(std::istream & stream);
    void executeLiteral(std::istream & stream);

    void generateRandomArguments(std::vector<T> & args,size_t count, size_t size);

    public:

    TestParser(Tester<T> & tester,std::ostream & _logStream,std::ostream & _errStream);
    void executeTests(std::istream & stream);
};

template<typename T>
TestParser<T>::TestParser(Tester<T> & tester) : _tester(tester), _engine(2137)
{

}

template<typename T>
void TestParser<T>::executeTests(std::istream & stream){
    while(stream){
        parseCommand(stream);
    }
}

template<typename T>
void TestParser<T>::parseCommand(std::istream & stream){
    std::string buffer;
    stream >> buffer;
    if(buffer == "test"){
        startNewTest(stream);
    }else if(buffer == "case"){
        executeLiteral(stream);
    }else if(buffer == "rand"){
        executeRandom(stream);
    }else{
        throw std::runtime_error("Invalid syntax, keyword: "+buffer+ " not recognized");
    }
}

template<typename T>
void TestParser<T>::startNewTest(std::istream & stream)
{
    stream>>_currentTest;
}

template<typename T>
void TestParser<T>::executeLiteral(std::istream & stream){
    auto size = 2137;//_tester.getArgumentCount(_currentTest);
    std::vector<T> arguments;
    std::string buffer;
    while(size--){
        stream>>buffer;
        arguments.push_back(T(buffer));
    }
    int expected;
    stream >> expected;
    _tester.assert(_currentTest,arguments,expected == 1);
}

template<typename T>
void TestParser<T>::executeRandom(std::istream & stream){
    size_t argCount,argSize;
    stream>>argCount>>argSize;
    std::vector<T> arguments;
    while(argCount--){
        generateRandomArguments(arguments,_tester.getArgumentCount(_currentTest),argSize);
        _tester.assert(_currentTest,arguments,true);
    }
}
    
template<typename T>
void TestParser<T>::generateRandomArguments(std::vector<T> & args,size_t count, size_t size){
    args.clear();
    while(count--){
        args.push_back(T::createRandom(size));
    }
}
#endif /*TESTPARSER*/