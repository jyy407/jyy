#ifndef __SE_PAGE_H__
#define __SE_PAGE_H__

#include "DictProducer.h"
#include "WordSegmentation.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
#include <map>

using std::string;
using std::vector;
using std::unordered_map;
using std::set;
using std::pair;
using std::map;

namespace SearchEngine
{

class Configuration;

//还可以将内容也存起来，多加一个数据成员而已
struct RSSIteam 
{
    string _title;
    string _link;
    string _description;
};

class Compare
{
    bool operator()(const pair<int,double> & lhs, const pair<int, double> & rhs) 
    {
            return lhs.first < rhs.first;
    }
};

class PageLib
{
public:
    PageLib();
    ~PageLib() 
    {}
    std::unordered_map<int, map<string, int>> & getWordsMap() 
    {
        return _wordMap;
    }
    std::unordered_map<int, vector<string>> & get_TopK() {
        return _topK;
    }
private:
    void create();
    void store();
    void handleInvertIndex();
    void handleWordMap();
    void handleTopK();
    void PageDeDuplication();
private:
    int    _DOCICNUM;//去重后的文章总数
    vector<RSSIteam> _rss;//解析rss，将其存在vector中
    Configuration *  _config;//读取配置文件获取xml文件的路径
    DictProducer     _dict;//创建cppjieba对象（这两个数据成员可以优化）
    WordSegmentation &_jieba;//分词对象的引用
    unordered_map<string, set<pair<int , double>>> _invertIndex;//倒排索引
    unordered_map<int, map<string, int>> _wordMap;//没有用到
    unordered_map<int, vector<string>>   _topK;//没有用到
};

} // namespace search_engine

#endif // __SE_PAGE_H__
