#pragma once

#include <string>
#include <type_traits>
#include <stdexcept>
#include <vector>
#include <filesystem>

template <class, class = void>
struct has_toString_member : std::false_type {};

template <class T>
struct has_toString_member<T, std::void_t<decltype(&T::toString)>>
    : std::true_type {};

#define ASSERT_LOGIC(STATEMENT, STR) \
  if (!(STATEMENT)) throw std::logic_error(STR)

#define ASSERT_RUNTIME(STATEMENT, STR) \
  if (!(STATEMENT)) throw std::runtime_error(STR)

extern std::vector<std::string> words;

// extract the first n lines from a file
// if the value of `n` is not provides, then extract all contents
std::string read(std::string path, int n);

std::vector<std::string> split(const std::string& str, char delimiter);

std::string randomlyPickAWord();