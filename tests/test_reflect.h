#pragma once

#include "../include/Reflect.h"
#include "types.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

TEST_CASE("Simple struct information", "[Reflect]")
{
    Simple s{42, 3.14f, 2.718};

    REQUIRE(ReflectionInfo<Simple>::class_name == std::string("Simple"));
    REQUIRE(Reflex::field_count<Simple>() == 3);
    REQUIRE(Reflex::field_count(s) == 3);
}