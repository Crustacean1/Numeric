#include <fstream>
#include <iostream>
#include <string>

#include "numeric/arithm.h"
#include "numeric/basicio.h"
#include "numeric/basicmul.h"
#include "numeric/numeric.h"

#include "tester/tester.h"
#include "tester/testparser.h"

using Integer = KCrypt::Numeric<Arithm, BasicIo>;

bool testStringIdempotency(Integer& a) {
  std::string ogInt(a);

  return a == Integer(ogInt);
}

bool testEquality(Integer& a,Integer &b)
{
  return a == b;
}

bool testAddition(Integer& a, Integer& b) {
  auto c = a + b;
  std::cout<<a<<"\n"<<b<<"\n"<<c<<std::endl;
  c += b;
  return (c == a);
}

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Invalid argument count, proper syntax is: ./Numeric "
                 "[test_file_name]"
              << std::endl;
    return -1;
  }
  std::ifstream testFile(argv[1]);
  if (!testFile) {
    std::cerr << "Failed to open test file: " << argv[1] << " now closing"
              << std::endl;
    return -1;
  }
  Integer num;

  Tester<Integer> tester;
  tester.addTest("string_idempotency", testStringIdempotency);
  tester.addTest("equality", testEquality);
  tester.addTest("addition", testAddition);

  Logger logger(std::cout, std::cerr);

  TestParser parser(tester, logger);

  parser.executeTests(testFile);
}
