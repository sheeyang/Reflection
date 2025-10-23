#pragma once

#include "../include/ReflectionLibrary.h"
#include "../include/ReflectionLibrary/GenericValue.h"
#include "types.h"
#include <catch2/catch_all.hpp>

using namespace ReflectionLibrary;

TEST_CASE("GenericValue basic type construction", "[value]")
{
  SECTION("Null value")
  {
    GenericValue v;
    REQUIRE(v.isNull());
    REQUIRE_FALSE(v.isBool());
    REQUIRE_FALSE(v.isInt());
  }

  SECTION("Boolean value")
  {
    GenericValue v_true(true);
    GenericValue v_false(false);
    REQUIRE(v_true.isBool());
    REQUIRE(v_true.getBool() == true);
    REQUIRE(v_false.getBool() == false);
  }

  SECTION("Integer values")
  {
    GenericValue v1(42);
    GenericValue v2(static_cast<int64_t>(123456789));
    GenericValue v3(static_cast<unsigned int>(999));
    REQUIRE(v1.isInt());
    REQUIRE(v1.getInt() == 42);
    REQUIRE(v2.getInt() == 123456789);
    REQUIRE(v3.getInt() == 999);
  }

  SECTION("Double values")
  {
    GenericValue v(3.14);
    REQUIRE(v.isDouble());
    REQUIRE(v.getDouble() == Catch::Approx(3.14));
  }

  SECTION("String values")
  {
    GenericValue v1("hello");
    GenericValue v2(std::string("world"));
    REQUIRE(v1.isString());
    REQUIRE(v2.isString());
    REQUIRE(v1.getString() == "hello");
    REQUIRE(v2.getString() == "world");
  }

  SECTION("Array values")
  {
    Array arr = { GenericValue(1), GenericValue(2), GenericValue(3) };
    GenericValue v(arr);
    REQUIRE(v.isArray());
    REQUIRE(v.size() == 3);
  }

  SECTION("Object values")
  {
    Object obj;
    obj["key"] = GenericValue("value");
    GenericValue v(obj);
    REQUIRE(v.isObject());
    REQUIRE(v.has("key"));
  }
}

TEST_CASE("GenericValue proxy implicit conversions", "[value]")
{
  GenericValue v;

  SECTION("Direct int assignment")
  {
    v["age"] = 30;
    REQUIRE(v["age"].isInt());
    REQUIRE(v["age"].getInt() == 30);
  }

  SECTION("Direct string assignment")
  {
    v["name"] = "John";
    REQUIRE(v["name"].isString());
    REQUIRE(v["name"].getString() == "John");
  }

  SECTION("Direct double assignment")
  {
    v["height"] = 5.9;
    REQUIRE(v["height"].isDouble());
    REQUIRE(v["height"].getDouble() == Catch::Approx(5.9));
  }

  SECTION("Direct bool assignment")
  {
    v["employed"] = true;
    REQUIRE(v["employed"].isBool());
    REQUIRE(v["employed"].getBool() == true);
  }

  SECTION("Mixed types")
  {
    v["int"] = 42;
    v["string"] = "test";
    v["double"] = 3.14;
    v["bool"] = false;

    REQUIRE(v["int"].getInt() == 42);
    REQUIRE(v["string"].getString() == "test");
    REQUIRE(v["double"].getDouble() == Catch::Approx(3.14));
    REQUIRE(v["bool"].getBool() == false);
  }
}

TEST_CASE("GenericValue nested access", "[value]")
{
  GenericValue v;

  SECTION("Nested objects")
  {
    v["level1"]["level2"]["level3"] = 42;
    REQUIRE(v["level1"]["level2"]["level3"].getInt() == 42);
  }

  SECTION("Object with array")
  {
    v["data"][0] = "first";
    v["data"][1] = "second";
    v["data"][2] = "third";

    REQUIRE(v["data"][0].getString() == "first");
    REQUIRE(v["data"][1].getString() == "second");
    REQUIRE(v["data"][2].getString() == "third");
    REQUIRE(v["data"].size() == 3);
  }

  SECTION("Array of objects")
  {
    v["users"][0]["name"] = "Alice";
    v["users"][0]["age"] = 25;
    v["users"][1]["name"] = "Bob";
    v["users"][1]["age"] = 30;

    REQUIRE(v["users"][0]["name"].getString() == "Alice");
    REQUIRE(v["users"][0]["age"].getInt() == 25);
    REQUIRE(v["users"][1]["name"].getString() == "Bob");
    REQUIRE(v["users"][1]["age"].getInt() == 30);
  }

  SECTION("Deep nesting")
  {
    v["a"]["b"]["c"]["d"]["e"]["f"] = "deep";
    REQUIRE(v["a"]["b"]["c"]["d"]["e"]["f"].getString() == "deep");
  }
}

TEST_CASE("GenericValue array operations", "[value]")
{
  GenericValue v;

  SECTION("Push back")
  {
    v.push_back(GenericValue(1));
    v.push_back(GenericValue(2));
    v.push_back(GenericValue(3));

    REQUIRE(v.isArray());
    REQUIRE(v.size() == 3);
    REQUIRE(v[0].getInt() == 1);
    REQUIRE(v[1].getInt() == 2);
    REQUIRE(v[2].getInt() == 3);
  }

  SECTION("Array index assignment")
  {
    v.push_back(GenericValue(0));
    v.push_back(GenericValue(0));
    v.push_back(GenericValue(0));

    v[0] = 10;
    v[1] = 20;
    v[2] = 30;

    REQUIRE(v[0].getInt() == 10);
    REQUIRE(v[1].getInt() == 20);
    REQUIRE(v[2].getInt() == 30);
  }

  SECTION("Mixed type array")
  {
    v.push_back(GenericValue(42));
    v.push_back(GenericValue("string"));
    v.push_back(GenericValue(3.14));
    v.push_back(GenericValue(true));

    REQUIRE(v[0].isInt());
    REQUIRE(v[1].isString());
    REQUIRE(v[2].isDouble());
    REQUIRE(v[3].isBool());
  }
}

TEST_CASE("GenericValue to_value conversions", "[value]")
{
  SECTION("Simple struct")
  {
    Simple s{ 42, 3.14f, 2.71 };
    GenericValue v = to_value(s);

    REQUIRE(v.isObject());
    REQUIRE(v["integerValue"].getInt() == 42);
    REQUIRE(v["floatValue"].getDouble() == Catch::Approx(3.14f));
    REQUIRE(v["doubleValue"].getDouble() == Catch::Approx(2.71));
  }

  SECTION("Nested struct")
  {
    Nested n{ 123, {1, 2.0f, 3.0}, {true, 'C', 456, 789, 101112L, 131415LL, 'D', 40000, 5000000, 60000000UL, 7000000000ULL, 2.34f, 5.67, 8.90L} };
    GenericValue v = to_value(n);

    REQUIRE(v.isObject());
    REQUIRE(v["count"].getInt() == 123);
    REQUIRE(v["simpleStruct"]["integerValue"].getInt() == 1);
    REQUIRE(v["primitiveTypes"]["booleanValue"].getBool() == true);
  }

  SECTION("Vector")
  {
    std::vector<int> vec = { 10, 20, 30, 40, 50 };
    GenericValue v = to_value(vec);

    REQUIRE(v.isArray());
    REQUIRE(v.size() == 5);
    REQUIRE(v[0].getInt() == 10);
    REQUIRE(v[4].getInt() == 50);
  }

  SECTION("Map")
  {
    std::map<std::string, int> map = { {"one", 1}, {"two", 2}, {"three", 3} };
    GenericValue v = to_value(map);

    REQUIRE(v.isArray());
    REQUIRE(v.size() == 3);
  }

  SECTION("Pair")
  {
    std::pair<std::string, float> p = { "pi", 3.14f };
    GenericValue v = to_value(p);

    REQUIRE(v.isObject());
    REQUIRE(v["first"].getString() == "pi");
    REQUIRE(v["second"].getDouble() == Catch::Approx(3.14f));
  }

  SECTION("Tuple")
  {
    std::tuple<int, float, std::string> t = { 100, 200.0f, "three hundred" };
    GenericValue v = to_value(t);

    REQUIRE(v.isArray());
    REQUIRE(v.size() == 3);
    REQUIRE(v[0].getInt() == 100);
    REQUIRE(v[1].getDouble() == Catch::Approx(200.0f));
    REQUIRE(v[2].getString() == "three hundred");
  }
}

TEST_CASE("GenericValue from_value conversions", "[value]")
{
  SECTION("Simple struct roundtrip")
  {
    Simple original{ 42, 3.14f, 2.71 };
    GenericValue v = to_value(original);

    Simple restored;
    REQUIRE(from_value(restored, v));
    REQUIRE(restored.integerValue == 42);
    REQUIRE(restored.floatValue == Catch::Approx(3.14f));
    REQUIRE(restored.doubleValue == Catch::Approx(2.71));
  }

  SECTION("Nested struct roundtrip")
  {
    Nested original{ 123, {1, 2.0f, 3.0}, {true, 'C', 456, 789, 101112L, 131415LL, 'D', 40000, 5000000, 60000000UL, 7000000000ULL, 2.34f, 5.67, 8.90L} };
    GenericValue v = to_value(original);

    Nested restored;
    REQUIRE(from_value(restored, v));
    REQUIRE(restored.count == 123);
    REQUIRE(restored.simpleStruct.integerValue == 1);
    REQUIRE(restored.primitiveTypes.booleanValue == true);
    REQUIRE(restored.primitiveTypes.characterValue == 'C');
  }

  SECTION("Vector roundtrip")
  {
    std::vector<int> original = { 10, 20, 30, 40, 50 };
    GenericValue v = to_value(original);

    std::vector<int> restored;
    REQUIRE(from_value(restored, v));
    REQUIRE(restored == original);
  }

  SECTION("Primitive types")
  {
    GenericValue v_int(42);
    int i;
    REQUIRE(from_value(i, v_int));
    REQUIRE(i == 42);

    GenericValue v_str("hello");
    std::string s;
    REQUIRE(from_value(s, v_str));
    REQUIRE(s == "hello");

    GenericValue v_bool(true);
    bool b;
    REQUIRE(from_value(b, v_bool));
    REQUIRE(b == true);
  }
}

TEST_CASE("GenericValue modify through proxy", "[value]")
{
  SECTION("Modify nested values")
  {
    Simple s{ 42, 3.14f, 2.71 };
    GenericValue v = to_value(s);

    v["integerValue"] = 100;
    v["floatValue"] = 9.99;
    v["doubleValue"] = 1.23;

    REQUIRE(v["integerValue"].getInt() == 100);
    REQUIRE(v["floatValue"].getDouble() == Catch::Approx(9.99));
    REQUIRE(v["doubleValue"].getDouble() == Catch::Approx(1.23));

    Simple restored;
    from_value(restored, v);
    REQUIRE(restored.integerValue == 100);
    REQUIRE(restored.floatValue == Catch::Approx(9.99f));
    REQUIRE(restored.doubleValue == Catch::Approx(1.23));
  }

  SECTION("Modify array elements")
  {
    GenericValue v;
    v.push_back(GenericValue(1));
    v.push_back(GenericValue(2));
    v.push_back(GenericValue(3));

    v[0] = 10;
    v[1] = 20;
    v[2] = 30;

    REQUIRE(v[0].getInt() == 10);
    REQUIRE(v[1].getInt() == 20);
    REQUIRE(v[2].getInt() == 30);
  }

  SECTION("Modify nested object in array")
  {
    GenericValue v;
    v["items"][0]["name"] = "original";
    v["items"][0]["value"] = 100;

    v["items"][0]["name"] = "modified";
    v["items"][0]["value"] = 200;

    REQUIRE(v["items"][0]["name"].getString() == "modified");
    REQUIRE(v["items"][0]["value"].getInt() == 200);
  }
}

TEST_CASE("GenericValue error handling", "[value]")
{
  GenericValue v;

  SECTION("Type mismatch errors")
  {
    v["num"] = 42;
    REQUIRE_THROWS_AS(v["num"].getString(), std::runtime_error);
    REQUIRE_THROWS_AS(v["num"].getBool(), std::runtime_error);
    REQUIRE_THROWS_AS(v["num"].getArray(), std::runtime_error);
  }

  SECTION("Array index out of bounds")
  {
    v.push_back(GenericValue(1));
    v.push_back(GenericValue(2));

    REQUIRE_THROWS_AS(v[5].getInt(), std::out_of_range);
  }

  SECTION("Key not found in object")
  {
    v["existing"] = 42;

    // Reading non-existent key should throw
    REQUIRE_THROWS_AS(v["nonexistent"].getInt(), std::out_of_range);
  }

  SECTION("Cannot index non-array")
  {
    v["key"] = "value";
    REQUIRE_THROWS_AS(v[0].getString(), std::runtime_error);
  }

  SECTION("Cannot access key on non-object")
  {
    GenericValue arr;
    arr.push_back(GenericValue(1));

    // Const access should throw
    const GenericValue& const_ref = arr;
    REQUIRE_THROWS_AS(const_ref["key"].getInt(), std::runtime_error);
  }
}

TEST_CASE("GenericValue const correctness", "[value]")
{
  SECTION("Const object access")
  {
    GenericValue v;
    v["name"] = "John";
    v["age"] = 30;

    const GenericValue& const_v = v;
    REQUIRE(const_v["name"].getString() == "John");
    REQUIRE(const_v["age"].getInt() == 30);
  }

  SECTION("Const array access")
  {
    GenericValue v;
    v.push_back(GenericValue(10));
    v.push_back(GenericValue(20));
    v.push_back(GenericValue(30));

    const GenericValue& const_v = v;
    REQUIRE(const_v[0].getInt() == 10);
    REQUIRE(const_v[1].getInt() == 20);
    REQUIRE(const_v[2].getInt() == 30);
  }
}

TEST_CASE("GenericValue has() method", "[value]")
{
  GenericValue v;
  v["existing"] = 42;

  SECTION("Existing key")
  {
    REQUIRE(v.has("existing"));
  }

  SECTION("Non-existing key")
  {
    REQUIRE_FALSE(v.has("nonexistent"));
  }

  SECTION("Non-object type")
  {
    GenericValue arr;
    arr.push_back(GenericValue(1));
    REQUIRE_FALSE(arr.has("key"));
  }
}

TEST_CASE("GenericValue size() method", "[value]")
{
  SECTION("Array size")
  {
    GenericValue v;
    REQUIRE(v.size() == 0);

    v.push_back(GenericValue(1));
    v.push_back(GenericValue(2));
    v.push_back(GenericValue(3));
    REQUIRE(v.size() == 3);
  }

  SECTION("Object size")
  {
    GenericValue v;
    v["a"] = 1;
    v["b"] = 2;
    v["c"] = 3;
    REQUIRE(v.size() == 3);
  }

  SECTION("String size")
  {
    GenericValue v("hello");
    REQUIRE(v.size() == 5);
  }

  SECTION("Non-sized types")
  {
    GenericValue v(42);
    REQUIRE(v.size() == 0);
  }
}

TEST_CASE("GenericValue complex scenarios", "[value]")
{
  SECTION("ComplexTypes roundtrip with modifications")
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

    GenericValue v = to_value(original);

    // Modify through proxy
    v["stringValue"] = "Modified!";
    v["intVector"][0] = 999;

    ComplexTypes restored;
    REQUIRE(from_value(restored, v));
    REQUIRE(restored.stringValue == "Modified!");
    REQUIRE(restored.intVector[0] == 999);
  }

  SECTION("Dynamic structure building")
  {
    GenericValue config;
    config["server"]["host"] = "localhost";
    config["server"]["port"] = 8080;
    config["server"]["ssl"]["enabled"] = true;
    config["server"]["ssl"]["cert"] = "/path/to/cert";
    config["features"][0] = "auth";
    config["features"][1] = "logging";
    config["features"][2] = "metrics";

    REQUIRE(config["server"]["host"].getString() == "localhost");
    REQUIRE(config["server"]["port"].getInt() == 8080);
    REQUIRE(config["server"]["ssl"]["enabled"].getBool() == true);
    REQUIRE(config["server"]["ssl"]["cert"].getString() == "/path/to/cert");
    REQUIRE(config["features"].size() == 3);
    REQUIRE(config["features"][0].getString() == "auth");
  }

  SECTION("Array of complex objects")
  {
    GenericValue users;
    users[0]["name"] = "Alice";
    users[0]["age"] = 25;
    users[0]["skills"][0] = "C++";
    users[0]["skills"][1] = "Python";

    users[1]["name"] = "Bob";
    users[1]["age"] = 30;
    users[1]["skills"][0] = "Rust";
    users[1]["skills"][1] = "Go";
    users[1]["skills"][2] = "JavaScript";

    REQUIRE(users[0]["skills"].size() == 2);
    REQUIRE(users[1]["skills"].size() == 3);
    REQUIRE(users[0]["skills"][0].getString() == "C++");
    REQUIRE(users[1]["skills"][2].getString() == "JavaScript");
  }
}

TEST_CASE("GenericValue with ContainsNotReflected", "[value]")
{
  ContainsNotReflected original;
  original.notReflectedStruct.xValue = 100;
  original.notReflectedStruct.yValue = 200.5f;

  SECTION("to_value")
  {
    GenericValue v = to_value(original);
    REQUIRE(v.isObject());
    REQUIRE(v["aValue"].getInt() == 100);
    REQUIRE(v["bValue"].getDouble() == Catch::Approx(200.5f));
  }

  SECTION("Roundtrip")
  {
    GenericValue v = to_value(original);

    v["aValue"] = 150;
    v["bValue"] = 250.5;

    ContainsNotReflected restored;
    REQUIRE(from_value(restored, v));
    REQUIRE(restored.notReflectedStruct.xValue == 150);
    REQUIRE(restored.notReflectedStruct.yValue == Catch::Approx(250.5f));
  }
}

TEST_CASE("GenericValue type conversions", "[value]")
{
  SECTION("Int to double conversion in from_value")
  {
    GenericValue v(42);
    double d;
    REQUIRE(from_value(d, v));
    REQUIRE(d == Catch::Approx(42.0));
  }

  SECTION("Failed conversions")
  {
    GenericValue v("not a number");
    int i;
    REQUIRE_FALSE(from_value(i, v));

    GenericValue v2(42);
    std::string s;
    REQUIRE_FALSE(from_value(s, v2));
  }
}

TEST_CASE("GenericValue JSON serialization", "[value][json]")
{
  SECTION("Null value")
  {
    GenericValue v;
    std::string json = value_to_json(v, false);
    REQUIRE(json == "null");

    auto restored = value_from_json(json);
    REQUIRE(restored.has_value());
    REQUIRE(restored->isNull());
  }

  SECTION("Boolean values")
  {
    GenericValue v_true(true);
    GenericValue v_false(false);

    std::string json_true = value_to_json(v_true, false);
    std::string json_false = value_to_json(v_false, false);

    REQUIRE(json_true == "true");
    REQUIRE(json_false == "false");

    auto restored_true = value_from_json(json_true);
    auto restored_false = value_from_json(json_false);

    REQUIRE(restored_true.has_value());
    REQUIRE(restored_true->getBool() == true);
    REQUIRE(restored_false.has_value());
    REQUIRE(restored_false->getBool() == false);
  }

  SECTION("Integer values")
  {
    GenericValue v(42);
    std::string json = value_to_json(v, false);
    REQUIRE(json == "42");

    auto restored = value_from_json(json);
    REQUIRE(restored.has_value());
    REQUIRE(restored->isInt());
    REQUIRE(restored->getInt() == 42);
  }

  SECTION("Double values")
  {
    GenericValue v(3.14);
    std::string json = value_to_json(v, false);

    auto restored = value_from_json(json);
    REQUIRE(restored.has_value());
    REQUIRE(restored->isDouble());
    REQUIRE(restored->getDouble() == Catch::Approx(3.14));
  }

  SECTION("String values")
  {
    GenericValue v("hello world");
    std::string json = value_to_json(v, false);
    REQUIRE(json == "\"hello world\"");

    auto restored = value_from_json(json);
    REQUIRE(restored.has_value());
    REQUIRE(restored->isString());
    REQUIRE(restored->getString() == "hello world");
  }

  SECTION("String with special characters")
  {
    GenericValue v("Line 1\nLine 2\tTabbed");
    std::string json = value_to_json(v, false);

    auto restored = value_from_json(json);
    REQUIRE(restored.has_value());
    REQUIRE(restored->getString() == "Line 1\nLine 2\tTabbed");
  }

  SECTION("Array values")
  {
    GenericValue v;
    v.push_back(GenericValue(1));
    v.push_back(GenericValue(2));
    v.push_back(GenericValue(3));

    std::string json = value_to_json(v, false);

    auto restored = value_from_json(json);
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

    std::string json = value_to_json(v, false);

    auto restored = value_from_json(json);
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

    std::string json = value_to_json(v, false);

    auto restored = value_from_json(json);
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

    std::string json = value_to_json(v, false);

    auto restored = value_from_json(json);
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

    std::string json = value_to_json(v, false);

    auto restored = value_from_json(json);
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

    std::string json = value_to_json(v, false);

    auto restored = value_from_json(json);
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

    std::string json = value_to_json(v, false);

    auto restored = value_from_json(json);
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

    std::string compact = value_to_json(v, false);
    std::string pretty = value_to_json(v, true);

    // Pretty should have more characters due to newlines and spaces
    REQUIRE(pretty.length() > compact.length());
    REQUIRE(pretty.find('\n') != std::string::npos);

    // Both should parse to the same value
    auto restored_compact = value_from_json(compact);
    auto restored_pretty = value_from_json(pretty);

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
    auto result = value_from_json("{ invalid json }");
    REQUIRE_FALSE(result.has_value());
  }

  SECTION("Empty string")
  {
    auto result = value_from_json("");
    REQUIRE_FALSE(result.has_value());
  }

  SECTION("Truncated JSON")
  {
    auto result = value_from_json("{\"name\":\"John\"");
    REQUIRE_FALSE(result.has_value());
  }

  SECTION("Invalid escape sequences")
  {
    auto result = value_from_json("\"invalid\\xescape\"");
    REQUIRE_FALSE(result.has_value());
  }
}

TEST_CASE("GenericValue JSON roundtrip with structs", "[value][json]")
{
  SECTION("Simple struct")
  {
    Simple original{ 42, 3.14f, 2.71 };
    GenericValue v = to_value(original);

    std::string json = value_to_json(v, false);
    auto restored_value = value_from_json(json);

    REQUIRE(restored_value.has_value());

    Simple restored;
    REQUIRE(from_value(restored, *restored_value));
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

    GenericValue v = to_value(original);
    std::string json = value_to_json(v, true);

    auto restored_value = value_from_json(json);
    REQUIRE(restored_value.has_value());

    Nested restored;
    REQUIRE(from_value(restored, *restored_value));
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

    GenericValue v = to_value(original);
    std::string json = value_to_json(v, true);

    auto restored_value = value_from_json(json);
    REQUIRE(restored_value.has_value());

    ComplexTypes restored;
    REQUIRE(from_value(restored, *restored_value));
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
    std::string json = value_to_json(v, false);
    REQUIRE(json == "[]");

    auto restored = value_from_json(json);
    REQUIRE(restored.has_value());
    REQUIRE(restored->isArray());
    REQUIRE(restored->size() == 0);
  }

  SECTION("Empty object")
  {
    GenericValue v(Object{});
    std::string json = value_to_json(v, false);
    REQUIRE(json == "{}");

    auto restored = value_from_json(json);
    REQUIRE(restored.has_value());
    REQUIRE(restored->isObject());
    REQUIRE(restored->size() == 0);
  }

  SECTION("Large numbers")
  {
    GenericValue v(9223372036854775807LL); // max int64_t
    std::string json = value_to_json(v, false);

    auto restored = value_from_json(json);
    REQUIRE(restored.has_value());
    REQUIRE(restored->getInt() == 9223372036854775807LL);
  }

  SECTION("Negative numbers")
  {
    GenericValue v(-12345);
    std::string json = value_to_json(v, false);

    auto restored = value_from_json(json);
    REQUIRE(restored.has_value());
    REQUIRE(restored->getInt() == -12345);
  }

  SECTION("Zero values")
  {
    GenericValue v_int(0);
    GenericValue v_double(0.0);

    auto restored_int = value_from_json(value_to_json(v_int, false));
    auto restored_double = value_from_json(value_to_json(v_double, false));

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

    std::string json = value_to_json(config, true);

    // Save and reload
    auto loaded = value_from_json(json);
    REQUIRE(loaded.has_value());

    REQUIRE((*loaded)["server"]["host"].getString() == "localhost");
    REQUIRE((*loaded)["server"]["port"].getInt() == 8080);
    REQUIRE((*loaded)["server"]["ssl"]["enabled"].getBool() == true);
    REQUIRE((*loaded)["database"]["name"].getString() == "myapp");
    REQUIRE((*loaded)["features"].size() == 3);
    REQUIRE((*loaded)["debug"].getBool() == false);
  }
}