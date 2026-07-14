#include "../include/HTMLparser.h"
#include <gtest/gtest.h>

TEST(HTMLParserTest, ExtractsAbsoluteURL) {
  HTMLParser parser;
  auto links = parser.extractLinks(
      "<a href=\"https://example.com/page\">Link</a>", "https://base.com");

  ASSERT_EQ(links.size(), 1);
  EXPECT_EQ(links[0], "https://example.com/page");
}

TEST(HTMLParserTest, ExtractsSingleQuotedHref) {
  HTMLParser parser;
  auto links = parser.extractLinks(
      "<a href='https://example.com/page'>Link</a>", "https://base.com");

  ASSERT_EQ(links.size(), 1);
  EXPECT_EQ(links[0], "https://example.com/page");
}

TEST(HTMLParserTest, ExtractsUnquotedHref) {
  HTMLParser parser;
  auto links = parser.extractLinks("<a href=https://example.com/page>Link</a>",
                                   "https://base.com");

  ASSERT_EQ(links.size(), 1);
  EXPECT_EQ(links[0], "https://example.com/page");
}

TEST(HTMLParserTest, ExtractsMultipleLinks) {
  HTMLParser parser;
  auto links = parser.extractLinks("<a href=\"https://one.com\">One</a>"
                                   "<a href=\"https://two.com\">Two</a>",
                                   "https://base.com");

  ASSERT_EQ(links.size(), 2);
  EXPECT_EQ(links[0], "https://one.com");
  EXPECT_EQ(links[1], "https://two.com");
}

TEST(HTMLParserTest, HandlesUppercaseAnchorAndHref) {
  HTMLParser parser;
  auto links = parser.extractLinks("<A HREF=\"https://example.com\">Link</A>",
                                   "https://base.com");

  ASSERT_EQ(links.size(), 1);
  EXPECT_EQ(links[0], "https://example.com");
}

TEST(HTMLParserTest, HandlesSpacesAroundEquals) {
  HTMLParser parser;
  auto links = parser.extractLinks("<a href = \"https://example.com\">Link</a>",
                                   "https://base.com");

  ASSERT_EQ(links.size(), 1);
  EXPECT_EQ(links[0], "https://example.com");
}

TEST(HTMLParserTest, ConvertsRootRelativeURL) {
  HTMLParser parser;
  auto links = parser.extractLinks("<a href=\"/about\">About</a>",
                                   "https://example.com/products/page.html");

  ASSERT_EQ(links.size(), 1);
  EXPECT_EQ(links[0], "https://example.com/about");
}

TEST(HTMLParserTest, ConvertsRelativeURL) {
  HTMLParser parser;
  auto links = parser.extractLinks("<a href=\"about.html\">About</a>",
                                   "https://example.com/pages/index.html");

  ASSERT_EQ(links.size(), 1);
  EXPECT_EQ(links[0], "https://example.com/pages/about.html");
}

TEST(HTMLParserTest, HandlesDotSlashRelativeURL) {
  HTMLParser parser;
  auto links = parser.extractLinks("<a href=\"./about.html\">About</a>",
                                   "https://example.com/pages/index.html");

  ASSERT_EQ(links.size(), 1);
  EXPECT_EQ(links[0], "https://example.com/pages/about.html");
}

TEST(HTMLParserTest, HandlesParentDirectoryURL) {
  HTMLParser parser;
  auto links =
      parser.extractLinks("<a href=\"../about.html\">About</a>",
                          "https://example.com/pages/products/index.html");

  ASSERT_EQ(links.size(), 1);
  EXPECT_EQ(links[0], "https://example.com/pages/about.html");
}

TEST(HTMLParserTest, HandlesProtocolRelativeURL) {
  HTMLParser parser;
  auto links = parser.extractLinks("<a href=\"//cdn.example.com/file\">CDN</a>",
                                   "https://example.com");

  ASSERT_EQ(links.size(), 1);
  EXPECT_EQ(links[0], "https://cdn.example.com/file");
}

TEST(HTMLParserTest, IgnoresFragmentLinks) {
  HTMLParser parser;
  auto links = parser.extractLinks("<a href=\"#section\">Section</a>",
                                   "https://example.com");

  EXPECT_TRUE(links.empty());
}

TEST(HTMLParserTest, IgnoresJavaScriptLinks) {
  HTMLParser parser;
  auto links = parser.extractLinks("<a href=\"javascript:void(0)\">Click</a>",
                                   "https://example.com");

  EXPECT_TRUE(links.empty());
}

TEST(HTMLParserTest, IgnoresMailtoLinks) {
  HTMLParser parser;
  auto links = parser.extractLinks(
      "<a href=\"mailto:test@example.com\">Email</a>", "https://example.com");

  EXPECT_TRUE(links.empty());
}

TEST(HTMLParserTest, IgnoresTelephoneLinks) {
  HTMLParser parser;
  auto links = parser.extractLinks("<a href=\"tel:1234567890\">Call</a>",
                                   "https://example.com");

  EXPECT_TRUE(links.empty());
}

TEST(HTMLParserTest, IgnoresEmptyHref) {
  HTMLParser parser;
  auto links =
      parser.extractLinks("<a href=\"\">Empty</a>", "https://example.com");

  EXPECT_TRUE(links.empty());
}

TEST(HTMLParserTest, IgnoresAnchorWithoutHref) {
  HTMLParser parser;
  auto links = parser.extractLinks("<a class=\"link\">No href</a>",
                                   "https://example.com");

  EXPECT_TRUE(links.empty());
}

TEST(HTMLParserTest, HandlesEmptyHTML) {
  HTMLParser parser;

  auto links = parser.extractLinks("", "https://example.com");

  EXPECT_TRUE(links.empty());
}

TEST(HTMLParserTest, IgnoresMalformedUnclosedAnchorTag) {
  HTMLParser parser;

  auto links = parser.extractLinks("<a href=\"https://example.com\"",
                                   "https://base.com");

  EXPECT_TRUE(links.empty());
}

TEST(HTMLParserTest, DoesNotTreatSimilarTagAsAnchor) {
  HTMLParser parser;

  auto links = parser.extractLinks(
      "<article href=\"https://example.com\">Article</article>",
      "https://base.com");

  EXPECT_TRUE(links.empty());
}

TEST(HTMLParserTest, HandlesMultipleParentDirectories) {
  HTMLParser parser;

  auto links = parser.extractLinks("<a href=\"../../about.html\">About</a>",
                                   "https://example.com/a/b/index.html");

  ASSERT_EQ(links.size(), 1);
  EXPECT_EQ(links[0], "https://example.com/about.html");
}

TEST(HTMLParserTest, HandlesBaseURLWithTrailingSlash) {
  HTMLParser parser;

  auto links = parser.extractLinks("<a href=\"page.html\">Page</a>",
                                   "https://example.com/docs/");

  ASSERT_EQ(links.size(), 1);
  EXPECT_EQ(links[0], "https://example.com/docs/page.html");
}

TEST(HTMLParserTest, PreservesQueryParametersInLink) {
  HTMLParser parser;

  auto links =
      parser.extractLinks("<a href=\"search?q=crawler&page=2\">Search</a>",
                          "https://example.com/index.html");

  ASSERT_EQ(links.size(), 1);
  EXPECT_EQ(links[0], "https://example.com/search?q=crawler&page=2");
}

// new test cases

TEST(HTMLParserEdgeTest, DoesNotTreatDataHrefAsHref) {
  HTMLParser parser;

  auto links = parser.extractLinks(
      "<a data-href=\"wrong.html\" href=\"correct.html\">Link</a>",
      "https://example.com/index.html");

  ASSERT_EQ(links.size(), 1);
  EXPECT_EQ(links[0], "https://example.com/correct.html");
}

TEST(HTMLParserEdgeTest, IgnoresUppercaseJavaScriptLink) {
  HTMLParser parser;

  auto links = parser.extractLinks("<a href=\"JAVASCRIPT:void(0)\">Click</a>",
                                   "https://example.com");

  EXPECT_TRUE(links.empty());
}

TEST(HTMLParserEdgeTest, IgnoresUppercaseMailtoLink) {
  HTMLParser parser;

  auto links = parser.extractLinks(
      "<a href=\"MAILTO:test@example.com\">Email</a>", "https://example.com");

  EXPECT_TRUE(links.empty());
}

TEST(HTMLParserEdgeTest, HandlesTabBeforeHref) {
  HTMLParser parser;

  auto links = parser.extractLinks("<a\thref=\"page.html\">Page</a>",
                                   "https://example.com/index.html");

  ASSERT_EQ(links.size(), 1);
  EXPECT_EQ(links[0], "https://example.com/page.html");
}

TEST(HTMLParserEdgeTest, HandlesNewlineBeforeHref) {
  HTMLParser parser;

  auto links = parser.extractLinks("<a\nhref=\"page.html\">Page</a>",
                                   "https://example.com/index.html");

  ASSERT_EQ(links.size(), 1);
  EXPECT_EQ(links[0], "https://example.com/page.html");
}

TEST(HTMLParserEdgeTest, DoesNotMoveAboveDomainRoot) {
  HTMLParser parser;

  auto links = parser.extractLinks("<a href=\"../../../page.html\">Page</a>",
                                   "https://example.com/a/index.html");

  ASSERT_EQ(links.size(), 1);
  EXPECT_EQ(links[0], "https://example.com/page.html");
}

TEST(HTMLParserEdgeTest, HandlesHrefAfterOtherAttributes) {
  HTMLParser parser;

  auto links = parser.extractLinks(
      "<a class=\"button\" id=\"main\" href=\"page.html\">Page</a>",
      "https://example.com/index.html");

  ASSERT_EQ(links.size(), 1);
  EXPECT_EQ(links[0], "https://example.com/page.html");
}

TEST(HTMLParserEdgeTest, IgnoresHrefTextInsideAnotherAttributeValue) {
  HTMLParser parser;

  auto links = parser.extractLinks(
      "<a title=\"some href=fake.html\" href=\"real.html\">Page</a>",
      "https://example.com/index.html");

  ASSERT_EQ(links.size(), 1);
  EXPECT_EQ(links[0], "https://example.com/real.html");
}

TEST(HTMLParserEdgeTest, HandlesGreaterThanSymbolInsideQuotedAttribute) {
  HTMLParser parser;

  auto links =
      parser.extractLinks("<a title=\"1 > 0\" href=\"page.html\">Page</a>",
                          "https://example.com/index.html");

  ASSERT_EQ(links.size(), 1);
  EXPECT_EQ(links[0], "https://example.com/page.html");
}

TEST(HTMLParserEdgeTest, IgnoresHrefInsideHTMLComment) {
  HTMLParser parser;

  auto links =
      parser.extractLinks("<!-- <a href=\"hidden.html\">Hidden</a> -->",
                          "https://example.com/index.html");

  EXPECT_TRUE(links.empty());
}