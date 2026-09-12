#include <libcsv/options.hpp>
#include <libcsv/reader.hpp>
#include <libcsv/types.hpp>

#include <gtest/gtest.h>

#include <sstream>
#include <string>

namespace {

using libcsv::ParseOptions;
using libcsv::Reader;
using libcsv::Table;

TEST(ReaderTest, EmptyInputProducesEmptyTable) {
  EXPECT_TRUE(Reader{}.parse("").empty());
}

TEST(ReaderTest, ParsesSingleAndMultipleFields) {
  EXPECT_EQ(Reader{}.parse("value"), (Table{{"value"}}));
  EXPECT_EQ(Reader{}.parse("a,b,c"), (Table{{"a", "b", "c"}}));
}

TEST(ReaderTest, ParsesLfRowsWithAndWithoutFinalEnding) {
  const Table expected{{"a", "b"}, {"c", "d"}};

  EXPECT_EQ(Reader{}.parse("a,b\nc,d"), expected);
  EXPECT_EQ(Reader{}.parse("a,b\nc,d\n"), expected);
}

TEST(ReaderTest, ParsesCrLfRowsWithAndWithoutFinalEnding) {
  const Table expected{{"a", "b"}, {"c", "d"}};

  EXPECT_EQ(Reader{}.parse("a,b\r\nc,d"), expected);
  EXPECT_EQ(Reader{}.parse("a,b\r\nc,d\r\n"), expected);
}

TEST(ReaderTest, ParsesMixedRecordEndings) {
  EXPECT_EQ(Reader{}.parse("a\r\nb\nc\r\n"), (Table{{"a"}, {"b"}, {"c"}}));
}

TEST(ReaderTest, PreservesLeadingMiddleAndTrailingEmptyFields) {
  EXPECT_EQ(Reader{}.parse(",a,,b,"), (Table{{"", "a", "", "b", ""}}));
}

TEST(ReaderTest, ParsesDelimiterOnlyRecord) {
  EXPECT_EQ(Reader{}.parse(",,"), (Table{{"", "", ""}}));
}

TEST(ReaderTest, PreservesEmptyFieldsBeforeRecordEndings) {
  EXPECT_EQ(Reader{}.parse("a,\n,b\n"), (Table{{"a", ""}, {"", "b"}}));
}

TEST(ReaderTest, BlankLfRecordsContainOneEmptyField) {
  EXPECT_EQ(Reader{}.parse("\na\n\n"), (Table{{""}, {"a"}, {""}}));
}

TEST(ReaderTest, BlankCrLfRecordsContainOneEmptyField) {
  EXPECT_EQ(Reader{}.parse("\r\na\r\n\r\n"), (Table{{""}, {"a"}, {""}}));
}

TEST(ReaderTest, SkipsBlankRecordsWhenConfigured) {
  ParseOptions options;
  options.skip_empty_lines = true;

  EXPECT_EQ(Reader{options}.parse("\r\nalpha,beta\r\n\n,\r\n"),
            (Table{{"alpha", "beta"}, {"", ""}}));
}

TEST(ReaderTest, DoesNotSkipDelimiterOnlyRecords) {
  ParseOptions options;
  options.skip_empty_lines = true;

  EXPECT_EQ(Reader{options}.parse(",,\n"), (Table{{"", "", ""}}));
}

TEST(ReaderTest, DoesNotSkipWhitespaceOnlyRecords) {
  ParseOptions options;
  options.skip_empty_lines = true;

  EXPECT_EQ(Reader{options}.parse(" \n\t\r\n"), (Table{{" "}, {"\t"}}));
}

TEST(ReaderTest, TreatsBareCarriageReturnAsFieldContent) {
  EXPECT_EQ(Reader{}.parse("a\rb"), (Table{{"a\rb"}}));
}

TEST(ReaderTest, RemovesOnlyCrImmediatelyBeforeLf) {
  EXPECT_EQ(Reader{}.parse("a\r\r\n"), (Table{{"a\r"}}));
}

TEST(ReaderTest, TreatsQuotesAsOrdinaryCharacters) {
  EXPECT_EQ(Reader{}.parse("\"a,b\",c"), (Table{{"\"a", "b\"", "c"}}));
}

TEST(ReaderTest, QuoteOptionDoesNotProtectDelimitersYet) {
  ParseOptions options;
  options.delimiter = ';';
  options.quote = '\'';

  EXPECT_EQ(Reader{options}.parse("'a;b';c"), (Table{{"'a", "b'", "c"}}));
}

TEST(ReaderTest, LineEndingsInsideQuotesStillSplitRows) {
  EXPECT_EQ(Reader{}.parse("\"first\nsecond\",tail"),
            (Table{{"\"first"}, {"second\"", "tail"}}));
}

TEST(ReaderTest, SupportsCustomDelimiter) {
  ParseOptions options;
  options.delimiter = ';';

  EXPECT_EQ(Reader{options}.parse("a,b;c;;d"), (Table{{"a,b", "c", "", "d"}}));
}

TEST(ReaderTest, SupportsTabDelimiter) {
  ParseOptions options;
  options.delimiter = '\t';

  EXPECT_EQ(Reader{options}.parse("a\tb\nc\td"),
            (Table{{"a", "b"}, {"c", "d"}}));
}

TEST(ReaderTest, ParsesInputStreams) {
  std::istringstream input{"a,b\r\nc,d\r\n"};

  EXPECT_EQ(Reader{}.parse(input), (Table{{"a", "b"}, {"c", "d"}}));
}

TEST(ReaderTest, AppliesOptionsToInputStreams) {
  ParseOptions options;
  options.delimiter = ';';
  options.skip_empty_lines = true;
  std::istringstream input{"\r\na;b\r\n"};

  EXPECT_EQ(Reader{options}.parse(input), (Table{{"a", "b"}}));
}

TEST(ReaderTest, ParsesFromCurrentStreamPosition) {
  std::istringstream input{"ignored,a,b"};
  std::string ignored;
  std::getline(input, ignored, ',');

  EXPECT_EQ(Reader{}.parse(input), (Table{{"a", "b"}}));
}

TEST(ReaderTest, ConsumesInputStream) {
  std::istringstream input{"a,b"};
  const Reader reader;

  EXPECT_EQ(reader.parse(input), (Table{{"a", "b"}}));
  EXPECT_TRUE(reader.parse(input).empty());
}

} // namespace
