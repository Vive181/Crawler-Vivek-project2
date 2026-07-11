#include "../include/HTMLparser.h"
#include "../include/KMP.h"

HTMLParser::HTMLParser() {}

std::vector<std::string> HTMLParser::extractLinks(const std::string &html,
                                                  const std::string &baseURL) {
  std::vector<std::string> links;

  std::string lowerHTML = html;

  for (char &ch : lowerHTML) {
    if (ch >= 'A' && ch <= 'Z') {
      ch = ch - 'A' + 'a';
    }
  }

  // Find all <a tags
  auto anchorPositions = KMP::search(lowerHTML, "<a");

  // Loop through each <a tag
  for (size_t position : anchorPositions) {

    size_t afterA = position + 2;

    if (afterA >= lowerHTML.size()) {
      continue;
    }

    char nextChar = lowerHTML[afterA];

    if (nextChar != ' ' && nextChar != '\t' && nextChar != '\n' &&
        nextChar != '\r' && nextChar != '>') {
      continue;
    }

    size_t tagEnd = html.find('>', position);

    if (tagEnd == std::string::npos) {
      continue;
    }

    std::string tag = html.substr(position, tagEnd - position + 1);

    // Create lowercase copy for case-insensitive href search
    std::string lowerTag = tag;

    for (char &ch : lowerTag) {
      if (ch >= 'A' && ch <= 'Z') {
        ch = ch - 'A' + 'a';
      }
    }

    // Search href inside lowercase tag
    auto hrefPositions = KMP::search(lowerTag, "href");

    size_t hrefPosition = std::string::npos;

    for (size_t hrefPos : hrefPositions) {
      // href ke pehle whitespace hona chahiye
      if (hrefPos == 0 || lowerTag[hrefPos - 1] == ' ' ||
          lowerTag[hrefPos - 1] == '\t' || lowerTag[hrefPos - 1] == '\n' ||
          lowerTag[hrefPos - 1] == '\r') {
        hrefPosition = hrefPos;
        break;
      }
    }

    if (hrefPosition == std::string::npos) {
      continue;
    }

    size_t hrefStart = hrefPosition + 4;
    // Skip spaces after href
    while (hrefStart < tag.size() &&
           (tag[hrefStart] == ' ' || tag[hrefStart] == '\t' ||
            tag[hrefStart] == '\n' || tag[hrefStart] == '\r')) {
      hrefStart++;
    }

    // Check '='
    if (hrefStart >= tag.size() || tag[hrefStart] != '=') {
      continue;
    }

    hrefStart++;

    // Skip spaces after '='
    while (hrefStart < tag.size() &&
           (tag[hrefStart] == ' ' || tag[hrefStart] == '\t' ||
            tag[hrefStart] == '\n' || tag[hrefStart] == '\r')) {
      hrefStart++;
    }

    if (hrefStart >= tag.size()) {
      continue;
    }

    std::string link;

    char firstChar = tag[hrefStart];

    // Quoted href: href="page.html" or href='page.html'
    if (firstChar == '"' || firstChar == '\'') {
      size_t valueEnd = tag.find(firstChar, hrefStart + 1);

      if (valueEnd == std::string::npos) {
        continue;
      }

      link = tag.substr(hrefStart + 1, valueEnd - hrefStart - 1);
    }

    // Unquoted href: href=page.html
    else {
      size_t valueEnd = hrefStart;

      while (valueEnd < tag.size() && tag[valueEnd] != ' ' &&
             tag[valueEnd] != '\t' && tag[valueEnd] != '\n' &&
             tag[valueEnd] != '\r' && tag[valueEnd] != '>') {
        valueEnd++;
      }

      link = tag.substr(hrefStart, valueEnd - hrefStart);
    }

    if (!isIgnoredLink(link)) {
      link = makeAbsoluteURL(link, baseURL);
      links.push_back(link);
    }
  }

  return links;
}

bool HTMLParser::isIgnoredLink(const std::string &link) {
  if (link.empty()) {
    return true;
  }

  if (link[0] == '#') {
    return true;
  }

  if (link.compare(0, 11, "javascript:") == 0) {
    return true;
  }

  if (link.compare(0, 7, "mailto:") == 0) {
    return true;
  }

  if (link.compare(0, 4, "tel:") == 0) {
    return true;
  }

  return false;
}

std::string HTMLParser::makeAbsoluteURL(const std::string &link,
                                        const std::string &baseURL) {
  if (link.empty()) {
    return link;
  }

  // Already absolute URL
  if (link.compare(0, 7, "http://") == 0 ||
      link.compare(0, 8, "https://") == 0) {
    return link;
  }

  // Handle protocol-relative URL
  // Example: //example.com/page
  if (link.compare(0, 2, "//") == 0) {
    size_t protocolEnd = baseURL.find("://");

    if (protocolEnd == std::string::npos) {
      return link;
    }

    std::string protocol = baseURL.substr(0, protocolEnd);

    return protocol + ":" + link;
  }

  // Handle ./page.html
  if (link.compare(0, 2, "./") == 0) {
    return makeAbsoluteURL(link.substr(2), baseURL);
  }

  // Handle /page.html
  if (link[0] == '/') {
    size_t protocolEnd = baseURL.find("://");

    if (protocolEnd == std::string::npos) {
      return link;
    }

    size_t domainEnd = baseURL.find('/', protocolEnd + 3);

    std::string origin;

    if (domainEnd == std::string::npos) {
      origin = baseURL;
    } else {
      origin = baseURL.substr(0, domainEnd);
    }

    return origin + link;
  }

  size_t protocolEnd = baseURL.find("://");

  if (protocolEnd == std::string::npos) {
    return link;
  }

  size_t pathStart = baseURL.find('/', protocolEnd + 3);

  // Base: https://example.com
  if (pathStart == std::string::npos) {
    return baseURL + "/" + link;
  }

  // Find base directory
  std::string baseDirectory;

  if (baseURL.back() == '/') {
    baseDirectory = baseURL;
  } else {
    size_t lastSlash = baseURL.find_last_of('/');
    baseDirectory = baseURL.substr(0, lastSlash + 1);
  }

  // Handle ../ and multiple ../../
  std::string remainingLink = link;

  while (remainingLink.compare(0, 3, "../") == 0) {
    remainingLink = remainingLink.substr(3);

    // Remove trailing slash
    if (!baseDirectory.empty() && baseDirectory.back() == '/') {
      baseDirectory.pop_back();
    }

    size_t lastSlash = baseDirectory.find_last_of('/');

    // Do not move above the domain
    if (lastSlash == std::string::npos || lastSlash <= protocolEnd + 2) {
      return baseDirectory + "/" + remainingLink;
    }

    baseDirectory = baseDirectory.substr(0, lastSlash + 1);
  }

  return baseDirectory + remainingLink;
}