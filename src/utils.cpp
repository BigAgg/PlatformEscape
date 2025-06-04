#include "utils.h"

#include <algorithm>
#include <fstream>

namespace utils {
namespace strings {
bool StrContains(const std::string &input, const std::string &substring) {
  return (input.find(substring) != std::string::npos);
}

void RemoveAllSubstrings(std::string &input, const std::string &toRemove) {
  size_t pos;
  while ((pos = input.find(toRemove)) != std::string::npos) {
    input.erase(pos, toRemove.length());
  }
}

std::pair<std::string, std::string> Splitlines(const std::string &input,
                                               const std::string &splitat) {
  size_t pos = input.find(splitat);
  if (pos == std::string::npos) {
    return {input, ""}; // No delimiter found
  }

  std::string left = input.substr(0, pos);
  std::string right = input.substr(pos + splitat.length());

  return {left, right};
}

bool IsNumber(const std::string &input) {
  try {
    if (input.size() == 0)
      return false;
    std::string str = input;
    std::replace(str.begin(), str.end(), ',', '.');
    // Checking for prefix
    size_t start = 0;
    if (input[0] == '-' || input[0] == '+') {
      if (input.size() == 1)
        return false;
      start = 1;
    }
    // Checking for any characters
    int dotcount = 0;
    for (size_t i = start; i < input.size(); ++i) {
      if (input[i] > 255 || input[i] < 0)
        return false;
      if (std::isdigit(input[i])) {
        continue;
      } else if (input[i] == '.' || input[i] == ',') {
        dotcount++;
        if (dotcount > 1)
          return false;
        continue;
      } else {
        return false;
      }
    }
    if (dotcount == 1)
      return true;
  } catch (const std::exception &e) {
    return false;
  }
  return true;
}

bool IsInteger(const std::string &input) {
  // First make sure there are no invalid characters
  size_t start = 0;
  if (input[0] == '+' || input[0] == '-')
    start = 1;
  for (size_t i = start; i < input.size(); i++) {
    if (input[0] > 255 || input[0] < 0)
      return false;
    if (!std::isdigit(input[i]))
      return false;
  }
  return true;
}
} // namespace strings
namespace files {
void SaveStringToFile(std::ofstream &file, const std::string &str) {
  size_t strSize = str.size();
  file.write((char *)&strSize, sizeof(size_t));
  file.write((char *)&str, strSize);
}

std::string LoadStringFromFile(std::ifstream &file) {
  std::string str;
  size_t strSize = 0;
  file.read((char *)&strSize, sizeof(size_t));
  file.read((char *)&str, strSize);
  return str;
}
} // namespace files
} // namespace utils
