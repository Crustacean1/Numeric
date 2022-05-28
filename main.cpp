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
  KCrypt::BasicIo io(cmp, add);

  NumericGeneratorFactory factory(io, add);
  Logger _logger(std::cout, std::cerr, 5);
  Tester<KCrypt::Numeric> tester(_logger, factory);

  tester.addTest("addition",Tests::addition);
  tester.addTest("subtraction",Tests::subtraction);

  tester.readStream(file);

  return tester.execute();
}
