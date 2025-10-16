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

    REQUIRE(output.find("Type: Simple") != std::string::npos);
    REQUIRE(output.find("integerValue = 42") != std::string::npos);
    REQUIRE(output.find("floatValue = 3.14") != std::string::npos);
    REQUIRE(output.find("doubleValue = 2.71") != std::string::npos);
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

    REQUIRE(output.find("Type: AllPrimitiveTypes") != std::string::npos);
    REQUIRE(output.find("booleanValue = true") != std::string::npos);
    REQUIRE(output.find("characterValue = 'A'") != std::string::npos);
    REQUIRE(output.find("shortValue = 123") != std::string::npos);
    REQUIRE(output.find("integerValue = 456") != std::string::npos);
    REQUIRE(output.find("longValue = 789") != std::string::npos);
    REQUIRE(output.find("longLongValue = 101112") != std::string::npos);
    REQUIRE(output.find("unsignedCharValue = 'B'") != std::string::npos);
    REQUIRE(output.find("unsignedShortValue = 234") != std::string::npos);
    REQUIRE(output.find("unsignedIntValue = 567") != std::string::npos);
    REQUIRE(output.find("unsignedLongValue = 890") != std::string::npos);
    REQUIRE(output.find("unsignedLongLongValue = 111213") != std::string::npos);
    REQUIRE(output.find("floatValue = 1.23") != std::string::npos);
    REQUIRE(output.find("doubleValue = 4.56") != std::string::npos);
    REQUIRE(output.find("longDoubleValue = 7.89") != std::string::npos);
}

TEST_CASE("Print Nested struct", "[print]")
{
    Nested obj{123, {1, 2.0f, 3.0}, {true, 'C', 456, 789, 101112L, 131415LL, 200, 40000, 5000000, 60000000UL, 7000000000ULL, 2.34f, 5.67, 8.90L}};

    CaptureStdout capture;
    Reflex::print(obj);
    std::string output = capture.get_output();

    REQUIRE(output.find("Type: Nested") != std::string::npos);
    REQUIRE(output.find("count = 123") != std::string::npos);
    REQUIRE(output.find("simpleStruct (Simple):") != std::string::npos);
    REQUIRE(output.find("integerValue = 1") != std::string::npos);
    REQUIRE(output.find("floatValue = 2") != std::string::npos);
    REQUIRE(output.find("doubleValue = 3") != std::string::npos);
    REQUIRE(output.find("primitiveTypes (AllPrimitiveTypes):") != std::string::npos);
    REQUIRE(output.find("booleanValue = true") != std::string::npos);
    REQUIRE(output.find("characterValue = 'C'") != std::string::npos);
}

TEST_CASE("Print DeeplyNested struct", "[print]")
{
    DeeplyNested obj{{456, {4, 5.0f, 6.0}, {false, 'D', 789, 101112, 131415L, 161718LL, 200, 50000, 6000000, 70000000UL, 8000000000ULL, 3.45f, 6.78, 9.01L}}, {7, 8.0f, 9.0}};

    CaptureStdout capture;
    Reflex::print(obj);
    std::string output = capture.get_output();

    REQUIRE(output.find("Type: DeeplyNested") != std::string::npos);
    REQUIRE(output.find("nestedStruct (Nested):") != std::string::npos);
    REQUIRE(output.find("count = 456") != std::string::npos);
    REQUIRE(output.find("simpleStruct (Simple):") != std::string::npos);
    REQUIRE(output.find("integerValue = 4") != std::string::npos);
    REQUIRE(output.find("integerValue = 7") != std::string::npos);
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

    REQUIRE(output.find("Type: ComplexTypes") != std::string::npos);
    REQUIRE(output.find("cString = \"Hello, World!\"") != std::string::npos);
    REQUIRE(output.find("stringValue = \"Reflect in C++\"") != std::string::npos);
    REQUIRE(output.find("doubleArray = [") != std::string::npos);
    REQUIRE(output.find("0 = 1.1") != std::string::npos);
    REQUIRE(output.find("1 = 2.2") != std::string::npos);
    REQUIRE(output.find("2 = 3.3") != std::string::npos);
    REQUIRE(output.find("intVector = [") != std::string::npos);
    REQUIRE(output.find("0 = 10") != std::string::npos);
    REQUIRE(output.find("stringFloatPair = {") != std::string::npos);
    REQUIRE(output.find("first = \"pi\"") != std::string::npos);
    REQUIRE(output.find("second = 3.14") != std::string::npos);
    REQUIRE(output.find("intFloatStringTuple = (") != std::string::npos);
    REQUIRE(output.find("0 = 100") != std::string::npos);
    REQUIRE(output.find("1 = 200") != std::string::npos);
    REQUIRE(output.find("2 = \"three hundred\"") != std::string::npos);
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

    REQUIRE(output.find("Type: NestedComplex") != std::string::npos);
    REQUIRE(output.find("vectorOfVectors = [") != std::string::npos);
    REQUIRE(output.find("0 = [") != std::string::npos);
    REQUIRE(output.find("0 = \"a\"") != std::string::npos);
    REQUIRE(output.find("1 = \"b\"") != std::string::npos);
    REQUIRE(output.find("arrayOfArrays = [") != std::string::npos);
    REQUIRE(output.find("arrayOfVectorsOfStructs = [") != std::string::npos);
    REQUIRE(output.find("(Simple):") != std::string::npos);
}

TEST_CASE("Print ContainsNotReflected struct", "[print]")
{
    ContainsNotReflected obj;
    obj.notReflectedStruct.xValue = 100;
    obj.notReflectedStruct.yValue = 200.5f;

    CaptureStdout capture;
    Reflex::print(obj);
    std::string output = capture.get_output();

    REQUIRE(output.find("Type: ContainsNotReflected") != std::string::npos);
    REQUIRE(output.find("aValue = 100") != std::string::npos);
    REQUIRE(output.find("bValue = 200.5") != std::string::npos);
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