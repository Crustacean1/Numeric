#include <fstream>
#include <iostream>
#include <string>

#include "numeric/basicio.h"
#include "numeric/numeric.h"
#include "tester/logger.h"

#include "tester/tester.h"
#include "tests/arithm_tests.h"


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

  Logger logger(std::cout, std::cerr);
  Tester<Tests::Integer, BasicIo> tester(logger);

  tester.addTest("string_idempotency", Tests::testStringIdempotency);
  tester.addTest("equality", Tests::testEquality);
  tester.addTest("comparision", Tests::testComparision);
  tester.addTest("addition", Tests::testAddition);
  tester.addTest("leftShiftLiteral", Tests::testLeftShiftLiteral);

  tester.readStream(testFile);
  return tester.execute();
}
