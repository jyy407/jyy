#include "../../include/LlamaInference/chatGPT.h"
#include <iostream>

using namespace SearchEngine;

int main() {
    chatGPT chat("../../models/qwen2.5-coder-7b-instruct-q5_k_m.gguf");

    std::cout << "User: 什么是人工智能？\n";
    std::cout << "AI: " << chat.chat("user1", "什么是人工智能？") << "\n";

    std::cout << "User: 刚才我问了你什么？\n";
    std::cout << "AI: " << chat.chat("user1", "刚才我问了你什么？") << "\n";

    return 0;
}
