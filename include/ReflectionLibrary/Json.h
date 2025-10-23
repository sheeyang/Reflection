#pragma once

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <string>
#include <type_traits>
#include <optional>

namespace ReflectionLibrary
{
  // Serialize value to JSON
  template <typename T>
  static void serialize_value(const T& value, rapidjson::Value& json_val, rapidjson::Document::AllocatorType& allocator)
  {
    using ValueType = std::decay_t<T>;

    if constexpr (std::is_same_v<ValueType, bool>)
    {
      json_val.SetBool(value);
    }
    else if constexpr (std::is_integral_v<ValueType> && std::is_signed_v<ValueType>)
    {
      if constexpr (sizeof(ValueType) <= sizeof(int))
      {
        json_val.SetInt(static_cast<int>(value));
      }
      else
      {
        json_val.SetInt64(static_cast<int64_t>(value));
      }
    }
    else if constexpr (std::is_integral_v<ValueType> && std::is_unsigned_v<ValueType>)
    {
      if constexpr (sizeof(ValueType) <= sizeof(unsigned int))
      {
        json_val.SetUint(static_cast<unsigned int>(value));
      }
      else
      {
        json_val.SetUint64(static_cast<uint64_t>(value));
      }
    }
    else if constexpr (std::is_floating_point_v<ValueType>)
    {
      json_val.SetDouble(static_cast<double>(value));
    }
    else if constexpr (std::is_same_v<ValueType, std::string>)
    {
      json_val.SetString(value.c_str(), static_cast<rapidjson::SizeType>(value.length()), allocator);
    }
    else if constexpr (std::is_same_v<ValueType, const char*> || std::is_same_v<ValueType, char*>)
    {
      json_val.SetString(value, allocator);
    }
    else if constexpr (is_pair_v<ValueType>)
    {
      json_val.SetObject();
      rapidjson::Value first_val, second_val;
      serialize_value(value.first, first_val, allocator);
      serialize_value(value.second, second_val, allocator);
      json_val.AddMember("first", first_val, allocator);
      json_val.AddMember("second", second_val, allocator);
    }
    else if constexpr (is_tuple_v<ValueType>)
    {
      json_val.SetArray();
      std::apply([&](const auto &...args)
        { ((
          [&]()
          {
            rapidjson::Value elem;
            serialize_value(args, elem, allocator);
            json_val.PushBack(elem, allocator);
          }()),
          ...); }, value);
    }
    else if constexpr (is_iterable_v<ValueType>)
    {
      json_val.SetArray();
      for (const auto& elem : value)
      {
        rapidjson::Value elem_val;
        serialize_value(elem, elem_val, allocator);
        json_val.PushBack(elem_val, allocator);
      }
    }
    else if constexpr (is_reflectable_v<ValueType>)
    {
      json_val.SetObject();
      auto& obj_ref = const_cast<ValueType&>(value);
      for_each_field(obj_ref, [&](std::string_view name, auto& field_value, int)
        {
          rapidjson::Value field_val;
          serialize_value(field_value, field_val, allocator);
          rapidjson::Value key(name.data(), static_cast<rapidjson::SizeType>(name.length()), allocator);
          json_val.AddMember(key, field_val, allocator); });
    }
    else
    {
      // Fallback for unsupported types
      json_val.SetNull();
    }
  }

  // Deserialize value from JSON
  template <typename T>
  static bool deserialize_value(T& value, const rapidjson::Value& json_val)
  {
    using ValueType = std::decay_t<T>;

    if constexpr (std::is_same_v<ValueType, bool>)
    {
      if (!json_val.IsBool())
        return false;
      value = json_val.GetBool();
      return true;
    }
    else if constexpr (std::is_integral_v<ValueType> && std::is_signed_v<ValueType>)
    {
      if (json_val.IsInt64())
        value = static_cast<ValueType>(json_val.GetInt64());
      else if (json_val.IsInt())
        value = static_cast<ValueType>(json_val.GetInt());
      else
        return false;
      return true;
    }
    else if constexpr (std::is_integral_v<ValueType> && std::is_unsigned_v<ValueType>)
    {
      if (json_val.IsUint64())
        value = static_cast<ValueType>(json_val.GetUint64());
      else if (json_val.IsUint())
        value = static_cast<ValueType>(json_val.GetUint());
      else
        return false;
      return true;
    }
    else if constexpr (std::is_floating_point_v<ValueType>)
    {
      if (json_val.IsDouble())
        value = static_cast<ValueType>(json_val.GetDouble());
      else if (json_val.IsInt())
        value = static_cast<ValueType>(json_val.GetInt());
      else
        return false;
      return true;
    }
    else if constexpr (std::is_same_v<ValueType, std::string>)
    {
      if (!json_val.IsString())
        return false;
      value = json_val.GetString();
      return true;
    }
    else if constexpr (is_pair_v<ValueType>)
    {
      if (!json_val.IsObject())
        return false;
      if (json_val.HasMember("first") && !deserialize_value(value.first, json_val["first"]))
        return false;
      if (json_val.HasMember("second") && !deserialize_value(value.second, json_val["second"]))
        return false;
      return true;
    }
    else if constexpr (is_tuple_v<ValueType>)
    {
      if (!json_val.IsArray())
        return false;
      size_t idx = 0;
      bool success = true;
      std::apply([&](auto &...args)
        { (([&]()
          {
            if (idx < json_val.Size() && success)
              success = deserialize_value(args, json_val[static_cast<rapidjson::SizeType>(idx)]);
            ++idx; }()),
          ...); }, value);
      return success;
    }
    else if constexpr (is_iterable_v<ValueType>)
    {
      if (!json_val.IsArray())
        return false;
      value.clear();

      // Handle maps/sets differently from vectors/lists
      if constexpr (requires { typename ValueType::key_type; typename ValueType::mapped_type; })
      {
        // Map type
        for (auto it = json_val.Begin(); it != json_val.End(); ++it)
        {
          std::pair<typename ValueType::key_type, typename ValueType::mapped_type> elem{};
          if (!deserialize_value(elem, *it))
            return false;
          value.insert(std::move(elem));
        }
      }
      else
      {
        // Sequential container
        for (auto it = json_val.Begin(); it != json_val.End(); ++it)
        {
          typename ValueType::value_type elem{};
          if (!deserialize_value(elem, *it))
            return false;
          value.insert(value.end(), std::move(elem));
        }
      }
      return true;
    }
    else if constexpr (is_reflectable_v<ValueType>)
    {
      if (!json_val.IsObject())
        return false;
      bool success = true;
      for_each_field(value, [&](std::string_view name, auto& field_value, int)
        {
          if (json_val.HasMember(name.data()))
          {
            if (!deserialize_value(field_value, json_val[name.data()]))
              success = false;
          } });
          return success;
    }
    else
    {
      // Fallback for unsupported types
      return false;
    }
  }

  // Serialize object to JSON value
  template <typename T>
  static void serialize_obj(T& obj, rapidjson::Value& json_val, rapidjson::Document::AllocatorType& allocator)
  {
    json_val.SetObject();
    for_each_field(obj, [&](std::string_view name, auto& field_value, int)
      {
        rapidjson::Value field_val;
        serialize_value(field_value, field_val, allocator);
        rapidjson::Value key(name.data(), static_cast<rapidjson::SizeType>(name.length()), allocator);
        json_val.AddMember(key, field_val, allocator); });
  }

  // Deserialize object from JSON value
  template <typename T>
  static bool deserialize_obj(T& obj, const rapidjson::Value& json_val)
  {
    if (!json_val.IsObject())
      return false;

    bool success = true;
    for_each_field(obj, [&](std::string_view name, auto& field_value, int)
      {
        if (json_val.HasMember(name.data()))
        {
          if (!deserialize_value(field_value, json_val[name.data()]))
            success = false;
        } });
        return success;
  }

  // Serialize object to JSON string
  template <typename T>
  static std::string to_json(T& obj, bool pretty = false)
  {
    rapidjson::Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();

    serialize_obj(obj, doc, allocator);

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

  // Deserialize object from JSON string
  template <typename T>
  static std::optional<T> from_json(const std::string& json_str)
  {
    rapidjson::Document doc;
    doc.Parse(json_str.c_str());

    if (doc.HasParseError() || !doc.IsObject())
      return std::nullopt;

    T obj{};
    if (!deserialize_obj(obj, doc))
      return std::nullopt;

    return obj;
  }

  // Helper function to convert GenericValue to RapidJSON Value
  static void generic_value_to_rapidjson(const GenericValue& value, rapidjson::Value& json_val, rapidjson::Document::AllocatorType& allocator)
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
      const auto& str = value.getString();
      json_val.SetString(str.c_str(), static_cast<rapidjson::SizeType>(str.length()), allocator);
    }
    else if (value.isArray())
    {
      json_val.SetArray();
      const auto& arr = value.getArray();
      for (const auto& elem : arr)
      {
        rapidjson::Value elem_val;
        generic_value_to_rapidjson(elem, elem_val, allocator);
        json_val.PushBack(elem_val, allocator);
      }
    }
    else if (value.isObject())
    {
      json_val.SetObject();
      const auto& obj = value.getObject();
      for (const auto& [key, val] : obj)
      {
        rapidjson::Value key_val(key.c_str(), static_cast<rapidjson::SizeType>(key.length()), allocator);
        rapidjson::Value val_json;
        generic_value_to_rapidjson(val, val_json, allocator);
        json_val.AddMember(key_val, val_json, allocator);
      }
    }
  }

  // Helper function to convert RapidJSON Value to GenericValue
  static GenericValue rapidjson_value_to_generic_value(const rapidjson::Value& json_val)
  {
    if (json_val.IsNull())
    {
      return GenericValue();
    }
    else if (json_val.IsBool())
    {
      return GenericValue(json_val.GetBool());
    }
    else if (json_val.IsInt64())
    {
      return GenericValue(json_val.GetInt64());
    }
    else if (json_val.IsInt())
    {
      return GenericValue(static_cast<int64_t>(json_val.GetInt()));
    }
    else if (json_val.IsUint64())
    {
      return GenericValue(static_cast<int64_t>(json_val.GetUint64()));
    }
    else if (json_val.IsUint())
    {
      return GenericValue(static_cast<int64_t>(json_val.GetUint()));
    }
    else if (json_val.IsDouble())
    {
      return GenericValue(json_val.GetDouble());
    }
    else if (json_val.IsString())
    {
      return GenericValue(std::string(json_val.GetString()));
    }
    else if (json_val.IsArray())
    {
      Array arr;
      for (auto it = json_val.Begin(); it != json_val.End(); ++it)
      {
        arr.push_back(rapidjson_value_to_generic_value(*it));
      }
      return GenericValue(std::move(arr));
    }
    else if (json_val.IsObject())
    {
      Object obj;
      for (auto it = json_val.MemberBegin(); it != json_val.MemberEnd(); ++it)
      {
        obj[it->name.GetString()] = rapidjson_value_to_generic_value(it->value);
      }
      return GenericValue(std::move(obj));
    }

    return GenericValue();
  }

  std::string generic_value_to_json(const GenericValue& value, bool pretty)
  {
    rapidjson::Document doc;
    auto& allocator = doc.GetAllocator();

    generic_value_to_rapidjson(value, doc, allocator);

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

  std::optional<GenericValue> generic_value_from_json(const std::string& json_str)
  {
    rapidjson::Document doc;
    doc.Parse(json_str.c_str());

    if (doc.HasParseError())
    {
      return std::nullopt;
    }

    return rapidjson_value_to_generic_value(doc);
  }

} // namespace ReflectionLibrary