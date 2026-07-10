#include "../include/frameworkdetector.h"

FrameworkType FrameworkDetector::detect(const FetchResult &result) {
  const std::string &html = result.html;

  if (hasNextJSMarkers(html)) {
    return FrameworkType::NEXTJS;
  }

  if (hasReactMarkers(html)) {
    return FrameworkType::REACT;
  }

  if (hasAngularMarkers(html)) {
    return FrameworkType::ANGULAR;
  }

  if (hasVueMarkers(html)) {
    return FrameworkType::VUE;
  }

  if (hasHeavyJavaScript(html)) {
    return FrameworkType::UNKNOWN;
  }

  return FrameworkType::STATIC_HTML;
}

bool FrameworkDetector::contains(const std::string &html,
                                 const std::string &keyword) {
  return html.find(keyword) != std::string::npos;
}

bool FrameworkDetector::hasReactMarkers(const std::string &html) {
  return contains(html, "id=\"root\"") || contains(html, "react");
}

bool FrameworkDetector::hasNextJSMarkers(const std::string &html) {
  return contains(html, "__NEXT_DATA__") || contains(html, "id=\"__next\"");
}

bool FrameworkDetector::hasAngularMarkers(const std::string &html) {
  return contains(html, "ng-version") || contains(html, "ng-app");
}

bool FrameworkDetector::hasVueMarkers(const std::string &html) {
  return contains(html, "id=\"app\"") || contains(html, "data-server-rendered");
}

bool FrameworkDetector::hasHeavyJavaScript(const std::string &html) {
  int scriptCount = 0;

  size_t position = 0;

  while (true) {
    position = html.find("<script", position);

    if (position == std::string::npos) {
      break;
    }

    scriptCount++;

    position += 7;
  }

  return scriptCount >= 10;
}