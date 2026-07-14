#include "../include/KMP.h"
#include <gtest/gtest.h>


TEST(KMPTest, FindsSingleMatch) {
  EXPECT_EQ(KMP::search("hello world", "world"), std::vector<size_t>({6}));
}

TEST(KMPTest, FindsMultipleMatches) {
  EXPECT_EQ(KMP::search("abcabcabc", "abc"), std::vector<size_t>({0, 3, 6}));
}

TEST(KMPTest, FindsOverlappingMatches) {
  EXPECT_EQ(KMP::search("aaaa", "aa"), std::vector<size_t>({0, 1, 2}));
}

TEST(KMPTest, FindsPatternAtBeginning) {
  EXPECT_EQ(KMP::search("abcdef", "abc"), std::vector<size_t>({0}));
}

TEST(KMPTest, FindsPatternAtEnd) {
  EXPECT_EQ(KMP::search("abcdef", "def"), std::vector<size_t>({3}));
}

TEST(KMPTest, ReturnsEmptyWhenPatternNotFound) {
  EXPECT_TRUE(KMP::search("abcdef", "xyz").empty());
}

TEST(KMPTest, ReturnsEmptyForEmptyText) {
  EXPECT_TRUE(KMP::search("", "abc").empty());
}

TEST(KMPTest, ReturnsEmptyForEmptyPattern) {
  EXPECT_TRUE(KMP::search("abc", "").empty());
}

TEST(KMPTest, ReturnsEmptyWhenBothAreEmpty) {
  EXPECT_TRUE(KMP::search("", "").empty());
}

TEST(KMPTest, FindsPatternEqualToText) {
  EXPECT_EQ(KMP::search("crawler", "crawler"), std::vector<size_t>({0}));
}

TEST(KMPTest, ReturnsEmptyWhenPatternLongerThanText) {
  EXPECT_TRUE(KMP::search("abc", "abcdef").empty());
}

TEST(KMPTest, IsCaseSensitive) {
  EXPECT_TRUE(KMP::search("Hello", "hello").empty());
}

TEST(KMPTest, FindsSpecialCharacters) {
  EXPECT_EQ(KMP::search("<a href=\"page\">", "<a"), std::vector<size_t>({0}));
}

TEST(KMPTest, FindsMultipleAnchorPatterns) {
  std::string html = "<a href=\"one\">One</a><a href=\"two\">Two</a>";

  EXPECT_EQ(KMP::search(html, "<a"), std::vector<size_t>({0, 21}));
}