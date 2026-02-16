#ifndef __CHATENGINE_H__
#define __CHATENGINE_H__

#include <string>
#include <mutex>
#include <vector>
#include <deque>
#include "../../third/llama.cpp/include/llama.h"

using std::deque;
using std::vector;
using std::mutex;
using std::string;

struct llama_model;
struct llama_context;
struct llama_sampler;


namespace SearchEngine
{

struct Message {
    std::string role;
    std::string content;
};

class chatEngine
{
public:
    explicit chatEngine(llama_model* model);
    
    ~chatEngine();

    string chat(const string &queaation);
    
private:
    void init();
    string build_prompt();
    void cut_history_if_need();
private:
    mutex _mtx;

    llama_model* _model;
    llama_context* _ctx;
    llama_sampler* _sampler;

    deque<Message> _history;
    
    static constexpr int MAX_TURNS = 3;
    static constexpr int N_CTX = 2048;
    static constexpr int N_BATCH = 2048;
    static constexpr int MAX_NEW_TOKENS = 128;
    static constexpr int MAX_CONTEXT_TOKENS = 1000;
    static constexpr int SAFETY_MARGIN = 128;
};

}
#endif
