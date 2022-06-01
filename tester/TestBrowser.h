#ifndef TEST_BROWSER
#define TEST_BROWSER

#include <filesystem>
#include <regex>
#include <string>
#include <vector>

class TestBrowser {
  std::regex _extensionRegex;
  void getTestsFromDir(std::vector<std::string> &tests,
                       const std::filesystem::path &startDir);

public:
  TestBrowser(const std::string &defaultExtension);
  // TODO: Coroutines
  std::vector<std::string> getTestsFromDir(const std::string &dir = "");
};

#endif /*TEST_BROWSER*/
