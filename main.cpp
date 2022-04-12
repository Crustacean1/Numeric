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

  Logger logger(std::cout, std::cerr, 1);
  Tester<Tests::Integer, BasicIo> tester(logger);

  tester.addTest("stringIdempotency", Tests::stringIdempotency);

  tester.addTest("equality", Tests::equality);
  tester.addTest("comparision", Tests::comparision);

  tester.addTest("addition", Tests::addition);
  tester.addTest("leftShift", Tests::leftShift);
  tester.addTest("rightShift", Tests::rightShift);
  tester.addTest("anyShift", Tests::anyShift);
  tester.addTest("basicMul", Tests::basicMultiplication);
  tester.addTest("basicDiv", Tests::basicDivision);

  tester.readStream(testFile);
  return tester.execute();
}
