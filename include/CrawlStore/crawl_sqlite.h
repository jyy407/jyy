#ifndef CRAWL_SQLITE_H
#define CRAWL_SQLITE_H

#include <string>
#include <vector>

struct CrawlPageRow {
    std::string url;
    std::string title;
    std::string desc;
};

/// 打开库、建表。失败时 insert/search 变为空操作。
bool crawl_sqlite_init();

/// 抓取成功后写入；url 唯一，重复则覆盖标题与摘要。
bool crawl_sqlite_insert(const std::string &url, const std::string &title,
                         const std::string &desc);

/// 按查询匹配：空格分词后各词需在 title/desc/url 中至少出现一处（AND）；无空格则整句匹配；UTF-8。
std::vector<CrawlPageRow> crawl_sqlite_search(const std::string &query, int limit);

#endif
