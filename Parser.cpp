#include <string>
#include "Headers.hpp"

int main() {
	struct ListNode* begin;
	begin = load("C:\\Users\\ִלטענטי\\source\\repos\\LinkedListParser\\in.txt");
	write(begin, "C:\\Users\\ִלטענטי\\source\\repos\\LinkedListParser\\out");
	cleanup(begin);
};