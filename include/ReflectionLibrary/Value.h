#pragma once

#include <variant>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <type_traits>
#include <sstream>
#include "ReflectionLibrary/Concepts.h"
#include "ReflectionLibrary/Helpers.h"

namespace ReflectionLibrary
{
    // Forward declarations
    class Value;
    class ValueProxy;

    // Type aliases
    using Null = std::monostate;
    using Boolean = bool;
    using Integer = int64_t;
    using Double = double;
    using String = std::string;
    using Array = std::vector<Value>;
    using Object = std::map<std::string, Value>;

    // Proxy class for lazy evaluation and implicit conversions
    class ValueProxy
    {
    private:
        Value *parent_;
        std::string key_;
        size_t index_;
        bool is_object_access_;
        std::vector<std::string> path_; // For better error messages

    public:
        ValueProxy(Value *parent, const std::string &key, std::vector<std::string> path = {});
        ValueProxy(Value *parent, size_t index, std::vector<std::string> path = {});

        // Conversion to Value for reading
        operator Value() const;
        const Value &get() const;

        // Assignment operators for writing (implicit conversions)
        ValueProxy &operator=(const Value &val);
        ValueProxy &operator=(Value &&val);
        ValueProxy &operator=(bool b);
        ValueProxy &operator=(int i);
        ValueProxy &operator=(int64_t i);
        ValueProxy &operator=(unsigned int i);
        ValueProxy &operator=(double d);
        ValueProxy &operator=(const char *s);
        ValueProxy &operator=(const std::string &s);
        ValueProxy &operator=(const Array &a);
        ValueProxy &operator=(const Object &o);

        // Chaining - object access
        ValueProxy operator[](const std::string &key);
        ValueProxy operator[](const char *key);

        // Chaining - array access
        ValueProxy operator[](size_t index);
        ValueProxy operator[](int index);

        // Type checking
        bool isNull() const;
        bool isBool() const;
        bool isInt() const;
        bool isDouble() const;
        bool isString() const;
        bool isArray() const;
        bool isObject() const;

        // Getters
        Boolean getBool() const;
        Integer getInt() const;
        Double getDouble() const;
        const String &getString() const;
        const Array &getArray() const;
        Array &getArray();
        const Object &getObject() const;
        Object &getObject();

        // Utility
        size_t size() const;
        bool has(const std::string &key) const;
        void push_back(const Value &val);
        void push_back(Value &&val);

    private:
        void ensure_writable();
        Value &get_writable();
        std::string build_path_string() const;
    };

    // Main Value class for dynamic type storage
    class Value
    {
    public:
        using VariantType = std::variant<Null, Boolean, Integer, Double, String, Array, Object>;

    private:
        VariantType data_;

    public:
        // Constructors
        Value();
        Value(bool b);
        Value(int i);
        Value(int64_t i);
        Value(unsigned int i);
        Value(double d);
        Value(const char *s);
        Value(const String &s);
        Value(String &&s);
        Value(const Array &a);
        Value(Array &&a);
        Value(const Object &o);
        Value(Object &&o);

        // Type checking
        bool isNull() const;
        bool isBool() const;
        bool isInt() const;
        bool isDouble() const;
        bool isString() const;
        bool isArray() const;
        bool isObject() const;

        // Getters with type checking
        Boolean getBool() const;
        Integer getInt() const;
        Double getDouble() const;
        const String &getString() const;
        String &getString();
        const Array &getArray() const;
        Array &getArray();
        const Object &getObject() const;
        Object &getObject();

        // Proxy-based access - returns proxy for lazy evaluation
        ValueProxy operator[](const std::string &key);
        ValueProxy operator[](const char *key);
        ValueProxy operator[](size_t index);
        ValueProxy operator[](int index);

        // Const versions for read-only access
        const Value &operator[](const std::string &key) const;
        const Value &operator[](const char *key) const;
        const Value &operator[](size_t index) const;
        const Value &operator[](int index) const;

        // Check if key exists
        bool has(const std::string &key) const;

        // Get array/object/string size
        size_t size() const;

        // Push back for arrays
        void push_back(const Value &val);
        void push_back(Value &&val);

        // Get underlying variant for advanced use
        const VariantType &variant() const;
        VariantType &variant();

        // Friend class for internal access
        friend class ValueProxy;
    };

    // Convert any C++ type to Value
    template <typename T>
    Value to_value(const T &obj);

    // Convert Value back to C++ type
    template <typename T>
    bool from_value(T &obj, const Value &value);

    // Convenience function to convert from Value (returns optional-like behavior)
    template <typename T>
    T from_value(const Value &value);

} // namespace ReflectionLibrary

#include "Value.inl"