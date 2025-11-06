
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

  SECTION("Concept is_optional_v")
  {
    REQUIRE(ReflectionLibrary::is_optional_v<std::optional<int>> == true);
    REQUIRE(ReflectionLibrary::is_optional_v<std::optional<std::string>> == true);
    REQUIRE(ReflectionLibrary::is_optional_v<std::optional<Simple>> == true);
    REQUIRE(ReflectionLibrary::is_optional_v<int> == false);
    REQUIRE(ReflectionLibrary::is_optional_v<std::string> == false);
    REQUIRE(ReflectionLibrary::is_optional_v<Simple> == false);
  }
}