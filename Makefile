CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

git-cc: $(OBJ_FILES)
	g++ -o $@ $^ -lncurses -lmenu

obj/%.o: src/%.cpp
	g++ -c -o $@ $<

clean:
	rm obj/*.o

CC_FLAGS += -MMD
-include $(OBJFILES:.o=.d)
