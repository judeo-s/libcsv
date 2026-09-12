#include <libcsv/error.hpp>

#include <gtest/gtest.h>

#include <stdexcept>

namespace {

using libcsv::ParseError;

TEST(ParseErrorTest, PreservesLocation) {
  const ParseError error{"Invalid field", 12, 34};

  EXPECT_EQ(error.line(), 12U);
  EXPECT_EQ(error.column(), 34U);
}

TEST(ParseErrorTest, IncludesLocationInMessage) {
  const ParseError error{"Invalid field", 12, 34};

  EXPECT_STREQ(error.what(), "Invalid field Line: 12 Column: 34");
}

TEST(ParseErrorTest, IsARuntimeError) {
  const ParseError error{"Failure", 2, 5};
  const std::runtime_error &base = error;

  EXPECT_STREQ(base.what(), "Failure Line: 2 Column: 5");
}

TEST(ParseErrorTest, CopiesMessageAndLocation) {
  const ParseError original{"Failure", 7, 9};
  const ParseError copy{original};

  EXPECT_STREQ(copy.what(), original.what());
  EXPECT_EQ(copy.line(), 7U);
  EXPECT_EQ(copy.column(), 9U);
}

} // namespace
