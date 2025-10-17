#pragma once

#include "../include/Reflex.h"
#include "../include/Reflex/Json.h"
#include "types.h"
#include <catch2/catch_all.hpp>

TEST_CASE("JSON serialize/deserialize Simple struct", "[json]")
{
    Simple original{42, 3.14f, 2.71};

    SECTION("Serialize")
    {
        std::string json = Reflex::to_json(original);
        REQUIRE_FALSE(json.empty());
        REQUIRE_THAT(json, Catch::Matchers::ContainsSubstring("integerValue"));
        REQUIRE_THAT(json, Catch::Matchers::ContainsSubstring("42"));
    }

    SECTION("Roundtrip")
    {
        std::string json = Reflex::to_json(original);
        auto result = Reflex::from_json<Simple>(json);

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
        1.23f, 4.56, 7.89L};

    SECTION("Roundtrip")
    {
        std::string json = Reflex::to_json(original);
        auto result = Reflex::from_json<AllPrimitiveTypes>(json);

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
        {true, 'C', 456, 789, 101112L, 131415LL, 'D', 40000, 5000000, 60000000UL, 7000000000ULL, 2.34f, 5.67, 8.90L}};

    SECTION("Roundtrip")
    {
        std::string json = Reflex::to_json(original);
        auto result = Reflex::from_json<Nested>(json);

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
        {7, 8.0f, 9.0}};

    SECTION("Roundtrip")
    {
        std::string json = Reflex::to_json(original);
        auto result = Reflex::from_json<DeeplyNested>(json);

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
        {100, 200.0f, "three hundred"}};

    SECTION("Serialize")
    {
        std::string json = Reflex::to_json(original);
        REQUIRE_FALSE(json.empty());
        REQUIRE_THAT(json, Catch::Matchers::ContainsSubstring("Hello, World!"));
    }

    SECTION("Roundtrip")
    {
        std::string json = Reflex::to_json(original);
        auto result = Reflex::from_json<ComplexTypes>(json);

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
        {{{Simple{10, 1.0f, 2.0}, Simple{20, 3.0f, 4.0}}, {Simple{30, 5.0f, 6.0}}}}};

    SECTION("Roundtrip")
    {
        std::string json = Reflex::to_json(original);
        auto result = Reflex::from_json<NestedComplex>(json);

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

TEST_CASE("JSON error handling", "[json]")
{
    SECTION("Invalid JSON")
    {
        std::string invalid_json = "{ invalid json }";
        auto result = Reflex::from_json<Simple>(invalid_json);
        REQUIRE_FALSE(result.has_value());
    }

    SECTION("Empty JSON")
    {
        std::string empty_json = "";
        auto result = Reflex::from_json<Simple>(empty_json);
        REQUIRE_FALSE(result.has_value());
    }

    SECTION("Wrong type in JSON")
    {
        std::string json = R"({"integerValue": "not_a_number", "floatValue": 3.14, "doubleValue": 2.71})";
        auto result = Reflex::from_json<Simple>(json);
        // Should fail due to type mismatch
        REQUIRE_FALSE(result.has_value());
    }

    SECTION("Missing fields")
    {
        std::string json = R"({"integerValue": 42})";
        auto result = Reflex::from_json<Simple>(json);
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

        std::string json = Reflex::to_json(obj);
        auto result = Reflex::from_json<ComplexTypes>(json);

        REQUIRE(result.has_value());
        REQUIRE(result->stringValue == "");
    }

    SECTION("Empty containers")
    {
        ComplexTypes obj;
        obj.intVector.clear();
        obj.doubleList.clear();
        obj.stringFloatMap.clear();

        std::string json = Reflex::to_json(obj);
        auto result = Reflex::from_json<ComplexTypes>(json);

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

        std::string json_true = Reflex::to_json(obj_true);
        std::string json_false = Reflex::to_json(obj_false);

        auto result_true = Reflex::from_json<AllPrimitiveTypes>(json_true);
        auto result_false = Reflex::from_json<AllPrimitiveTypes>(json_false);

        REQUIRE(result_true.has_value());
        REQUIRE(result_true->booleanValue == true);
        REQUIRE(result_false.has_value());
        REQUIRE(result_false->booleanValue == false);
    }

    SECTION("Special characters in strings")
    {
        ComplexTypes obj;
        obj.stringValue = "Test\n\t\"quotes\"\\backslash";

        std::string json = Reflex::to_json(obj);
        auto result = Reflex::from_json<ComplexTypes>(json);

        REQUIRE(result.has_value());
        REQUIRE(result->stringValue == "Test\n\t\"quotes\"\\backslash");
    }
}