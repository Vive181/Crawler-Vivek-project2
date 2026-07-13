#include "../include/crawler.h"
#include <iostream>

Crawler::Crawler(int maxDepth) : maxDepth(maxDepth) {}

void Crawler::crawl(const std::string &seedURL) {
  std::cout << "Entered crawl()" << std::endl;
  // Integration logic will be added here
  if (!URLValidator::isValid(seedURL)) {
    return;
  }

  std::string normalizedURL = normalizer.normalize(seedURL);

  std::cout << "Before addURL" << std::endl;

  if (!seenstorage.addSeenURL(normalizedURL)) {
    return;
  }

  URLNode seedNode(normalizedURL, 0, 0);
  frontier.addURL(seedNode);

  std::cout << "Before while loop" << std::endl;

  while (!frontier.isEmpty()) {

    std::cout << "Inside while loop" << std::endl;

    URLNode currentNode = frontier.getNextURL();

    std::cout << "\nProcessing: " << currentNode.url
              << " | Depth: " << currentNode.depth
              << " | Frontier size: " << frontier.size() << std::endl;

    if (currentNode.depth > maxDepth) {
      continue;
    }

    std::cout << "Before fetch: " << currentNode.url << std::endl;
    FetchResult result = fetcher.fetch(currentNode.url);
    std::cout << "after fetch: " << currentNode.url << std::endl;
    if (!result.success) {
      std::cout << "Fetch failed" << std::endl;
      continue;
    }

    // Successful HTML ab result.html me available hai
    FrameworkType framework = frameworkDetector.detect(result);
    std::cout << "Framework detected" << std::endl;

    std::string finalHTML = result.html;

    if (framework != FrameworkType::STATIC_HTML) {
      std::cout << "Dynamic framework detected" << std::endl;

      if (!renderEngine.createTab(currentNode.url)) {
        std::cout << "createTab FAILED" << std::endl;
        continue;
      }
      std::cout << "createTab SUCCESS" << std::endl;

      if (!renderEngine.connect()) {
        std::cout << "connect FAILED" << std::endl;
        continue;
      }
      std::cout << "connect SUCCESS" << std::endl;

      if (!renderEngine.navigate(currentNode.url)) {
        std::cout << "navigate FAILED" << std::endl;
        renderEngine.disconnect();
        continue;
      }
      std::cout << "navigate SUCCESS" << std::endl;
    }

    if (!pageStorage.storePage(currentNode.url, finalHTML, currentNode.depth)) {
      std::cout << "Page storage failed" << std::endl;
      continue;
    }
    // ADD HTML PARSER LOGIC HERE
    std::cout << "Page stored successfully" << std::endl;

    if (currentNode.depth >= maxDepth) {
      continue;
    }

    std::vector<std::string> extractedLinks =
        htmlParser.extractLinks(finalHTML, currentNode.url);
    std::cout << "Extracted links count: " << extractedLinks.size()
              << std::endl;

    for (const std::string &link : extractedLinks) {

      if (!URLValidator::isValid(link)) {
        continue;
      }

      std::string normalizedLink = normalizer.normalize(link);

      if (!seenstorage.addSeenURL(normalizedLink)) {
        continue;
      }

      URLNode newNode(normalizedLink, currentNode.depth + 1, 0);
      frontier.addURL(newNode);

      std::cout << "After fetch" << std::endl;
    }
  }
}