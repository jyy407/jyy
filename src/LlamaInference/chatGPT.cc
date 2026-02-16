#include "../../include/LlamaInference/chatGPT.h"
#include <iostream>

using std::cout;
using std::endl;
using namespace SearchEngine;

static llama_model *get_model_once(const string &path)
{
    static llama_model *share_model = nullptr;
    static std::once_flag once ;
    std::call_once(once, [&](){
                   auto params = llama_model_default_params();
                   share_model = llama_model_load_from_file(path.c_str(), params);
                   if(!share_model){
                   std::cerr << "fail to create model" << endl;
                   }
                   });

    return share_model;
}



chatGPT::chatGPT(const string &model_path)
: _model_path(model_path)
{
    _model = get_model_once(model_path);
}

string chatGPT::chat(const string &user_id, const string &text)
{
    Session *sess = nullptr;
    {
        std::lock_guard<mutex> lg(_mtx);

        auto it = _session.find(user_id);
        if (it == _session.end()) {
            it = _session.emplace(user_id, Session{}).first;
        }
        auto &iter = it->second;
        if(!iter._engine)
        {
            iter._engine = std::make_unique<chatEngine>(_model);
        }
        iter.last_active = std::chrono::steady_clock::now();
        sess = &iter;
    }
    return sess->_engine->chat(text);
}

void chatGPT::cleanup_sessions(int timeout_sec)
{
    std::lock_guard<std::mutex> lock(_mtx);

    auto now = std::chrono::steady_clock::now();
    for (auto it = _session.begin(); it != _session.end(); ) {
        auto idle = std::chrono::duration_cast<std::chrono::seconds>(
            now - it->second.last_active
        ).count();

        if (idle > timeout_sec) {
            it = _session.erase(it); // ~ChatEngine 自动释放 ctx
        } else {
            ++it;
        }
    }
}























