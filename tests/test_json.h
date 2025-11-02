#pragma once

#include "../include/ReflectionLibrary.h"
#include "../include/ReflectionLibrary/Json.h"
#include "types.h"
#include <catch2/catch_all.hpp>

using namespace ReflectionLibrary;

TEST_CASE("JSON serialize/deserialize Simple struct", "[json]")
{
  Simple original{ 42, 3.14f, 2.71 };

  SECTION("Serialize")
  {
    std::string json = ReflectionLibrary::to_json(original);
    REQUIRE_FALSE(json.empty());
    REQUIRE_THAT(json, Catch::Matchers::ContainsSubstring("integerValue"));
    REQUIRE_THAT(json, Catch::Matchers::ContainsSubstring("42"));
  }

  SECTION("Roundtrip")
  {
    std::string json = ReflectionLibrary::to_json(original);
    auto result = ReflectionLibrary::from_json<Simple>(json);

    REQUIRE(result.has_value());
    REQUIRE(result->integerValue == 42);
    REQUIRE(result->floatValue == 3.14f);
    REQUIRE(result->doubleValue == 2.71);
  }
}

TEST_CASE("JSON serialize/deserialize AllPrimitiveTypes struct", "[json]")
{
  AllPrimitiveTypes original{
      true, 'A', 123, 456, 789L, 101112LL,
      'B', 234, 567, 890UL, 111213ULL,
      1.23f, 4.56, 7.89L };

  SECTION("Roundtrip")
  {
    std::string json = ReflectionLibrary::to_json(original);
    auto result = ReflectionLibrary::from_json<AllPrimitiveTypes>(json);

    REQUIRE(result.has_value());
    REQUIRE(result->booleanValue == true);
    REQUIRE(result->characterValue == 'A');
    REQUIRE(result->shortValue == 123);
    REQUIRE(result->integerValue == 456);
    REQUIRE(result->longValue == 789L);
    REQUIRE(result->longLongValue == 101112LL);
    REQUIRE(result->unsignedCharValue == 'B');
    REQUIRE(result->unsignedShortValue == 234);
    REQUIRE(result->unsignedIntValue == 567);
    REQUIRE(result->unsignedLongValue == 890UL);
    REQUIRE(result->unsignedLongLongValue == 111213ULL);
    REQUIRE(result->floatValue == Catch::Approx(1.23f));
    REQUIRE(result->doubleValue == Catch::Approx(4.56));
    REQUIRE(result->longDoubleValue == Catch::Approx(7.89L));
  }
}

TEST_CASE("JSON serialize/deserialize Nested struct", "[json]")
{
  Nested original{
      123,
      {1, 2.0f, 3.0},
      {true, 'C', 456, 789, 101112L, 131415LL, 'D', 40000, 5000000, 60000000UL, 7000000000ULL, 2.34f, 5.67, 8.90L} };

  SECTION("Roundtrip")
  {
    std::string json = ReflectionLibrary::to_json(original);
    auto result = ReflectionLibrary::from_json<Nested>(json);

    REQUIRE(result.has_value());
    REQUIRE(result->count == 123);
    REQUIRE(result->simpleStruct.integerValue == 1);
    REQUIRE(result->simpleStruct.floatValue == 2.0f);
    REQUIRE(result->simpleStruct.doubleValue == 3.0);
    REQUIRE(result->primitiveTypes.booleanValue == true);
    REQUIRE(result->primitiveTypes.characterValue == 'C');
    REQUIRE(result->primitiveTypes.shortValue == 456);
  }
}

TEST_CASE("JSON serialize/deserialize DeeplyNested struct", "[json]")
{
  DeeplyNested original{
      {456, {4, 5.0f, 6.0}, {false, 'D', 789, 101112, 131415L, 161718LL, 'E', 50000, 6000000, 70000000UL, 8000000000ULL, 3.45f, 6.78, 9.01L}},
      {7, 8.0f, 9.0} };

  SECTION("Roundtrip")
  {
    std::string json = ReflectionLibrary::to_json(original);
    auto result = ReflectionLibrary::from_json<DeeplyNested>(json);

    REQUIRE(result.has_value());
    REQUIRE(result->nestedStruct.count == 456);
    REQUIRE(result->nestedStruct.simpleStruct.integerValue == 4);
    REQUIRE(result->nestedStruct.simpleStruct.floatValue == 5.0f);
    REQUIRE(result->nestedStruct.primitiveTypes.booleanValue == false);
    REQUIRE(result->simpleStruct.integerValue == 7);
    REQUIRE(result->simpleStruct.floatValue == 8.0f);
  }
}

TEST_CASE("JSON serialize/deserialize ComplexTypes struct", "[json]")
{
  ComplexTypes original{
      "Hello, World!",
      {1.1, 2.2, 3.3},
      {10, 20, 30, 40, 50},
      {"pi", 3.14f},
      {{"pi", 3.14f}, {"e", 2.71f}},
      {{1, "one"}, {2, "two"}, {3, "three"}},
      {0.1, 0.2, 0.3, 0.4, 0.5},
      {100, 200.0f, "three hundred"} };

  SECTION("Serialize")
  {
    std::string json = ReflectionLibrary::to_json(original);
    REQUIRE_FALSE(json.empty());
    REQUIRE_THAT(json, Catch::Matchers::ContainsSubstring("Hello, World!"));
  }

  SECTION("Roundtrip")
  {
    std::string json = ReflectionLibrary::to_json(original);
    auto result = ReflectionLibrary::from_json<ComplexTypes>(json);

    REQUIRE(result.has_value());
    REQUIRE(result->stringValue == "Hello, World!");
    REQUIRE(result->doubleArray[0] == Catch::Approx(1.1));
    REQUIRE(result->doubleArray[1] == Catch::Approx(2.2));
    REQUIRE(result->doubleArray[2] == Catch::Approx(3.3));
    REQUIRE(result->intVector.size() == 5);
    REQUIRE(result->intVector[0] == 10);
    REQUIRE(result->intVector[4] == 50);
    REQUIRE(result->stringFloatPair.first == "pi");
    REQUIRE(result->stringFloatPair.second == Catch::Approx(3.14f));
    REQUIRE(result->stringFloatMap.size() == 2);
    REQUIRE(result->stringFloatMap["pi"] == Catch::Approx(3.14f));
    REQUIRE(result->stringFloatMap["e"] == Catch::Approx(2.71f));
    REQUIRE(result->intStringUnorderedMap.size() == 3);
    REQUIRE(result->intStringUnorderedMap[1] == "one");
    REQUIRE(result->intStringUnorderedMap[2] == "two");
    REQUIRE(result->doubleList.size() == 5);
    REQUIRE(std::get<0>(result->intFloatStringTuple) == 100);
    REQUIRE(std::get<1>(result->intFloatStringTuple) == Catch::Approx(200.0f));
    REQUIRE(std::get<2>(result->intFloatStringTuple) == "three hundred");
  }
}

TEST_CASE("JSON serialize/deserialize NestedComplex struct", "[json]")
{
  NestedComplex original{
      {{"a", "b"}, {"c", "d", "e"}},
      {{{1, 2, 3}, {4, 5, 6}}},
      {{{Simple{10, 1.0f, 2.0}, Simple{20, 3.0f, 4.0}}, {Simple{30, 5.0f, 6.0}}}} };

  SECTION("Roundtrip")
  {
    std::string json = ReflectionLibrary::to_json(original);
    auto result = ReflectionLibrary::from_json<NestedComplex>(json);

    REQUIRE(result.has_value());
    REQUIRE(result->vectorOfVectors.size() == 2);
    REQUIRE(result->vectorOfVectors[0].size() == 2);
    REQUIRE(result->vectorOfVectors[0][0] == "a");
    REQUIRE(result->vectorOfVectors[1][2] == "e");

    REQUIRE(result->arrayOfArrays[0][0] == 1);
    REQUIRE(result->arrayOfArrays[1][2] == 6);

    REQUIRE(result->arrayOfVectorsOfStructs[0].size() == 2);
    REQUIRE(result->arrayOfVectorsOfStructs[0][0].integerValue == 10);
    REQUIRE(result->arrayOfVectorsOfStructs[1][0].integerValue == 30);
  }
}

TEST_CASE("JSON serialize/deserialize ContainsNotReflected struct", "[json]")
{
  ContainsNotReflected original;
  original.notReflectedStruct.xValue = 100;
  original.notReflectedStruct.yValue = 200.5f;

  SECTION("Serialize")
  {
    std::string json = ReflectionLibrary::to_json(original);
    REQUIRE_FALSE(json.empty());
    REQUIRE_THAT(json, Catch::Matchers::ContainsSubstring("aValue"));
    REQUIRE_THAT(json, Catch::Matchers::ContainsSubstring("100"));
    REQUIRE_THAT(json, Catch::Matchers::ContainsSubstring("bValue"));
    REQUIRE_THAT(json, Catch::Matchers::ContainsSubstring("200.5"));
  }

  SECTION("Roundtrip")
  {
    std::string json = ReflectionLibrary::to_json(original);
    auto result = ReflectionLibrary::from_json<ContainsNotReflected>(json);

    REQUIRE(result.has_value());
    REQUIRE(result->notReflectedStruct.xValue == 100);
    REQUIRE(result->notReflectedStruct.yValue == Catch::Approx(200.5f));
  }
}

TEST_CASE("JSON error handling", "[json]")
{
  SECTION("Invalid JSON")
  {
    std::string invalid_json = "{ invalid json }";
    auto result = ReflectionLibrary::from_json<Simple>(invalid_json);
    REQUIRE_FALSE(result.has_value());
  }

  SECTION("Empty JSON")
  {
    std::string empty_json = "";
    auto result = ReflectionLibrary::from_json<Simple>(empty_json);
    REQUIRE_FALSE(result.has_value());
  }

  SECTION("Wrong type in JSON")
  {
    std::string json = R"({"integerValue": "not_a_number", "floatValue": 3.14, "doubleValue": 2.71})";
    auto result = ReflectionLibrary::from_json<Simple>(json);
    // Should fail due to type mismatch
    REQUIRE_FALSE(result.has_value());
  }

  SECTION("Missing fields")
  {
    std::string json = R"({"integerValue": 42})";
    auto result = ReflectionLibrary::from_json<Simple>(json);
    // Should succeed with default values for missing fields
    REQUIRE(result.has_value());
    REQUIRE(result->integerValue == 42);
    REQUIRE(result->floatValue == 0.0f);
    REQUIRE(result->doubleValue == 0.0);
  }
}

TEST_CASE("JSON edge cases", "[json]")
{
  SECTION("Empty string")
  {
    ComplexTypes obj;
    obj.stringValue = "";

    std::string json = ReflectionLibrary::to_json(obj);
    auto result = ReflectionLibrary::from_json<ComplexTypes>(json);

    REQUIRE(result.has_value());
    REQUIRE(result->stringValue == "");
  }

  SECTION("Empty containers")
  {
    ComplexTypes obj;
    obj.intVector.clear();
    obj.doubleList.clear();
    obj.stringFloatMap.clear();

    std::string json = ReflectionLibrary::to_json(obj);
    auto result = ReflectionLibrary::from_json<ComplexTypes>(json);

    REQUIRE(result.has_value());
    REQUIRE(result->intVector.empty());
    REQUIRE(result->doubleList.empty());
    REQUIRE(result->stringFloatMap.empty());
  }

  SECTION("Boolean values")
  {
    AllPrimitiveTypes obj_true;
    obj_true.booleanValue = true;

    AllPrimitiveTypes obj_false;
    obj_false.booleanValue = false;

    std::string json_true = ReflectionLibrary::to_json(obj_true);
    std::string json_false = ReflectionLibrary::to_json(obj_false);

    auto result_true = ReflectionLibrary::from_json<AllPrimitiveTypes>(json_true);
    auto result_false = ReflectionLibrary::from_json<AllPrimitiveTypes>(json_false);

    REQUIRE(result_true.has_value());
    REQUIRE(result_true->booleanValue == true);
    REQUIRE(result_false.has_value());
    REQUIRE(result_false->booleanValue == false);
  }

  SECTION("Special characters in strings")
  {
    ComplexTypes obj;
    obj.stringValue = "Test\n\t\"quotes\"\\backslash";

    std::string json = ReflectionLibrary::to_json(obj);
    auto result = ReflectionLibrary::from_json<ComplexTypes>(json);

    REQUIRE(result.has_value());
    REQUIRE(result->stringValue == "Test\n\t\"quotes\"\\backslash");
  }
}


TEST_CASE("GenericValue JSON serialization", "[value][json]")
{
  SECTION("Null value")
  {
    GenericValue v;
    std::string json = generic_value_to_json(v, false);
    REQUIRE(json == "null");

    auto restored = generic_value_from_json(json);
    REQUIRE(restored.has_value());
    REQUIRE(restored->isNull());
  }

  SECTION("Boolean values")
  {
    GenericValue v_true(true);
    GenericValue v_false(false);

    std::string json_true = generic_value_to_json(v_true, false);
    std::string json_false = generic_value_to_json(v_false, false);

    REQUIRE(json_true == "true");
    REQUIRE(json_false == "false");

    auto restored_true = generic_value_from_json(json_true);
    auto restored_false = generic_value_from_json(json_false);

    REQUIRE(restored_true.has_value());
    REQUIRE(restored_true->getBool() == true);
    REQUIRE(restored_false.has_value());
    REQUIRE(restored_false->getBool() == false);
  }

  SECTION("Integer values")
  {
    GenericValue v(42);
    std::string json = generic_value_to_json(v, false);
    REQUIRE(json == "42");

    auto restored = generic_value_from_json(json);
    REQUIRE(restored.has_value());
    REQUIRE(restored->isInt());
    REQUIRE(restored->getInt() == 42);
  }

  SECTION("Double values")
  {
    GenericValue v(3.14);
    std::string json = generic_value_to_json(v, false);

    auto restored = generic_value_from_json(json);
    REQUIRE(restored.has_value());
    REQUIRE(restored->isDouble());
    REQUIRE(restored->getDouble() == Catch::Approx(3.14));
  }

  SECTION("String values")
  {
    GenericValue v("hello world");
    std::string json = generic_value_to_json(v, false);
    REQUIRE(json == "\"hello world\"");

    auto restored = generic_value_from_json(json);
    REQUIRE(restored.has_value());
    REQUIRE(restored->isString());
    REQUIRE(restored->getString() == "hello world");
  }

  SECTION("String with special characters")
  {
    GenericValue v("Line 1\nLine 2\tTabbed");
    std::string json = generic_value_to_json(v, false);

    auto restored = generic_value_from_json(json);
    REQUIRE(restored.has_value());
    REQUIRE(restored->getString() == "Line 1\nLine 2\tTabbed");
  }

  SECTION("Array values")
  {
    GenericValue v;
    v.push_back(GenericValue(1));
    v.push_back(GenericValue(2));
    v.push_back(GenericValue(3));

    std::string json = generic_value_to_json(v, false);

    auto restored = generic_value_from_json(json);
    REQUIRE(restored.has_value());
    REQUIRE(restored->isArray());
    REQUIRE(restored->size() == 3);
    REQUIRE((*restored)[0].getInt() == 1);
    REQUIRE((*restored)[1].getInt() == 2);
    REQUIRE((*restored)[2].getInt() == 3);
  }

  SECTION("Object values")
  {
    GenericValue v;
    v["name"] = "John";
    v["age"] = 30;
    v["active"] = true;

    std::string json = generic_value_to_json(v, false);

    auto restored = generic_value_from_json(json);
    REQUIRE(restored.has_value());
    REQUIRE(restored->isObject());
    REQUIRE((*restored)["name"].getString() == "John");
    REQUIRE((*restored)["age"].getInt() == 30);
    REQUIRE((*restored)["active"].getBool() == true);
  }

  SECTION("Mixed type array")
  {
    GenericValue v;
    v.push_back(GenericValue(42));
    v.push_back(GenericValue("string"));
    v.push_back(GenericValue(3.14));
    v.push_back(GenericValue(true));
    v.push_back(GenericValue());

    std::string json = generic_value_to_json(v, false);

    auto restored = generic_value_from_json(json);
    REQUIRE(restored.has_value());
    REQUIRE((*restored)[0].isInt());
    REQUIRE((*restored)[1].isString());
    REQUIRE((*restored)[2].isDouble());
    REQUIRE((*restored)[3].isBool());
    REQUIRE((*restored)[4].isNull());
  }
}

TEST_CASE("GenericValue JSON nested structures", "[value][json]")
{
  SECTION("Nested objects")
  {
    GenericValue v;
    v["person"]["name"] = "Alice";
    v["person"]["age"] = 25;
    v["person"]["address"]["city"] = "New York";
    v["person"]["address"]["zip"] = 10001;

    std::string json = generic_value_to_json(v, false);

    auto restored = generic_value_from_json(json);
    REQUIRE(restored.has_value());
    REQUIRE((*restored)["person"]["name"].getString() == "Alice");
    REQUIRE((*restored)["person"]["age"].getInt() == 25);
    REQUIRE((*restored)["person"]["address"]["city"].getString() == "New York");
    REQUIRE((*restored)["person"]["address"]["zip"].getInt() == 10001);
  }

  SECTION("Array of objects")
  {
    GenericValue v;
    v[0]["name"] = "Alice";
    v[0]["age"] = 25;
    v[1]["name"] = "Bob";
    v[1]["age"] = 30;

    std::string json = generic_value_to_json(v, false);

    auto restored = generic_value_from_json(json);
    REQUIRE(restored.has_value());
    REQUIRE(restored->isArray());
    REQUIRE((*restored)[0]["name"].getString() == "Alice");
    REQUIRE((*restored)[1]["name"].getString() == "Bob");
  }

  SECTION("Object with arrays")
  {
    GenericValue v;
    v["numbers"][0] = 1;
    v["numbers"][1] = 2;
    v["numbers"][2] = 3;
    v["strings"][0] = "a";
    v["strings"][1] = "b";

    std::string json = generic_value_to_json(v, false);

    auto restored = generic_value_from_json(json);
    REQUIRE(restored.has_value());
    REQUIRE((*restored)["numbers"].size() == 3);
    REQUIRE((*restored)["strings"].size() == 2);
    REQUIRE((*restored)["numbers"][0].getInt() == 1);
    REQUIRE((*restored)["strings"][0].getString() == "a");
  }

  SECTION("Deep nesting")
  {
    GenericValue v;
    v["a"]["b"]["c"]["d"]["e"]["f"] = "deep value";

    std::string json = generic_value_to_json(v, false);

    auto restored = generic_value_from_json(json);
    REQUIRE(restored.has_value());
    REQUIRE((*restored)["a"]["b"]["c"]["d"]["e"]["f"].getString() == "deep value");
  }
}

TEST_CASE("GenericValue JSON pretty printing", "[value][json]")
{
  SECTION("Pretty vs compact")
  {
    GenericValue v;
    v["name"] = "John";
    v["age"] = 30;
    v["items"][0] = "item1";
    v["items"][1] = "item2";

    std::string compact = generic_value_to_json(v, false);
    std::string pretty = generic_value_to_json(v, true);

    // Pretty should have more characters due to newlines and spaces
    REQUIRE(pretty.length() > compact.length());
    REQUIRE(pretty.find('\n') != std::string::npos);

    // Both should parse to the same value
    auto restored_compact = generic_value_from_json(compact);
    auto restored_pretty = generic_value_from_json(pretty);

    REQUIRE(restored_compact.has_value());
    REQUIRE(restored_pretty.has_value());
    REQUIRE((*restored_compact)["name"].getString() == (*restored_pretty)["name"].getString());
    REQUIRE((*restored_compact)["age"].getInt() == (*restored_pretty)["age"].getInt());
  }
}

TEST_CASE("GenericValue JSON error handling", "[value][json]")
{
  SECTION("Invalid JSON")
  {
    auto result = generic_value_from_json("{ invalid json }");
    REQUIRE_FALSE(result.has_value());
  }

  SECTION("Empty string")
  {
    auto result = generic_value_from_json("");
    REQUIRE_FALSE(result.has_value());
  }

  SECTION("Truncated JSON")
  {
    auto result = generic_value_from_json("{\"name\":\"John\"");
    REQUIRE_FALSE(result.has_value());
  }

  SECTION("Invalid escape sequences")
  {
    auto result = generic_value_from_json("\"invalid\\xescape\"");
    REQUIRE_FALSE(result.has_value());
  }
}

TEST_CASE("GenericValue JSON roundtrip with structs", "[value][json]")
{
  SECTION("Simple struct")
  {
    Simple original{ 42, 3.14f, 2.71 };
    GenericValue v = to_generic_value(original);

    std::string json = generic_value_to_json(v, false);
    auto restored_value = generic_value_from_json(json);

    REQUIRE(restored_value.has_value());

    Simple restored;
    REQUIRE(from_generic_value(restored, *restored_value));
    REQUIRE(restored.integerValue == 42);
    REQUIRE(restored.floatValue == Catch::Approx(3.14f));
    REQUIRE(restored.doubleValue == Catch::Approx(2.71));
  }

  SECTION("Nested struct")
  {
    Nested original{
        123,
        {1, 2.0f, 3.0},
        {true, 'C', 456, 789, 101112L, 131415LL, 'D', 40000, 5000000,
         60000000UL, 7000000000ULL, 2.34f, 5.67, 8.90L} };

    GenericValue v = to_generic_value(original);
    std::string json = generic_value_to_json(v, true);

    auto restored_value = generic_value_from_json(json);
    REQUIRE(restored_value.has_value());

    Nested restored;
    REQUIRE(from_generic_value(restored, *restored_value));
    REQUIRE(restored.count == 123);
    REQUIRE(restored.simpleStruct.integerValue == 1);
    REQUIRE(restored.primitiveTypes.booleanValue == true);
    REQUIRE(restored.primitiveTypes.characterValue == 'C');
  }

  SECTION("ComplexTypes")
  {
    ComplexTypes original{
        "Hello, World!",
        {1.1, 2.2, 3.3},
        {10, 20, 30, 40, 50},
        {"pi", 3.14f},
        {{"pi", 3.14f}, {"e", 2.71f}},
        {{1, "one"}, {2, "two"}, {3, "three"}},
        {0.1, 0.2, 0.3, 0.4, 0.5},
        {100, 200.0f, "three hundred"} };

    GenericValue v = to_generic_value(original);
    std::string json = generic_value_to_json(v, true);

    auto restored_value = generic_value_from_json(json);
    REQUIRE(restored_value.has_value());

    ComplexTypes restored;
    REQUIRE(from_generic_value(restored, *restored_value));
    REQUIRE(restored.stringValue == "Hello, World!");
    REQUIRE(restored.intVector.size() == 5);
    REQUIRE(restored.intVector[0] == 10);
  }
}

TEST_CASE("GenericValue JSON special cases", "[value][json]")
{
  SECTION("Empty array")
  {
    GenericValue v(Array{});
    std::string json = generic_value_to_json(v, false);
    REQUIRE(json == "[]");

    auto restored = generic_value_from_json(json);
    REQUIRE(restored.has_value());
    REQUIRE(restored->isArray());
    REQUIRE(restored->size() == 0);
  }

  SECTION("Empty object")
  {
    GenericValue v(Object{});
    std::string json = generic_value_to_json(v, false);
    REQUIRE(json == "{}");

    auto restored = generic_value_from_json(json);
    REQUIRE(restored.has_value());
    REQUIRE(restored->isObject());
    REQUIRE(restored->size() == 0);
  }

  SECTION("Large numbers")
  {
    GenericValue v(9223372036854775807LL); // max int64_t
    std::string json = generic_value_to_json(v, false);

    auto restored = generic_value_from_json(json);
    REQUIRE(restored.has_value());
    REQUIRE(restored->getInt() == 9223372036854775807LL);
  }

  SECTION("Negative numbers")
  {
    GenericValue v(-12345);
    std::string json = generic_value_to_json(v, false);

    auto restored = generic_value_from_json(json);
    REQUIRE(restored.has_value());
    REQUIRE(restored->getInt() == -12345);
  }

  SECTION("Zero values")
  {
    GenericValue v_int(0);
    GenericValue v_double(0.0);

    auto restored_int = generic_value_from_json(generic_value_to_json(v_int, false));
    auto restored_double = generic_value_from_json(generic_value_to_json(v_double, false));

    REQUIRE(restored_int.has_value());
    REQUIRE(restored_int->getInt() == 0);
    REQUIRE(restored_double.has_value());
    REQUIRE(restored_double->getDouble() == 0.0);
  }
}

TEST_CASE("GenericValue JSON configuration example", "[value][json]")
{
  SECTION("Configuration file simulation")
  {
    GenericValue config;
    config["server"]["host"] = "localhost";
    config["server"]["port"] = 8080;
    config["server"]["ssl"]["enabled"] = true;
    config["server"]["ssl"]["cert_path"] = "/etc/ssl/cert.pem";
    config["database"]["host"] = "db.example.com";
    config["database"]["port"] = 5432;
    config["database"]["name"] = "myapp";
    config["features"][0] = "authentication";
    config["features"][1] = "logging";
    config["features"][2] = "monitoring";
    config["debug"] = false;

    std::string json = generic_value_to_json(config, true);

    // Save and reload
    auto loaded = generic_value_from_json(json);
    REQUIRE(loaded.has_value());

    REQUIRE((*loaded)["server"]["host"].getString() == "localhost");
    REQUIRE((*loaded)["server"]["port"].getInt() == 8080);
    REQUIRE((*loaded)["server"]["ssl"]["enabled"].getBool() == true);
    REQUIRE((*loaded)["database"]["name"].getString() == "myapp");
    REQUIRE((*loaded)["features"].size() == 3);
    REQUIRE((*loaded)["debug"].getBool() == false);
  }
}

TEST_CASE("JSON serialize/deserialize EmptyStruct", "[json]")
{
  EmptyStruct original;

  SECTION("Serialize")
  {
    std::string json = ReflectionLibrary::to_json(original);
    REQUIRE_FALSE(json.empty());
    // Empty struct should serialize to empty object
    REQUIRE(json == "{}");
  }

  SECTION("Roundtrip")
  {
    std::string json = ReflectionLibrary::to_json(original);
    auto result = ReflectionLibrary::from_json<EmptyStruct>(json);

    REQUIRE(result.has_value());
  }

  SECTION("Deserialize from empty object")
  {
    std::string json = "{}";
    auto result = ReflectionLibrary::from_json<EmptyStruct>(json);

    REQUIRE(result.has_value());
  }
}