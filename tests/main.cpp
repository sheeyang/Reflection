#include "../include/Reflect.h"

#include <iostream>

struct Simple
{
    int a;
    float b;
    double c;
};
REFLECT(Simple, a, b, c)

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
REFLECT(AllPrimitiveTypes, a, b, c, d, e, f, g, h, i, j, k, l, m, n)

struct Nested
{
    Simple s;
    AllPrimitiveTypes apt;
};
REFLECT(Nested, s, apt)

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
    auto print_field = [](const char *name, auto &value)
    {
        if constexpr (std::is_fundamental_v<std::decay_t<decltype(value)>>)
        {
            std::cout << name << " = " << value << "\n";
        }
    };

    Simple s{42, 3.14f, 2.718};

    std::cout << Reflex<Simple>::class_name << "\n";
    std::cout << "Field count: " << Reflex<Simple>::field_count() << "\n";
    Reflex<Simple>::for_each_field(s, print_field);

    // AllPrimitiveTypes apt{true, 'x', 123, 456, 789L, 101112LL, 200, 30000, 4000000, 50000000UL, 6000000000ULL, 1.23f, 4.56, 7.89L};
    // std::cout << "\n"
    //           << Reflex<AllPrimitiveTypes>::class_name << "\n";
    // std::cout << "Field count: " << Reflex<AllPrimitiveTypes>::field_count() << "\n";
    // Reflex<AllPrimitiveTypes>::for_each_field(apt, print_field);

    // Reflex<Simple>::for_each_field(s, [](const std::string &name, auto &value)
    //                                 { std::cout << name << " = " << value << "\n"; });

    Nested n{{1, 2.0f, 3.0}, {true, 'y', 456, 789, 101112L, 131415LL, 200, 40000, 5000000, 60000000UL, 7000000000ULL, 2.34f, 5.67, 8.90L}};

    std::cout << "\n"
              << Reflex<Nested>::class_name << "\n";
    std::cout << "Field count: " << Reflex<Nested>::field_count() << "\n";
    Reflex<Nested>::for_each_field(n, print_field);

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