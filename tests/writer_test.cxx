#include <libcsv/options.hpp>
#include <libcsv/types.hpp>
#include <libcsv/writer.hpp>

#include <gtest/gtest.h>

#include <sstream>

namespace {

using libcsv::LineEnding;
using libcsv::Row;
using libcsv::Table;
using libcsv::WriteOptions;
using libcsv::Writer;

TEST(WriterTest, EmptyTableProducesEmptyOutput) {
  EXPECT_TRUE(Writer{}.write(Table{}).empty());
}

TEST(WriterTest, WritesSingleFieldWithDefaultCrLf) {
  EXPECT_EQ(Writer{}.write({{"value"}}), "value\r\n");
}

TEST(WriterTest, WritesMultipleFieldsAndRows) {
  EXPECT_EQ(Writer{}.write({{"a", "b"}, {"c", "d"}}), "a,b\r\nc,d\r\n");
}

TEST(WriterTest, UsesPositionsRatherThanFieldValues) {
  EXPECT_EQ(Writer{}.write({{"same", "same", "different", "same"}}),
            "same,same,different,same\r\n");
}

TEST(WriterTest, UsesPositionsRatherThanRowValues) {
  EXPECT_EQ(Writer{}.write({{"same"}, {"same"}, {"same"}}),
            "same\r\nsame\r\nsame\r\n");
}

TEST(WriterTest, PreservesEmptyFields) {
  EXPECT_EQ(Writer{}.write({{"", "middle", ""}, {""}}), ",middle,\r\n\r\n");
}

TEST(WriterTest, WritesEmptyRowsAsBlankRecords) {
  const Table table{Row{}, Row{}};

  EXPECT_EQ(Writer{}.write(table), "\r\n\r\n");
}

TEST(WriterTest, LeavesOrdinaryFieldsUnquoted) {
  EXPECT_EQ(Writer{}.write({{"alpha", "beta 123", " leading", "trailing "}}),
            "alpha,beta 123, leading,trailing \r\n");
}

TEST(WriterTest, QuotesFieldsContainingDelimiter) {
  EXPECT_EQ(Writer{}.write({{"alpha,beta", "gamma"}}),
            "\"alpha,beta\",gamma\r\n");
}

TEST(WriterTest, DoublesEmbeddedQuotes) {
  EXPECT_EQ(Writer{}.write({{"He said \"hello\""}}),
            "\"He said \"\"hello\"\"\"\r\n");
}

TEST(WriterTest, EscapesFieldContainingOnlyQuote) {
  EXPECT_EQ(Writer{}.write({{"\""}}), "\"\"\"\"\r\n");
}

TEST(WriterTest, QuotesEmbeddedLfCrAndCrLf) {
  EXPECT_EQ(Writer{}.write({{"line one\nline two"}}),
            "\"line one\nline two\"\r\n");
  EXPECT_EQ(Writer{}.write({{"left\rright"}}), "\"left\rright\"\r\n");
  EXPECT_EQ(Writer{}.write({{"first\r\nsecond"}}), "\"first\r\nsecond\"\r\n");
}

TEST(WriterTest, EscapesAllSpecialCharactersTogether) {
  EXPECT_EQ(Writer{}.write({{"a,\"b\"\r\nc"}}), "\"a,\"\"b\"\"\r\nc\"\r\n");
}

TEST(WriterTest, SupportsCustomDelimiterAndQuote) {
  WriteOptions options;
  options.delimiter = ';';
  options.quote = '\'';
  options.line_ending = LineEnding::lf;

  EXPECT_EQ(Writer{options}.write({{"a,b", "a;b", "can't"}}),
            "a,b;'a;b';'can''t'\n");
}

TEST(WriterTest, NonDelimiterPunctuationDoesNotTriggerQuotes) {
  WriteOptions options;
  options.delimiter = ';';

  EXPECT_EQ(Writer{options}.write({{"a,b", "c"}}), "a,b;c\r\n");
}

TEST(WriterTest, WritesConfiguredLineEndings) {
  WriteOptions options;
  options.line_ending = LineEnding::lf;
  EXPECT_EQ(Writer{options}.write({{"a"}, {"b"}}), "a\nb\n");

  options.line_ending = LineEnding::crlf;
  EXPECT_EQ(Writer{options}.write({{"a"}, {"b"}}), "a\r\nb\r\n");
}

TEST(WriterTest, OmitsOnlyFinalLfWhenConfigured) {
  WriteOptions options;
  options.line_ending = LineEnding::lf;
  options.final_line_ending = false;

  EXPECT_EQ(Writer{options}.write({{"a", "b"}, {"c", "d"}}), "a,b\nc,d");
}

TEST(WriterTest, OmitsOnlyFinalCrLfWhenConfigured) {
  WriteOptions options;
  options.final_line_ending = false;

  EXPECT_EQ(Writer{options}.write({{"a"}, {"b"}}), "a\r\nb");
  EXPECT_EQ(Writer{options}.write({{"a", "b"}}), "a,b");
  EXPECT_TRUE(Writer{options}.write(Table{}).empty());
}

TEST(WriterTest, WritesDirectlyToOutputStream) {
  std::ostringstream output;

  Writer{}.write({{"a,b", "c"}, {"d", "e"}}, output);

  EXPECT_EQ(output.str(), "\"a,b\",c\r\nd,e\r\n");
}

TEST(WriterTest, StreamOverloadAppendsAtCurrentPosition) {
  std::ostringstream output;
  output << "prefix:";

  Writer{}.write({{"a", "b"}}, output);

  EXPECT_EQ(output.str(), "prefix:a,b\r\n");
}

TEST(WriterTest, StreamAndStringOverloadsProduceSameBytes) {
  const Table table{{"a,b", "a\"b"}, {"line\nbreak", ""}};
  const Writer writer;
  std::ostringstream output;

  writer.write(table, output);

  EXPECT_EQ(output.str(), writer.write(table));
}

} // namespace
