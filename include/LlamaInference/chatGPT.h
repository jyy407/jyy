#ifndef __CHATGPT_H__
#define __CHATGPT_H__

#include <string>
#include <mutex>
#include <vector>
#include <unordered_map>
#include <memory>
#include <chrono>
#include "chatEngine.h"
#include "../../third/llama.cpp/include/llama.h"

using std::unordered_map;
using std::vector;
using std::mutex;
using std::string;

namespace SearchEngine
{

class chatGPT
{
public:
    explicit chatGPT(const string &model_path);
    string chat(const string &user_id, const string &text);

    void cleanup_sessions(int timeout = 300);

private:


    struct Session
    {
        std::unique_ptr<chatEngine> _engine;
        std::chrono::steady_clock::time_point last_active;
    };

    mutex _mtx;

    llama_model* _model;
    string _model_path;
    unordered_map<string, Session> _session;


};

}
#endif
