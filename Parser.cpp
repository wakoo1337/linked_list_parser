#include <string>
#include "Headers.hpp"

int main() {
	struct ListNode* begin;
	begin = load("inlet.in");
	if (begin) {
		write(begin, "outlet.out");
		cleanup(begin);
		return 0;
	};
	return 255;
};