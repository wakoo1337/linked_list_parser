#include "ListNode.hpp"

struct ListNode* load(std::string path);
bool write(struct ListNode* first, std::string path);
void cleanup(struct ListNode* node);