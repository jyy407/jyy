#ifndef __LLAMA_INFERENCE_H__
#define __LLAMA_INFERENCE_H__

#include <string>
#include <mutex>
#include <vector>

using std::vector;
using std::mutex;
using std::string;

struct llama_model;
struct llama_context;
struct llama_sampler;

namespace SearchEngine
{

class LlamaInference
{
public:
    static LlamaInference *getInstance();
    void Destroy();
    bool Init(const string &modelpath);;
    string generateRespone(const string &queaation);
    void reset_chat();
private:
    LlamaInference()
    : _model(nullptr), _ctx(nullptr), _sampler(nullptr){}
    ~LlamaInference() {}
    bool createctx(int tokensize);
private:
    static LlamaInference *_pInstance;
    mutex _mtx;

    llama_model* _model;
    llama_context* _ctx;
    llama_sampler* _sampler;
    int32_t n_past = 0; // 新增：记录 KV 缓存的当前位置
};

}
#endif
