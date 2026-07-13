#include "../include/crawler.h"
#include <iostream>

int main() {

  std::cout << "Before crawler object\n";

  Crawler crawler(2);

  std::cout << "Before crawl\n";

  crawler.crawl("https://leetcode.com/contest/weekly-contest-511/");

  std::cout << "After crawl\n";

  return 0;
}