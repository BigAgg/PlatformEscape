#pragma once

#include <fstream>
#include <string>

namespace utils {
namespace strings {
bool IsNumber(const std::string &input);
bool IsInteger(const std::string &input);
std::pair<std::string, std::string> Splitlines(const std::string &input,
                                               const std::string &splitat);
bool StrContains(const std::string &input, const std::string &substring);
void RemoveAllSubstrings(std::string &input, const std::string &toRemove);
} // namespace strings

namespace files {
// Saves the size of a string and the string to a file
void SaveStringToFile(std::ofstream &file, const std::string &str);
// Loads a string from a file
std::string LoadStringFromFile(std::ifstream &file);
} // namespace files
} // namespace utils
