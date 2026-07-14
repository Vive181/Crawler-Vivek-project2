#include <gtest/gtest.h>

#include "../include/fetcher.h"

// Successful HTTPS request
TEST(FetcherTest, FetchesValidURLSuccessfully) {
  Fetcher fetcher;

  FetchResult result = fetcher.fetch("https://example.com");

  EXPECT_TRUE(result.success);
  EXPECT_EQ(result.requestedURL, "https://example.com");
  EXPECT_FALSE(result.html.empty());
}

// HTTP status code should be captured
TEST(FetcherTest, CapturesHTTPStatusCode) {
  Fetcher fetcher;

  FetchResult result = fetcher.fetch("https://example.com");

  EXPECT_TRUE(result.success);
  EXPECT_EQ(result.statusCode, 200);
}

// Final URL should be stored
TEST(FetcherTest, CapturesFinalURL) {
  Fetcher fetcher;

  FetchResult result = fetcher.fetch("https://example.com");

  EXPECT_TRUE(result.success);
  EXPECT_FALSE(result.finalURL.empty());
}

// Content type should be captured
TEST(FetcherTest, CapturesContentType) {
  Fetcher fetcher;

  FetchResult result = fetcher.fetch("https://example.com");

  EXPECT_TRUE(result.success);
  EXPECT_FALSE(result.contentType.empty());
}

// Invalid domain should fail
TEST(FetcherTest, HandlesInvalidDomain) {
  Fetcher fetcher;

  FetchResult result =
      fetcher.fetch("https://this-domain-should-not-exist-123456789.invalid");

  EXPECT_FALSE(result.success);
  EXPECT_FALSE(result.errorMessage.empty());
}

// Malformed URL should fail
TEST(FetcherTest, HandlesMalformedURL) {
  Fetcher fetcher;

  FetchResult result = fetcher.fetch("://invalid-url");

  EXPECT_FALSE(result.success);
  EXPECT_FALSE(result.errorMessage.empty());
}

// Requested URL should always be preserved
TEST(FetcherTest, PreservesRequestedURLOnFailure) {
  Fetcher fetcher;

  std::string url = "https://this-domain-should-not-exist-123456789.invalid";

  FetchResult result = fetcher.fetch(url);

  EXPECT_EQ(result.requestedURL, url);
}

// Custom user agent should not break fetching
TEST(FetcherTest, AcceptsCustomUserAgent) {
  Fetcher fetcher;

  fetcher.setUserAgent("CrawlerTest/2.0");

  FetchResult result = fetcher.fetch("https://example.com");

  EXPECT_TRUE(result.success);
}

// Custom timeout should be accepted
TEST(FetcherTest, AcceptsCustomTimeout) {
  Fetcher fetcher;

  fetcher.setTimeout(5);

  FetchResult result = fetcher.fetch("https://example.com");

  EXPECT_TRUE(result.success);
}

// Custom redirect limit should be accepted
TEST(FetcherTest, AcceptsCustomRedirectLimit) {
  Fetcher fetcher;

  fetcher.setMaxRedirects(3);

  FetchResult result = fetcher.fetch("https://example.com");

  EXPECT_TRUE(result.success);
}

// Empty URL should fail
TEST(FetcherTest, HandlesEmptyURL) {
  Fetcher fetcher;

  FetchResult result = fetcher.fetch("");

  EXPECT_FALSE(result.success);
}

// HTML should contain response data
TEST(FetcherTest, StoresResponseBody) {
  Fetcher fetcher;

  FetchResult result = fetcher.fetch("https://example.com");

  ASSERT_TRUE(result.success);
  EXPECT_GT(result.html.size(), 0);
}

// Supports plain HTTP
TEST(FetcherEdgeTest, SupportsHTTPProtocol) {
  Fetcher fetcher;

  FetchResult result = fetcher.fetch("http://example.com");

  EXPECT_TRUE(result.success);
  EXPECT_GT(result.statusCode, 0);
}

// Preserves URL containing query parameters
TEST(FetcherEdgeTest, HandlesQueryParameters) {
  Fetcher fetcher;

  std::string url = "https://httpbin.org/get?name=Vivek&page=1";

  FetchResult result = fetcher.fetch(url);

  EXPECT_TRUE(result.success);
  EXPECT_EQ(result.requestedURL, url);
}