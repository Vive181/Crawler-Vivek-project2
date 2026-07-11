#ifndef SEEN_STORAGE_H
#define SEEN_STORAGE_H

#include "Set.h"
#include <string>

class SeenStorage {

private:
  Set<std::string> seenURLs;

public:
  SeenStorage();

  bool addSeenURL(const std::string &url);

  bool isSeen(const std::string &url) const;

  bool removeSeenURL(const std::string &url);

  int size() const;

  void clear();
};

#endif