#include "../include/Seenstorage.h"
#include <gtest/gtest.h>

TEST(SeenStorageTest, IsEmptyWhenCreated) {
  SeenStorage storage;
  EXPECT_EQ(storage.size(), 0);
}

TEST(SeenStorageTest, AddsNewURLSuccessfully) {
  SeenStorage storage;

  EXPECT_TRUE(storage.addSeenURL("https://example.com"));
  EXPECT_EQ(storage.size(), 1);
}

TEST(SeenStorageTest, AddedURLIsSeen) {
  SeenStorage storage;

  storage.addSeenURL("https://example.com");

  EXPECT_TRUE(storage.isSeen("https://example.com"));
}

TEST(SeenStorageTest, UnaddedURLIsNotSeen) {
  SeenStorage storage;

  EXPECT_FALSE(storage.isSeen("https://example.com"));
}

TEST(SeenStorageTest, RejectsDuplicateURL) {
  SeenStorage storage;

  EXPECT_TRUE(storage.addSeenURL("https://example.com"));
  EXPECT_FALSE(storage.addSeenURL("https://example.com"));
}

TEST(SeenStorageTest, DuplicateDoesNotIncreaseSize) {
  SeenStorage storage;

  storage.addSeenURL("https://example.com");
  storage.addSeenURL("https://example.com");

  EXPECT_EQ(storage.size(), 1);
}

TEST(SeenStorageTest, StoresMultipleUniqueURLs) {
  SeenStorage storage;

  storage.addSeenURL("https://example.com");
  storage.addSeenURL("https://google.com");
  storage.addSeenURL("https://github.com");

  EXPECT_EQ(storage.size(), 3);
}

TEST(SeenStorageTest, RemovesExistingURL) {
  SeenStorage storage;

  storage.addSeenURL("https://example.com");

  EXPECT_TRUE(storage.removeSeenURL("https://example.com"));
  EXPECT_FALSE(storage.isSeen("https://example.com"));
}

TEST(SeenStorageTest, RemoveDecreasesSize) {
  SeenStorage storage;

  storage.addSeenURL("https://example.com");
  storage.removeSeenURL("https://example.com");

  EXPECT_EQ(storage.size(), 0);
}

TEST(SeenStorageTest, RemovingNonExistingURLReturnsFalse) {
  SeenStorage storage;

  EXPECT_FALSE(storage.removeSeenURL("https://example.com"));
}

TEST(SeenStorageTest, ClearRemovesAllURLs) {
  SeenStorage storage;

  storage.addSeenURL("https://example.com");
  storage.addSeenURL("https://google.com");

  storage.clear();

  EXPECT_EQ(storage.size(), 0);
  EXPECT_FALSE(storage.isSeen("https://example.com"));
  EXPECT_FALSE(storage.isSeen("https://google.com"));
}

TEST(SeenStorageTest, CanAddURLAgainAfterRemoval) {
  SeenStorage storage;

  storage.addSeenURL("https://example.com");
  storage.removeSeenURL("https://example.com");

  EXPECT_TRUE(storage.addSeenURL("https://example.com"));
  EXPECT_TRUE(storage.isSeen("https://example.com"));
}

TEST(SeenStorageTest, CanReuseStorageAfterClear) {
  SeenStorage storage;

  storage.addSeenURL("https://old.com");
  storage.clear();

  EXPECT_TRUE(storage.addSeenURL("https://new.com"));
  EXPECT_EQ(storage.size(), 1);
}

TEST(SeenStorageTest, TreatsDifferentURLStringsAsDifferentURLs) {
  SeenStorage storage;

  storage.addSeenURL("https://example.com");

  EXPECT_FALSE(storage.isSeen("https://EXAMPLE.COM"));
  EXPECT_FALSE(storage.isSeen("https://example.com/"));
}