#pragma once

#include <ostream>
#include <string>

#include <libcsv/options.hpp>
#include <libcsv/types.hpp>

namespace libcsv {

class Writer {
public:
  explicit Writer(WriteOptions options = {});

  [[nodiscard]] std::string write(const Table &table) const;
  void write(const Table &table, std::ostream &output) const;
  [[nodiscard]] const WriteOptions &options() const noexcept;

private:
  WriteOptions options_;
};

} // namespace libcsv
