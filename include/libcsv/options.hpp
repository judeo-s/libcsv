#pragma once

namespace libcsv {

enum class LineEnding {
  lf,
  crlf,
};

struct ParseOptions {
  char delimiter = ',';
  char quote = '"';
  bool skip_empty_lines = false;
};

struct WriteOptions {
  char delimiter = ',';
  char quote = '"';
  LineEnding line_ending = LineEnding::crlf;
  bool final_line_ending = true;
};

} // namespace libcsv
