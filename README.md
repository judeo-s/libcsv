# libcsv

A small C++20 library for parsing and generating CSV, designed for easy reuse
with CMake `FetchContent`.

> [!NOTE]
> This project is currently an implementation scaffold. Basic parsing is
> available, but quoted-field parsing and writing are not yet implemented.

## Goals

- A small, predictable API
- Standard C++20 with no runtime dependencies
- Parse from strings and input streams
- Write to strings and output streams
- Configurable delimiters, quote characters, and output line endings
- Useful parse errors with line and column information

`libcsv` treats every value as a string. Header rows, schemas, and numeric type
conversion are intentionally left to the application.

## Add to a CMake project

```cmake
include(FetchContent)

FetchContent_Declare(
    libcsv
    GIT_REPOSITORY https://github.com/judeo-s/libcsv.git
    GIT_TAG main # Prefer a release tag or commit in production.
)

FetchContent_MakeAvailable(libcsv)

target_link_libraries(your_target PRIVATE libcsv::csv)
```

Including the library enables C++20 for the consuming target. Tests and
examples are disabled automatically when `libcsv` is used as a subproject.

## Parse CSV

```cpp
#include <libcsv/csv.hpp>

#include <iostream>
#include <string_view>

int main() {
  constexpr std::string_view input =
      "name,language\r\n"
      "Ada,C++\r\n"
      "Linus,C\r\n";

  const libcsv::Table table = libcsv::Reader{}.parse(input);

  for (const libcsv::Row &row : table) {
    for (const libcsv::Field &field : row) {
      std::cout << field << '\n';
    }
  }
}
```

Use another delimiter by passing parse options:

```cpp
libcsv::ParseOptions options;
options.delimiter = ';';
options.skip_empty_lines = true;

libcsv::Reader reader{options};
libcsv::Table table = reader.parse(input);
```

## Generate CSV

```cpp
#include <libcsv/csv.hpp>

#include <iostream>

int main() {
  const libcsv::Table table{
      {"name", "note"},
      {"Ada", "commas, quotes, and newlines are escaped"},
  };

  libcsv::Writer writer;
  std::cout << writer.write(table);
}
```

To write directly to a stream:

```cpp
writer.write(table, output_stream);
```

## Public API

| Header | Purpose |
| --- | --- |
| `<libcsv/csv.hpp>` | Includes the complete public API |
| `<libcsv/types.hpp>` | `Field`, `Row`, and `Table` aliases |
| `<libcsv/options.hpp>` | Parse and write configuration |
| `<libcsv/error.hpp>` | `ParseError` with line and column accessors |
| `<libcsv/reader.hpp>` | The `Reader` interface |
| `<libcsv/writer.hpp>` | The `Writer` interface |

The core data model is deliberately simple:

```cpp
using Field = std::string;
using Row = std::vector<Field>;
using Table = std::vector<Row>;
```

## Current parser behavior

The basic reader:

- Recognize LF and CRLF record endings while parsing
- Preserve empty fields, including leading and trailing empty fields
- Support configurable delimiters
- Treat quote characters as ordinary field content

An empty input produces an empty table. A blank record produces one empty
field unless `skip_empty_lines` is enabled.

Quoted fields are not interpreted, so delimiters and line endings inside
quotes still split fields and records.

## Build locally

```sh
cmake -S . -B build -DLIBCSV_BUILD_TESTS=ON -DLIBCSV_BUILD_EXAMPLES=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

Available options:

| Option | Default at top level | Description |
| --- | --- | --- |
| `LIBCSV_BUILD_TESTS` | `ON` | Build the test suite |
| `LIBCSV_BUILD_EXAMPLES` | `ON` | Build examples |

## Implementation roadmap

- Implement and test `ParseError`
- Validate reader and writer options
- Add quote-aware parsing and useful parse errors
- Implement field escaping and output line endings
- Test empty fields, quotes, embedded newlines, malformed input, and custom options

## License

[MIT](LICENSE)
