#pragma once

#include <type_traits>

template<class, class = void>
struct has_toString_member : std::false_type {};
 
template<class T>
struct has_toString_member<T, std::void_t<decltype(&T::toString)>> : std::true_type {};