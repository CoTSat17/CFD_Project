#include "pch.h"

#include "gtest/gtest.h"

extern "C" {
#include <file_reader.h>
}



TEST(FileReader_Test, Init_test) {
	Nodes* nodes = (Nodes*)malloc(sizeof(Nodes));
	Elements* elements = (Elements*)malloc(sizeof(Elements));
	file_read(nodes, elements);
}