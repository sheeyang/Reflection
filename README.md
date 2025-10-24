# ReflectionLibrary

A modern C++20 reflection library that provides compile-time reflection capabilities for C++ structs and classes, along with powerful runtime type manipulation through `GenericValue`.

## Features

- 🔍 **Compile-time Reflection**: Introspect struct fields at compile time
- 🎯 **Type-safe**: Leverages C++20 concepts for type safety
- 📝 **JSON Serialization**: Convert structs to/from JSON seamlessly
- 🖨️ **Pretty Printing**: Print any reflectable struct with nested support
- 🔄 **Generic Value System**: Dynamic type system similar to JSON values
- 🎨 **Custom Reflection**: Support for non-reflectable types through custom reflectors
- 🚀 **Header-only**: Easy integration into any project

## Requirements

- C++20 compatible compiler (MSVC, GCC 10+, Clang 10+)
- CMake 3.15+ (for building tests)
- RapidJSON (automatically fetched by CMake)

## Quick Start

### Basic Reflection

```cpp
#include <ReflectionLibrary.h>

// Define a simple struct
struct Person
{
    std::string name;
    int age;
    double height;
};

// Make it reflectable with a single macro
REFLECT_FIELDS(Person, name, age, height)

int main()
{
    Person person{"Alice", 30, 5.6};
    
    // Print the struct
    ReflectionLibrary::print(person);
    
    // Get class name
    auto className = ReflectionLibrary::get_class_name(person);
    std::cout << "Class: " << className << std::endl;
    
    // Get field count
    auto fieldCount = ReflectionLibrary::get_field_count(person);
    std::cout << "Fields: " << fieldCount << std::endl;
    
    return 0;
}
```

**Output:**
```
Type: Person
  name = "Alice"
  age = 30
  height = 5.6
Class: Person
Fields: 3
```

### Field Iteration

```cpp
#include <ReflectionLibrary.h>

struct Point
{
    double x;
    double y;
    double z;
};
REFLECT_FIELDS(Point, x, y, z)

int main()
{
    Point p{1.0, 2.0, 3.0};
    
    // Iterate over all fields
    ReflectionLibrary::for_each_field(p, 
        [](std::string_view name, auto& value, int nest_level) {
            std::cout << name << " = " << value << std::endl;
        });
    
    // Modify field by name
    ReflectionLibrary::set_field_value(p, "x", 10.0);
    std::cout << "Updated x: " << p.x << std::endl;
    
    return 0;
}
```

### JSON Serialization

```cpp
#include <ReflectionLibrary.h>

struct Product
{
    std::string name;
    double price;
    int quantity;
    bool inStock;
};
REFLECT_FIELDS(Product, name, price, quantity, inStock)

int main()
{
    Product product{"Laptop", 999.99, 5, true};
    
    // Serialize to JSON
    std::string json = ReflectionLibrary::to_json(product, true);
    std::cout << "JSON:\n" << json << std::endl;
    
    // Deserialize from JSON
    std::string jsonStr = R"({
        "name": "Phone",
        "price": 599.99,
        "quantity": 10,
        "inStock": true
    })";
    
    auto result = ReflectionLibrary::from_json<Product>(jsonStr);
    if (result.has_value())
    {
        Product p = result.value();
        std::cout << "Loaded: " << p.name << ", $" << p.price << std::endl;
    }
    
    return 0;
}
```

**Output:**
```json
JSON:
{
    "name": "Laptop",
    "price": 999.99,
    "quantity": 5,
    "inStock": true
}
Loaded: Phone, $599.99
```

### Nested Structures

```cpp
#include <ReflectionLibrary.h>

struct Address
{
    std::string street;
    std::string city;
    int zipCode;
};
REFLECT_FIELDS(Address, street, city, zipCode)

struct Employee
{
    std::string name;
    int id;
    Address address;
};
REFLECT_FIELDS(Employee, name, id, address)

int main()
{
    Employee emp{
        "Bob Smith",
        12345,
        {"123 Main St", "Springfield", 62701}
    };
    
    // Print with nested structure
    ReflectionLibrary::print(emp);
    
    // Serialize to JSON with nesting
    std::string json = ReflectionLibrary::to_json(emp, true);
    std::cout << json << std::endl;
    
    return 0;
}
```

**Output:**
```
Type: Employee
  name = "Bob Smith"
  id = 12345
  address (Address):
    street = "123 Main St"
    city = "Springfield"
    zipCode = 62701
```

### Complex Types Support

The library supports various STL containers:

```cpp
#include <ReflectionLibrary.h>
#include <vector>
#include <map>
#include <array>

struct DataCollection
{
    std::vector<int> numbers;
    std::map<std::string, double> scores;
    std::array<std::string, 3> names;
    std::pair<int, std::string> idName;
};
REFLECT_FIELDS(DataCollection, numbers, scores, names, idName)

int main()
{
    DataCollection data{
        {1, 2, 3, 4, 5},
        {{"math", 95.5}, {"physics", 88.0}},
        {"Alice", "Bob", "Charlie"},
        {101, "John"}
    };
    
    // Print complex types
    ReflectionLibrary::print(data);
    
    // Serialize to JSON
    std::string json = ReflectionLibrary::to_json(data, true);
    std::cout << json << std::endl;
    
    return 0;
}
```

## GenericValue - Dynamic Type System

`GenericValue` provides a JSON-like dynamic type system for C++:

### Basic Usage

```cpp
#include <ReflectionLibrary.h>

using namespace ReflectionLibrary;

int main()
{
    // Create values of different types
    GenericValue nullVal;
    GenericValue boolVal(true);
    GenericValue intVal(42);
    GenericValue doubleVal(3.14);
    GenericValue stringVal("Hello");
    
    // Type checking
    if (intVal.isInt())
        std::cout << "Integer: " << intVal.getInt() << std::endl;
    
    // Arrays
    GenericValue arr;
    arr = Array{GenericValue(1), GenericValue(2), GenericValue(3)};
    std::cout << "Array size: " << arr.size() << std::endl;
    
    // Objects
    GenericValue obj;
    Object objData;
    objData["name"] = GenericValue("Alice");
    objData["age"] = GenericValue(30);
    obj = objData;
    
    std::cout << "Name: " << obj["name"].getString() << std::endl;
    std::cout << "Age: " << obj["age"].getInt() << std::endl;
    
    return 0;
}
```

### Dynamic Object Construction

```cpp
#include <ReflectionLibrary.h>

using namespace ReflectionLibrary;

int main()
{
    GenericValue person;
    
    // Build object dynamically
    person["name"] = "Alice";
    person["age"] = 30;
    person["email"] = "alice@example.com";
    
    // Nested objects
    person["address"]["street"] = "123 Main St";
    person["address"]["city"] = "Boston";
    person["address"]["zip"] = 12345;
    
    // Arrays
    person["hobbies"][0] = "Reading";
    person["hobbies"][1] = "Cycling";
    person["hobbies"][2] = "Cooking";
    
    // Convert to JSON
    std::string json = generic_value_to_json(person, true);
    std::cout << json << std::endl;
    
    return 0;
}
```

### Converting Structs to GenericValue

```cpp
#include <ReflectionLibrary.h>

struct Config
{
    std::string appName;
    int port;
    bool debug;
    std::vector<std::string> allowedHosts;
};
REFLECT_FIELDS(Config, appName, port, debug, allowedHosts)

int main()
{
    Config config{
        "MyApp",
        8080,
        true,
        {"localhost", "127.0.0.1"}
    };
    
    // Convert to GenericValue
    GenericValue gv = to_generic_value(config);
    
    // Modify dynamically
    gv["port"] = 9090;
    gv["allowedHosts"][2] = "192.168.1.1";
    
    // Convert back to struct
    auto newConfig = from_generic_value<Config>(gv);
    if (newConfig.has_value())
    {
        std::cout << "New port: " << newConfig->port << std::endl;
        std::cout << "Hosts: " << newConfig->allowedHosts.size() << std::endl;
    }
    
    return 0;
}
```

## Custom Reflection

For types you cannot modify (third-party libraries, legacy code), use custom reflectors:

```cpp
#include <ReflectionLibrary.h>

// Third-party struct we cannot modify
struct LegacyPoint
{
    int x_coord;
    int y_coord;
};

// Wrapper with custom reflector
struct Point
{
    LegacyPoint point;
    
    struct Reflector
    {
        int x;
        int y;
        
        
        // Transforms the reflectable fields (x, y) into the actual Point object
        // Example: Called during deserialization (JSON/GenericValue -> Point)
        static Point create(const Reflector& r)
        {
            Point p;
            p.point.x_coord = r.x;
            p.point.y_coord = r.y;
            return p;
        }
        
        // Extracts the reflectable fields (x, y) from the Point object
        // Example: Called during serialization (Point -> JSON/GenericValue)
        static Reflector reflect(const Point& p)
        {
            Reflector r;
            r.x = p.point.x_coord;
            r.y = p.point.y_coord;
            return r;
        }
    };
};
REFLECT_CUSTOM(Point, x, y)

int main()
{
    Point p;
    p.point.x_coord = 10;
    p.point.y_coord = 20;
    
    // Works with all reflection features
    ReflectionLibrary::print(p);
    std::string json = ReflectionLibrary::to_json(p, true);
    std::cout << json << std::endl;
    
    return 0;
}
```

## API Reference

### Macros

#### `REFLECT_FIELDS(ClassName, field1, field2, ...)`
Makes a struct reflectable by specifying its fields.

```cpp
struct MyStruct { int a; float b; };
REFLECT_FIELDS(MyStruct, a, b)
```

#### `REFLECT_CUSTOM(ClassName, field1, field2, ...)`
Enables reflection for types with custom `Reflector` nested struct.

```cpp
REFLECT_CUSTOM(CustomType, field1, field2)
```

### Helper Functions

#### `class_name<T>`
Get the class name as a compile-time string_view.

```cpp
constexpr auto name = ReflectionLibrary::class_name<MyStruct>;
```

#### `get_class_name(obj)`
Get the class name from an instance.

```cpp
auto name = ReflectionLibrary::get_class_name(myObj);
```

#### `field_count<T>`
Get the number of fields at compile time.

```cpp
constexpr size_t count = ReflectionLibrary::field_count<MyStruct>;
```

#### `get_field_count(obj)`
Get the number of fields from an instance.

```cpp
size_t count = ReflectionLibrary::get_field_count(myObj);
```

#### `for_each_field(obj, func, nest_level = 0)`
Iterate over all fields with a callback.

```cpp
ReflectionLibrary::for_each_field(obj, 
    [](std::string_view name, auto& value, int nest_level) {
        // Process each field
    });
```

#### `set_field_value(obj, field_name, value)`
Set a field value by name.

```cpp
ReflectionLibrary::set_field_value(obj, "age", 25);
```

### Printing

#### `print(obj)`
Print a reflectable object to stdout.

```cpp
ReflectionLibrary::print(myObj);
```

### JSON Functions

#### `to_json(obj, pretty = false)`
Serialize an object to JSON string.

```cpp
std::string json = ReflectionLibrary::to_json(obj, true);
```

#### `from_json<T>(json_str)`
Deserialize JSON string to an object.

```cpp
auto result = ReflectionLibrary::from_json<MyStruct>(jsonStr);
if (result.has_value()) {
    MyStruct obj = result.value();
}
```

### GenericValue Functions

#### `to_generic_value(obj)`
Convert any reflectable object to GenericValue.

```cpp
GenericValue gv = ReflectionLibrary::to_generic_value(obj);
```

#### `from_generic_value<T>(value)`
Convert GenericValue back to typed object.

```cpp
auto result = ReflectionLibrary::from_generic_value<MyStruct>(gv);
```

#### `generic_value_to_json(value, pretty)`
Convert GenericValue to JSON string.

```cpp
std::string json = ReflectionLibrary::generic_value_to_json(gv, true);
```

#### `generic_value_from_json(json_str)`
Parse JSON string to GenericValue.

```cpp
auto result = ReflectionLibrary::generic_value_from_json(jsonStr);
```

## Concepts

The library provides several C++20 concepts for type checking:

### `is_reflectable_v<T>`
Check if a type is reflectable.

```cpp
if constexpr (ReflectionLibrary::is_reflectable_v<MyStruct>) {
    // Use reflection features
}
```

### `has_reflector_v<T>`
Check if a type has a custom Reflector.

```cpp
if constexpr (ReflectionLibrary::has_reflector_v<MyType>) {
    // Use custom reflector
}
```

### `is_iterable_v<T>`
Check if a type is iterable (has size, begin, end).

```cpp
if constexpr (ReflectionLibrary::is_iterable_v<T>) {
    // Iterate over elements
}
```

### `is_tuple_v<T>`, `is_pair_v<T>`
Check for std::tuple or std::pair types.

## Building Tests

```bash
mkdir build
cd build
cmake ..
cmake --build .
ctest
```

Or with Visual Studio:
```bash
cmake -B build -G "Visual Studio 17 2022"
cmake --build build --config Debug
cd build
ctest -C Debug
```

## Project Structure

```
ReflectionLibrary/
├── include/
│   ├── ReflectionLibrary.h          # Main header
│   └── ReflectionLibrary/
│       ├── Concepts.h               # C++20 concepts
│       ├── GenericValue.h           # Dynamic type system
│       ├── GenericValue.inl         # Implementation
│       ├── Helpers.h                # Helper functions
│       ├── Json.h                   # JSON serialization
│       ├── Macros.h                 # Reflection macros
│       └── Print.h                  # Pretty printing
├── tests/
│   ├── main.cpp                     # Test entry point
│   ├── types.h                      # Test types
│   ├── test_reflect.h               # Reflection tests
│   ├── test_print.h                 # Print tests
│   ├── test_json.h                  # JSON tests
│   ├── test_concepts.h              # Concept tests
│   └── test_value.h                 # GenericValue tests
└── CMakeLists.txt
```

## Limitations

- Maximum 20 fields per struct (can be extended in Macros.h)
- Requires C++20 compiler
- Single inheritance not directly supported (use custom reflectors)
- No support for private/protected members (by design)

## Examples

See the `tests/` directory for comprehensive examples:
- `test_reflect.h` - Reflection features
- `test_print.h` - Pretty printing
- `test_json.h` - JSON serialization/deserialization
- `test_value.h` - GenericValue usage
- `test_concepts.h` - Concept checking

## License

This project is available under the MIT License.

## Contributing

Contributions are welcome! Please ensure:
- Code follows the existing style
- All tests pass
- New features include tests
- Documentation is updated

## Credits

- Uses [RapidJSON](https://github.com/Tencent/rapidjson) for JSON parsing
- Uses [Catch2](https://github.com/catchorg/Catch2) for testing
