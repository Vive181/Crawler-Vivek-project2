#include "URLfrontier.h"
#include "URLvalidator.h"

#include <iostream>

int main() {
  URLValidator validator;
  URLFrontier frontier;

  // Test URLs
  std::string urls[] = {"https://apple.com",
                        "http://google.com",
                        "https://github.com",
                        "ftp://example.com",
                        "www.google.com",
                        "https://example.",
                        "",
                        "https://openai.com"};

  // Validate URLs and add only valid ones to the frontier
  for (const auto &url : urls) {
    if (validator.isValid(url)) {
      frontier.addURL(URLNode(url, 0));
      std::cout << "Added: " << url << '\n';
    } else {
      std::cout << "Invalid: " << url << '\n';
    }
  }

  std::cout << "\n----- URLs in Frontier -----\n";

  // Retrieve URLs from the frontier
  while (!frontier.isEmpty()) {
    URLNode node = frontier.getNextURL();
    std::cout << "URL: " << node.url << " | Depth: " << node.depth << '\n';
  }

  return 0;
}