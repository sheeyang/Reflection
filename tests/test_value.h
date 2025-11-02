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

TEST_CASE("GenericValue to_generic_value conversions", "[value]")
{
  SECTION("Simple struct")
  {
    Simple s{ 42, 3.14f, 2.71 };
    GenericValue v = to_generic_value(s);

    REQUIRE(v.isObject());
    REQUIRE(v["integerValue"].getInt() == 42);
    REQUIRE(v["floatValue"].getDouble() == Catch::Approx(3.14f));
    REQUIRE(v["doubleValue"].getDouble() == Catch::Approx(2.71));
  }

  SECTION("Nested struct")
  {
    Nested n{ 123, {1, 2.0f, 3.0}, {true, 'C', 456, 789, 101112L, 131415LL, 'D', 40000, 5000000, 60000000UL, 7000000000ULL, 2.34f, 5.67, 8.90L} };
    GenericValue v = to_generic_value(n);

    REQUIRE(v.isObject());
    REQUIRE(v["count"].getInt() == 123);
    REQUIRE(v["simpleStruct"]["integerValue"].getInt() == 1);
    REQUIRE(v["primitiveTypes"]["booleanValue"].getBool() == true);
  }

  SECTION("Vector")
  {
    std::vector<int> vec = { 10, 20, 30, 40, 50 };
    GenericValue v = to_generic_value(vec);

    REQUIRE(v.isArray());
    REQUIRE(v.size() == 5);
    REQUIRE(v[0].getInt() == 10);
    REQUIRE(v[4].getInt() == 50);
  }

  SECTION("Map")
  {
    std::map<std::string, int> map = { {"one", 1}, {"two", 2}, {"three", 3} };
    GenericValue v = to_generic_value(map);

    REQUIRE(v.isArray());
    REQUIRE(v.size() == 3);
  }

  SECTION("Pair")
  {
    std::pair<std::string, float> p = { "pi", 3.14f };
    GenericValue v = to_generic_value(p);

    REQUIRE(v.isObject());
    REQUIRE(v["first"].getString() == "pi");
    REQUIRE(v["second"].getDouble() == Catch::Approx(3.14f));
  }

  SECTION("Tuple")
  {
    std::tuple<int, float, std::string> t = { 100, 200.0f, "three hundred" };
    GenericValue v = to_generic_value(t);

    REQUIRE(v.isArray());
    REQUIRE(v.size() == 3);
    REQUIRE(v[0].getInt() == 100);
    REQUIRE(v[1].getDouble() == Catch::Approx(200.0f));
    REQUIRE(v[2].getString() == "three hundred");
  }
}

TEST_CASE("GenericValue from_generic_value conversions", "[value]")
{
  SECTION("Simple struct roundtrip")
  {
    Simple original{ 42, 3.14f, 2.71 };
    GenericValue v = to_generic_value(original);

    Simple restored;
    REQUIRE(from_generic_value(restored, v));
    REQUIRE(restored.integerValue == 42);
    REQUIRE(restored.floatValue == Catch::Approx(3.14f));
    REQUIRE(restored.doubleValue == Catch::Approx(2.71));
  }

  SECTION("Nested struct roundtrip")
  {
    Nested original{ 123, {1, 2.0f, 3.0}, {true, 'C', 456, 789, 101112L, 131415LL, 'D', 40000, 5000000, 60000000UL, 7000000000ULL, 2.34f, 5.67, 8.90L} };
    GenericValue v = to_generic_value(original);

    Nested restored;
    REQUIRE(from_generic_value(restored, v));
    REQUIRE(restored.count == 123);
    REQUIRE(restored.simpleStruct.integerValue == 1);
    REQUIRE(restored.primitiveTypes.booleanValue == true);
    REQUIRE(restored.primitiveTypes.characterValue == 'C');
  }

  SECTION("Vector roundtrip")
  {
    std::vector<int> original = { 10, 20, 30, 40, 50 };
    GenericValue v = to_generic_value(original);

    std::vector<int> restored;
    REQUIRE(from_generic_value(restored, v));
    REQUIRE(restored == original);
  }

  SECTION("Primitive types")
  {
    GenericValue v_int(42);
    int i;
    REQUIRE(from_generic_value(i, v_int));
    REQUIRE(i == 42);

    GenericValue v_str("hello");
    std::string s;
    REQUIRE(from_generic_value(s, v_str));
    REQUIRE(s == "hello");

    GenericValue v_bool(true);
    bool b;
    REQUIRE(from_generic_value(b, v_bool));
    REQUIRE(b == true);
  }
}

TEST_CASE("GenericValue modify through proxy", "[value]")
{
  SECTION("Modify nested values")
  {
    Simple s{ 42, 3.14f, 2.71 };
    GenericValue v = to_generic_value(s);

    v["integerValue"] = 100;
    v["floatValue"] = 9.99;
    v["doubleValue"] = 1.23;

    REQUIRE(v["integerValue"].getInt() == 100);
    REQUIRE(v["floatValue"].getDouble() == Catch::Approx(9.99));
    REQUIRE(v["doubleValue"].getDouble() == Catch::Approx(1.23));

    Simple restored;
    from_generic_value(restored, v);
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

    GenericValue v = to_generic_value(original);

    // Modify through proxy
    v["stringValue"] = "Modified!";
    v["intVector"][0] = 999;

    ComplexTypes restored;
    REQUIRE(from_generic_value(restored, v));
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

  SECTION("to_generic_value")
  {
    GenericValue v = to_generic_value(original);
    REQUIRE(v.isObject());
    REQUIRE(v["aValue"].getInt() == 100);
    REQUIRE(v["bValue"].getDouble() == Catch::Approx(200.5f));
  }

  SECTION("Roundtrip")
  {
    GenericValue v = to_generic_value(original);

    v["aValue"] = 150;
    v["bValue"] = 250.5;

    ContainsNotReflected restored;
    REQUIRE(from_generic_value(restored, v));
    REQUIRE(restored.notReflectedStruct.xValue == 150);
    REQUIRE(restored.notReflectedStruct.yValue == Catch::Approx(250.5f));
  }
}

TEST_CASE("GenericValue type conversions", "[value]")
{
  SECTION("Int to double conversion in from_generic_value")
  {
    GenericValue v(42);
    double d;
    REQUIRE(from_generic_value(d, v));
    REQUIRE(d == Catch::Approx(42.0));
  }

  SECTION("Failed conversions")
  {
    GenericValue v("not a number");
    int i;
    REQUIRE_FALSE(from_generic_value(i, v));

    GenericValue v2(42);
    std::string s;
    REQUIRE_FALSE(from_generic_value(s, v2));
  }
}

TEST_CASE("GenericValue EmptyStruct handling", "[value]")
{
  SECTION("to_generic_value with empty struct")
  {
    EmptyStruct s;
    GenericValue v = to_generic_value(s);
    REQUIRE(v.isObject());
    REQUIRE(v.size() == 0);
  }

  SECTION("from_generic_value with empty struct")
  {
    GenericValue v(Object{});
    EmptyStruct restored;
    REQUIRE(from_generic_value(restored, v));
  }

  SECTION("Roundtrip empty struct")
  {
    EmptyStruct original;
    GenericValue v = to_generic_value(original);

    EmptyStruct restored;
    REQUIRE(from_generic_value(restored, v));
  }
}
