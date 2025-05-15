#include "logging.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace strings {
bool ends_with(const std::string &value, const std::string &ending) {
  if (ending.size() > value.size())
    return false;
  return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

std::string GetTimestamp() {
  const auto now = std::chrono::system_clock::now();
  return std::format("{:%d-%m-%Y %H:%M:%OS}", now);
}
} // namespace strings

namespace logging {
static std::fstream logfile;
static std::string logfilePath;
static std::string logfileName;
static std::streambuf *oldOutBuf;
static std::vector<std::string> errors;
static std::vector<std::string> warnings;

namespace fs = std::filesystem;

void log(const std::string &type, const std::string &msg) {
  if (type == "ERROR") {
    std::cerr << strings::GetTimestamp() << "\t" << type << ":\t" << msg
              << "\n";
    errors.push_back(msg);
    logfile.flush();
  } else if (type == "WARNING")
    warnings.push_back(msg);
  std::cout << strings::GetTimestamp() << "\t" << type << ":\t" << msg << "\n";
}
void startlogging(const std::string &path, const std::string &filename) {
  if (fs::is_directory(path))
    fs::create_directories(path);
  logfilePath = path;
  logfileName = filename;
  logfile.open(path + filename, std::ios::out);
  oldOutBuf = std::cout.rdbuf();
  std::cout.rdbuf(logfile.rdbuf());
}
void stoplogging() {
  logfile.flush();
  logfile.close();
  std::cout.rdbuf(oldOutBuf);
}
void backuplog(const std::string &path, bool crash) {
  // Check if the logfile exists
  if (!fs::exists(logfilePath + logfileName)) {
    return;
  }
  // Create Directories
  if (!fs::exists(path)) {
    fs::create_directories(path);
  }
  // Getting amount of files in directory
  int count = 0;
  for (const auto &entry : fs::directory_iterator(path)) {
    count++;
  }
  // Creating outfile string
  std::string outFile = path + logfileName;
  if (crash)
    outFile += "_crash";
  outFile += "_" + std::to_string(count) + ".log";
  // Copying logfile to desired Directory
  logfile.flush();
  fs::copy_file(logfilePath + logfileName, outFile);
}
void deletelog(const std::string &path) { std::remove(path.c_str()); }

std::vector<std::string> GetErrors() { return errors; }
std::string GetError() { return errors.back(); }
std::vector<std::string> GetWarnings() { return warnings; }
std::string GetWarning() { return warnings.back(); }
} // namespace logging
