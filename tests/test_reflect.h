#pragma once

#include "../include/ReflectionLibrary.h"
#include "types.h"
#include <catch2/catch_all.hpp>

TEST_CASE("Simple struct reflection", "[reflection]")
{
  Simple obj{ 42, 3.14f, 2.71 };

  SECTION("Class name")
  {
    REQUIRE(ReflectionLibrary::class_name<Simple> == std::string("Simple"));
    REQUIRE(ReflectionLibrary::get_class_name(obj) == "Simple");
  }

  SECTION("Field count")
  {
    REQUIRE(ReflectionLibrary::field_count<Simple> == 3);
    REQUIRE(ReflectionLibrary::get_field_count(obj) == 3);
  }

  SECTION("Set field value")
  {
    ReflectionLibrary::set_field_value(obj, "integerValue", 100);
    REQUIRE(obj.integerValue == 100);
  }

  SECTION("Field values")
  {
    REQUIRE(obj.integerValue == 42);
    REQUIRE(obj.floatValue == 3.14f);
    REQUIRE(obj.doubleValue == 2.71);
  }

  SECTION("Field iteration")
  {
    std::vector<std::string> names;
    std::vector<int> nest_levels;
    ReflectionLibrary::for_each_field(obj, [&](std::string_view name, auto& value, int nest_level)
      {
        names.push_back(std::string(name));
        nest_levels.push_back(nest_level); });
    REQUIRE(names.size() == 3);
    REQUIRE(names == std::vector<std::string>{"integerValue", "floatValue", "doubleValue"});
    REQUIRE(nest_levels == std::vector<int>{0, 0, 0}); // All top-level fields
  }
}

TEST_CASE("AllPrimitiveTypes struct reflection", "[reflection]")
{
  AllPrimitiveTypes obj{
      true, 'A', 123, 456, 789L, 101112LL,
      'B', 234, 567, 890UL, 111213ULL,
      1.23f, 4.56, 7.89L };

  SECTION("Class name")
  {
    REQUIRE(ReflectionLibrary::get_class_name(obj) == "AllPrimitiveTypes");
  }

  SECTION("Field count")
  {
    REQUIRE(ReflectionLibrary::get_field_count(obj) == 14);
  }

  SECTION("Field iteration")
  {
    std::vector<std::string> names;
    std::vector<int> nest_levels;
    ReflectionLibrary::for_each_field(obj, [&](std::string_view name, auto& value, int nest_level)
      {
        names.push_back(std::string(name));
        nest_levels.push_back(nest_level); });
    REQUIRE(names.size() == 14);
    REQUIRE(names == std::vector<std::string>{
      "booleanValue", "characterValue", "shortValue", "integerValue", "longValue", "longLongValue",
        "unsignedCharValue", "unsignedShortValue", "unsignedIntValue", "unsignedLongValue", "unsignedLongLongValue",
        "floatValue", "doubleValue", "longDoubleValue"});
    REQUIRE(nest_levels == std::vector<int>(14, 0)); // All top-level
  }

  SECTION("Field values")
  {
    REQUIRE(obj.booleanValue == true);
    REQUIRE(obj.characterValue == 'A');
    REQUIRE(obj.shortValue == 123);
    REQUIRE(obj.integerValue == 456);
    REQUIRE(obj.longValue == 789L);
    REQUIRE(obj.longLongValue == 101112LL);
    REQUIRE(obj.unsignedCharValue == 'B');
    REQUIRE(obj.unsignedShortValue == 234);
    REQUIRE(obj.unsignedIntValue == 567);
    REQUIRE(obj.unsignedLongValue == 890UL);
    REQUIRE(obj.unsignedLongLongValue == 111213ULL);
    REQUIRE(obj.floatValue == 1.23f);
    REQUIRE(obj.doubleValue == 4.56);
    REQUIRE(obj.longDoubleValue == 7.89L);
  }
}

TEST_CASE("Nested struct reflection", "[reflection]")
{
  Nested obj{ 123, {1, 2.0f, 3.0}, {true, 'C', 456, 789, 101112L, 131415LL, 'D', 40000, 5000000, 60000000UL, 7000000000ULL, 2.34f, 5.67, 8.90L} };

  SECTION("Class name")
  {
    REQUIRE(ReflectionLibrary::get_class_name(obj) == "Nested");
  }

  SECTION("Field count")
  {
    REQUIRE(ReflectionLibrary::get_field_count(obj) == 3);
  }

  SECTION("Field iteration")
  {
    int i = 0;
    std::vector<std::string> names;
    std::vector<int> nest_levels;
    ReflectionLibrary::for_each_field(obj, [&](std::string_view name, auto& value, int nest_level)
      {
        names.push_back(std::string(name));
        nest_levels.push_back(nest_level);
        i++; });
    REQUIRE(i == 3);
    REQUIRE(names.size() == 3);
    REQUIRE(names == std::vector<std::string>{"count", "simpleStruct", "primitiveTypes"});
    REQUIRE(nest_levels == std::vector<int>{0, 0, 0});
  }

  SECTION("Field values")
  {
    REQUIRE(obj.count == 123);
    REQUIRE(obj.simpleStruct.integerValue == 1);
    REQUIRE(obj.simpleStruct.floatValue == 2.0f);
    REQUIRE(obj.simpleStruct.doubleValue == 3.0);
    REQUIRE(obj.primitiveTypes.booleanValue == true);
    REQUIRE(obj.primitiveTypes.characterValue == 'C');
    REQUIRE(obj.primitiveTypes.shortValue == 456);
    REQUIRE(obj.primitiveTypes.integerValue == 789);
    REQUIRE(obj.primitiveTypes.longValue == 101112L);
    REQUIRE(obj.primitiveTypes.longLongValue == 131415LL);
    REQUIRE(obj.primitiveTypes.unsignedCharValue == 'D');
    REQUIRE(obj.primitiveTypes.unsignedShortValue == 40000);
    REQUIRE(obj.primitiveTypes.unsignedIntValue == 5000000);
    REQUIRE(obj.primitiveTypes.unsignedLongValue == 60000000UL);
    REQUIRE(obj.primitiveTypes.unsignedLongLongValue == 7000000000ULL);
    REQUIRE(obj.primitiveTypes.floatValue == 2.34f);
    REQUIRE(obj.primitiveTypes.doubleValue == 5.67);
    REQUIRE(obj.primitiveTypes.longDoubleValue == 8.90L);
  }
}

TEST_CASE("DeeplyNested struct reflection", "[reflection]")
{
  DeeplyNested obj{ {456, {4, 5.0f, 6.0}, {false, 'D', 789, 101112, 131415L, 161718LL, 'E', 50000, 6000000, 70000000UL, 8000000000ULL, 3.45f, 6.78, 9.01L}}, {7, 8.0f, 9.0} };

  SECTION("Class name")
  {
    REQUIRE(ReflectionLibrary::get_class_name(obj) == "DeeplyNested");
  }

  SECTION("Field count")
  {
    REQUIRE(ReflectionLibrary::get_field_count(obj) == 2);
  }

  SECTION("Field iteration")
  {
    int i = 0;
    std::vector<std::string> names;
    std::vector<int> nest_levels;
    ReflectionLibrary::for_each_field(obj, [&](std::string_view name, auto& value, int nest_level)
      {
        names.push_back(std::string(name));
        nest_levels.push_back(nest_level);
        i++; });
    REQUIRE(i == 2);
    REQUIRE(names.size() == 2);
    REQUIRE(names == std::vector<std::string>{"nestedStruct", "simpleStruct"});
    REQUIRE(nest_levels == std::vector<int>{0, 0});
  }

  SECTION("Field values")
  {
    REQUIRE(obj.nestedStruct.count == 456);
    REQUIRE(obj.nestedStruct.simpleStruct.integerValue == 4);
    REQUIRE(obj.nestedStruct.simpleStruct.floatValue == 5.0f);
    REQUIRE(obj.nestedStruct.simpleStruct.doubleValue == 6.0);
    REQUIRE(obj.nestedStruct.primitiveTypes.booleanValue == false);
    REQUIRE(obj.nestedStruct.primitiveTypes.characterValue == 'D');
    REQUIRE(obj.nestedStruct.primitiveTypes.shortValue == 789);
    REQUIRE(obj.nestedStruct.primitiveTypes.integerValue == 101112);
    REQUIRE(obj.nestedStruct.primitiveTypes.longValue == 131415L);
    REQUIRE(obj.nestedStruct.primitiveTypes.longLongValue == 161718LL);
    REQUIRE(obj.nestedStruct.primitiveTypes.unsignedCharValue == 'E');
    REQUIRE(obj.nestedStruct.primitiveTypes.unsignedShortValue == 50000);
    REQUIRE(obj.nestedStruct.primitiveTypes.unsignedIntValue == 6000000);
    REQUIRE(obj.nestedStruct.primitiveTypes.unsignedLongValue == 70000000UL);
    REQUIRE(obj.nestedStruct.primitiveTypes.unsignedLongLongValue == 8000000000ULL);
    REQUIRE(obj.nestedStruct.primitiveTypes.floatValue == 3.45f);
    REQUIRE(obj.nestedStruct.primitiveTypes.doubleValue == 6.78);
    REQUIRE(obj.nestedStruct.primitiveTypes.longDoubleValue == 9.01L);
    REQUIRE(obj.simpleStruct.integerValue == 7);
    REQUIRE(obj.simpleStruct.floatValue == 8.0f);
    REQUIRE(obj.simpleStruct.doubleValue == 9.0);
  }
}

TEST_CASE("ComplexTypes struct reflection", "[reflection]")
{
  ComplexTypes obj{
      "Hello, World!",
      {1.1, 2.2, 3.3},
      {10, 20, 30, 40, 50},
      {"pi", 3.14f},
      {{"pi", 3.14f}, {"e", 2.71f}},
      {{1, "one"}, {2, "two"}, {3, "three"}},
      {0.1, 0.2, 0.3, 0.4, 0.5},
      {100, 200.0f, "three hundred"} };

  SECTION("Class name")
  {
    REQUIRE(ReflectionLibrary::get_class_name(obj) == "ComplexTypes");
  }

  SECTION("Field count")
  {
    REQUIRE(ReflectionLibrary::get_field_count(obj) == 8);
  }

  SECTION("Set field value")
  {
    ReflectionLibrary::set_field_value(obj, "stringValue", std::string("Goodbye, World."));
    REQUIRE(obj.stringValue == "Goodbye, World.");
  }

  SECTION("Field values")
  {
    REQUIRE(obj.stringValue == "Hello, World!");
    REQUIRE(obj.doubleArray == std::array<double, 3>{1.1, 2.2, 3.3});
    REQUIRE(obj.intVector == std::vector<int>{10, 20, 30, 40, 50});
    REQUIRE(obj.stringFloatPair == std::pair<std::string, float>{"pi", 3.14f});
    REQUIRE(obj.stringFloatMap == std::map<std::string, float>{{"pi", 3.14f}, { "e", 2.71f }});
    REQUIRE(obj.intStringUnorderedMap == std::unordered_map<int, std::string>{{1, "one"}, { 2, "two" }, { 3, "three" }});
    REQUIRE(obj.doubleList == std::list<double>{0.1, 0.2, 0.3, 0.4, 0.5});
    REQUIRE(obj.intFloatStringTuple == std::tuple<int, float, std::string>{100, 200.0f, "three hundred"});
  }

  SECTION("Field iteration")
  {
    std::vector<std::string> names;
    std::vector<int> nest_levels;
    ReflectionLibrary::for_each_field(obj, [&](std::string_view name, auto& value, int nest_level)
      {
        names.push_back(std::string(name));
        nest_levels.push_back(nest_level); });
    REQUIRE(names.size() == 8);
    REQUIRE(names == std::vector<std::string>{
      "stringValue", "doubleArray", "intVector", "stringFloatPair",
        "stringFloatMap", "intStringUnorderedMap", "doubleList", "intFloatStringTuple"});
    REQUIRE(nest_levels == std::vector<int>(8, 0));
  }
}

TEST_CASE("ContainsNotReflected struct reflection", "[reflection]")
{
  ContainsNotReflected obj;
  obj.notReflectedStruct.xValue = 100;
  obj.notReflectedStruct.yValue = 200.5f;

  SECTION("Class name")
  {
    REQUIRE(ReflectionLibrary::get_class_name(obj) == "ContainsNotReflected");
  }

  SECTION("Field count")
  {
    REQUIRE(ReflectionLibrary::get_field_count(obj) == 2);
  }

  SECTION("Set field value")
  {
    ReflectionLibrary::set_field_value(obj, "aValue", 150);
    REQUIRE(obj.notReflectedStruct.xValue == 150);
    ReflectionLibrary::set_field_value(obj, "bValue", 250.5f);
    REQUIRE(obj.notReflectedStruct.yValue == 250.5f);
  }

  SECTION("Field values")
  {
    REQUIRE(obj.notReflectedStruct.xValue == 100);
    REQUIRE(obj.notReflectedStruct.yValue == 200.5f);
  }

  SECTION("Field iteration")
  {
    std::vector<std::string> names;
    std::vector<int> nest_levels;
    ReflectionLibrary::for_each_field(obj, [&](std::string_view name, auto& value, int nest_level)
      {
        names.push_back(std::string(name));
        nest_levels.push_back(nest_level); });
    REQUIRE(names.size() == 2);
    REQUIRE(names == std::vector<std::string>{"aValue", "bValue"});
    REQUIRE(nest_levels == std::vector<int>{0, 0}); // All top-level fields
  }
}

TEST_CASE("EmptyStruct reflection", "[reflection]")
{
  EmptyStruct obj;

  SECTION("Class name")
  {
    REQUIRE(ReflectionLibrary::get_class_name(obj) == "EmptyStruct");
    REQUIRE(ReflectionLibrary::class_name<EmptyStruct> == std::string("EmptyStruct"));
  }

  SECTION("Field count")
  {
    REQUIRE(ReflectionLibrary::get_field_count(obj) == 0);
    REQUIRE(ReflectionLibrary::field_count<EmptyStruct> == 0);
  }

  SECTION("Field iteration")
  {
    int count = 0;
    ReflectionLibrary::for_each_field(obj, [&](std::string_view name, auto& value, int nest_level)
      {
        count++; });
    REQUIRE(count == 0); // No fields to iterate
  }
}