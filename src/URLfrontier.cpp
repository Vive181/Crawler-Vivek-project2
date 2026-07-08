#include "../include/URLfrontier.h"

// Constructor
URLFrontier::URLFrontier() {}

// Add a URL to the frontier
void URLFrontier::addURL(const URLNode &node) { frontierQueue.enqueue(node); }

// Remove and return the next URL
URLNode URLFrontier::getNextURL() { return frontierQueue.dequeue(); }

// Return the next URL without removing it
const URLNode &URLFrontier::peekNextURL() const {
  return frontierQueue.front();
}

// Check whether the frontier is empty
bool URLFrontier::isEmpty() const { return frontierQueue.empty(); }

// Return the number of URLs
size_t URLFrontier::size() const { return frontierQueue.size(); }

// Remove all URLs
void URLFrontier::clear() { frontierQueue.clear(); }