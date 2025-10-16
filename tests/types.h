#pragma once

#include "../include/Reflect.h"

#include <array>
#include <vector>
#include <map>
#include <unordered_map>

struct Simple
{
    int a;
    float b;
    double c;
};
REFLECT_FIELDS(Simple, a, b, c)

struct AllPrimitiveTypes
{
    bool a;
    char b;
    short c;
    int d;
    long e;
    long long f;
    unsigned char g;
    unsigned short h;
    unsigned int i;
    unsigned long j;
    unsigned long long k;
    float l;
    double m;
    long double n;
};
REFLECT_FIELDS(AllPrimitiveTypes, a, b, c, d, e, f, g, h, i, j, k, l, m, n)

struct Nested
{
    int a;
    Simple s;
    AllPrimitiveTypes apt;
};
REFLECT_FIELDS(Nested, a, s, apt)

struct DeeplyNested
{
    Nested n;
    Simple s;
};
REFLECT_FIELDS(DeeplyNested, n, s)

struct ComplexTypes
{
    const char *cstr;
    std::string str;
    std::array<double, 3> arr;
    std::vector<int> vec;
    std::pair<std::string, float> pair;
    std::map<std::string, float> mp;
    std::unordered_map<int, std::string> ump;
    std::list<double> lst;
    std::tuple<int, float, std::string> tpl;
};
REFLECT_FIELDS(ComplexTypes,
               cstr,
               str,
               arr,
               vec,
               pair,
               mp,
               ump,
               lst,
               tpl)

struct NestedComplex
{
    std::vector<std::vector<std::string>> vec_of_vec;
    std::array<std::array<int, 3>, 2> arr_of_arr;
    std::array<std::vector<Simple>, 2> arr_of_vec_of_struct;
};
REFLECT_FIELDS(NestedComplex, vec_of_vec, arr_of_arr, arr_of_vec_of_struct)

struct NotReflected
{
    int x;
    float y;
};

struct ContainsNotReflected
{
    NotReflected nr;

    struct Reflector
    {
        int a;
        float b;

        static ContainsNotReflected create(const Reflector &r)
        {
            ContainsNotReflected obj;
            obj.nr.x = r.a;
            obj.nr.y = r.b;
            return obj;
        }

        static Reflector reflect(const ContainsNotReflected &obj)
        {
            Reflector r;
            r.a = obj.nr.x;
            r.b = obj.nr.y;
            return r;
        }
    };
};
REFLECT_CUSTOM(ContainsNotReflected, a, b)