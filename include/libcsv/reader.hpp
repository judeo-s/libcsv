#pragma once

#include <istream>
#include <string_view>

#include <libcsv/options.hpp>
#include <libcsv/types.hpp>

namespace libcsv {

class Reader {
public:
  explicit Reader(ParseOptions options = {});

  [[nodiscard]] Table parse(std::string_view input) const;
  [[nodiscard]] Table parse(std::istream &input) const;
  [[nodiscard]] const ParseOptions &options() const noexcept;

private:
  ParseOptions options_;
};

} // namespace libcsv
