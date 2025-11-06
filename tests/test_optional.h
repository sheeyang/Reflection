#pragma once

#include "../include/ReflectionLibrary.h"
#include "types.h"
#include <catch2/catch_all.hpp>
#include <sstream>

TEST_CASE("Optional struct reflection", "[optional]")
{
  SECTION("Class name")
  {
    WithOptional obj;
    REQUIRE(ReflectionLibrary::get_class_name(obj) == "WithOptional");
  }

  SECTION("Field count")
  {
    WithOptional obj;
    REQUIRE(ReflectionLibrary::get_field_count(obj) == 4);
  }

  SECTION("Field iteration with all values present")
  {
    WithOptional obj{ 42, "hello", Simple{1, 2.0f, 3.0}, 100 };

    std::vector<std::string> names;
    ReflectionLibrary::for_each_field(obj, [&](std::string_view name, auto& value, int nest_level)
      {
        names.push_back(std::string(name));
      });

    REQUIRE(names.size() == 4);
    REQUIRE(names == std::vector<std::string>{"optionalInt", "optionalString", "optionalStruct", "regularInt"});
  }

  SECTION("Field iteration with no optional values present")
  {
    WithOptional obj{ std::nullopt, std::nullopt, std::nullopt, 100 };

    std::vector<std::string> names;
    ReflectionLibrary::for_each_field(obj, [&](std::string_view name, auto& value, int nest_level)
      {
        names.push_back(std::string(name));
      });

    REQUIRE(names.size() == 4);
    REQUIRE(names == std::vector<std::string>{"optionalInt", "optionalString", "optionalStruct", "regularInt"});
  }

  SECTION("Access optional values")
  {
    WithOptional obj{ 42, "hello", Simple{1, 2.0f, 3.0}, 100 };

    REQUIRE(obj.optionalInt.has_value());
    REQUIRE(obj.optionalInt.value() == 42);
    REQUIRE(obj.optionalString.has_value());
    REQUIRE(obj.optionalString.value() == "hello");
    REQUIRE(obj.optionalStruct.has_value());
    REQUIRE(obj.optionalStruct.value().integerValue == 1);
    REQUIRE(obj.regularInt == 100);
  }

  SECTION("Access empty optional values")
  {
    WithOptional obj{ std::nullopt, std::nullopt, std::nullopt, 100 };

    REQUIRE_FALSE(obj.optionalInt.has_value());
    REQUIRE_FALSE(obj.optionalString.has_value());
    REQUIRE_FALSE(obj.optionalStruct.has_value());
    REQUIRE(obj.regularInt == 100);
  }
}

TEST_CASE("Print Optional struct", "[optional][print]")
{
  SECTION("Print with all optional values present")
  {
    WithOptional obj{ 42, "hello", Simple{1, 2.0f, 3.0}, 100 };

    std::ostringstream oss;
    std::streambuf* old_cout = std::cout.rdbuf(oss.rdbuf());

    ReflectionLibrary::print(obj);

    std::cout.rdbuf(old_cout);
    std::string output = oss.str();

    REQUIRE_THAT(output, Catch::Matchers::ContainsSubstring("Type: WithOptional"));
    REQUIRE_THAT(output, Catch::Matchers::ContainsSubstring("optionalInt"));
    REQUIRE_THAT(output, Catch::Matchers::ContainsSubstring("optionalString"));
    REQUIRE_THAT(output, Catch::Matchers::ContainsSubstring("optionalStruct"));
    REQUIRE_THAT(output, Catch::Matchers::ContainsSubstring("regularInt"));
  }

  SECTION("Print with empty optional values")
  {
    WithOptional obj{ std::nullopt, std::nullopt, std::nullopt, 100 };

    std::ostringstream oss;
    std::streambuf* old_cout = std::cout.rdbuf(oss.rdbuf());

    ReflectionLibrary::print(obj);

    std::cout.rdbuf(old_cout);
    std::string output = oss.str();

    REQUIRE_THAT(output, Catch::Matchers::ContainsSubstring("Type: WithOptional"));
    REQUIRE_THAT(output, Catch::Matchers::ContainsSubstring("regularInt"));
  }
}

TEST_CASE("JSON serialize/deserialize Optional struct", "[optional][json]")
{
  SECTION("Serialize with all optional values present")
  {
    WithOptional obj{ 42, "hello", Simple{1, 2.0f, 3.0}, 100 };

    std::string json = ReflectionLibrary::to_json(obj);
    REQUIRE_FALSE(json.empty());
    REQUIRE_THAT(json, Catch::Matchers::ContainsSubstring("optionalInt"));
    REQUIRE_THAT(json, Catch::Matchers::ContainsSubstring("optionalString"));
    REQUIRE_THAT(json, Catch::Matchers::ContainsSubstring("regularInt"));
    REQUIRE_THAT(json, Catch::Matchers::ContainsSubstring("100"));
  }

  SECTION("Serialize with empty optional values")
  {
    WithOptional obj{ std::nullopt, std::nullopt, std::nullopt, 100 };

    std::string json = ReflectionLibrary::to_json(obj);
    REQUIRE_FALSE(json.empty());
    REQUIRE_THAT(json, Catch::Matchers::ContainsSubstring("regularInt"));
    REQUIRE_THAT(json, Catch::Matchers::ContainsSubstring("100"));
  }

  SECTION("Roundtrip with all optional values present")
  {
    WithOptional original{ 42, "hello", Simple{1, 2.0f, 3.0}, 100 };

    std::string json = ReflectionLibrary::to_json(original);
    auto result = ReflectionLibrary::from_json<WithOptional>(json);

    REQUIRE(result.has_value());
    REQUIRE(result->regularInt == 100);

    if (result->optionalInt.has_value())
    {
      REQUIRE(result->optionalInt.value() == 42);
    }
    if (result->optionalString.has_value())
    {
      REQUIRE(result->optionalString.value() == "hello");
    }
    if (result->optionalStruct.has_value())
    {
      REQUIRE(result->optionalStruct.value().integerValue == 1);
      REQUIRE(result->optionalStruct.value().floatValue == 2.0f);
      REQUIRE(result->optionalStruct.value().doubleValue == 3.0);
    }
  }

  SECTION("Roundtrip with empty optional values")
  {
    WithOptional original{ std::nullopt, std::nullopt, std::nullopt, 100 };

    std::string json = ReflectionLibrary::to_json(original);
    auto result = ReflectionLibrary::from_json<WithOptional>(json);

    REQUIRE(result.has_value());
    REQUIRE(result->regularInt == 100);
  }

  SECTION("Roundtrip with mixed optional values")
  {
    WithOptional original{ 42, std::nullopt, Simple{5, 6.0f, 7.0}, 200 };

    std::string json = ReflectionLibrary::to_json(original);
    auto result = ReflectionLibrary::from_json<WithOptional>(json);

    REQUIRE(result.has_value());
    REQUIRE(result->regularInt == 200);

    if (result->optionalInt.has_value())
    {
      REQUIRE(result->optionalInt.value() == 42);
    }
    REQUIRE_FALSE(result->optionalString.has_value());
    if (result->optionalStruct.has_value())
    {
      REQUIRE(result->optionalStruct.value().integerValue == 5);
    }
  }
}
