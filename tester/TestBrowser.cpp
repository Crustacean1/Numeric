#include "TestBrowser.h"

namespace fs = std::filesystem;

TestBrowser::TestBrowser(const std::string &testExtension)
    : _extensionRegex(std::string("(([a-z]|[A-Z])*\\.") + testExtension +
                      std::string(")")) {}

void TestBrowser::getTestsFromDir(std::vector<std::string> &tests,
                                  const std::filesystem::path &startDir) {
  for (const auto file : fs::directory_iterator{startDir}) {
    if (file.is_directory()) {
      getTestsFromDir(tests, file);
      continue;
    }
    std::string filename = file.path().filename();
    if (std::regex_match(filename, _extensionRegex)) {
      tests.push_back(file.path());
    }
  }
}
std::vector<std::string>
TestBrowser::getTestsFromDir(const std::string & path) {
  auto startDir = fs::current_path() / path;
  std::vector<std::string> tests;
  getTestsFromDir(tests, startDir);
  return tests;
}
