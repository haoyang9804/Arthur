#include "../include/type.hpp"

std::unordered_map<uint16_t, std::string> TypeContext = {
  {1, "Integer"},
  {2, "Bool"},
  {3, "Address"},
  {4, "Bytes"},
  {5, "String"},
  {6, "Enum"}
};

uint16_t user_defined_type_index = 257;