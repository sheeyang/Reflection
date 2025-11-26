#pragma once

#include "../include/ReflectionLibrary.h"

#include <array>
#include <vector>
#include <map>
#include <unordered_map>
#include <optional>

struct Simple
{
  int integerValue;
  float floatValue;
  double doubleValue;
};
REFLECT_FIELDS(Simple, integerValue, floatValue, doubleValue)

struct AllPrimitiveTypes
{
  bool booleanValue;
  char characterValue;
  short shortValue;
  int integerValue;
  long longValue;
  long long longLongValue;
  unsigned char unsignedCharValue;
  unsigned short unsignedShortValue;
  unsigned int unsignedIntValue;
  unsigned long unsignedLongValue;
  unsigned long long unsignedLongLongValue;
  float floatValue;
  double doubleValue;
  long double longDoubleValue;
};
REFLECT_FIELDS(AllPrimitiveTypes, booleanValue, characterValue, shortValue, integerValue, longValue, longLongValue, unsignedCharValue, unsignedShortValue, unsignedIntValue, unsignedLongValue, unsignedLongLongValue, floatValue, doubleValue, longDoubleValue)

struct Nested
{
  int count;
  Simple simpleStruct;
  AllPrimitiveTypes primitiveTypes;
};
REFLECT_FIELDS(Nested, count, simpleStruct, primitiveTypes)

struct DeeplyNested
{
  Nested nestedStruct;
  Simple simpleStruct;
};
REFLECT_FIELDS(DeeplyNested, nestedStruct, simpleStruct)

struct ComplexTypes
{
  std::string stringValue;
  std::array<double, 3> doubleArray;
  std::vector<int> intVector;
  std::pair<std::string, float> stringFloatPair;
  std::map<std::string, float> stringFloatMap;
  std::unordered_map<int, std::string> intStringUnorderedMap;
  std::list<double> doubleList;
  std::tuple<int, float, std::string> intFloatStringTuple;
};
REFLECT_FIELDS(ComplexTypes,
  stringValue,
  doubleArray,
  intVector,
  stringFloatPair,
  stringFloatMap,
  intStringUnorderedMap,
  doubleList,
  intFloatStringTuple)

  struct NestedComplex
{
  std::vector<std::vector<std::string>> vectorOfVectors;
  std::array<std::array<int, 3>, 2> arrayOfArrays;
  std::array<std::vector<Simple>, 2> arrayOfVectorsOfStructs;
};
REFLECT_FIELDS(NestedComplex, vectorOfVectors, arrayOfArrays, arrayOfVectorsOfStructs)

struct NotReflected
{
  int xValue;
  float yValue;
};

struct ContainsNotReflected
{
  NotReflected notReflectedStruct;

  struct Reflector
  {
    int aValue;
    float bValue;

    static ContainsNotReflected create(const Reflector& r)
    {
      ContainsNotReflected obj;
      obj.notReflectedStruct.xValue = r.aValue;
      obj.notReflectedStruct.yValue = r.bValue;
      return obj;
    }

    static Reflector reflect(const ContainsNotReflected& obj)
    {
      Reflector r;
      r.aValue = obj.notReflectedStruct.xValue;
      r.bValue = obj.notReflectedStruct.yValue;
      return r;
    }
  };
};
REFLECT_CUSTOM(ContainsNotReflected, aValue, bValue)

struct EmptyStruct
{
};
REFLECT_FIELDS(EmptyStruct)

struct WithOptional
{
  std::optional<int> optionalInt;
  std::optional<std::string> optionalString;
  std::optional<Simple> optionalStruct;
  int regularInt;
};
REFLECT_FIELDS(WithOptional, optionalInt, optionalString, optionalStruct, regularInt)

enum class Color
{
  Red,
  Green,
  Blue,
  Yellow
};
REFLECT_ENUM(Color, Red, Green, Blue, Yellow)

enum class Status
{
  Pending,
  Active,
  Completed,
  Cancelled
};
REFLECT_ENUM(Status, Pending, Active, Completed, Cancelled)

struct WithEnum
{
  Color color;
  Status status;
  int value;
};
REFLECT_FIELDS(WithEnum, color, status, value)

// Namespace tests
namespace TestNamespace
{
  struct NamespacedSimple
  {
    int x;
    double y;
    std::string name;
  };

  struct NamespacedNested
  {
    NamespacedSimple simple;
    int count;
  };

  struct NamespacedComplex
  {
    std::vector<int> numbers;
    std::map<std::string, double> values;
    std::optional<std::string> optionalText;
  };

  enum class NamespacedColor
  {
    Red,
    Green,
    Blue
  };

  enum class NamespacedStatus
  {
    Idle,
    Running,
    Stopped
  };

  struct NamespacedWithEnum
  {
    NamespacedColor color;
    NamespacedStatus status;
    int value;
  };
}

REFLECT_FIELDS(TestNamespace::NamespacedSimple, x, y, name)
REFLECT_FIELDS(TestNamespace::NamespacedNested, simple, count)
REFLECT_FIELDS(TestNamespace::NamespacedComplex, numbers, values, optionalText)
REFLECT_ENUM(TestNamespace::NamespacedColor, Red, Green, Blue)
REFLECT_ENUM(TestNamespace::NamespacedStatus, Idle, Running, Stopped)
REFLECT_FIELDS(TestNamespace::NamespacedWithEnum, color, status, value)

// Nested namespace tests
namespace Outer
{
  namespace Inner
  {
    struct DeepNamespacedStruct
    {
      int id;
      std::string description;
    };

    enum class DeepNamespacedEnum
    {
      First,
      Second,
      Third
    };
  }
}

REFLECT_FIELDS(Outer::Inner::DeepNamespacedStruct, id, description)
REFLECT_ENUM(Outer::Inner::DeepNamespacedEnum, First, Second, Third)