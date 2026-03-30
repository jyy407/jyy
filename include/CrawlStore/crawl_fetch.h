#ifndef CRAWL_FETCH_H
#define CRAWL_FETCH_H

#include <string>
#include <vector>

struct CrawlFetchedPage {
    std::string url;
    std::string title;
    std::string desc;
    std::string html;
    bool ok = false;
};

std::string crawl_trim(const std::string &s);

/// 抓取页面：支持 http:// 与 https://（依赖 OpenSSL 与 CPPHTTPLIB_OPENSSL_SUPPORT）。
CrawlFetchedPage crawl_fetch_http_page(const std::string &url);

/// 从 HTML 中提取 href，并转为绝对 URL（过滤 mailto/javascript 等）。
std::vector<std::string> crawl_extract_hrefs(const std::string &html,
                                            const std::string &base_url);

/// 相对链接解析为绝对 URL（与 base 同 scheme：http 或 https）。
std::string crawl_resolve_url(const std::string &base_url, const std::string &href);

std::string crawl_normalize_url_key(const std::string &url);

/// 从 URL 得到小写 host（不含端口），失败返回空。
std::string crawl_url_host(const std::string &url);

#endif
