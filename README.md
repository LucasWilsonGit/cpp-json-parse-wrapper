# cpp-json-parse-wrapper
a C++20 wrapper for sheredom's single header C json parser (https://github.com/sheredom/json.h)

Example usage
```C++
int item;
std::string optional;
std::vector<std::vector<int>> nDimensional;


std::string json_input = "{\"item\": 51, \"optional\": \"Hello, world\", \"nDimensional\": [[2506, 3321, 5], [1,1], [-1, -15, -20, 256]]}";

json_parser(json_input).get(item).get_named_optional(optional, "optional").get(nDimensional);

std::cout << item << "\n" << optional << "\n";

for (const auto& x: nDimensional) {
  for (const auto y: x) {
    std::cout << y << " ";
  }
  std::cout << "\n";
}

```

Output:

```
51
Hello, world
2506 3321 5
1 1
-1 -15 -20 256
```
