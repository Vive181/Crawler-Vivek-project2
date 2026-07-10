#ifndef FRAMEWORKDETECTOR_H
#define FRAMEWORKDETECTOR_H

#include "fetchresult.h"

#include <string>

enum class FrameworkType { STATIC_HTML, REACT, NEXTJS, ANGULAR, VUE, UNKNOWN };

class FrameworkDetector {
public:
  FrameworkType detect(const FetchResult &result);

private:
  bool contains(const std::string &html, const std::string &keyword);

  bool hasReactMarkers(const std::string &html);

  bool hasNextJSMarkers(const std::string &html);

  bool hasAngularMarkers(const std::string &html);

  bool hasVueMarkers(const std::string &html);

  bool hasHeavyJavaScript(const std::string &html);
};

#endif