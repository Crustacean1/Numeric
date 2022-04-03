#ifndef NORMALIZEDTEST
#define NORMALIZEDTEST

#include <functional>
#include <vector>

template <size_t... Seq, typename T, typename... Q>
bool executeFunction(bool (*func)(Q...), std::vector<T> &params,
                     std::index_sequence<Seq...>) {
  return func(std::forward<Q>(params[Seq])...);
}

template <size_t N, typename T, typename... Q>
bool executeFunction(bool (*func)(Q...), std::vector<T> &params) {
  return executeFunction(func, params, std::make_index_sequence<N>());
}

template <typename T> class NormalizedTest {
public:
  using NormalizedTestFunction = std::function<bool(std::vector<T> &params)>;

  NormalizedTest() {}
  NormalizedTest(NormalizedTestFunction func, size_t cnt)
      : _func(func), _paramCount(cnt) {}

  bool execute(std::vector<T> &params);
  size_t getArgumentCount() const;

private:
  NormalizedTestFunction _func;
  size_t _paramCount;
};

template <typename T> bool NormalizedTest<T>::execute(std::vector<T> &params) {
  return _func(params);
}

template <typename T> size_t NormalizedTest<T>::getArgumentCount() const {
  return _paramCount;
}

#endif /*NORMALIZEDTEST*/
