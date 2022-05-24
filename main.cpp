#include <fstream>
#include <iostream>
#include <string>

#include "numeric/BasicIo.h"
#include "numeric/Numeric.h"
#include "numeric/BufferInstance.h"
#include "tester/logger.h"

#include "tester/tester.h"
#include "tests/arithm_tests.h"
#include "tests/ArgumentGenerator/NumericGeneratorFactory.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Invalid argument count, proper syntax is: ./Numeric "
                 "[test_file_name]"
              << std::endl;
    return -1;
  }

  Logger logger(std::cout, std::cerr, 5);

  KCrypt::BufferInstance::init();

  KCrypt::BasicIo io;
  KCrypt::Comparator cmp;
  KCrypt::AddEngine adder(cmp);

  NumericGeneratorFactory factory(io, adder);
  Tester<Tests::Integer> tester(logger, factory);

  tester.addTest("stringIdempotency", Tests::stringIdempotency);
  tester.addTest("equality", Tests::equality);
  tester.addTest("comparision", Tests::comparision);
  tester.addTest("addition", Tests::addition);
  tester.addTest("subtraction", Tests::subtraction);
  tester.addTest("leftShift", Tests::leftShift);
  tester.addTest("rightShift", Tests::rightShift);
  tester.addTest("anyShift", Tests::anyShift);
  tester.addTest("basicMul", Tests::basicMultiplication);
  tester.addTest("basicDiv", Tests::basicDivision);
  tester.addTest("mulDivReciprocity", Tests::mulDivReciprocity);
  tester.addTest("mulPerf", Tests::mulPerf);
  tester.addTest("newtonDiv",Tests::newtonDiv);

  int result = 0;
  for (int i = 1; i < argc && result == 0; ++i) {
    std::ifstream testFile(argv[i]);
    if (!testFile) {
      std::cerr << "Failed to open test file: " << argv[i] << " now closing"
                << std::endl;
      return -1;
    }
    tester.readStream(testFile);
    result = tester.execute();
  }

  KCrypt::BufferInstance::destroy();

  return result;
}
