#include "../include/Reflect.h"

#include <iostream>

struct Simple
{
    int a;
    float b;
    double c;
};
REFLECT(Simple, a, b, c)

// struct AllPrimitiveTypes
// {
//     bool a;
//     char b;
//     short c;
//     int d;
//     long e;
//     long long f;
//     unsigned char g;
//     unsigned short h;
//     unsigned int i;
//     unsigned long j;
//     unsigned long long k;
//     float l;
//     double m;
//     long double n;
// };
// REFLECT(AllPrimitiveTypes, a, b, c, d, e, f, g, h, i, j, k, l, m, n)

// struct Nested
// {
//     Simple s;
//     AllPrimitiveTypes apt;
// };
// REFLECT(Nested, s, apt)

// struct DeeplyNested
// {
//     Nested n;
//     Simple s;
// };
// REFLECT(DeeplyNested, n, s)

// struct ComplexTypes
// {
//     const char *cstr;
//     std::string str;
//     std::vector<int> vec;
//     std::map<std::string, float> mp;
//     std::unordered_map<int, std::string> ump;
//     std::list<double> lst;
//     std::array<double, 3> arr;
//     std::tuple<int, float, std::string> tpl;
// };
// REFLECT(ComplexTypes, cstr, str, vec, mp, ump, lst, arr, tpl)

int main()
{
    Simple s{42, 3.14f, 2.718};

    ReflectionInfo<Simple> info;
    std::cout << info.class_name << "\n";
    std::apply([&](auto &&...field)
               { ((std::cout << field.first << " = " << s.*(field.second) << "\n"), ...); },
               info.fields);

    // Reflect<Simple>::for_each_field(s, [](const std::string &name, auto &value)
    //                                 { std::cout << name << " = " << value << "\n"; });

    // auto names = Reflect<Simple>::get_field_names();
    // for (const auto &name : names)
    // {
    //     std::cout << "Field name: " << name << "\n";
    // }

    // size_t count = Reflect<Simple>::field_count();
    // std::cout << "Field count: " << count << "\n";

    // auto value = get_field_by_name(s, "a");
    // if (value)
    //     std::cout << "Field 'a' value: " << *value << "\n";
    // else
    //     std::cout << "Field 'a' not found\n";

    return 0;
}