#pragma once

#include "../include/ReflectionLibrary.h"
#include "types.h"
#include <catch2/catch_all.hpp>

using namespace ReflectionLibrary;

TEST_CASE("Enum reflection - is_reflected_enum_v concept", "[enum]")
{
  SECTION("Reflected enums")
  {
    REQUIRE(is_reflected_enum_v<Color> == true);
    REQUIRE(is_reflected_enum_v<Status> == true);
  }

  SECTION("Non-enum types")
  {
    REQUIRE(is_reflected_enum_v<int> == false);
    REQUIRE(is_reflected_enum_v<std::string> == false);
    REQUIRE(is_reflected_enum_v<Simple> == false);
  }
}

TEST_CASE("Enum reflection - enum_to_string", "[enum]")
{
  SECTION("Color enum")
  {
    REQUIRE(enum_to_string(Color::Red) == "Red");
    REQUIRE(enum_to_string(Color::Green) == "Green");
    REQUIRE(enum_to_string(Color::Blue) == "Blue");
    REQUIRE(enum_to_string(Color::Yellow) == "Yellow");
  }

  SECTION("Status enum")
  {
    REQUIRE(enum_to_string(Status::Pending) == "Pending");
    REQUIRE(enum_to_string(Status::Active) == "Active");
    REQUIRE(enum_to_string(Status::Completed) == "Completed");
    REQUIRE(enum_to_string(Status::Cancelled) == "Cancelled");
  }
}

TEST_CASE("Enum reflection - string_to_enum", "[enum]")
{
  SECTION("Color enum - valid strings")
  {
    auto red = string_to_enum<Color>("Red");
    REQUIRE(red.has_value());
    REQUIRE(red.value() == Color::Red);

    auto green = string_to_enum<Color>("Green");
    REQUIRE(green.has_value());
    REQUIRE(green.value() == Color::Green);

    auto blue = string_to_enum<Color>("Blue");
    REQUIRE(blue.has_value());
    REQUIRE(blue.value() == Color::Blue);

    auto yellow = string_to_enum<Color>("Yellow");
    REQUIRE(yellow.has_value());
    REQUIRE(yellow.value() == Color::Yellow);
  }

  SECTION("Status enum - valid strings")
  {
    auto pending = string_to_enum<Status>("Pending");
    REQUIRE(pending.has_value());
    REQUIRE(pending.value() == Status::Pending);

    auto active = string_to_enum<Status>("Active");
    REQUIRE(active.has_value());
    REQUIRE(active.value() == Status::Active);

    auto completed = string_to_enum<Status>("Completed");
    REQUIRE(completed.has_value());
    REQUIRE(completed.value() == Status::Completed);

    auto cancelled = string_to_enum<Status>("Cancelled");
    REQUIRE(cancelled.has_value());
    REQUIRE(cancelled.value() == Status::Cancelled);
  }

  SECTION("Invalid strings")
  {
    auto invalid_color = string_to_enum<Color>("Purple");
    REQUIRE_FALSE(invalid_color.has_value());

    auto invalid_status = string_to_enum<Status>("Unknown");
    REQUIRE_FALSE(invalid_status.has_value());

    auto empty = string_to_enum<Color>("");
    REQUIRE_FALSE(empty.has_value());
  }

  SECTION("Case sensitive")
  {
    auto lowercase = string_to_enum<Color>("red");
    REQUIRE_FALSE(lowercase.has_value());

    auto uppercase = string_to_enum<Color>("RED");
    REQUIRE_FALSE(uppercase.has_value());
  }
}

TEST_CASE("Enum reflection - enum_value_count", "[enum]")
{
  SECTION("Count enum values")
  {
    REQUIRE(enum_value_count<Color> == 4);
    REQUIRE(enum_value_count<Status> == 4);
  }
}

TEST_CASE("Enum reflection - get_enum_name", "[enum]")
{
  SECTION("Enum type names")
  {
    REQUIRE(get_enum_name<Color>() == "Color");
    REQUIRE(get_enum_name<Status>() == "Status");
  }
}

TEST_CASE("Enum reflection - roundtrip conversion", "[enum]")
{
  SECTION("Color roundtrip")
  {
    for (auto c : { Color::Red, Color::Green, Color::Blue, Color::Yellow })
    {
      auto str = enum_to_string(c);
      auto converted = string_to_enum<Color>(str);
      REQUIRE(converted.has_value());
      REQUIRE(converted.value() == c);
    }
  }

  SECTION("Status roundtrip")
  {
    for (auto s : { Status::Pending, Status::Active, Status::Completed, Status::Cancelled })
    {
      auto str = enum_to_string(s);
      auto converted = string_to_enum<Status>(str);
      REQUIRE(converted.has_value());
      REQUIRE(converted.value() == s);
    }
  }
}

TEST_CASE("Enum reflection - to_generic_value", "[enum]")
{
  SECTION("Convert enum to GenericValue")
  {
    GenericValue gv_red = to_generic_value(Color::Red);
    REQUIRE(gv_red.isString());
    REQUIRE(gv_red.getString() == "Red");

    GenericValue gv_active = to_generic_value(Status::Active);
    REQUIRE(gv_active.isString());
    REQUIRE(gv_active.getString() == "Active");
  }
}

TEST_CASE("Enum reflection - from_generic_value", "[enum]")
{
  SECTION("Convert GenericValue to enum - valid")
  {
    GenericValue gv_green("Green");
    Color color;
    bool success = from_generic_value(color, gv_green);
    REQUIRE(success);
    REQUIRE(color == Color::Green);

    GenericValue gv_completed("Completed");
    Status status;
    success = from_generic_value(status, gv_completed);
    REQUIRE(success);
    REQUIRE(status == Status::Completed);
  }

  SECTION("Convert GenericValue to enum - invalid string")
  {
    GenericValue gv_invalid("Invalid");
    Color color;
    bool success = from_generic_value(color, gv_invalid);
    REQUIRE_FALSE(success);
  }

  SECTION("Convert GenericValue to enum - wrong type")
  {
    GenericValue gv_int(42);
    Color color;
    bool success = from_generic_value(color, gv_int);
    REQUIRE_FALSE(success);
  }
}

TEST_CASE("Enum reflection - JSON serialization", "[enum]")
{
  SECTION("Serialize enum to JSON")
  {
    Color color = Color::Blue;
    std::string json = to_json(color);
    REQUIRE_FALSE(json.empty());
    REQUIRE_THAT(json, Catch::Matchers::ContainsSubstring("Blue"));
  }

  SECTION("Deserialize enum from JSON")
  {
    std::string json = "\"Yellow\"";
    auto result = from_json<Color>(json);
    REQUIRE(result.has_value());
    REQUIRE(result.value() == Color::Yellow);
  }

  SECTION("Roundtrip enum through JSON")
  {
    Color original = Color::Green;
    std::string json = to_json(original);
    auto result = from_json<Color>(json);
    REQUIRE(result.has_value());
    REQUIRE(result.value() == original);
  }

  SECTION("Invalid JSON string")
  {
    std::string json = "\"InvalidColor\"";
    auto result = from_json<Color>(json);
    REQUIRE_FALSE(result.has_value());
  }
}

TEST_CASE("Enum reflection - struct with enums", "[enum]")
{
  SECTION("Serialize struct with enums")
  {
    WithEnum obj{ Color::Red, Status::Active, 42 };
    std::string json = to_json(obj);
    REQUIRE_FALSE(json.empty());
    REQUIRE_THAT(json, Catch::Matchers::ContainsSubstring("Red"));
    REQUIRE_THAT(json, Catch::Matchers::ContainsSubstring("Active"));
    REQUIRE_THAT(json, Catch::Matchers::ContainsSubstring("42"));
  }

  SECTION("Deserialize struct with enums")
  {
    std::string json = R"({"color":"Blue","status":"Completed","value":100})";
    auto result = from_json<WithEnum>(json);
    REQUIRE(result.has_value());
    REQUIRE(result->color == Color::Blue);
    REQUIRE(result->status == Status::Completed);
    REQUIRE(result->value == 100);
  }

  SECTION("Roundtrip struct with enums")
  {
    WithEnum original{ Color::Yellow, Status::Pending, 999 };
    std::string json = to_json(original);
    auto result = from_json<WithEnum>(json);
    REQUIRE(result.has_value());
    REQUIRE(result->color == Color::Yellow);
    REQUIRE(result->status == Status::Pending);
    REQUIRE(result->value == 999);
  }
}

TEST_CASE("Enum reflection - print enum", "[enum]")
{
  SECTION("Print enum value")
  {
    Color color = Color::Green;

    std::ostringstream oss;
    auto old_buf = std::cout.rdbuf(oss.rdbuf());

    print_value("color", color, 0);

    std::cout.rdbuf(old_buf);

    std::string output = oss.str();
    REQUIRE_THAT(output, Catch::Matchers::ContainsSubstring("color"));
    REQUIRE_THAT(output, Catch::Matchers::ContainsSubstring("Green"));
  }

  SECTION("Print struct with enums")
  {
    WithEnum obj{ Color::Blue, Status::Cancelled, 123 };

    std::ostringstream oss;
    auto old_buf = std::cout.rdbuf(oss.rdbuf());

    print(obj);

    std::cout.rdbuf(old_buf);

    std::string output = oss.str();
    REQUIRE_THAT(output, Catch::Matchers::ContainsSubstring("WithEnum"));
    REQUIRE_THAT(output, Catch::Matchers::ContainsSubstring("Blue"));
    REQUIRE_THAT(output, Catch::Matchers::ContainsSubstring("Cancelled"));
    REQUIRE_THAT(output, Catch::Matchers::ContainsSubstring("123"));
  }
}

TEST_CASE("Enum reflection - GenericValue with enums", "[enum]")
{
  SECTION("Store enum in GenericValue object")
  {
    GenericValue obj;
    obj["color"] = to_generic_value(Color::Red);
    obj["status"] = to_generic_value(Status::Active);

    REQUIRE(obj["color"].isString());
    REQUIRE(obj["color"].getString() == "Red");
    REQUIRE(obj["status"].getString() == "Active");
  }

  SECTION("Extract enum from GenericValue object")
  {
    GenericValue obj;
    obj["color"] = "Green";
    obj["status"] = "Completed";

    Color color;
    Status status;

    REQUIRE(from_generic_value(color, obj["color"]));
    REQUIRE(from_generic_value(status, obj["status"]));

    REQUIRE(color == Color::Green);
    REQUIRE(status == Status::Completed);
  }
}

TEST_CASE("Enum reflection - optional enums", "[enum]")
{
  SECTION("Optional enum with value")
  {
    std::optional<Color> opt_color = Color::Yellow;
    GenericValue gv = to_generic_value(opt_color);

    REQUIRE(gv.isString());
    REQUIRE(gv.getString() == "Yellow");
  }

  SECTION("Optional enum without value")
  {
    std::optional<Color> opt_color = std::nullopt;
    GenericValue gv = to_generic_value(opt_color);

    REQUIRE(gv.isNull());
  }

  SECTION("Deserialize optional enum with value")
  {
    GenericValue gv("Blue");
    std::optional<Color> opt_color;

    bool success = from_generic_value(opt_color, gv);
    REQUIRE(success);
    REQUIRE(opt_color.has_value());
    REQUIRE(opt_color.value() == Color::Blue);
  }

  SECTION("Deserialize optional enum with null")
  {
    GenericValue gv;
    std::optional<Color> opt_color;

    bool success = from_generic_value(opt_color, gv);
    REQUIRE(success);
    REQUIRE_FALSE(opt_color.has_value());
  }
}

TEST_CASE("Enum reflection - enum in collections", "[enum]")
{
  SECTION("Vector of enums")
  {
    std::vector<Color> colors = { Color::Red, Color::Green, Color::Blue };
    GenericValue gv = to_generic_value(colors);

    REQUIRE(gv.isArray());
    REQUIRE(gv.size() == 3);
    REQUIRE(gv[0].getString() == "Red");
    REQUIRE(gv[1].getString() == "Green");
    REQUIRE(gv[2].getString() == "Blue");
  }

  SECTION("Map with enum values")
  {
    std::map<std::string, Color> color_map = {
      {"primary", Color::Red},
      {"secondary", Color::Blue}
    };

    GenericValue gv = to_generic_value(color_map);
    REQUIRE(gv.isArray());
    REQUIRE(gv.size() == 2);
  }

  SECTION("Deserialize vector of enums")
  {
    GenericValue gv_array;
    gv_array.push_back(GenericValue("Red"));
    gv_array.push_back(GenericValue("Yellow"));

    std::vector<Color> colors;
    bool success = from_generic_value(colors, gv_array);

    REQUIRE(success);
    REQUIRE(colors.size() == 2);
    REQUIRE(colors[0] == Color::Red);
    REQUIRE(colors[1] == Color::Yellow);
  }
}

TEST_CASE("Namespaced enum reflection - is_reflected_enum_v concept", "[enum][namespace]")
{
  using namespace TestNamespace;

  SECTION("Reflected namespaced enums")
  {
    REQUIRE(is_reflected_enum_v<NamespacedColor> == true);
    REQUIRE(is_reflected_enum_v<NamespacedStatus> == true);
  }

  SECTION("Deep namespaced enum")
  {
    using namespace Outer::Inner;
    REQUIRE(is_reflected_enum_v<DeepNamespacedEnum> == true);
  }
}

TEST_CASE("Namespaced enum reflection - enum_to_string", "[enum][namespace]")
{
  using namespace TestNamespace;

  SECTION("NamespacedColor enum")
  {
    REQUIRE(enum_to_string(NamespacedColor::Red) == "Red");
    REQUIRE(enum_to_string(NamespacedColor::Green) == "Green");
    REQUIRE(enum_to_string(NamespacedColor::Blue) == "Blue");
  }

  SECTION("NamespacedStatus enum")
  {
    REQUIRE(enum_to_string(NamespacedStatus::Idle) == "Idle");
    REQUIRE(enum_to_string(NamespacedStatus::Running) == "Running");
    REQUIRE(enum_to_string(NamespacedStatus::Stopped) == "Stopped");
  }

  SECTION("DeepNamespacedEnum")
  {
    using namespace Outer::Inner;
    REQUIRE(enum_to_string(DeepNamespacedEnum::First) == "First");
    REQUIRE(enum_to_string(DeepNamespacedEnum::Second) == "Second");
    REQUIRE(enum_to_string(DeepNamespacedEnum::Third) == "Third");
  }
}

TEST_CASE("Namespaced enum reflection - string_to_enum", "[enum][namespace]")
{
  using namespace TestNamespace;

  SECTION("NamespacedColor - valid strings")
  {
    auto red = string_to_enum<NamespacedColor>("Red");
    REQUIRE(red.has_value());
    REQUIRE(red.value() == NamespacedColor::Red);

    auto green = string_to_enum<NamespacedColor>("Green");
    REQUIRE(green.has_value());
    REQUIRE(green.value() == NamespacedColor::Green);

    auto blue = string_to_enum<NamespacedColor>("Blue");
    REQUIRE(blue.has_value());
    REQUIRE(blue.value() == NamespacedColor::Blue);
  }

  SECTION("NamespacedColor - invalid strings")
  {
    auto invalid = string_to_enum<NamespacedColor>("Purple");
    REQUIRE_FALSE(invalid.has_value());
  }

  SECTION("NamespacedStatus - valid strings")
  {
    auto idle = string_to_enum<NamespacedStatus>("Idle");
    REQUIRE(idle.has_value());
    REQUIRE(idle.value() == NamespacedStatus::Idle);

    auto running = string_to_enum<NamespacedStatus>("Running");
    REQUIRE(running.has_value());
    REQUIRE(running.value() == NamespacedStatus::Running);
  }

  SECTION("DeepNamespacedEnum - valid strings")
  {
    using namespace Outer::Inner;

    auto first = string_to_enum<DeepNamespacedEnum>("First");
    REQUIRE(first.has_value());
    REQUIRE(first.value() == DeepNamespacedEnum::First);

    auto second = string_to_enum<DeepNamespacedEnum>("Second");
    REQUIRE(second.has_value());
    REQUIRE(second.value() == DeepNamespacedEnum::Second);
  }
}

TEST_CASE("Namespaced enum reflection - enum_value_count", "[enum][namespace]")
{
  using namespace TestNamespace;

  SECTION("NamespacedColor count")
  {
    REQUIRE(enum_value_count<NamespacedColor> == 3);
  }

  SECTION("NamespacedStatus count")
  {
    REQUIRE(enum_value_count<NamespacedStatus> == 3);
  }

  SECTION("DeepNamespacedEnum count")
  {
    using namespace Outer::Inner;
    REQUIRE(enum_value_count<DeepNamespacedEnum> == 3);
  }
}

TEST_CASE("Namespaced enum reflection - get_enum_name", "[enum][namespace]")
{
  using namespace TestNamespace;

  SECTION("NamespacedColor name")
  {
    REQUIRE(get_enum_name<NamespacedColor>() == "TestNamespace::NamespacedColor");
  }

  SECTION("NamespacedStatus name")
  {
    REQUIRE(get_enum_name<NamespacedStatus>() == "TestNamespace::NamespacedStatus");
  }

  SECTION("DeepNamespacedEnum name")
  {
    using namespace Outer::Inner;
    REQUIRE(get_enum_name<DeepNamespacedEnum>() == "Outer::Inner::DeepNamespacedEnum");
  }
}

TEST_CASE("Namespaced enum in struct - to_generic_value", "[enum][namespace]")
{
  using namespace TestNamespace;

  SECTION("Serialize NamespacedWithEnum")
  {
    NamespacedWithEnum obj{ NamespacedColor::Blue, NamespacedStatus::Running, 99 };
    GenericValue gv = to_generic_value(obj);

    REQUIRE(gv.isObject());
    REQUIRE(gv["color"].getString() == "Blue");
    REQUIRE(gv["status"].getString() == "Running");
    REQUIRE(gv["value"].getInt() == 99);
  }
}

TEST_CASE("Namespaced enum in struct - from_generic_value", "[enum][namespace]")
{
  using namespace TestNamespace;

  SECTION("Deserialize NamespacedWithEnum")
  {
    GenericValue gv;
    gv["color"] = "Green";
    gv["status"] = "Idle";
    gv["value"] = 42;

    NamespacedWithEnum obj;
    bool success = from_generic_value(obj, gv);

    REQUIRE(success);
    REQUIRE(obj.color == NamespacedColor::Green);
    REQUIRE(obj.status == NamespacedStatus::Idle);
    REQUIRE(obj.value == 42);
  }
}

TEST_CASE("Namespaced enum JSON roundtrip", "[enum][namespace]")
{
  using namespace TestNamespace;

  SECTION("NamespacedWithEnum roundtrip")
  {
    NamespacedWithEnum original{ NamespacedColor::Red, NamespacedStatus::Stopped, 123 };
    std::string json = to_json(original);
    auto result = from_json<NamespacedWithEnum>(json);

    REQUIRE(result.has_value());
    REQUIRE(result->color == NamespacedColor::Red);
    REQUIRE(result->status == NamespacedStatus::Stopped);
    REQUIRE(result->value == 123);
  }
}
