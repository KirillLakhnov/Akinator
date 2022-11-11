all: main.cpp akinator.cpp libraries/common.cpp libraries/FileProcessing.cpp tree.cpp libraries/stack.cpp libraries/stack_common.cpp
	g++ main.cpp akinator.cpp libraries/common.cpp libraries/FileProcessing.cpp tree.cpp libraries/stack.cpp libraries/stack_common.cpp -o akinator

debug: main.cpp akinator.cpp libraries/common.cpp libraries/FileProcessing.cpp tree.cpp libraries/stack.cpp libraries/stack_common.cpp
	clang main.cpp akinator.cpp libraries/common.cpp libraries/FileProcessing.cpp tree.cpp libraries/stack.cpp libraries/stack_common.cpp -fsanitize=address -g -o akinator