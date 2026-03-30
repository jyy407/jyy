#ifndef __REMOTECHATENGINE_H__
#define __REMOTECHATENGINE_H__

#include "chatEngine.h"
#include <deque>
#include <mutex>
#include <string>

namespace SearchEngine {

/// 调用远程 OpenAI 兼容接口（默认 Ollama：/v1/chat/completions）。
/// Windows 宿主机：安装 Ollama，设 OLLAMA_HOST=0.0.0.0:11434，防火墙放行 11434。
/// 虚拟机：export LLM_REMOTE_URL=http://<宿主机IP>:11434
class RemoteChatEngine {
public:
    explicit RemoteChatEngine(const std::string &base_url);

    std::string chat(const std::string &question);

private:
    void ensure_system_prompt();
    void cut_history_if_need();
    std::string post_chat_completions();

    std::mutex _mtx;
    std::string _base_url;
    std::deque<Message> _history;

    static constexpr int MAX_TURNS = 3;
    static constexpr int MAX_NEW_TOKENS = 128;
};

} // namespace SearchEngine

#endif
