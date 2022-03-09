module;
#include <iostream>
#include <fstream>
#include <functional>
#include <map>
#include <string>
#include <tuple>

export module tester;

template<size_t... Seq,typename... Q,typename T>
std::tuple<Q...> packArrayToTuple(std::vector<T> const & array,std::index_sequence<Seq...>)
{
    return std::make_tuple(Q{array[Seq]}...);
}
template<size_t N,typename... Q,typename T>
std::tuple<Q...> packArrayToTuple(std::vector<T> const & array){
    return packArrayToTuple(array,std::make_index_sequence<N>());
}

template<typename T>
class NormalizedTest{
    std::function<bool(std::vector<T> params)> _testFunc;
    public:
    NormalizedTest(std::function<bool(std::vector<T> params)> testFunc,std::string name,size_t cnt) :
     _testFunc(testFunc),paramCount(cnt),testName(name){}
    size_t paramCount;
    std::string testName;
    bool execute(std::vector<T> params);
};

export template<typename T>
class Tester{
    std::vector<NormalizedTest<T>> _tests;
    template<typename... Q>
    using TestFunc = bool(*)(Q...);
    public:

    template<typename... Q>
    void addTest(std::string testName,TestFunc<Q...>);
    bool execute(std::ostream & testStream);
    void setReportStream(std::ostream & stream);
};


template<typename T>
template<typename... Q>
void Tester<T>::addTest(std::string testName,TestFunc<Q...> testFunc)
{
    std::vector<T> arr = {1,2,3};
    auto params = packArrayToTuple<3>(arr);
    std::cout<<std::get<0>(params)<<std::endl;
}

template<typename T>
bool Tester<T>::execute(std::ostream& testStream){
    return false;
}

template<typename T>
void Tester<T>::setReportStream(std::ostream& reportStream){
}