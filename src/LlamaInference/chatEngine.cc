#include <iostream>
#include "../../include/LlamaInference/chatEngine.h"


using std::cout;
using std::endl;
using namespace SearchEngine;

chatEngine::chatEngine(llama_model *model)
: _model(model)
, _ctx(nullptr)
, _sampler(nullptr)
{
    
}

chatEngine::~chatEngine()
{
    if(_sampler)
    {
        llama_sampler_free(_sampler);
        _sampler = nullptr;
    }
    if(_ctx)
    {
        llama_free(_ctx);
        _ctx = nullptr;
    }


}

string chatEngine::chat(const string &question)
{
    std::lock_guard<mutex> lg(_mtx);
    
    init();

    const llama_vocab *vocab = llama_model_get_vocab(_model);

    _history.push_back({"user", question});

    cut_history_if_need();

    string prompt = build_prompt();
    int n_token1 = -llama_tokenize(vocab, prompt.data(), prompt.size(),
                                        nullptr, 0, true, false);
    if(n_token1 <= 0)
    {
        return "error : tokenize fail";
    }

    vector<llama_token> tokens_list(n_token1);
    int n_token2 = llama_tokenize(vocab, prompt.data(), prompt.size(),
                                        tokens_list.data(), tokens_list.size(), true, false);
    
    llama_memory_clear(llama_get_memory(_ctx), true);

    llama_batch batch = llama_batch_get_one(tokens_list.data(), tokens_list.size());

    int n_decode = 0;
    string response;
    while(n_decode < MAX_NEW_TOKENS)
    {
        if(llama_decode(_ctx, batch) != 0)
        {
            break;
        }
        
        llama_token new_token_id = llama_sampler_sample(_sampler, _ctx, -1);

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

    _history.push_back({"assistant", response});
    return response;

}

void chatEngine::init()
{
    if(_ctx)
    {
        return;
    }

    llama_context_params ctx_params = llama_context_default_params();
    ctx_params.n_ctx = N_CTX;
    ctx_params.n_batch = N_BATCH;

    _ctx = llama_init_from_model(_model, ctx_params);
    if(!_ctx)
    {
        cout << "failed to create context" << endl;
        return;
    }
    
    _sampler = llama_sampler_chain_init(llama_sampler_chain_params());
    llama_sampler_chain_add(_sampler, llama_sampler_init_top_k(40));
    llama_sampler_chain_add(_sampler, llama_sampler_init_temp(0.8f));
    llama_sampler_chain_add(_sampler, llama_sampler_init_dist(LLAMA_DEFAULT_SEED));

    _history.push_back({"system","你是一个专业、简洁、有帮助的AI助手。"});
}

void chatEngine::cut_history_if_need() {
    // history[0] 是 system
    const size_t max_size = 1 + MAX_TURNS * 2;

    if (_history.size() <= max_size)
        return;

    // 从 system 后面开始删，删到只剩 max_size
    _history.erase(
        _history.begin() + 1,
        _history.end() - MAX_TURNS * 2
    );
}



std::string chatEngine::build_prompt() {
    std::string prompt;

    for (auto &m : _history) {
        if (m.role == "system")      prompt += "<|system|>\n";
        else if (m.role == "user")   prompt += "<|user|>\n";
        else                         prompt += "<|assistant|>\n";

        prompt += m.content;
        prompt += "\n";
    }

    prompt += "<|assistant|>\n";
    return prompt;
}











