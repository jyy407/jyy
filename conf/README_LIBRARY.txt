【如何扩大离线网页库（传统搜索的主要数据来源）】

1. 准备更多 RSS/XML
   - 配置 offline/page/conf/page.conf 中的 XMLDIR（默认 data/xml）
   - 将新的 .xml 放入该目录，格式与现有 rss 一致（channel/item/title/link/description）

2. 重新生成索引（在 offline/page 工程下按你原来的方式编译并运行生成程序）
   - 会更新 offline/page/data/ripepage.dat、offset.dat、invertIndex.dat

3. 部署到在线检索
   - WebPageQuery 从 ../offline/page/data/ 加载这三个文件（相对 online 工作目录）
   - 替换/同步这三份 .dat 后重启 web_server / bin/server

【网页补充抓取（不替代离线库）】

- 复制 conf/crawl_seeds.txt.example 为 conf/crawl_seeds.txt，填入若干 http:// 种子页
- 若存在有效种子列表且未设置 SEARCH_WEB_SUPPLEMENT=off，则 /search 会在本地结果后按配额合并抓取快照
- 本地已有结果时，仅当标题/摘要与查询词相关才合并；本地无结果时，会尽量填入成功抓取的种子页
- 环境变量（可选）：
  SEARCH_WEB_SUPPLEMENT=0 关闭补充抓取
  CRAWL_SEEDS_PATH=/绝对路径/自己的种子文件.txt
  CRAWL_MAX_FETCH=8 单次搜索最多请求种子数（1～24）

注意：请在 robots.txt 与站点条款允许范围内做教学用途抓取；生产环境应使用正规搜索 API 或授权数据源。

【SQLite 抓取缓存（/search 合并优先）】

- 数据库默认路径：online/data/crawl_pages.db（自动创建 data/）
- 环境变量 CRAWL_SQLITE_PATH 可指定绝对路径
- /search 合并顺序：① SQLite 命中 ② 离线索引 ③ 实时种子抓取（成功后写入 SQLite，下次优先命中）
- 系统需安装 libsqlite3-dev（Linux: sudo apt install libsqlite3-dev）
- 链接：make web 时已加 -lsqlite3

【BFS 爬虫 worker（同域、深度/页数上限）】

- 编译：在 online 目录执行 `make crawl_worker`，生成 `bin/crawl_worker`
- 运行前准备：`conf/crawl_seeds.txt` 中至少一行 `http://` 种子（可为 https，程序会改为 http 抓取）
- 在 online 目录执行：`./bin/crawl_worker`
- 行为：从种子 BFS 扩展链接，仅同种子所在主机（可多个种子多个主机）；成功页面写入 `data/crawl_pages.db`
- 环境变量（可选）：
  CRAWL_MAX_PAGES=50      最多入库页数（1～2000）
  CRAWL_MAX_DEPTH=2      从种子算起的最大深度（0 仅种子页）
  CRAWL_DELAY_MS=400      两次请求之间间隔（毫秒，防过快）
  CRAWL_SAME_HOST=1       1=只跟同域链接；0=允许任意 http（慎用，仍受 MAX_PAGES 限制）
  CRAWL_SEEDS_PATH=...    种子文件路径
- 完成后启动 `bin/web_server`，/search 会优先命中 SQLite 中已爬页面
