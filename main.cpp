#include <fstream>
#include <iostream>
#include <string>

#include "numeric/BasicIo.h"
#include "numeric/Buffer/BufferInstance.h"
#include "numeric/Numeric.h"
#include "tester/logger.h"

#include "tester/tester.h"
#include "tests/ArgumentGenerator/NumericGeneratorFactory.h"
#include "tests/arithm_tests.h"

int main(int argc, char **argv) {
  Logger logger(std::cout, std::cerr, 5);

  if (argc != 2) {
    logger.logError(
        0,
        "Invalid argument count, proper syntax is: ./Numeric [test_file_name]");
    return -1;
  }
  std::ifstream file(argv[1]);

  if (!file) {
    logger.logError(0, "Coldn't open file: ", argv[1]);
    return -1;
  }

  KCrypt::BufferInstance::init();

  KCrypt::BufferInstance &buffInst = KCrypt::BufferInstance::getInstance();

  KCrypt::CompEngine cmp;
  KCrypt::AddEngine add(cmp);
  KCrypt::IoEngine io(cmp, add);

  NumericGeneratorFactory factory(io, add);
  Logger _logger(std::cout, std::cerr, 5);
  Tester<KCrypt::Numeric> tester(_logger, factory);

  tester.addTest("Equality", Tests::equality);

  tester.addTest("AdditionSelfTest",Tests::addition);
  tester.addTest("SubtractionSelfTest",Tests::subtraction);

  tester.addTest("LeftShiftValueTest", Tests::leftShift);
  tester.addTest("RightShiftValueTest", Tests::rightShift);
  tester.addTest("ShiftSelfTest", Tests::anyShift);

  tester.addTest("DivisionValueTest", Tests::basicDivision);
  tester.addTest("MultiplicationDivisionSelfTest", Tests::mulDivReciprocity);
  tester.addTest("MulPerformanceTest", Tests::mulPerf);
  tester.addTest("DivisionFloorTest", Tests::divFloor);
  tester.addTest("NastyDivisionFloorTest", Tests::nastyDivFloor);

  tester.addTest("ModularExponentiationValueTest",Tests::modExponentValue);

  tester.addTest("GcdValueTest",Tests::extGcdValue);

  tester.readStream(file);

  return tester.execute();
}
