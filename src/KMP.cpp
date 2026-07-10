#include "../include/KMP.h"

std::vector<int> KMP::buildLPS(const std::string &pattern) {
  std::vector<int> lps(pattern.size(), 0);

  int length = 0;
  size_t i = 1;

  while (i < pattern.size()) {
    if (pattern[i] == pattern[length]) {
      length++;
      lps[i] = length;
      i++;
    } else {
      if (length != 0) {
        length = lps[length - 1];
      } else {
        lps[i] = 0;
        i++;
      }
    }
  }

  return lps;
}