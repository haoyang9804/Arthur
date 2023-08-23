#include "../include/utils.hpp"
#include "../include/config.hpp"

#include <fstream>
#include <random>
#include <sstream>
#include <iostream>

std::vector<std::string> words = {};

std::string read(std::string path, int n = -1) {
  std::cout << std::filesystem::current_path() << std::endl;
  ASSERT_RUNTIME(std::filesystem::exists(path), "Path " + path + " does not exists");
  std::ifstream infile(path);
  std::stringstream buffer;
  
  if (n == -1) {
    buffer << infile.rdbuf();
  }
  else {
    std::string line;
    int line_count = 0;
    while (std::getline(infile, line) && line_count++ < n) {
      buffer << line << "\n";
    }
  }
  std::string contents = buffer.str();
  return contents;
}

std::vector<std::string> split(const std::string& str, char delimiter) {
  std::vector<std::string> tokens;
  std::stringstream ss(str);
  std::string token;
  while (getline(ss, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

std::string randomlyPickAWord() {
  if (words.empty()) {
    words = split(read("words", statNum), '\n');
  }
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, words.size() - 1);
  int random_index = dis(gen);
  return words[random_index];
}