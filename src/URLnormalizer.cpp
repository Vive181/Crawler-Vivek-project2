#include "../include/URLnormalizer.h"

#include <algorithm>
#include <cctype>

std::string URLNormalizer::normalize(const std::string &url) {
  std::string normalized = url;

  normalized = normalizeProtocol(normalized);
  normalized = normalizeDomain(normalized);
  normalized = removeDefaultPort(normalized);
  normalized = removeFragment(normalized);
  normalized = normalizeTrailingSlash(normalized);

  return normalized;
}

std::string URLNormalizer::normalizeProtocol(const std::string &url) {
  std::string result = url;

  size_t pos = result.find("://");

  if (pos != std::string::npos) {
    std::transform(result.begin(), result.begin() + pos, result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
  }

  return result;
}

std::string URLNormalizer::normalizeDomain(const std::string &url) {
  std::string result = url;

  size_t protocolEnd = result.find("://");

  if (protocolEnd == std::string::npos)
    return result;

  size_t domainStart = protocolEnd + 3;
  size_t domainEnd = result.find_first_of(":/?#", domainStart);

  if (domainEnd == std::string::npos)
    domainEnd = result.length();

  std::transform(result.begin() + domainStart, result.begin() + domainEnd,
                 result.begin() + domainStart,
                 [](unsigned char c) { return std::tolower(c); });

  return result;
}

std::string URLNormalizer::removeDefaultPort(const std::string &url) {
  std::string result = url;

  if (result.compare(0, 7, "http://") == 0) {
    size_t portPos = result.find(":80", 7);

    if (portPos != std::string::npos &&
        (portPos + 3 == result.size() || result[portPos + 3] == '/')) {
      result.erase(portPos, 3);
    }
  }

  if (result.compare(0, 8, "https://") == 0) {
    size_t portPos = result.find(":443", 8);

    if (portPos != std::string::npos &&
        (portPos + 4 == result.size() || result[portPos + 4] == '/')) {
      result.erase(portPos, 4);
    }
  }

  return result;
}

std::string URLNormalizer::removeFragment(const std::string &url) {
  size_t fragmentPos = url.find('#');

  if (fragmentPos == std::string::npos)
    return url;

  return url.substr(0, fragmentPos);
}

std::string URLNormalizer::normalizeTrailingSlash(const std::string &url) {
  if (url.length() <= 8)
    return url;

  if (url.back() == '/')
    return url.substr(0, url.length() - 1);

  return url;
}