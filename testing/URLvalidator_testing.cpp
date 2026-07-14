#include "../include/URLvalidator.h"
#include <gtest/gtest.h>

// -------------------- Valid URLs --------------------

TEST(URLValidatorTest, AcceptsValidHttpURL) {
  EXPECT_TRUE(URLValidator::isValid("http://example.com"));
}

TEST(URLValidatorTest, AcceptsValidHttpsURL) {
  EXPECT_TRUE(URLValidator::isValid("https://example.com"));
}

TEST(URLValidatorTest, AcceptsURLWithPath) {
  EXPECT_TRUE(URLValidator::isValid("https://example.com/page"));
}

TEST(URLValidatorTest, AcceptsURLWithSubdomain) {
  EXPECT_TRUE(URLValidator::isValid("https://www.example.com"));
}

// -------------------- Empty URL --------------------

TEST(URLValidatorTest, RejectsEmptyURL) {
  EXPECT_FALSE(URLValidator::isValid(""));
}

// -------------------- Invalid Protocol --------------------

TEST(URLValidatorTest, RejectsURLWithoutProtocol) {
  EXPECT_FALSE(URLValidator::isValid("example.com"));
}

TEST(URLValidatorTest, RejectsFTPProtocol) {
  EXPECT_FALSE(URLValidator::isValid("ftp://example.com"));
}

TEST(URLValidatorTest, RejectsInvalidProtocol) {
  EXPECT_FALSE(URLValidator::isValid("htp://example.com"));
}

// -------------------- Missing Domain --------------------

TEST(URLValidatorTest, RejectsHttpWithoutDomain) {
  EXPECT_FALSE(URLValidator::isValid("http://"));
}

TEST(URLValidatorTest, RejectsHttpsWithoutDomain) {
  EXPECT_FALSE(URLValidator::isValid("https://"));
}

// -------------------- Invalid Domain --------------------

TEST(URLValidatorTest, RejectsDomainWithoutDot) {
  EXPECT_FALSE(URLValidator::isValid("https://localhost"));
}

TEST(URLValidatorTest, RejectsDomainContainingSpace) {
  EXPECT_FALSE(URLValidator::isValid("https://example site.com"));
}

TEST(URLValidatorTest, RejectsDomainEndingWithDot) {
  EXPECT_FALSE(URLValidator::isValid("https://example."));
}

TEST(URLValidatorTest, AcceptsURLWithQueryParameters) {
  EXPECT_TRUE(URLValidator::isValid("https://example.com/search?q=crawler"));
}

TEST(URLValidatorTest, AcceptsURLWithFragment) {
  EXPECT_TRUE(URLValidator::isValid("https://example.com/page#section"));
}

TEST(URLValidatorTest, AcceptsURLWithPort) {
  EXPECT_TRUE(URLValidator::isValid("https://example.com:8080"));
}

TEST(URLValidatorTest, RejectsUppercaseProtocol) {
  EXPECT_FALSE(URLValidator::isValid("HTTPS://example.com"));
}

TEST(URLValidatorTest, RejectsProtocolOnlyWithPath) {
  EXPECT_FALSE(URLValidator::isValid("https:///page"));
}

TEST(URLValidatorTest, RejectsDomainStartingWithDot) {
  EXPECT_FALSE(URLValidator::isValid("https://.com"));
}

TEST(URLValidatorTest, RejectsConsecutiveDots) {
  EXPECT_FALSE(URLValidator::isValid("https://example..com"));
}

TEST(URLValidatorTest, RejectsWhitespaceURL) {
  EXPECT_FALSE(URLValidator::isValid("   "));
}