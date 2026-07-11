#include "../include/pagestorage.h"

#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <string>

PageStorage::PageStorage() {
  connection = mysql_init(nullptr);

  if (connection == nullptr) {
    throw std::runtime_error("MySQL initialization failed");
  }

  const char *dbPassword = std::getenv("CRAWLER_DB_PASSWORD");

  if (dbPassword == nullptr) {
    mysql_close(connection);
    connection = nullptr;

    throw std::runtime_error(
        "CRAWLER_DB_PASSWORD environment variable is not set");
  }

  if (mysql_real_connect(connection, "localhost", "root", dbPassword,
                         "crawler_db", 3306, nullptr, 0) == nullptr) {

    std::string error = mysql_error(connection);

    mysql_close(connection);
    connection = nullptr;

    throw std::runtime_error("MySQL connection failed: " + error);
  }
}

PageStorage::~PageStorage() {
  if (connection != nullptr) {
    mysql_close(connection);
    connection = nullptr;
  }
}

bool PageStorage::storePage(const std::string &url, const std::string &html,
                            int depth) {
  MYSQL_STMT *stmt = mysql_stmt_init(connection);

  if (stmt == nullptr) {
    return false;
  }

  const char *query = "INSERT INTO pages (url, html, depth) VALUES (?, ?, ?) "
                      "ON DUPLICATE KEY UPDATE "
                      "html = VALUES(html), "
                      "depth = VALUES(depth), "
                      "crawled_at = CURRENT_TIMESTAMP";

  if (mysql_stmt_prepare(stmt, query, strlen(query)) != 0) {
    mysql_stmt_close(stmt);
    return false;
  }

  MYSQL_BIND bind[3] = {};

  unsigned long urlLength = url.length();
  unsigned long htmlLength = html.length();

  bind[0].buffer_type = MYSQL_TYPE_STRING;
  bind[0].buffer = const_cast<char *>(url.data());
  bind[0].buffer_length = urlLength;
  bind[0].length = &urlLength;

  bind[1].buffer_type = MYSQL_TYPE_LONG_BLOB;
  bind[1].buffer = const_cast<char *>(html.data());
  bind[1].buffer_length = htmlLength;
  bind[1].length = &htmlLength;

  bind[2].buffer_type = MYSQL_TYPE_LONG;
  bind[2].buffer = &depth;

  if (mysql_stmt_bind_param(stmt, bind) != 0) {
    mysql_stmt_close(stmt);
    return false;
  }

  bool success = mysql_stmt_execute(stmt) == 0;

  mysql_stmt_close(stmt);

  return success;
}

std::string PageStorage::getPage(const std::string &url) {
  MYSQL_STMT *stmt = mysql_stmt_init(connection);

  if (stmt == nullptr) {
    return "";
  }

  const char *query = "SELECT html FROM pages WHERE url = ? LIMIT 1";

  if (mysql_stmt_prepare(stmt, query, strlen(query)) != 0) {
    mysql_stmt_close(stmt);
    return "";
  }

  MYSQL_BIND param[1] = {};
  unsigned long urlLength = url.length();

  param[0].buffer_type = MYSQL_TYPE_STRING;
  param[0].buffer = const_cast<char *>(url.data());
  param[0].buffer_length = urlLength;
  param[0].length = &urlLength;

  if (mysql_stmt_bind_param(stmt, param) != 0 ||
      mysql_stmt_execute(stmt) != 0) {
    mysql_stmt_close(stmt);
    return "";
  }

  MYSQL_BIND result[1] = {};
  unsigned long htmlLength = 0;

  result[0].buffer_type = MYSQL_TYPE_LONG_BLOB;
  result[0].length = &htmlLength;

  if (mysql_stmt_bind_result(stmt, result) != 0 ||
      mysql_stmt_store_result(stmt) != 0) {
    mysql_stmt_close(stmt);
    return "";
  }

  int fetchStatus = mysql_stmt_fetch(stmt);

  if (fetchStatus != 0 && fetchStatus != MYSQL_DATA_TRUNCATED) {
    mysql_stmt_close(stmt);
    return "";
  }

  if (htmlLength == 0) {
    mysql_stmt_close(stmt);
    return "";
  }

  std::string html(htmlLength, '\0');

  result[0].buffer = html.data();
  result[0].buffer_length = htmlLength;

  if (mysql_stmt_fetch_column(stmt, &result[0], 0, 0) != 0) {
    mysql_stmt_close(stmt);
    return "";
  }

  mysql_stmt_close(stmt);
  return html;
}

bool PageStorage::hasPage(const std::string &url) {
  MYSQL_STMT *stmt = mysql_stmt_init(connection);

  if (stmt == nullptr) {
    return false;
  }

  const char *query = "SELECT 1 FROM pages WHERE url = ? LIMIT 1";

  if (mysql_stmt_prepare(stmt, query, strlen(query)) != 0) {
    mysql_stmt_close(stmt);
    return false;
  }

  MYSQL_BIND param[1] = {};

  unsigned long urlLength = url.length();

  param[0].buffer_type = MYSQL_TYPE_STRING;
  param[0].buffer = const_cast<char *>(url.data());
  param[0].buffer_length = urlLength;
  param[0].length = &urlLength;

  if (mysql_stmt_bind_param(stmt, param) != 0) {
    mysql_stmt_close(stmt);
    return false;
  }

  if (mysql_stmt_execute(stmt) != 0) {
    mysql_stmt_close(stmt);
    return false;
  }

  int value = 0;
  MYSQL_BIND result[1] = {};

  result[0].buffer_type = MYSQL_TYPE_LONG;
  result[0].buffer = &value;

  if (mysql_stmt_bind_result(stmt, result) != 0) {
    mysql_stmt_close(stmt);
    return false;
  }

  bool exists = mysql_stmt_fetch(stmt) == 0;

  mysql_stmt_close(stmt);

  return exists;
}

int PageStorage::pageCount() {
  const char *query = "SELECT COUNT(*) FROM pages";

  if (mysql_query(connection, query) != 0) {
    return -1;
  }

  MYSQL_RES *result = mysql_store_result(connection);

  if (result == nullptr) {
    return -1;
  }

  MYSQL_ROW row = mysql_fetch_row(result);

  if (row == nullptr) {
    mysql_free_result(result);
    return -1;
  }

  int count = std::stoi(row[0]);

  mysql_free_result(result);

  return count;
}

bool PageStorage::removePage(const std::string &url) {
  MYSQL_STMT *stmt = mysql_stmt_init(connection);

  if (stmt == nullptr) {
    return false;
  }

  const char *query = "DELETE FROM pages WHERE url = ?";

  if (mysql_stmt_prepare(stmt, query, strlen(query)) != 0) {
    mysql_stmt_close(stmt);
    return false;
  }

  MYSQL_BIND bind[1] = {};
  unsigned long urlLength = url.length();

  bind[0].buffer_type = MYSQL_TYPE_STRING;
  bind[0].buffer = const_cast<char *>(url.data());
  bind[0].buffer_length = urlLength;
  bind[0].length = &urlLength;

  if (mysql_stmt_bind_param(stmt, bind) != 0 || mysql_stmt_execute(stmt) != 0) {
    mysql_stmt_close(stmt);
    return false;
  }

  bool removed = mysql_stmt_affected_rows(stmt) > 0;

  mysql_stmt_close(stmt);

  return removed;
}

void PageStorage::clear() {
  const char *query = "DELETE FROM pages";

  mysql_query(connection, query);
}