#include <cstddef>
#include <libcsv/error.hpp>
#include <stdexcept>
#include <string>

namespace libcsv {
ParseError::ParseError(const std::string &message, std::size_t line,
                       std::size_t column)
    : std::runtime_error(message + " Line: " + std::to_string(line) +
                         " Column: " + std::to_string(column)),
      line_{line}, column_(column) {}

std::size_t ParseError::column() const noexcept { return column_; }

std::size_t ParseError::line() const noexcept { return line_; }
} // namespace libcsv
