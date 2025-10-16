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
