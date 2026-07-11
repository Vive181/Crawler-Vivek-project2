#ifndef URLNORMALIZER_H
#define URLNORMALIZER_H

#include <string>

class URLNormalizer {
public:
  // Normalize a valid URL and return its canonical form
  std::string normalize(const std::string &url);

private:
  // Convert protocol (http/https) to lowercase
  std::string normalizeProtocol(const std::string &url);

  // Convert domain name to lowercase
  std::string normalizeDomain(const std::string &url);

  // Remove default ports (:80 and :443)
  std::string removeDefaultPort(const std::string &url);

  // Remove URL fragment (#...)
  std::string removeFragment(const std::string &url);

  // Remove unnecessary trailing slash
  std::string normalizeTrailingSlash(const std::string &url);
};

#endif