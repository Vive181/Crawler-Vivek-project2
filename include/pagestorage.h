#ifndef PAGE_STORAGE_H
#define PAGE_STORAGE_H

#include <mysql/mysql.h>
#include <string>

class PageStorage {
private:
  MYSQL *connection;

public:
  PageStorage();
  ~PageStorage();

  PageStorage(const PageStorage &) = delete;
  PageStorage &operator=(const PageStorage &) = delete;

  bool storePage(const std::string &url, const std::string &html, int depth);

  std::string getPage(const std::string &url);

  bool hasPage(const std::string &url);

  int pageCount();

  bool removePage(const std::string &url);

  void clear();
};

#endif