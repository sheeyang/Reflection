#include "../include/Reflect.h"
#include "types.h"
#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <iostream>

// Helper to capture stdout
class CaptureStdout
{
private:
    std::streambuf *old_stdout;
    std::ostringstream captured;

public:
    CaptureStdout() : old_stdout(std::cout.rdbuf())
    {
        std::cout.rdbuf(captured.rdbuf());
    }

    ~CaptureStdout()
    {
        std::cout.rdbuf(old_stdout);
    }

    std::string get_output() const
    {
        return captured.str();
    }
};

TEST_CASE("Print Simple struct", "[print]")
{
    Simple obj{42, 3.14f, 2.71};

    CaptureStdout capture;
    Reflex::print(obj);
    std::string output = capture.get_output();

    std::string expected =
        "Type: Simple\n"
        "  integerValue = 42\n"
        "  floatValue = 3.14\n"
        "  doubleValue = 2.71\n";

    REQUIRE(output == expected);
}

TEST_CASE("Print AllPrimitiveTypes struct", "[print]")
{
    AllPrimitiveTypes obj{
        true, 'A', 123, 456, 789L, 101112LL,
        'B', 234, 567, 890UL, 111213ULL,
        1.23f, 4.56, 7.89L};

    CaptureStdout capture;
    Reflex::print(obj);
    std::string output = capture.get_output();

    std::string expected =
        "Type: AllPrimitiveTypes\n"
        "  booleanValue = true\n"
        "  characterValue = 'A'\n"
        "  shortValue = 123\n"
        "  integerValue = 456\n"
        "  longValue = 789\n"
        "  longLongValue = 101112\n"
        "  unsignedCharValue = 'B'\n"
        "  unsignedShortValue = 234\n"
        "  unsignedIntValue = 567\n"
        "  unsignedLongValue = 890\n"
        "  unsignedLongLongValue = 111213\n"
        "  floatValue = 1.23\n"
        "  doubleValue = 4.56\n"
        "  longDoubleValue = 7.89\n";

    REQUIRE(output == expected);
}

TEST_CASE("Print Nested struct", "[print]")
{
    Nested obj{123, {1, 2.0f, 3.0}, {true, 'C', 456, 789, 101112L, 131415LL, 200, 40000, 5000000, 60000000UL, 7000000000ULL, 2.34f, 5.67, 8.90L}};

    CaptureStdout capture;
    Reflex::print(obj);
    std::string output = capture.get_output();

    std::string expected =
        "Type: Nested\n"
        "  count = 123\n"
        "  simpleStruct (Simple):\n"
        "    integerValue = 1\n"
        "    floatValue = 2\n"
        "    doubleValue = 3\n"
        "  primitiveTypes (AllPrimitiveTypes):\n"
        "    booleanValue = true\n"
        "    characterValue = 'C'\n"
        "    shortValue = 456\n"
        "    integerValue = 789\n"
        "    longValue = 101112\n"
        "    longLongValue = 131415\n"
        "    unsignedCharValue = 'È'\n"
        "    unsignedShortValue = 40000\n"
        "    unsignedIntValue = 5000000\n"
        "    unsignedLongValue = 60000000\n"
        "    unsignedLongLongValue = 7000000000\n"
        "    floatValue = 2.34\n"
        "    doubleValue = 5.67\n"
        "    longDoubleValue = 8.9\n";

    REQUIRE(output == expected);
}

TEST_CASE("Print DeeplyNested struct", "[print]")
{
    DeeplyNested obj{{456, {4, 5.0f, 6.0}, {false, 'D', 789, 101112, 131415L, 161718LL, 200, 50000, 6000000, 70000000UL, 8000000000ULL, 3.45f, 6.78, 9.01L}}, {7, 8.0f, 9.0}};

    CaptureStdout capture;
    Reflex::print(obj);
    std::string output = capture.get_output();

    std::string expected =
        "Type: DeeplyNested\n"
        "  nestedStruct (Nested):\n"
        "    count = 456\n"
        "    simpleStruct (Simple):\n"
        "      integerValue = 4\n"
        "      floatValue = 5\n"
        "      doubleValue = 6\n"
        "    primitiveTypes (AllPrimitiveTypes):\n"
        "      booleanValue = false\n"
        "      characterValue = 'D'\n"
        "      shortValue = 789\n"
        "      integerValue = 101112\n"
        "      longValue = 131415\n"
        "      longLongValue = 161718\n"
        "      unsignedCharValue = 'È'\n"
        "      unsignedShortValue = 50000\n"
        "      unsignedIntValue = 6000000\n"
        "      unsignedLongValue = 70000000\n"
        "      unsignedLongLongValue = 8000000000\n"
        "      floatValue = 3.45\n"
        "      doubleValue = 6.78\n"
        "      longDoubleValue = 9.01\n"
        "  simpleStruct (Simple):\n"
        "    integerValue = 7\n"
        "    floatValue = 8\n"
        "    doubleValue = 9\n";

    REQUIRE(output == expected);
}

TEST_CASE("Print ComplexTypes struct", "[print]")
{
    ComplexTypes obj{
        "Hello, World!",
        "Reflect in C++",
        {1.1, 2.2, 3.3},
        {10, 20, 30, 40, 50},
        {"pi", 3.14f},
        {{"pi", 3.14f}, {"e", 2.71f}},
        {{1, "one"}, {2, "two"}, {3, "three"}},
        {0.1, 0.2, 0.3, 0.4, 0.5},
        {100, 200.0f, "three hundred"}};

    CaptureStdout capture;
    Reflex::print(obj);
    std::string output = capture.get_output();

    std::string expected =
        "Type: ComplexTypes\n"
        "  cString = \"Hello, World!\"\n"
        "  stringValue = \"Reflect in C++\"\n"
        "  doubleArray = [\n"
        "    0 = 1.1\n"
        "    1 = 2.2\n"
        "    2 = 3.3\n"
        "  ]\n"
        "  intVector = [\n"
        "    0 = 10\n"
        "    1 = 20\n"
        "    2 = 30\n"
        "    3 = 40\n"
        "    4 = 50\n"
        "  ]\n"
        "  stringFloatPair = {\n"
        "    first = \"pi\"\n"
        "    second = 3.14\n"
        "  }\n"
        "  stringFloatMap = [\n"
        "    0 = {\n"
        "      first = \"e\"\n"
        "      second = 2.71\n"
        "    }\n"
        "    1 = {\n"
        "      first = \"pi\"\n"
        "      second = 3.14\n"
        "    }\n"
        "  ]\n"
        "  intStringUnorderedMap = [\n"
        "    0 = {\n"
        "      first = 3\n"
        "      second = \"three\"\n"
        "    }\n"
        "    1 = {\n"
        "      first = 2\n"
        "      second = \"two\"\n"
        "    }\n"
        "    2 = {\n"
        "      first = 1\n"
        "      second = \"one\"\n"
        "    }\n"
        "  ]\n"
        "  doubleList = [\n"
        "    0 = 0.1\n"
        "    1 = 0.2\n"
        "    2 = 0.3\n"
        "    3 = 0.4\n"
        "    4 = 0.5\n"
        "  ]\n"
        "  intFloatStringTuple = (\n"
        "    0 = 100\n"
        "    1 = 200\n"
        "    2 = \"three hundred\"\n"
        "  )\n";

    REQUIRE(output == expected);
}

TEST_CASE("Print NestedComplex struct", "[print]")
{
    NestedComplex obj{
        {{"a", "b"}, {"c", "d", "e"}},
        {{{1, 2, 3}, {4, 5, 6}}},
        {{{Simple{10, 1.0f, 2.0}, Simple{20, 3.0f, 4.0}}, {Simple{30, 5.0f, 6.0}}}}};

    CaptureStdout capture;
    Reflex::print(obj);
    std::string output = capture.get_output();

    std::string expected =
        "Type: NestedComplex\n"
        "  vectorOfVectors = [\n"
        "    0 = [\n"
        "      0 = \"a\"\n"
        "      1 = \"b\"\n"
        "    ]\n"
        "    1 = [\n"
        "      0 = \"c\"\n"
        "      1 = \"d\"\n"
        "      2 = \"e\"\n"
        "    ]\n"
        "  ]\n"
        "  arrayOfArrays = [\n"
        "    0 = [\n"
        "      0 = 1\n"
        "      1 = 2\n"
        "      2 = 3\n"
        "    ]\n"
        "    1 = [\n"
        "      0 = 4\n"
        "      1 = 5\n"
        "      2 = 6\n"
        "    ]\n"
        "  ]\n"
        "  arrayOfVectorsOfStructs = [\n"
        "    0 = [\n"
        "      0 (Simple):\n"
        "        integerValue = 10\n"
        "        floatValue = 1\n"
        "        doubleValue = 2\n"
        "      1 (Simple):\n"
        "        integerValue = 20\n"
        "        floatValue = 3\n"
        "        doubleValue = 4\n"
        "    ]\n"
        "    1 = [\n"
        "      0 (Simple):\n"
        "        integerValue = 30\n"
        "        floatValue = 5\n"
        "        doubleValue = 6\n"
        "    ]\n"
        "  ]\n";

    REQUIRE(output == expected);
}

TEST_CASE("Print ContainsNotReflected struct", "[print]")
{
    ContainsNotReflected obj;
    obj.notReflectedStruct.xValue = 100;
    obj.notReflectedStruct.yValue = 200.5f;

    CaptureStdout capture;
    Reflex::print(obj);
    std::string output = capture.get_output();

    std::string expected =
        "Type: ContainsNotReflected\n"
        "  aValue = 100\n"
        "  bValue = 200.5\n";

    REQUIRE(output == expected);
}

TEST_CASE("Print indentation levels", "[print]")
{
    DeeplyNested obj{{456, {4, 5.0f, 6.0}, {false, 'D', 789, 101112, 131415L, 161718LL, 200, 50000, 6000000, 70000000UL, 8000000000ULL, 3.45f, 6.78, 9.01L}}, {7, 8.0f, 9.0}};

    CaptureStdout capture;
    Reflex::print(obj);
    std::string output = capture.get_output();

    // Check for proper indentation (2 spaces for level 0, 4 for level 1, etc.)
    REQUIRE(output.find("  nestedStruct") != std::string::npos);     // Level 0: 2 spaces
    REQUIRE(output.find("    count") != std::string::npos);          // Level 1: 4 spaces
    REQUIRE(output.find("      integerValue") != std::string::npos); // Level 2: 6 spaces
}