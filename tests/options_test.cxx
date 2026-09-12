#include <libcsv/options.hpp>
#include <libcsv/reader.hpp>
#include <libcsv/writer.hpp>

#include <gtest/gtest.h>

#include <stdexcept>

namespace {

using libcsv::LineEnding;
using libcsv::ParseOptions;
using libcsv::Reader;
using libcsv::WriteOptions;
using libcsv::Writer;

TEST(ParseOptionsTest, HasExpectedDefaults) {
  const ParseOptions options{};

  EXPECT_EQ(options.delimiter, ',');
  EXPECT_EQ(options.quote, '"');
  EXPECT_FALSE(options.skip_empty_lines);
}

TEST(ParseOptionsTest, ReaderStoresConfiguredOptions) {
  ParseOptions options;
  options.delimiter = ';';
  options.quote = '\'';
  options.skip_empty_lines = true;

  const Reader reader{options};

  EXPECT_EQ(reader.options().delimiter, ';');
  EXPECT_EQ(reader.options().quote, '\'');
  EXPECT_TRUE(reader.options().skip_empty_lines);
}

TEST(ParseOptionsTest, ReaderCopiesOptions) {
  ParseOptions options;
  options.delimiter = ';';

  const Reader reader{options};
  options.delimiter = '|';

  EXPECT_EQ(reader.options().delimiter, ';');
}

TEST(ParseOptionsTest, ReaderRejectsLineEndingDelimiters) {
  for (const char delimiter : {'\r', '\n'}) {
    ParseOptions options;
    options.delimiter = delimiter;

    EXPECT_THROW(static_cast<void>(Reader{options}), std::invalid_argument);
  }
}

TEST(WriteOptionsTest, HasExpectedDefaults) {
  const WriteOptions options{};

  EXPECT_EQ(options.delimiter, ',');
  EXPECT_EQ(options.quote, '"');
  EXPECT_EQ(options.line_ending, LineEnding::crlf);
  EXPECT_TRUE(options.final_line_ending);
}

TEST(WriteOptionsTest, WriterStoresConfiguredOptions) {
  WriteOptions options;
  options.delimiter = ';';
  options.quote = '\'';
  options.line_ending = LineEnding::lf;
  options.final_line_ending = false;

  const Writer writer{options};

  EXPECT_EQ(writer.options().delimiter, ';');
  EXPECT_EQ(writer.options().quote, '\'');
  EXPECT_EQ(writer.options().line_ending, LineEnding::lf);
  EXPECT_FALSE(writer.options().final_line_ending);
}

TEST(WriteOptionsTest, WriterCopiesOptions) {
  WriteOptions options;
  options.delimiter = ';';

  const Writer writer{options};
  options.delimiter = '|';

  EXPECT_EQ(writer.options().delimiter, ';');
  EXPECT_EQ(writer.write({{"a", "b"}}), "a;b\r\n");
}

TEST(WriteOptionsTest, WriterRejectsLineEndingDelimiters) {
  for (const char delimiter : {'\r', '\n'}) {
    WriteOptions options;
    options.delimiter = delimiter;

    EXPECT_THROW(static_cast<void>(Writer{options}), std::invalid_argument);
  }
}

TEST(WriteOptionsTest, WriterRejectsLineEndingQuotes) {
  for (const char quote : {'\r', '\n'}) {
    WriteOptions options;
    options.quote = quote;

    EXPECT_THROW(static_cast<void>(Writer{options}), std::invalid_argument);
  }
}

TEST(WriteOptionsTest, WriterRejectsMatchingDelimiterAndQuote) {
  WriteOptions options;
  options.delimiter = ';';
  options.quote = ';';

  EXPECT_THROW(static_cast<void>(Writer{options}), std::invalid_argument);
}

} // namespace
