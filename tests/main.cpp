#include "../include/Reflect.h"

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <map>
#include <unordered_map>
#include <list>
#include <tuple>

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

int main()
{

    // Simple s{42, 3.14f, 2.718};

    // std::cout << Reflex<Simple>::class_name << "\n";
    // std::cout << "Field count: " << Reflex<Simple>::field_count() << "\n";
    // Reflex<Simple>::for_each_field(s, print_field);
    // std::cout << "\n";

    // Reflex::print(s);

    // AllPrimitiveTypes apt{true, 'x', 123, 456, 789L, 101112LL, 200, 30000, 4000000, 50000000UL, 6000000000ULL, 1.23f, 4.56, 7.89L};

    // Nested n{123, {1, 2.0f, 3.0}, {true, 'y', 456, 789, 101112L, 131415LL, 200, 40000, 5000000, 60000000UL, 7000000000ULL, 2.34f, 5.67, 8.90L}};
    // Reflex::print(n);

    // DeeplyNested dn{{456, {4, 5.0f, 6.0}, {false, 'z', 789, 101112, 131415L, 161718LL, 200, 50000, 6000000, 70000000UL, 8000000000ULL, 3.45f, 6.78, 9.01L}}, {7, 8.0f, 9.0}};
    // Reflex::print(dn);

    ComplexTypes ct{
        "Hello, World!",
        "Reflect in C++",
        {1.1, 2.2, 3.3},
        {10, 20, 30, 40, 50},
        {"pi", 3.14f},
        {{"pi", 3.14f}, {"e", 2.71f}},
        {{1, "one"}, {2, "two"}, {3, "three"}},
        {0.1, 0.2, 0.3, 0.4, 0.5},
        {100, 200.0f, "three hundred"}};
    Reflex::print(ct);

    Reflex::set_field_value(ct, "str", "Goodbye, World.");

    NestedComplex nc{
        {{"one", "two"}, {"three", "four", "five"}, {"six"}},
        {{{1, 2, 3}, {4, 5, 6}}},
        {{{{1, 1.1f, 1.11}, {2, 2.2f, 2.22}, {3, 3.3f, 3.33}}, {{4, 4.4f, 4.44}, {5, 5.5f, 5.55}, {6, 6.6f, 6.66}}}}};
    Reflex::print(nc);

    ContainsNotReflected cnr{{123, 4.56f}};
    Reflex::print(cnr);
    std::cout << "nr.x = " << cnr.nr.x << ", nr.y = " << cnr.nr.y << "\n";
    Reflex::set_field_value(cnr, "a", 789);
    Reflex::set_field_value(cnr, "b", 0.12f);
    Reflex::print(cnr);
    std::cout << "nr.x = " << cnr.nr.x << ", nr.y = " << cnr.nr.y << "\n";

    // auto names = Reflex<Simple>::get_field_names();
    // for (const auto &name : names)
    // {
    //     std::cout << "Field name: " << name << "\n";
    // }

    // size_t count = Reflex<Simple>::field_count();
    // std::cout << "Field count: " << count << "\n";

    // auto value = get_field_by_name(s, "a");
    // if (value)
    //     std::cout << "Field 'a' value: " << *value << "\n";
    // else
    //     std::cout << "Field 'a' not found\n";

    return 0;
}