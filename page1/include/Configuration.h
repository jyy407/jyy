#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include "DisallowCopyAssign.h"

#include <string>
#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>

using std::string;
using std::map;
using std::set;
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::pair;
using std::cerr;

namespace SearchEngine
{
// 单例模式
class Configuration 
{
public:
    static Configuration* getInstance(const char* filePath); 
    map<string, string>& getConfigs();     // 获取存放配置文件内å[MBY7¹的map 
private:
    explicit Configuration(const string& filePath); // 构造函数私有化
    ~Configuration();

private:
    DISALLOWCOPYANDASSIGN(Configuration);
    
    static Configuration *_pInstance;
    string _configFilePath;       // 配置文件路径
    map<string, string> _configs;   // 配置文件内容
};

Configuration* Configuration::_pInstance = nullptr; //饱汉模式，因为是离线部分不用考虑多线程安全

Configuration *Configuration::getInstance(const char* filePath) 
{
    // 静态成员函数
    if (!_pInstance) 
    {
        _pInstance = new Configuration(string(filePath));          
    }
    return _pInstance;
}

Configuration::Configuration(const string &filePath) 
: _configFilePath(filePath) 
{
    // 初始化配置文件部分
    ifstream ifs;
    ifs.open(_configFilePath);
    if (!ifs.good()) 
    { 
        std::cerr << "open en_file_dir faile" << endl; 
        return;
    }

    string line, key, val;
    while (getline(ifs, line), !ifs.eof()) 
    {
        istringstream iss(line); 
        iss >> key;
        iss >> val;
        pair<string, string> record (key, val);
        _configs.insert(record);
    }
    ifs.close();
}

map<string, string>& Configuration::getConfigs() 
{
    return _configs;
}

} // namespace SearchEngine

#endif // __CONFIGURATION_H__


