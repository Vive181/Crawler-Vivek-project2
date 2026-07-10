#include "URLfrontier.h"
#include "URLnormalizer.h"
#include "URLvalidator.h"
#include "fetcher.h"
#include "frameworkdetector.h"
#include "renderengine.h"

#include <iostream>

int main() {
  // -------------------------
  // Test Render Engine
  // -------------------------

  RenderEngine renderEngine;

  std::cout << "========== Render Engine Test ==========\n";

  if (renderEngine.createTab("https://example.com")) {
    std::cout << "CDP Tab Created Successfully\n";
  } else {
    std::cout << "Failed To Create CDP Tab\n";
  }

  std::cout << "\n=========================================\n\n";

  // -------------------------
  // Crawler Components
  // -------------------------

  URLValidator validator;
  URLNormalizer normalizer;
  URLFrontier frontier;
  Fetcher fetcher;
  FrameworkDetector detector;

  std::string urls[] = {"https://example.com", "https://react.dev",
                        "https://nextjs.org", "https://angular.dev",
                        "https://vuejs.org"};

  // Add URLs to Frontier

  for (const auto &url : urls) {
    if (!validator.isValid(url)) {
      std::cout << "Invalid URL : " << url << '\n';
      continue;
    }

    std::string normalizedURL = normalizer.normalize(url);

    if (normalizedURL.empty()) {
      std::cout << "Normalization Failed : " << url << '\n';

      continue;
    }

    frontier.addURL(URLNode(normalizedURL, 0));

    std::cout << "Added : " << normalizedURL << '\n';
  }

  std::cout << "\n========== Crawling ==========\n";

  while (!frontier.isEmpty()) {
    URLNode node = frontier.getNextURL();

    std::cout << "\nFetching : " << node.url << "\n";

    FetchResult result = fetcher.fetch(node.url);

    if (result.success) {
      std::cout << "Status Code : " << result.statusCode << '\n';

      std::cout << "Final URL : " << result.finalURL << '\n';

      std::cout << "Content Type : " << result.contentType << '\n';

      std::cout << "HTML Size : " << result.html.size() << " bytes\n";

      FrameworkType framework = detector.detect(result);

      std::cout << "Framework : ";

      switch (framework) {
      case FrameworkType::STATIC_HTML:
        std::cout << "Static HTML";
        break;

      case FrameworkType::REACT:
        std::cout << "React";
        break;

      case FrameworkType::NEXTJS:
        std::cout << "Next.js";
        break;

      case FrameworkType::ANGULAR:
        std::cout << "Angular";
        break;

      case FrameworkType::VUE:
        std::cout << "Vue";
        break;

      case FrameworkType::UNKNOWN:
        std::cout << "Unknown (Browser Rendering Recommended)";
        break;
      }

      std::cout << '\n';
    } else {
      std::cout << "Fetch Failed : " << result.errorMessage << '\n';
    }
  }

  if (renderEngine.createTab("https://example.com")) {
    std::cout << "CDP Tab Created\n";

    if (renderEngine.connect()) {
      std::cout << "WebSocket Connected\n";
    } else {
      std::cout << "WebSocket Failed\n";
    }
  } else {
    std::cout << "Tab Creation Failed\n";
  }

  if (renderEngine.sendCommand("Page.enable", {})) {
    std::cout << "Page Enabled\n";
  } else {
    std::cout << "Page Enable Failed\n";
  }

  if (renderEngine.navigate("https://example.com")) {
    std::cout << "Navigation Started\n";
  } else {
    std::cout << "Navigation Failed\n";
  }

  return 0;
}