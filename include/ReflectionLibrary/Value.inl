#pragma once

#include "Value.h"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

namespace ReflectionLibrary
{
    // ValueProxy implementation
    inline ValueProxy::ValueProxy(Value *parent, const std::string &key, std::vector<std::string> path)
        : parent_(parent), key_(key), index_(0), is_object_access_(true), path_(std::move(path))
    {
        path_.push_back(key);
    }

    inline ValueProxy::ValueProxy(Value *parent, size_t index, std::vector<std::string> path)
        : parent_(parent), key_(), index_(index), is_object_access_(false), path_(std::move(path))
    {
        path_.push_back("[" + std::to_string(index) + "]");
    }

    inline std::string ValueProxy::build_path_string() const
    {
        std::ostringstream oss;
        for (size_t i = 0; i < path_.size(); ++i)
        {
            if (i > 0 && path_[i][0] != '[')
                oss << ".";
            oss << path_[i];
        }
        return oss.str();
    }

    inline void ValueProxy::ensure_writable()
    {
        if (!parent_)
            throw std::runtime_error("Invalid proxy: null parent");

        if (is_object_access_)
        {
            if (!parent_->isObject() && !parent_->isNull())
            {
                throw std::runtime_error("Cannot access key '" + key_ + "' on non-object type at: " + build_path_string());
            }
            if (parent_->isNull())
            {
                parent_->data_ = Object{};
            }
        }
        else
        {
            if (!parent_->isArray() && !parent_->isNull())
            {
                throw std::runtime_error("Cannot index non-array type at: " + build_path_string());
            }
            if (parent_->isNull())
            {
                // Auto-create array and resize to accommodate the index
                parent_->data_ = Array{};
            }
            // Resize array if necessary
            auto &arr = parent_->getArray();
            if (index_ >= arr.size())
            {
                arr.resize(index_ + 1);
            }
        }
    }

    inline Value &ValueProxy::get_writable()
    {
        ensure_writable();
        if (is_object_access_)
        {
            return parent_->getObject()[key_];
        }
        else
        {
            return parent_->getArray()[index_];
        }
    }

    inline const Value &ValueProxy::get() const
    {
        if (!parent_)
            throw std::runtime_error("Invalid proxy: null parent");

        if (is_object_access_)
        {
            if (!parent_->isObject())
            {
                throw std::runtime_error("Cannot access key '" + key_ + "' on non-object type at: " + build_path_string());
            }
            const auto &obj = parent_->getObject();
            auto it = obj.find(key_);
            if (it == obj.end())
            {
                throw std::out_of_range("Key '" + key_ + "' not found at: " + build_path_string());
            }
            return it->second;
        }
        else
        {
            if (!parent_->isArray())
            {
                throw std::runtime_error("Cannot index non-array type at: " + build_path_string());
            }
            const auto &arr = parent_->getArray();
            if (index_ >= arr.size())
            {
                throw std::out_of_range("Array index " + std::to_string(index_) + " out of bounds at: " + build_path_string());
            }
            return arr[index_];
        }
    }

    inline ValueProxy::operator Value() const
    {
        return get();
    }

    // Assignment operators
    inline ValueProxy &ValueProxy::operator=(const Value &val)
    {
        get_writable() = val;
        return *this;
    }

    inline ValueProxy &ValueProxy::operator=(Value &&val)
    {
        get_writable() = std::move(val);
        return *this;
    }

    inline ValueProxy &ValueProxy::operator=(bool b)
    {
        get_writable() = Value(b);
        return *this;
    }

    inline ValueProxy &ValueProxy::operator=(int i)
    {
        get_writable() = Value(i);
        return *this;
    }

    inline ValueProxy &ValueProxy::operator=(int64_t i)
    {
        get_writable() = Value(i);
        return *this;
    }

    inline ValueProxy &ValueProxy::operator=(unsigned int i)
    {
        get_writable() = Value(i);
        return *this;
    }

    inline ValueProxy &ValueProxy::operator=(double d)
    {
        get_writable() = Value(d);
        return *this;
    }

    inline ValueProxy &ValueProxy::operator=(const char *s)
    {
        get_writable() = Value(s);
        return *this;
    }

    inline ValueProxy &ValueProxy::operator=(const std::string &s)
    {
        get_writable() = Value(s);
        return *this;
    }

    inline ValueProxy &ValueProxy::operator=(const Array &a)
    {
        get_writable() = Value(a);
        return *this;
    }

    inline ValueProxy &ValueProxy::operator=(const Object &o)
    {
        get_writable() = Value(o);
        return *this;
    }

    // Chaining
    inline ValueProxy ValueProxy::operator[](const std::string &key)
    {
        Value &val = get_writable();
        return ValueProxy(&val, key, path_);
    }

    inline ValueProxy ValueProxy::operator[](const char *key)
    {
        return (*this)[std::string(key)];
    }

    inline ValueProxy ValueProxy::operator[](size_t index)
    {
        Value &val = get_writable();
        return ValueProxy(&val, index, path_);
    }

    inline ValueProxy ValueProxy::operator[](int index)
    {
        return (*this)[static_cast<size_t>(index)];
    }

    // Type checking through proxy
    inline bool ValueProxy::isNull() const { return get().isNull(); }
    inline bool ValueProxy::isBool() const { return get().isBool(); }
    inline bool ValueProxy::isInt() const { return get().isInt(); }
    inline bool ValueProxy::isDouble() const { return get().isDouble(); }
    inline bool ValueProxy::isString() const { return get().isString(); }
    inline bool ValueProxy::isArray() const { return get().isArray(); }
    inline bool ValueProxy::isObject() const { return get().isObject(); }

    // Getters through proxy
    inline Boolean ValueProxy::getBool() const { return get().getBool(); }
    inline Integer ValueProxy::getInt() const { return get().getInt(); }
    inline Double ValueProxy::getDouble() const { return get().getDouble(); }
    inline const String &ValueProxy::getString() const { return get().getString(); }
    inline const Array &ValueProxy::getArray() const { return get().getArray(); }
    inline Array &ValueProxy::getArray() { return get_writable().getArray(); }
    inline const Object &ValueProxy::getObject() const { return get().getObject(); }
    inline Object &ValueProxy::getObject() { return get_writable().getObject(); }

    inline size_t ValueProxy::size() const { return get().size(); }
    inline bool ValueProxy::has(const std::string &key) const { return get().has(key); }
    inline void ValueProxy::push_back(const Value &val) { get_writable().push_back(val); }
    inline void ValueProxy::push_back(Value &&val) { get_writable().push_back(std::move(val)); }

    // Value implementation
    inline Value::Value() : data_(Null{}) {}
    inline Value::Value(bool b) : data_(b) {}
    inline Value::Value(int i) : data_(static_cast<Integer>(i)) {}
    inline Value::Value(int64_t i) : data_(i) {}
    inline Value::Value(unsigned int i) : data_(static_cast<Integer>(i)) {}
    inline Value::Value(double d) : data_(d) {}
    inline Value::Value(const char *s) : data_(String(s)) {}
    inline Value::Value(const String &s) : data_(s) {}
    inline Value::Value(String &&s) : data_(std::move(s)) {}
    inline Value::Value(const Array &a) : data_(a) {}
    inline Value::Value(Array &&a) : data_(std::move(a)) {}
    inline Value::Value(const Object &o) : data_(o) {}
    inline Value::Value(Object &&o) : data_(std::move(o)) {}

    inline bool Value::isNull() const { return std::holds_alternative<Null>(data_); }
    inline bool Value::isBool() const { return std::holds_alternative<Boolean>(data_); }
    inline bool Value::isInt() const { return std::holds_alternative<Integer>(data_); }
    inline bool Value::isDouble() const { return std::holds_alternative<Double>(data_); }
    inline bool Value::isString() const { return std::holds_alternative<String>(data_); }
    inline bool Value::isArray() const { return std::holds_alternative<Array>(data_); }
    inline bool Value::isObject() const { return std::holds_alternative<Object>(data_); }

    inline Boolean Value::getBool() const
    {
        if (!isBool())
            throw std::runtime_error("Value is not a boolean");
        return std::get<Boolean>(data_);
    }

    inline Integer Value::getInt() const
    {
        if (!isInt())
            throw std::runtime_error("Value is not an integer");
        return std::get<Integer>(data_);
    }

    inline Double Value::getDouble() const
    {
        if (!isDouble())
            throw std::runtime_error("Value is not a double");
        return std::get<Double>(data_);
    }

    inline const String &Value::getString() const
    {
        if (!isString())
            throw std::runtime_error("Value is not a string");
        return std::get<String>(data_);
    }

    inline String &Value::getString()
    {
        if (!isString())
            throw std::runtime_error("Value is not a string");
        return std::get<String>(data_);
    }

    inline const Array &Value::getArray() const
    {
        if (!isArray())
            throw std::runtime_error("Value is not an array");
        return std::get<Array>(data_);
    }

    inline Array &Value::getArray()
    {
        if (!isArray())
            throw std::runtime_error("Value is not an array");
        return std::get<Array>(data_);
    }

    inline const Object &Value::getObject() const
    {
        if (!isObject())
            throw std::runtime_error("Value is not an object");
        return std::get<Object>(data_);
    }

    inline Object &Value::getObject()
    {
        if (!isObject())
            throw std::runtime_error("Value is not an object");
        return std::get<Object>(data_);
    }

    inline ValueProxy Value::operator[](const std::string &key)
    {
        return ValueProxy(this, key);
    }

    inline ValueProxy Value::operator[](const char *key)
    {
        return ValueProxy(this, std::string(key));
    }

    inline ValueProxy Value::operator[](size_t index)
    {
        return ValueProxy(this, index);
    }

    inline ValueProxy Value::operator[](int index)
    {
        return ValueProxy(this, static_cast<size_t>(index));
    }

    inline const Value &Value::operator[](const std::string &key) const
    {
        if (!isObject())
        {
            throw std::runtime_error("Cannot access key on non-object type");
        }
        const auto &obj = getObject();
        auto it = obj.find(key);
        if (it == obj.end())
        {
            throw std::out_of_range("Key not found: " + key);
        }
        return it->second;
    }

    inline const Value &Value::operator[](const char *key) const
    {
        return (*this)[std::string(key)];
    }

    inline const Value &Value::operator[](size_t index) const
    {
        if (!isArray())
        {
            throw std::runtime_error("Cannot index non-array type");
        }
        const auto &arr = getArray();
        if (index >= arr.size())
        {
            throw std::out_of_range("Array index out of bounds");
        }
        return arr[index];
    }

    inline const Value &Value::operator[](int index) const
    {
        return (*this)[static_cast<size_t>(index)];
    }

    inline bool Value::has(const std::string &key) const
    {
        if (!isObject())
            return false;
        return getObject().find(key) != getObject().end();
    }

    inline size_t Value::size() const
    {
        if (isArray())
            return getArray().size();
        if (isObject())
            return getObject().size();
        if (isString())
            return getString().size();
        return 0;
    }

    inline void Value::push_back(const Value &val)
    {
        if (!isArray())
        {
            data_ = Array{};
        }
        getArray().push_back(val);
    }

    inline void Value::push_back(Value &&val)
    {
        if (!isArray())
        {
            data_ = Array{};
        }
        getArray().push_back(std::move(val));
    }

    inline const Value::VariantType &Value::variant() const { return data_; }
    inline Value::VariantType &Value::variant() { return data_; }

    // Convert any C++ type to Value
    template <typename T>
    inline Value to_value(const T &obj)
    {
        using ValueType = std::decay_t<T>;

        if constexpr (std::is_same_v<ValueType, bool>)
        {
            return Value(obj);
        }
        else if constexpr (std::is_integral_v<ValueType>)
        {
            return Value(static_cast<int64_t>(obj));
        }
        else if constexpr (std::is_floating_point_v<ValueType>)
        {
            return Value(static_cast<double>(obj));
        }
        else if constexpr (std::is_same_v<ValueType, std::string> ||
                           std::is_same_v<ValueType, const char *> ||
                           std::is_same_v<ValueType, char *>)
        {
            return Value(String(obj));
        }
        else if constexpr (is_pair_v<ValueType>)
        {
            Object result;
            result["first"] = to_value(obj.first);
            result["second"] = to_value(obj.second);
            return Value(std::move(result));
        }
        else if constexpr (is_tuple_v<ValueType>)
        {
            Array result;
            std::apply([&](const auto &...args)
                       { ((result.push_back(to_value(args))), ...); },
                       obj);
            return Value(std::move(result));
        }
        else if constexpr (is_iterable_v<ValueType>)
        {
            // Handle maps specially
            if constexpr (requires { typename ValueType::key_type; typename ValueType::mapped_type; })
            {
                Array result;
                for (const auto &[key, val] : obj)
                {
                    Object pair;
                    pair["first"] = to_value(key);
                    pair["second"] = to_value(val);
                    result.push_back(Value(std::move(pair)));
                }
                return Value(std::move(result));
            }
            else
            {
                // Sequential containers
                Array result;
                for (const auto &elem : obj)
                {
                    result.push_back(to_value(elem));
                }
                return Value(std::move(result));
            }
        }
        else if constexpr (is_reflectable_v<ValueType>)
        {
            Object result;
            auto &obj_ref = const_cast<ValueType &>(obj);
            for_each_field(obj_ref, [&](std::string_view name, auto &field_value, int)
                           { result[std::string(name)] = to_value(field_value); });
            return Value(std::move(result));
        }
        else
        {
            // Fallback for unsupported types
            return Value();
        }
    }

    // Convert Value back to C++ type
    template <typename T>
    inline bool from_value(T &obj, const Value &value)
    {
        using ValueType = std::decay_t<T>;

        if constexpr (std::is_same_v<ValueType, bool>)
        {
            if (!value.isBool())
                return false;
            obj = value.getBool();
            return true;
        }
        else if constexpr (std::is_integral_v<ValueType>)
        {
            if (!value.isInt())
                return false;
            obj = static_cast<ValueType>(value.getInt());
            return true;
        }
        else if constexpr (std::is_floating_point_v<ValueType>)
        {
            if (value.isDouble())
                obj = static_cast<ValueType>(value.getDouble());
            else if (value.isInt())
                obj = static_cast<ValueType>(value.getInt());
            else
                return false;
            return true;
        }
        else if constexpr (std::is_same_v<ValueType, std::string>)
        {
            if (!value.isString())
                return false;
            obj = value.getString();
            return true;
        }
        else if constexpr (is_pair_v<ValueType>)
        {
            if (!value.isObject())
                return false;
            if (value.has("first") && !from_value(obj.first, value["first"]))
                return false;
            if (value.has("second") && !from_value(obj.second, value["second"]))
                return false;
            return true;
        }
        else if constexpr (is_tuple_v<ValueType>)
        {
            if (!value.isArray())
                return false;
            const auto &arr = value.getArray();
            size_t idx = 0;
            bool success = true;
            std::apply([&](auto &...args)
                       { (([&]()
                           {
                    if (idx < arr.size() && success)
                        success = from_value(args, arr[idx]);
                    ++idx; }()),
                          ...); },
                       obj);
            return success;
        }
        else if constexpr (is_iterable_v<ValueType>)
        {
            if (!value.isArray())
                return false;
            const auto &arr = value.getArray();
            obj.clear();

            // Handle maps differently from vectors/lists
            if constexpr (requires { typename ValueType::key_type; typename ValueType::mapped_type; })
            {
                for (const auto &elem : arr)
                {
                    std::pair<typename ValueType::key_type, typename ValueType::mapped_type> pair{};
                    if (!from_value(pair, elem))
                        return false;
                    obj.insert(std::move(pair));
                }
            }
            else
            {
                for (const auto &elem : arr)
                {
                    typename ValueType::value_type val{};
                    if (!from_value(val, elem))
                        return false;
                    obj.insert(obj.end(), std::move(val));
                }
            }
            return true;
        }
        else if constexpr (is_reflectable_v<ValueType>)
        {
            if (!value.isObject())
                return false;
            bool success = true;
            for_each_field(obj, [&](std::string_view name, auto &field_value, int)
                           {
                std::string key(name);
                if (value.has(key))
                {
                    if (!from_value(field_value, value[key]))
                        success = false;
                } });
            return success;
        }
        else
        {
            return false;
        }
    }

    // Convenience function to convert from Value (returns optional-like behavior)
    template <typename T>
    inline T from_value(const Value &value)
    {
        T result{};
        if (!from_value(result, value))
        {
            throw std::runtime_error("Failed to convert Value to target type");
        }
        return result;
    }

    // Helper function to convert Value to RapidJSON Value
    static void value_to_rapidjson(const Value &value, rapidjson::Value &json_val, rapidjson::Document::AllocatorType &allocator)
    {
        if (value.isNull())
        {
            json_val.SetNull();
        }
        else if (value.isBool())
        {
            json_val.SetBool(value.getBool());
        }
        else if (value.isInt())
        {
            json_val.SetInt64(value.getInt());
        }
        else if (value.isDouble())
        {
            json_val.SetDouble(value.getDouble());
        }
        else if (value.isString())
        {
            const auto &str = value.getString();
            json_val.SetString(str.c_str(), static_cast<rapidjson::SizeType>(str.length()), allocator);
        }
        else if (value.isArray())
        {
            json_val.SetArray();
            const auto &arr = value.getArray();
            for (const auto &elem : arr)
            {
                rapidjson::Value elem_val;
                value_to_rapidjson(elem, elem_val, allocator);
                json_val.PushBack(elem_val, allocator);
            }
        }
        else if (value.isObject())
        {
            json_val.SetObject();
            const auto &obj = value.getObject();
            for (const auto &[key, val] : obj)
            {
                rapidjson::Value key_val(key.c_str(), static_cast<rapidjson::SizeType>(key.length()), allocator);
                rapidjson::Value val_json;
                value_to_rapidjson(val, val_json, allocator);
                json_val.AddMember(key_val, val_json, allocator);
            }
        }
    }

    // Helper function to convert RapidJSON Value to Value
    static Value rapidjson_to_value(const rapidjson::Value &json_val)
    {
        if (json_val.IsNull())
        {
            return Value();
        }
        else if (json_val.IsBool())
        {
            return Value(json_val.GetBool());
        }
        else if (json_val.IsInt64())
        {
            return Value(json_val.GetInt64());
        }
        else if (json_val.IsInt())
        {
            return Value(static_cast<int64_t>(json_val.GetInt()));
        }
        else if (json_val.IsUint64())
        {
            return Value(static_cast<int64_t>(json_val.GetUint64()));
        }
        else if (json_val.IsUint())
        {
            return Value(static_cast<int64_t>(json_val.GetUint()));
        }
        else if (json_val.IsDouble())
        {
            return Value(json_val.GetDouble());
        }
        else if (json_val.IsString())
        {
            return Value(std::string(json_val.GetString()));
        }
        else if (json_val.IsArray())
        {
            Array arr;
            for (auto it = json_val.Begin(); it != json_val.End(); ++it)
            {
                arr.push_back(rapidjson_to_value(*it));
            }
            return Value(std::move(arr));
        }
        else if (json_val.IsObject())
        {
            Object obj;
            for (auto it = json_val.MemberBegin(); it != json_val.MemberEnd(); ++it)
            {
                obj[it->name.GetString()] = rapidjson_to_value(it->value);
            }
            return Value(std::move(obj));
        }

        return Value();
    }

    std::string value_to_json(const Value &value, bool pretty)
    {
        rapidjson::Document doc;
        auto &allocator = doc.GetAllocator();

        value_to_rapidjson(value, doc, allocator);

        rapidjson::StringBuffer buffer;
        if (pretty)
        {
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
            doc.Accept(writer);
        }
        else
        {
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            doc.Accept(writer);
        }

        return buffer.GetString();
    }

    std::optional<Value> value_from_json(const std::string &json_str)
    {
        rapidjson::Document doc;
        doc.Parse(json_str.c_str());

        if (doc.HasParseError())
        {
            return std::nullopt;
        }

        return rapidjson_to_value(doc);
    }

} // namespace ReflectionLibrary