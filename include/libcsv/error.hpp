#pragma once

#include <cstddef>
#include <stdexcept>

namespace libcsv {

class ParseError : public std::runtime_error {
public:
  ParseError(const std::string &message, std::size_t line, std::size_t column);

  [[nodiscard]] std::size_t line() const noexcept;
  [[nodiscard]] std::size_t column() const noexcept;

private:
  std::size_t line_;
  std::size_t column_;
};

} // namespace libcsv
