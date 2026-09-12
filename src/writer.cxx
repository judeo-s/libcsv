#include "libcsv/options.hpp"
#include <cstddef>
#include <libcsv/types.hpp>
#include <libcsv/writer.hpp>
#include <ostream>
#include <sstream>
#include <stdexcept>

namespace libcsv {
namespace {

void write_field(std::ostream &output, const Field &field,
                 const WriteOptions &options) {
  const bool requires_quotes = field.find(options.delimiter) != Field::npos ||
                               field.find(options.quote) != Field::npos ||
                               field.find('\r') != Field::npos ||
                               field.find('\n') != Field::npos;
  if (!requires_quotes) {
    output << field;
    return;
  }
  output << options.quote;
  for (const char character : field) {
    if (character == options.quote) {
      output << options.quote;
    }
    output << character;
  }
  output << options.quote;
}

} // namespace

Writer::Writer(WriteOptions options) : options_{options} {
  if (options_.delimiter == '\r' || options_.delimiter == '\n') {
    throw std::invalid_argument(
        "CSV delimiter cannot be a line-ending character");
  }
  if (options_.quote == '\r' || options_.quote == '\n') {
    throw std::invalid_argument("CSV quote cannot be a line-ending character");
  }
  if (options_.delimiter == options_.quote) {
    throw std::invalid_argument(
        "CSV delimiter and quote characters must be different");
  }
}

std::string Writer::write(const Table &table) const {
  std::ostringstream output;
  write(table, output);
  return output.str();
}

void Writer::write(const Table &table, std::ostream &output) const {
  const char *line_ending =
      options_.line_ending == LineEnding::lf ? "\n" : "\r\n";
  for (std::size_t row_index = 0; row_index < table.size(); ++row_index) {
    const Row &row = table[row_index];
    for (std::size_t field_index = 0; field_index < row.size(); ++field_index) {
      if (field_index != 0) {
        output << options_.delimiter;
      }
      write_field(output, row[field_index], options_);
    }
    const bool is_last_row = row_index + 1 == table.size();
    if (!is_last_row || options_.final_line_ending) {
      output << line_ending;
    }
  }
}

const WriteOptions &Writer::options() const noexcept { return options_; }
} // namespace libcsv
