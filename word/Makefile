SRCS:=$(wildcard src/*.cc)
OBJS:=$(patsubst src/%.cc,objs/%.o, $(SRCS))
OUT:=word

CC = g++

$(OUT): $(OBJS)
	$(CC) $(OBJS) -o $@ -g

objs/%.o: src/%.cc  
	@$(CC) -c $^   -o $@ -g 


.PHONY: clean rebuild print 

clean: 
	rm -f $(OBJS)
	
rebuild: clean $(OUT)

print:
	echo $(OBJS)
