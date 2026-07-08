#ifndef URLFRONTIER_H
#define URLFRONTIER_H

#include "queue.h"
#include <cstddef>
#include <string>


// Represents a URL waiting to be crawled
struct URLNode {
  std::string url;
  int depth;

  URLNode(const std::string &u = "", int d = 0) : url(u), depth(d) {}
};

class URLFrontier {
private:
  Queue<URLNode> frontierQueue;

public:
  // Constructor
  URLFrontier();

  // Add a URL to the frontier
  void addURL(const URLNode &node);

  // Remove and return the next URL
  URLNode getNextURL();

  // Return the next URL without removing it
  const URLNode &peekNextURL() const;

  // Check whether the frontier is empty
  bool isEmpty() const;

  // Return the number of URLs
  size_t size() const;

  // Remove all URLs
  void clear();
};

#endif