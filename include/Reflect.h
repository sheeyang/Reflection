#pragma once

#include <tuple>
#include <type_traits>
#include <iostream>    // For std::cout
#include <string>      // For std::string and std::to_string
#include <string_view> // For std::string_view

// Count arguments (supports up to 20)
#define COUNT_ARGS(...) COUNT_ARGS_IMPL(__VA_ARGS__, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define COUNT_ARGS_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, N, ...) N

// Concatenation helper
#define CONCAT(A, B) CONCAT_IMPL(A, B)
#define CONCAT_IMPL(A, B) A##B

// Expand fields based on count
#define EXPAND_MACROS(CLASS, MACRO, N, ...) CONCAT(MACROS_EXPAND_, N)(CLASS, MACRO, __VA_ARGS__)

#define MACROS_EXPAND_1(C, M, F1) M(C, F1)
#define MACROS_EXPAND_2(C, M, F1, F2) MACROS_EXPAND_1(C, M, F1), M(C, F2)
#define MACROS_EXPAND_3(C, M, F1, F2, F3) MACROS_EXPAND_2(C, M, F1, F2), M(C, F3)
#define MACROS_EXPAND_4(C, M, F1, F2, F3, F4) MACROS_EXPAND_3(C, M, F1, F2, F3), M(C, F4)
#define MACROS_EXPAND_5(C, M, F1, F2, F3, F4, F5) MACROS_EXPAND_4(C, M, F1, F2, F3, F4), M(C, F5)
#define MACROS_EXPAND_6(C, M, F1, F2, F3, F4, F5, F6) MACROS_EXPAND_5(C, M, F1, F2, F3, F4, F5), M(C, F6)
#define MACROS_EXPAND_7(C, M, F1, F2, F3, F4, F5, F6, F7) MACROS_EXPAND_6(C, M, F1, F2, F3, F4, F5, F6), M(C, F7)
#define MACROS_EXPAND_8(C, M, F1, F2, F3, F4, F5, F6, F7, F8) MACROS_EXPAND_7(C, M, F1, F2, F3, F4, F5, F6, F7), M(C, F8)
#define MACROS_EXPAND_9(C, M, F1, F2, F3, F4, F5, F6, F7, F8, F9) MACROS_EXPAND_8(C, M, F1, F2, F3, F4, F5, F6, F7, F8), M(C, F9)
#define MACROS_EXPAND_10(C, M, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10) MACROS_EXPAND_9(C, M, F1, F2, F3, F4, F5, F6, F7, F8, F9), M(C, F10)
#define MACROS_EXPAND_11(C, M, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11) MACROS_EXPAND_10(C, M, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10), M(C, F11)
#define MACROS_EXPAND_12(C, M, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12) MACROS_EXPAND_11(C, M, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11), M(C, F12)
#define MACROS_EXPAND_13(C, M, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13) MACROS_EXPAND_12(C, M, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12), M(C, F13)
#define MACROS_EXPAND_14(C, M, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14) MACROS_EXPAND_13(C, M, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13), M(C, F14)
#define MACROS_EXPAND_15(C, M, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15) MACROS_EXPAND_14(C, M, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14), M(C, F15)
#define MACROS_EXPAND_16(C, M, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16) MACROS_EXPAND_15(C, M, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15), M(C, F16)
#define MACROS_EXPAND_17(C, M, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17) MACROS_EXPAND_16(C, M, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16), M(C, F17)
#define MACROS_EXPAND_18(C, M, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18) MACROS_EXPAND_17(C, M, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17), M(C, F18)
#define MACROS_EXPAND_19(C, M, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19) MACROS_EXPAND_18(C, M, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18), M(C, F19)
#define MACROS_EXPAND_20(C, M, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20) MACROS_EXPAND_19(C, M, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19), M(C, F20)
// Add more as needed...

// Forward declare the template
template <typename T>
struct ReflectionInfo;

template <typename T>
constexpr bool is_reflectable_v = requires { ReflectionInfo<T>::fields; };

#define FIELD_PAIR(CLASS, FIELD) std::make_pair(#FIELD, &CLASS::FIELD)

// Helper to check if a type is a std::tuple
template <typename T>
constexpr bool is_tuple_v = false;

template <typename... Ts>
constexpr bool is_tuple_v<std::tuple<Ts...>> = true;

// Helper to check if a type is a std::pair
template <typename T>
constexpr bool is_pair_v = false;

template <typename T1, typename T2>
constexpr bool is_pair_v<std::pair<T1, T2>> = true;

namespace Reflex
{
    template <typename T>
    static constexpr size_t field_count() { return std::tuple_size_v<decltype(ReflectionInfo<T>::fields)>; }

    template <typename T>
    static void for_each_field(T &obj, auto &&func, int nest_level = 0)
    {

        std::apply([&](auto &&...field)
                   { (([&]()
                       {
            auto &val = obj.*(field.second);
            func(field.first, val, nest_level);
            if constexpr (is_reflectable_v<std::decay_t<decltype(val)>>) {
                for_each_field(val, func, nest_level + 1);
            } }()),
                      ...); }, ReflectionInfo<T>::fields);
    }

    template <typename T, typename U>
    static void set_field_value(T &obj, std::string_view field_name, U &&value)
    {
        std::apply([&](auto &&...fields)
                   { (([&]()
                       {
            if (std::string_view(fields.first) == field_name) {
                using FieldType = std::decay_t<decltype(obj.*(fields.second))>;
                if constexpr (std::is_assignable_v<FieldType &, decltype(value)>) {
                    obj.*(fields.second) = std::forward<U>(value);
                }
            } }()),
                      ...); }, ReflectionInfo<T>::fields);
    }

    template <typename FieldType, typename T>
    static FieldType &get_field_value(T &obj, std::string_view field_name)
    {
        FieldType *result = nullptr;
        std::apply([&](auto &&...fields)
                   { (([&]()
                       {
            if (std::string_view(fields.first) == field_name) {
                using ActualFieldType = std::decay_t<decltype(obj.*(fields.second))>;
                if constexpr (std::is_same_v<ActualFieldType, FieldType>) {
                    result = &(obj.*(fields.second));
                }
            } }()),
                      ...); }, ReflectionInfo<T>::fields);

        if (result == nullptr)
        {
            throw std::runtime_error(std::string("Field '") + std::string(field_name) + "' not found or type mismatch");
        }
        return *result;
    }

    // Helper function to print any value recursively
    template <typename T>
    static void print_value(std::string_view indent, std::string_view name, T &value, int nest_level)
    {
        using ValueType = std::decay_t<T>;
        if constexpr (std::is_same_v<ValueType, bool>)
        {
            std::cout << indent << name << " = " << (value ? "true" : "false") << "\n";
        }
        else if constexpr (std::is_same_v<ValueType, char>)
        {
            std::cout << indent << name << " = '" << value << "'\n";
        }
        else if constexpr (std::is_arithmetic_v<ValueType>)
        {
            // Other arithmetic types (int, float, etc.)
            std::cout << indent << name << " = " << value << "\n";
        }
        else if constexpr (std::is_same_v<ValueType, std::string>)
        {
            std::cout << indent << name << " = \"" << value << "\"\n";
        }
        else if constexpr (std::is_same_v<ValueType, const char *>)
        {
            std::cout << indent << name << " = \"" << (value ? value : "null") << "\"\n";
        }
        else if constexpr (is_tuple_v<ValueType>)
        {
            std::cout << indent << name << " = (\n";
            std::apply([&](auto &...args)
                       {
                int i = 0;
                ((print_value(indent, std::to_string(i++), args, nest_level + 1)), ...); }, value);
            std::cout << indent << ")\n";
        }
        else if constexpr (is_pair_v<ValueType>)
        {
            std::cout << indent << name << " = {\n";
            print_value(indent, "first", value.first, nest_level + 1);
            print_value(indent, "second", value.second, nest_level + 1);
            std::cout << indent << "}\n";
        }
        else if constexpr (requires { value.size(); value.begin(); value.end(); })
        {
            // Iterable containers (arrays, vectors, lists, maps, etc.)
            std::cout << indent << name << " = [\n";
            int i = 0;
            for (auto &elem : value)
            {
                print_value(indent, std::to_string(i++), elem, nest_level + 1);
            }
            std::cout << indent << "]\n";
        }
        else if constexpr (is_reflectable_v<ValueType>)
        {
            // Nested reflectable type
            std::cout << indent << name << " (" << ReflectionInfo<ValueType>::class_name << "):\n";
            for_each_field(value, [&](std::string_view sub_name, auto &sub_value, int)
                           { print_value(indent, sub_name, sub_value, nest_level + 1); }, nest_level + 1);
        }
        else
        {
            // Fallback for other non-reflectable types
            std::cout << indent << name << " = [non-reflectable type]\n";
        }
    }

    template <typename T>
    static void print(T &obj)
    {
        std::cout << "Type: " << ReflectionInfo<T>::class_name << "\n";
        for_each_field(obj, [](std::string_view name, auto &value, int nest_level)
                       {
            std::string indent(2 + nest_level * 2, ' ');
            print_value(indent, name, value, nest_level); }, 0);
    }
} // namespace Reflex

#define REFLECT(CLASS_NAME, ...)                                                                                                     \
    template <>                                                                                                                      \
    struct ReflectionInfo<CLASS_NAME>                                                                                                \
    {                                                                                                                                \
        static constexpr const char *class_name = #CLASS_NAME;                                                                       \
        static constexpr auto fields = std::make_tuple(EXPAND_MACROS(CLASS_NAME, FIELD_PAIR, COUNT_ARGS(__VA_ARGS__), __VA_ARGS__)); \
    };
