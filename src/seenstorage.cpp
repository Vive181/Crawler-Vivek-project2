#include "../include/Seenstorage.h"

SeenStorage::SeenStorage() {}

bool SeenStorage::addSeenURL(const std::string &url) {
  if (seenURLs.contains(url)) {
    return false;
  }

  seenURLs.insert(url);

  return true;
}

bool SeenStorage::isSeen(const std::string &url) const {
  return seenURLs.contains(url);
}

bool SeenStorage::removeSeenURL(const std::string &url) {
  return seenURLs.remove(url);
}

int SeenStorage::size() const { return seenURLs.size(); }

void SeenStorage::clear() { seenURLs.clear(); }