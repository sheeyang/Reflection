#pragma once

#include <type_traits>
#include <concepts>
#include <array>

namespace ReflectionLibrary
{
  // Forward declaration
  template <typename T>
  struct ReflectionInfo;

  // Check if a type is reflectable
  template <typename T>
  concept is_reflectable_v = requires {
    ReflectionInfo<T>::class_name;
    ReflectionInfo<T>::fields;
    ReflectionInfo<T>::reflect;
    ReflectionInfo<T>::create;
  };

  // Check if a type is a std::tuple
  template <typename T>
  concept is_tuple_v = requires { std::tuple_size<T>::value; };

  // Check if a type is a std::pair
  template <typename T>
  concept is_pair_v = requires { typename T::first_type; typename T::second_type; };

  // Check if a type is iterable (has size, begin, end)
  template <typename T>
  concept is_iterable_v = requires(T value) { value.size(); value.begin(); value.end(); };

}