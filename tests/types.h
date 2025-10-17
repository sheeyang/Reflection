#pragma once

#include "../include/Reflex.h"

#include <array>
#include <vector>
#include <map>
#include <unordered_map>

struct Simple
{
    int integerValue;
    float floatValue;
    double doubleValue;
};
REFLECT_FIELDS(Simple, integerValue, floatValue, doubleValue)

struct AllPrimitiveTypes
{
    bool booleanValue;
    char characterValue;
    short shortValue;
    int integerValue;
    long longValue;
    long long longLongValue;
    unsigned char unsignedCharValue;
    unsigned short unsignedShortValue;
    unsigned int unsignedIntValue;
    unsigned long unsignedLongValue;
    unsigned long long unsignedLongLongValue;
    float floatValue;
    double doubleValue;
    long double longDoubleValue;
};
REFLECT_FIELDS(AllPrimitiveTypes, booleanValue, characterValue, shortValue, integerValue, longValue, longLongValue, unsignedCharValue, unsignedShortValue, unsignedIntValue, unsignedLongValue, unsignedLongLongValue, floatValue, doubleValue, longDoubleValue)

struct Nested
{
    int count;
    Simple simpleStruct;
    AllPrimitiveTypes primitiveTypes;
};
REFLECT_FIELDS(Nested, count, simpleStruct, primitiveTypes)

struct DeeplyNested
{
    Nested nestedStruct;
    Simple simpleStruct;
};
REFLECT_FIELDS(DeeplyNested, nestedStruct, simpleStruct)

struct ComplexTypes
{
    std::string stringValue;
    std::array<double, 3> doubleArray;
    std::vector<int> intVector;
    std::pair<std::string, float> stringFloatPair;
    std::map<std::string, float> stringFloatMap;
    std::unordered_map<int, std::string> intStringUnorderedMap;
    std::list<double> doubleList;
    std::tuple<int, float, std::string> intFloatStringTuple;
};
REFLECT_FIELDS(ComplexTypes,
               stringValue,
               doubleArray,
               intVector,
               stringFloatPair,
               stringFloatMap,
               intStringUnorderedMap,
               doubleList,
               intFloatStringTuple)

struct NestedComplex
{
    std::vector<std::vector<std::string>> vectorOfVectors;
    std::array<std::array<int, 3>, 2> arrayOfArrays;
    std::array<std::vector<Simple>, 2> arrayOfVectorsOfStructs;
};
REFLECT_FIELDS(NestedComplex, vectorOfVectors, arrayOfArrays, arrayOfVectorsOfStructs)

struct NotReflected
{
    int xValue;
    float yValue;
};

struct ContainsNotReflected
{
    NotReflected notReflectedStruct;

    struct Reflector
    {
        int aValue;
        float bValue;

        static ContainsNotReflected create(const Reflector &r)
        {
            ContainsNotReflected obj;
            obj.notReflectedStruct.xValue = r.aValue;
            obj.notReflectedStruct.yValue = r.bValue;
            return obj;
        }

        static Reflector reflect(const ContainsNotReflected &obj)
        {
            Reflector r;
            r.aValue = obj.notReflectedStruct.xValue;
            r.bValue = obj.notReflectedStruct.yValue;
            return r;
        }
    };
};
REFLECT_CUSTOM(ContainsNotReflected, aValue, bValue)