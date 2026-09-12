#include <libcsv/csv.hpp>

int main() {
  const libcsv::ParseOptions options{};
  return options.delimiter == ',' ? 0 : 1;
}
