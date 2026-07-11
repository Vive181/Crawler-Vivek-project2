#ifndef HTMLPARSER_H
#define HTMLPARSER_H

#include <string>
#include <vector>

class HTMLParser {
public:
  HTMLParser();

  std::vector<std::string> extractLinks(const std::string &html,
                                        const std::string &baseURL);

private:
  bool isIgnoredLink(const std::string &link);

  std::string makeAbsoluteURL(const std::string &link,
                              const std::string &baseURL);
};

#endif