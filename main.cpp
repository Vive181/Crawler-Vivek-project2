#include "HTMLparser.h"
#include "Set.h"
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
  HTMLParser parser;

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

  std::string testHTML = "<html>"
                         "<a href=\"one.html\">Normal</a>"
                         "<a href=\"./two.html\">Current Directory</a>"
                         "<a href=\"../three.html\">Parent Directory</a>"
                         "<a href=\"/four.html\">Root</a>"
                         "<a href=\"https://google.com\">Absolute</a>"
                         "<a HREF = 'five.html'>Uppercase</a>"
                         "<a href=\"#section\">Fragment</a>"
                         "<a href=\"javascript:void(0)\">JS</a>"
                         "</html>";

  auto links = parser.extractLinks(
      testHTML, "https://example.com/docs/tutorial/index.html");

  std::cout << "Links Found : " << links.size() << '\n';

  for (const auto &link : links) {
    std::cout << link << '\n';
  }

  if (!renderEngine.createTab("https://example.com")) {
    std::cout << "Tab Creation Failed\n";
    return 0;
  }

  std::cout << "CDP Tab Created\n";

  if (!renderEngine.connect()) {
    std::cout << "WebSocket Failed\n";
    return 0;
  }

  std::cout << "WebSocket Connected\n";

  if (renderEngine.sendCommand("Page.enable", {})) {
    std::cout << "Page Enabled\n";
  } else {
    std::cout << "Page Enable Failed\n";
  }

  if (renderEngine.sendCommand("Runtime.enable", {})) {
    std::cout << "Runtime Enabled\n";
  } else {
    std::cout << "Runtime Enable Failed\n";
  }

  if (renderEngine.navigate("https://example.com")) {
    std::cout << "Navigation Started\n";
  } else {
    std::cout << "Navigation Failed\n";
  }

  std::string html = renderEngine.getRenderedHTML();

  if (!html.empty()) {
    std::cout << "Rendered HTML Size : " << html.size() << '\n';

    std::cout << html.substr(0, 500) << "\n";
  } else {
    std::cout << "Rendered HTML Failed\n";
    std::cout << renderEngine.getLastResponse() << '\n';
  }

  std::cout << "\n========== Set Test ==========\n";

  Set<std::string> urlSet;

  urlSet.insert("https://example.com");
  urlSet.insert("https://google.com");
  urlSet.insert("https://example.com");

  std::cout << "Contains example: " << urlSet.contains("https://example.com")
            << "\n";

  std::cout << "Size: " << urlSet.size() << "\n";

  urlSet.remove("https://google.com");

  std::cout << "Size after remove: " << urlSet.size() << "\n";

  return 0;
}
