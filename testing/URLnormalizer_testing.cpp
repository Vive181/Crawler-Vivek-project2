#include "../include/URLnormalizer.h"
#include <gtest/gtest.h>

// Protocol normalization

TEST(URLNormalizerTest, ConvertsHTTPProtocolToLowercase) {
  URLNormalizer normalizer;
  EXPECT_EQ(normalizer.normalize("HTTP://example.com"), "http://example.com");
}

TEST(URLNormalizerTest, ConvertsHTTPSProtocolToLowercase) {
  URLNormalizer normalizer;
  EXPECT_EQ(normalizer.normalize("HTTPS://example.com"), "https://example.com");
}

// Domain normalization

TEST(URLNormalizerTest, ConvertsDomainToLowercase) {
  URLNormalizer normalizer;
  EXPECT_EQ(normalizer.normalize("https://EXAMPLE.COM"), "https://example.com");
}

TEST(URLNormalizerTest, PreservesPathCase) {
  URLNormalizer normalizer;
  EXPECT_EQ(normalizer.normalize("https://EXAMPLE.COM/MyPage"),
            "https://example.com/MyPage");
}

// Default ports

TEST(URLNormalizerTest, RemovesDefaultHTTPPort) {
  URLNormalizer normalizer;
  EXPECT_EQ(normalizer.normalize("http://example.com:80/page"),
            "http://example.com/page");
}

TEST(URLNormalizerTest, RemovesDefaultHTTPSPort) {
  URLNormalizer normalizer;
  EXPECT_EQ(normalizer.normalize("https://example.com:443/page"),
            "https://example.com/page");
}

TEST(URLNormalizerTest, PreservesNonDefaultPort) {
  URLNormalizer normalizer;
  EXPECT_EQ(normalizer.normalize("https://example.com:8080/page"),
            "https://example.com:8080/page");
}

// Fragment removal

TEST(URLNormalizerTest, RemovesFragment) {
  URLNormalizer normalizer;
  EXPECT_EQ(normalizer.normalize("https://example.com/page#section"),
            "https://example.com/page");
}

TEST(URLNormalizerTest, RemovesFragmentAfterQuery) {
  URLNormalizer normalizer;
  EXPECT_EQ(normalizer.normalize("https://example.com/page?id=10#section"),
            "https://example.com/page?id=10");
}

// Trailing slash

TEST(URLNormalizerTest, RemovesTrailingSlash) {
  URLNormalizer normalizer;
  EXPECT_EQ(normalizer.normalize("https://example.com/"),
            "https://example.com");
}

TEST(URLNormalizerTest, RemovesTrailingSlashFromPath) {
  URLNormalizer normalizer;
  EXPECT_EQ(normalizer.normalize("https://example.com/page/"),
            "https://example.com/page");
}

// Combined behavior

TEST(URLNormalizerTest, AppliesAllNormalizationsTogether) {
  URLNormalizer normalizer;

  EXPECT_EQ(normalizer.normalize("HTTPS://EXAMPLE.COM:443/Page/#section"),
            "https://example.com/Page");
}

// Stability

TEST(URLNormalizerTest, LeavesAlreadyNormalizedURLUnchanged) {
  URLNormalizer normalizer;
  EXPECT_EQ(normalizer.normalize("https://example.com/page"),
            "https://example.com/page");
}

TEST(URLNormalizerTest, NormalizationIsIdempotent) {
  URLNormalizer normalizer;

  std::string once =
      normalizer.normalize("HTTPS://EXAMPLE.COM:443/Page/#section");

  std::string twice = normalizer.normalize(once);

  EXPECT_EQ(once, twice);
}

TEST(URLNormalizerTest, RemovesDefaultHTTPPortWithoutPath) {
  URLNormalizer normalizer;
  EXPECT_EQ(normalizer.normalize("http://example.com:80"),
            "http://example.com");
}

TEST(URLNormalizerTest, RemovesDefaultHTTPSPortWithoutPath) {
  URLNormalizer normalizer;
  EXPECT_EQ(normalizer.normalize("https://example.com:443"),
            "https://example.com");
}

TEST(URLNormalizerTest, PreservesQueryParameters) {
  URLNormalizer normalizer;
  EXPECT_EQ(normalizer.normalize("https://EXAMPLE.COM/search?q=Hello"),
            "https://example.com/search?q=Hello");
}

TEST(URLNormalizerTest, HandlesFragmentOnlyAtRoot) {
  URLNormalizer normalizer;
  EXPECT_EQ(normalizer.normalize("https://example.com/#section"),
            "https://example.com");
}

TEST(URLNormalizerTest, HandlesEmptyString) {
  URLNormalizer normalizer;
  EXPECT_EQ(normalizer.normalize(""), "");
}

TEST(URLNormalizerTest, DoesNotRemovePortContainingDefaultPortPrefix) {
  URLNormalizer normalizer;
  EXPECT_EQ(normalizer.normalize("http://example.com:8080/page"),
            "http://example.com:8080/page");
}