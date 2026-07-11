#include "../include/KMP.h"

std::vector<size_t> KMP::search(const std::string &text,
                                const std::string &pattern) {
  std::vector<size_t> matches;

  if (text.empty() || pattern.empty()) {
    return matches;
  }

  std::vector<int> lps = buildLPS(pattern);

  size_t textIndex = 0;
  size_t patternIndex = 0;

  // Next step:
  // Main KMP loop

  while (textIndex < text.size()) {
    if (text[textIndex] == pattern[patternIndex]) {
      textIndex++;
      patternIndex++;
    }

    // Next step

    if (patternIndex == pattern.size()) {
      matches.push_back(textIndex - patternIndex);

      patternIndex = lps[patternIndex - 1];
    } else if (textIndex < text.size() &&
               text[textIndex] != pattern[patternIndex]) {

      if (patternIndex != 0) {
        patternIndex = lps[patternIndex - 1];
      } else {
        textIndex++;
      }
    }
  }

  return matches;
}

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