#ifndef FETCHER_H
#define FETCHER_H

#include <string>

#include "FetchResult.h"

class Fetcher {
public:
  Fetcher();

  FetchResult fetch(const std::string &url);

  void setUserAgent(const std::string &agent);

  void setTimeout(long seconds);

  void setMaxRedirects(long redirects);

private:
  std::string userAgent;

  long timeout;

  long maxRedirects;
};

#endif