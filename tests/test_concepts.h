
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

TEST_CASE("Concepts Tests - Namespaced types", "[concepts][namespace]")
{
  using namespace TestNamespace;

  SECTION("Concept is_reflectable_v for namespaced structs")
  {
    REQUIRE(ReflectionLibrary::is_reflectable_v<NamespacedSimple> == true);
    REQUIRE(ReflectionLibrary::is_reflectable_v<NamespacedNested> == true);
    REQUIRE(ReflectionLibrary::is_reflectable_v<NamespacedComplex> == true);
    REQUIRE(ReflectionLibrary::is_reflectable_v<NamespacedWithEnum> == true);
  }

  SECTION("Concept is_reflected_enum_v for namespaced enums")
  {
    REQUIRE(ReflectionLibrary::is_reflected_enum_v<NamespacedColor> == true);
    REQUIRE(ReflectionLibrary::is_reflected_enum_v<NamespacedStatus> == true);
  }

  SECTION("Deep namespaced types")
  {
    using namespace Outer::Inner;
    REQUIRE(ReflectionLibrary::is_reflectable_v<DeepNamespacedStruct> == true);
    REQUIRE(ReflectionLibrary::is_reflected_enum_v<DeepNamespacedEnum> == true);
  }
}