## Install

### CMake

#### fetch_content:
To integrate CLParser to existing CMake project, add the following to your `CMakeLists.txt`:
```CMake
include(FetchContent)
FetchContent_Declare(CLParser 
    GIT_REPOSITORY https://github.com/KovrikAlexandr/CLParser
    GIT_TAG main
)
FetchContent_MakeAvailable(CLParser)
```
This will produce target `CLParser`, which you can link the regular way:
```CMake
target_link_libraries(<your_target_name> PUBLIC/PRIVATE CLParser)
```

## Usage examples

```c++
#include <CLParser/CLParser.hpp>

using namespace CLParser;

int main(int argc, char* argv[]) {
    Parser parser;
}
```

### Adding new arguments

To add new command line arguments:
```c++
// Adds argument storing a single value
parser.AddArgument<int>('a', "param1");
// Can store std::string, int, bool

// Adds argument storing multiple values (vector)
parser.AddMultiArgument<int>('b', "param2");
// Can store std::string, int

// Add multiple value argument, which doesn`t require any prefixes while parsing
parser.AddPositional<int>('c', "param3");
// Can store std::string, int
```
For all arguments long name is required and short name is optional.

### Getting stored values
To get values from arguments:
```c++
// Returns const T&
int int_value = parser.GetValue<int>("param1");

// Returns const std::vector<T>&
const std::vector<int>& int_value = parser.GetValues<int>("param2");

// Returns const std::vector<T>& (Postional)
const std::vector<int>& int_value = parser.GetValues<int>("param3");
```

### Parsing
To parse command line arguments use `parser.Parse()`:
```c++
parser.Parse(argc, argv);
```

Parsing example:
```
./example.exe --param1=value1 -p value2 positionalValue
```
Return `false` if: 
- not all arguments were initialised(leads to UB)
- a non-existent argument name is entered
- entered value can`t be converted from string to required type


Library also provides `SplitString(const std::vector<std::string>& str)` function, which takes a string as an argument, separates it by "space" and returns the vector of received words. This can be used with `parser.Parse()`:
```c++
// Ignores the first argument (app)
parser.Parse("app --param1=value1 -p value2 positionalValue") 
```
