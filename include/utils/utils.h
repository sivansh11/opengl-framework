#ifndef UTILS_H
#define UTILS_H

#include "core.h"

enum FileType
{
    TEXT,
    BINARY
};
std::string read_file(const char *path, const FileType type=FileType::TEXT);
void split_string(const std::string& str, const char delimiter, std::vector<std::string> &out);
void save_string(std::string &str, const char *path);
std::vector<char> string_to_vector_of_char(std::string &&str);

// useful for combining multiple hashes 
template <typename T, typename... Rest>
void hashCombine(std::size_t& seed, const T& v, const Rest&... rest) {
  seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  (hashCombine(seed, rest), ...);
};

#endif