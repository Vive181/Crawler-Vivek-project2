#include "../include/URLfrontier.h"
#include <gtest/gtest.h>
#include <stdexcept>

TEST(URLFrontierTest, IsEmptyWhenCreated) {
  URLFrontier frontier;

  EXPECT_TRUE(frontier.isEmpty());
  EXPECT_EQ(frontier.size(), 0);
}

TEST(URLFrontierTest, AddsSingleURL) {
  URLFrontier frontier;

  frontier.addURL(URLNode("https://example.com", 0, 0));

  EXPECT_FALSE(frontier.isEmpty());
  EXPECT_EQ(frontier.size(), 1);
}

TEST(URLFrontierTest, GetsAddedURL) {
  URLFrontier frontier;

  frontier.addURL(URLNode("https://example.com", 0, 0));

  URLNode node = frontier.getNextURL();

  EXPECT_EQ(node.url, "https://example.com");
}

TEST(URLFrontierTest, PreservesDepth) {
  URLFrontier frontier;

  frontier.addURL(URLNode("https://example.com/page", 3, 0));

  URLNode node = frontier.getNextURL();

  EXPECT_EQ(node.depth, 3);
}

TEST(URLFrontierTest, PreservesRetryCount) {
  URLFrontier frontier;

  frontier.addURL(URLNode("https://example.com", 2, 4));

  URLNode node = frontier.getNextURL();

  EXPECT_EQ(node.retryCount, 4);
}

TEST(URLFrontierTest, MaintainsFIFOOrder) {
  URLFrontier frontier;

  frontier.addURL(URLNode("https://first.com", 0, 0));
  frontier.addURL(URLNode("https://second.com", 1, 0));
  frontier.addURL(URLNode("https://third.com", 2, 0));

  EXPECT_EQ(frontier.getNextURL().url, "https://first.com");
  EXPECT_EQ(frontier.getNextURL().url, "https://second.com");
  EXPECT_EQ(frontier.getNextURL().url, "https://third.com");
}

TEST(URLFrontierTest, GetNextURLRemovesURL) {
  URLFrontier frontier;

  frontier.addURL(URLNode("https://example.com", 0, 0));

  EXPECT_EQ(frontier.size(), 1);

  frontier.getNextURL();

  EXPECT_EQ(frontier.size(), 0);
  EXPECT_TRUE(frontier.isEmpty());
}

TEST(URLFrontierTest, PeekReturnsNextURL) {
  URLFrontier frontier;

  frontier.addURL(URLNode("https://example.com", 2, 1));

  const URLNode &node = frontier.peekNextURL();

  EXPECT_EQ(node.url, "https://example.com");
  EXPECT_EQ(node.depth, 2);
  EXPECT_EQ(node.retryCount, 1);
}

TEST(URLFrontierTest, PeekDoesNotRemoveURL) {
  URLFrontier frontier;

  frontier.addURL(URLNode("https://example.com", 0, 0));

  frontier.peekNextURL();

  EXPECT_EQ(frontier.size(), 1);
  EXPECT_FALSE(frontier.isEmpty());
}

TEST(URLFrontierTest, ClearRemovesAllURLs) {
  URLFrontier frontier;

  frontier.addURL(URLNode("https://first.com", 0, 0));
  frontier.addURL(URLNode("https://second.com", 1, 0));
  frontier.addURL(URLNode("https://third.com", 2, 0));

  frontier.clear();

  EXPECT_TRUE(frontier.isEmpty());
  EXPECT_EQ(frontier.size(), 0);
}

TEST(URLFrontierTest, CanReuseFrontierAfterClear) {
  URLFrontier frontier;

  frontier.addURL(URLNode("https://old.com", 0, 0));
  frontier.clear();

  frontier.addURL(URLNode("https://new.com", 1, 0));

  EXPECT_EQ(frontier.size(), 1);
  EXPECT_EQ(frontier.getNextURL().url, "https://new.com");
}

TEST(URLFrontierTest, GetNextURLFromEmptyFrontierThrowsException) {
  URLFrontier frontier;

  EXPECT_THROW(frontier.getNextURL(), std::out_of_range);
}

TEST(URLFrontierTest, PeekFromEmptyFrontierThrowsException) {
  URLFrontier frontier;

  EXPECT_THROW(frontier.peekNextURL(), std::out_of_range);
}