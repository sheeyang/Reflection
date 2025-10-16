#pragma once

#include "../include/Reflect.h"
#include "types.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

TEST_CASE("Simple struct reflection", "[reflection]")
{
    Simple obj{42, 3.14f, 2.71};

    SECTION("Class name")
    {
        REQUIRE(Reflex::class_name<Simple> == std::string("Simple"));
        REQUIRE(Reflex::get_class_name(obj) == "Simple");
    }

    SECTION("Field count")
    {
        REQUIRE(Reflex::field_count<Simple> == 3);
        REQUIRE(Reflex::get_field_count(obj) == 3);
    }

    SECTION("Set field value")
    {
        Reflex::set_field_value(obj, "integerValue", 100);
        REQUIRE(obj.integerValue == 100);
    }
}

TEST_CASE("AllPrimitiveTypes struct reflection", "[reflection]")
{
    AllPrimitiveTypes obj{
        true, 'A', 123, 456, 789L, 101112LL,
        'B', 234, 567, 890UL, 111213ULL,
        1.23f, 4.56, 7.89L};

    SECTION("Class name")
    {
        REQUIRE(Reflex::get_class_name(obj) == "AllPrimitiveTypes");
    }

    SECTION("Field count")
    {
        REQUIRE(Reflex::get_field_count(obj) == 14);
    }

    SECTION("Field iteration")
    {
        int count = 0;
        Reflex::for_each_field(obj, [&](std::string_view name, auto &value, int)
                               { count++; });
        REQUIRE(count == 14);
    }
}

TEST_CASE("Nested struct reflection", "[reflection]")
{
    Nested obj{123, {1, 2.0f, 3.0}, {true, 'C', 456, 789, 101112L, 131415LL, 200, 40000, 5000000, 60000000UL, 7000000000ULL, 2.34f, 5.67, 8.90L}};

    SECTION("Class name")
    {
        REQUIRE(Reflex::get_class_name(obj) == "Nested");
    }

    SECTION("Field count")
    {
        REQUIRE(Reflex::get_field_count(obj) == 3);
    }

    SECTION("Field iteration")
    {
        int count = 0;
        Reflex::for_each_field(obj, [&](std::string_view name, auto &value, int)
                               { count++; });
        // Total fields: 3 (Nested) + 3 (Simple) + 14 (AllPrimitiveTypes) = 20
        REQUIRE(count == 20);
    }
}

TEST_CASE("DeeplyNested struct reflection", "[reflection]")
{
    DeeplyNested obj{{456, {4, 5.0f, 6.0}, {false, 'D', 789, 101112, 131415L, 161718LL, 200, 50000, 6000000, 70000000UL, 8000000000ULL, 3.45f, 6.78, 9.01L}}, {7, 8.0f, 9.0}};

    SECTION("Class name")
    {
        REQUIRE(Reflex::get_class_name(obj) == "DeeplyNested");
    }

    SECTION("Field count")
    {
        REQUIRE(Reflex::get_field_count(obj) == 2);
    }

    SECTION("Field iteration")
    {
        int count = 0;
        Reflex::for_each_field(obj, [&](std::string_view name, auto &value, int)
                               { count++; });
        // Total fields: 2 (DeeplyNested) + 3 (Nested) + 3 (Simple) + 14 (AllPrimitiveTypes) + 3 (Simple) = 25
        REQUIRE(count == 25);
    }
}