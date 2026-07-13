#ifndef CRAWLER_H
#define CRAWLER_H

#include "HTMLparser.h"
#include "URLfrontier.h"
#include "URLnormalizer.h"
#include "URLvalidator.h"
#include "fetcher.h"
#include "frameworkdetector.h"
#include "pagestorage.h"
#include "renderengine.h"
#include "seenstorage.h"

#include <string>

class Crawler {
private:
  int maxDepth;

  URLValidator validator;
  URLNormalizer normalizer;
  SeenStorage seenstorage;
  URLFrontier frontier;
  Fetcher fetcher;
  FrameworkDetector frameworkDetector;
  RenderEngine renderEngine;
  PageStorage pageStorage;
  HTMLParser htmlParser;

public:
  explicit Crawler(int maxDepth);

  void crawl(const std::string &seedURL);
};

#endif