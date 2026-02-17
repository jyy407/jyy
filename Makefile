OUT = bin/server
CC = g++

# llama.cpp 动态库目录（用于链接 -lllama/-lggml）
LIB_BIN_PATH = $(abspath third/llama.cpp/build/bin)
# AI 推理源码编译所需头文件
LLAMA_INC_FLAGS = -Ithird/llama.cpp/include -Ithird/llama.cpp/ggml/include
# AI 推理最终链接参数（含运行时 rpath）
LLAMA_LIB_FLAGS = -L$(LIB_BIN_PATH) -lllama -lggml -Wl,-rpath,$(LIB_BIN_PATH)


SRCS  = $(wildcard src/Reactor/*.cc)
SRCS2 = $(wildcard src/SearchEngineServer/*.cc)
SRCS3 = $(wildcard src/KeyRecommander/*.cc)
SRCS4 = $(wildcard src/WebPageSearcher/*.cc)
SRCS5 = $(wildcard src/LRUCache/*.cc)
SRCS6 = $(wildcard src/*.cc)
SRCS7 = $(wildcard src/TimerManager/*.cc)
SRCS8 = src/LlamaInference/chatGPT.cc \
		src/LlamaInference/chatEngine.cc

OBJS  = $(patsubst src/Reactor/%.cc,objs/%.o, $(SRCS))
OBJS2 = $(patsubst src/SearchEngineServer/%.cc,objs/%.o, $(SRCS2))
OBJS3 = $(patsubst src/KeyRecommander/%.cc,objs/%.o, $(SRCS3))
OBJS4 = $(patsubst src/WebPageSearcher/%.cc,objs/%.o, $(SRCS4))
OBJS5 = $(patsubst src/LRUCache/%.cc,objs/%.o, $(SRCS5))
OBJS6 = $(patsubst src/%.cc,objs/%.o, $(SRCS6))
OBJS7 = $(patsubst src/TimerManager/%.cc,objs/%.o, $(SRCS7))
OBJS8 = $(patsubst src/LlamaInference/%.cc,objs/%.o, $(SRCS8))

OBJS_ALL = $(OBJS) $(OBJS2) $(OBJS3) $(OBJS4) $(OBJS5) $(OBJS6) $(OBJS7) $(OBJS8)

$(OUT): $(OBJS_ALL)
	$(CC) $^ -o $@  -lpthread -llog4cpp $(LLAMA_LIB_FLAGS)

objs/%.o: src/Reactor/%.cc 
	@$(CC) -c $^ -o $@  
objs/%.o: src/SearchEngineServer/%.cc
	@$(CC) -c $^ -o $@  
objs/%.o: src/KeyRecommander/%.cc
	@$(CC) -c $^ -o $@  
objs/%.o: src/WebPageSearcher/%.cc
	@$(CC) -c $^ -o $@  
objs/%.o: src/LRUCache/%.cc
	@$(CC) -c $^ -o $@  
objs/%.o: src/%.cc
	@$(CC) -c $^ -o $@  
objs/%.o: src/TimerManager/%.cc
	@$(CC) -c $^ -o $@  
objs/%.o: src/LlamaInference/%.cc
	@$(CC) -c $^ -o $@ $(LLAMA_INC_FLAGS)

.PHONY: clean rebuild print 

clean: 
	rm -f $(OBJS_ALL)
	
rebuild: clean $(OUT)

print:
	echo $(OBJS_ALL)



