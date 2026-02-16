#include <iostream>
#include "../../include/LlamaInference/LlamaInference.h"
#include "../../third/llama.cpp/include/llama.h"

using namespace SearchEngine;

using std::cout;
using std::endl;

LlamaInference *LlamaInference::_pInstance = nullptr;

LlamaInference *LlamaInference::getInstance()
{
    if(!_pInstance)
    {
        _pInstance = new LlamaInference();
    }
    return _pInstance;
}

void LlamaInference::Destroy()
{
    if(_sampler) llama_sampler_free(_sampler);
    if(_ctx) llama_free(_ctx);
    if(_model) llama_model_free(_model);
    llama_backend_free();
}

bool LlamaInference::Init(const string &modelpath)
{
    llama_backend_init();
    auto m = llama_model_default_params();
    _model = llama_model_load_from_file(modelpath.c_str(), m);
    if(!_model)
    {
        cout << "open model fail" << endl;
        return false;
    }
    //陪置上下文
    llama_context_params params = llama_context_default_params();
    params.n_ctx = 2048;
    params.n_batch = 2048;
    _ctx = llama_init_from_model(_model, params);
    if(!_ctx)
    {
        cout << "set context fail" << endl;
        return false;;
    }
    //配置采样器链：TOP_K, Temperature规则
    _sampler = llama_sampler_chain_init(llama_sampler_chain_default_params());
    llama_sampler_chain_add(_sampler, llama_sampler_init_top_k(40));
    llama_sampler_chain_add(_sampler, llama_sampler_init_temp(0.8f));
    llama_sampler_chain_add(_sampler, llama_sampler_init_dist(LLAMA_DEFAULT_SEED));

    return true;

}


string LlamaInference::generateRespone(const string &question)
{
    std::lock_guard<mutex> lg(_mtx);
    if(!_model || !_ctx)
    {
        return "AI Engine not Initialized"; 
    }
    // A. 清理旧缓存（非常重要！）
    // 先传 NULL 探测实际需要的 token 数 (add_special = true 包含起始符)
    llama_memory_clear(llama_get_memory(_ctx), true);
    const struct llama_vocab * vocab = llama_model_get_vocab(_model);
    int n_token1 = -llama_tokenize(vocab, question.c_str(), question.size(), 
                                 NULL, 0, true, false);
    vector<llama_token> tokens_list(n_token1);
    
    int n_token2 = llama_tokenize(vocab, question.c_str(), question.length()
                                  , tokens_list.data(), tokens_list.size(), true, false);
    if(n_token2 < 0)
    {
        return "fail to tokenize the question";
    }

    for (auto id : tokens_list) {  // C++11 范围 for 循环，非常简洁
        char buf[128];
        int n = llama_token_to_piece(vocab, id, buf, sizeof(buf), 0, true);
        if (n < 0) {
            fprintf(stderr, "%s: error: failed to convert token to piece\n", __func__);
            continue;
        }
        std::string s(buf, n);      // 从缓冲区构造字符串（长度为 n）
        printf("%s", s.c_str());
    }
    llama_batch batch = llama_batch_get_one(tokens_list.data(), tokens_list.size());
    int max_token = 32;
    int n_decode = 0;
    string response = "";
    llama_token new_token_id;

    while(n_decode < max_token)
    {
        if(llama_decode(_ctx, batch))
        {
            return "fail to encode";
        }
        new_token_id = llama_sampler_sample(_sampler, _ctx, -1);

        if(llama_vocab_is_eog(vocab, new_token_id))
        {
            break;
        }
        
        char buf[128];
        int n = llama_token_to_piece(vocab, new_token_id, buf, sizeof(buf), 0, true);
        if(n > 0)
        {
            response += string(buf, n);
        }
        batch = llama_batch_get_one(&new_token_id, 1);
        ++n_decode;
    }
    
    return response;

}


void LlamaInference::reset_chat() {
    n_past = 0;
    // 如果找到了清空 KV cache 的函数，在这里调用一下
}










