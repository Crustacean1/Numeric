#include <fstream>
#include <iostream>
#include <string>

#include "numeric/Buffer/BufferInstance.h"
#include "numeric/Numeric.h"
#include "numeric/ArithmFacade.h"
#include "tester/logger.h"

#include "tests/ArgumentGenerator/NumericGeneratorFactory.h"
#include "tests/arithm_tests.h"

#include "tester/TestBrowser.h"
#include "tester/tester.h"

int main(int argc, char **argv) {
  Logger logger(std::cout, std::cerr, 5);

  std::vector<std::string> testFiles;

  if (argc > 1) {
    for (int i = 1; i < argc; ++i) {
      testFiles.emplace_back(argv[i]);
    }
  } else {
    TestBrowser testBrowser("tst");
    testFiles = testBrowser.getTestsFromDir("testfiles");
    std::cout << "Found: " << testFiles.size() << " test files" << std::endl;
  }

  NumericGeneratorFactory factory(KCrypt::ArithmFacade::getInstance(0));
  Logger _logger(std::cout, std::cerr, 5);
  Tester<KCrypt::Numeric> tester(_logger, factory);

  Tests::setUpAllTests(tester);

  int result = 0;
  for (const auto &testFile : testFiles) {
    _logger.logInfo(2, "File: ", testFile);
    _logger.logInfo(2, "----------------------------------------");

    std::ifstream file(testFile);
    tester.readStream(file);
    result |= tester.execute();
  }
  KCrypt::ArithmFacade::releaseInstance(0);
  return result;
}
