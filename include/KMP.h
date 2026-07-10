#ifndef KMP_H
#define KMP_H

#include <string>
#include <vector>

class KMP {
public:
  // Returns all starting indices where pattern occurs in text
  static std::vector<size_t> search(const std::string &text,
                                    const std::string &pattern);

private:
  // Builds the Longest Prefix Suffix array
  static std::vector<int> buildLPS(const std::string &pattern);
};

#endif