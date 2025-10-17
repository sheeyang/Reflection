#include "../include/Reflex.h"
#include "types.h"
#include <catch2/catch_all.hpp>
#include <sstream>
#include <iostream>

// Helper to capture stdout
class CaptureStdout
{
private:
  std::streambuf *old_stdout;
  std::ostringstream captured;

public:
  CaptureStdout() : old_stdout(std::cout.rdbuf())
  {
    std::cout.rdbuf(captured.rdbuf());
  }

  ~CaptureStdout()
  {
    std::cout.rdbuf(old_stdout);
  }

  std::string get_output() const
  {
    return captured.str();
  }
};

TEST_CASE("Print Simple struct", "[print]")
{
  Simple obj{42, 3.14f, 2.71};

  CaptureStdout capture;
  Reflex::print(obj);
  std::string output = capture.get_output();

  std::string expected = R"(Type: Simple
  integerValue = 42
  floatValue = 3.14
  doubleValue = 2.71
)";

  REQUIRE(output == expected);
}

TEST_CASE("Print AllPrimitiveTypes struct", "[print]")
{
  AllPrimitiveTypes obj{
      true, 'A', 123, 456, 789L, 101112LL,
      'B', 234, 567, 890UL, 111213ULL,
      1.23f, 4.56, 7.89L};

  CaptureStdout capture;
  Reflex::print(obj);
  std::string output = capture.get_output();

  std::string expected = R"(Type: AllPrimitiveTypes
  booleanValue = true
  characterValue = 'A'
  shortValue = 123
  integerValue = 456
  longValue = 789
  longLongValue = 101112
  unsignedCharValue = 'B'
  unsignedShortValue = 234
  unsignedIntValue = 567
  unsignedLongValue = 890
  unsignedLongLongValue = 111213
  floatValue = 1.23
  doubleValue = 4.56
  longDoubleValue = 7.89
)";

  REQUIRE(output == expected);
}

TEST_CASE("Print Nested struct", "[print]")
{
  Nested obj{123, {1, 2.0f, 3.0}, {true, 'C', 456, 789, 101112L, 131415LL, 'D', 40000, 5000000, 60000000UL, 7000000000ULL, 2.34f, 5.67, 8.90L}};

  CaptureStdout capture;
  Reflex::print(obj);
  std::string output = capture.get_output();

  std::string expected = R"(Type: Nested
  count = 123
  simpleStruct (Simple):
    integerValue = 1
    floatValue = 2
    doubleValue = 3
  primitiveTypes (AllPrimitiveTypes):
    booleanValue = true
    characterValue = 'C'
    shortValue = 456
    integerValue = 789
    longValue = 101112
    longLongValue = 131415
    unsignedCharValue = 'D'
    unsignedShortValue = 40000
    unsignedIntValue = 5000000
    unsignedLongValue = 60000000
    unsignedLongLongValue = 7000000000
    floatValue = 2.34
    doubleValue = 5.67
    longDoubleValue = 8.9
)";

  REQUIRE(output == expected);
}

TEST_CASE("Print DeeplyNested struct", "[print]")
{
  DeeplyNested obj{{456, {4, 5.0f, 6.0}, {false, 'D', 789, 101112, 131415L, 161718LL, 'E', 50000, 6000000, 70000000UL, 8000000000ULL, 3.45f, 6.78, 9.01L}}, {7, 8.0f, 9.0}};

  CaptureStdout capture;
  Reflex::print(obj);
  std::string output = capture.get_output();

  std::string expected = R"(Type: DeeplyNested
  nestedStruct (Nested):
    count = 456
    simpleStruct (Simple):
      integerValue = 4
      floatValue = 5
      doubleValue = 6
    primitiveTypes (AllPrimitiveTypes):
      booleanValue = false
      characterValue = 'D'
      shortValue = 789
      integerValue = 101112
      longValue = 131415
      longLongValue = 161718
      unsignedCharValue = 'E'
      unsignedShortValue = 50000
      unsignedIntValue = 6000000
      unsignedLongValue = 70000000
      unsignedLongLongValue = 8000000000
      floatValue = 3.45
      doubleValue = 6.78
      longDoubleValue = 9.01
  simpleStruct (Simple):
    integerValue = 7
    floatValue = 8
    doubleValue = 9
)";

  REQUIRE(output == expected);
}

TEST_CASE("Print ComplexTypes struct", "[print]")
{
  ComplexTypes obj{
      "Hello, World!",
      {1.1, 2.2, 3.3},
      {10, 20, 30, 40, 50},
      {"pi", 3.14f},
      {{"pi", 3.14f}, {"e", 2.71f}},
      {{1, "one"}, {2, "two"}, {3, "three"}},
      {0.1, 0.2, 0.3, 0.4, 0.5},
      {100, 200.0f, "three hundred"}};

  CaptureStdout capture;
  Reflex::print(obj);
  std::string output = capture.get_output();

  std::string expected = R"(Type: ComplexTypes
  stringValue = "Hello, World!"
  doubleArray = [
    0 = 1.1
    1 = 2.2
    2 = 3.3
  ]
  intVector = [
    0 = 10
    1 = 20
    2 = 30
    3 = 40
    4 = 50
  ]
  stringFloatPair = {
    first = "pi"
    second = 3.14
  }
  stringFloatMap = [
    0 = {
      first = "e"
      second = 2.71
    }
    1 = {
      first = "pi"
      second = 3.14
    }
  ]
  intStringUnorderedMap = [
    0 = {
      first = 1
      second = "one"
    }
    1 = {
      first = 2
      second = "two"
    }
    2 = {
      first = 3
      second = "three"
    }
  ]
  doubleList = [
    0 = 0.1
    1 = 0.2
    2 = 0.3
    3 = 0.4
    4 = 0.5
  ]
  intFloatStringTuple = (
    0 = 100
    1 = 200
    2 = "three hundred"
  )
)";

  REQUIRE(output == expected);
}

TEST_CASE("Print NestedComplex struct", "[print]")
{
  NestedComplex obj{
      {{"a", "b"}, {"c", "d", "e"}},
      {{{1, 2, 3}, {4, 5, 6}}},
      {{{Simple{10, 1.0f, 2.0}, Simple{20, 3.0f, 4.0}}, {Simple{30, 5.0f, 6.0}}}}};

  CaptureStdout capture;
  Reflex::print(obj);
  std::string output = capture.get_output();

  std::string expected = R"(Type: NestedComplex
  vectorOfVectors = [
    0 = [
      0 = "a"
      1 = "b"
    ]
    1 = [
      0 = "c"
      1 = "d"
      2 = "e"
    ]
  ]
  arrayOfArrays = [
    0 = [
      0 = 1
      1 = 2
      2 = 3
    ]
    1 = [
      0 = 4
      1 = 5
      2 = 6
    ]
  ]
  arrayOfVectorsOfStructs = [
    0 = [
      0 (Simple):
        integerValue = 10
        floatValue = 1
        doubleValue = 2
      1 (Simple):
        integerValue = 20
        floatValue = 3
        doubleValue = 4
    ]
    1 = [
      0 (Simple):
        integerValue = 30
        floatValue = 5
        doubleValue = 6
    ]
  ]
)";

  REQUIRE(output == expected);
}

TEST_CASE("Print ContainsNotReflected struct", "[print]")
{
  ContainsNotReflected obj;
  obj.notReflectedStruct.xValue = 100;
  obj.notReflectedStruct.yValue = 200.5f;

  CaptureStdout capture;
  Reflex::print(obj);
  std::string output = capture.get_output();

  std::string expected = R"(Type: ContainsNotReflected
  aValue = 100
  bValue = 200.5
)";

  REQUIRE(output == expected);
}