
#include "../include/ReflectionLibrary.h"
#include "types.h"
#include <catch2/catch_all.hpp>

TEST_CASE("Concepts Tests", "[concepts]")
{
  SECTION("Concept has_reflector_v")
  {
    REQUIRE(ReflectionLibrary::has_reflector_v<ContainsNotReflected> == true);
    REQUIRE(ReflectionLibrary::has_reflector_v<Simple> == false);
  }
}