#include <libcsv/reader.hpp>

#include <cstddef>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace libcsv {

Reader::Reader(ParseOptions options) : options_{options} {
  if (options_.delimiter == '\r' || options_.delimiter == '\n') {
    throw std::invalid_argument(
        "CSV delimiter cannot be a line-ending character");
  }
}

Table Reader::parse(std::string_view input) const {
  if (input.empty()) {
    return {};
  }

  Table table;
  Row row;
  std::size_t field_start = 0;

  for (std::size_t index = 0; index < input.size(); ++index) {
    if (input[index] == options_.delimiter) {
      row.emplace_back(input.substr(field_start, index - field_start));
      field_start = index + 1;
      continue;
    }
    if (input[index] != '\n') {
      continue;
    }

    std::size_t field_end = index;
    if (field_end > field_start && input[field_end - 1] == '\r') {
      --field_end;
    }

    row.emplace_back(input.substr(field_start, field_end - field_start));
    const bool empty_line = row.size() == 1 && row.front().empty();

    if (!empty_line || !options_.skip_empty_lines) {
      table.push_back(std::move(row));
    }
    row.clear();
    field_start = index + 1;
  }

  if (input.back() != '\n') {
    row.emplace_back(input.substr(field_start));
    table.push_back(std::move(row));
  }

  return table;
}

Table Reader::parse(std::istream &input) const {
  std::stringstream buffer;
  buffer << input.rdbuf();
  return parse(buffer.str());
}

const ParseOptions &Reader::options() const noexcept { return options_; }

} // namespace libcsv
