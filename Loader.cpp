#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "Headers.hpp"

void cleanup(struct ListNode *node) {
	while (node) {
		struct ListNode* next;
		next = node->next;
		delete node;
		node = next;
	};
};

struct ListNode* load(std::string path) {
	std::ifstream f_stream{ path, std::ios::in };
	if (!f_stream.good()) {
		std::cerr << "Cannot open file" << std::endl;
		return nullptr;
	};
	std::string readed_line;
	std::vector<ListNode*> items{}; // Обеспечивает произвольный доступ к элементам, чтобы можно было установить указатели
	std::unordered_map<ListNode*, int> rands{}; // Таблица соответствия индексов элементов указателям
	struct ListNode* first = nullptr;
	struct ListNode* previous = nullptr;
	struct ListNode** prev_next = &first;
	while (std::getline(f_stream, readed_line)) {
		size_t sep_index = readed_line.find(';');
		if (sep_index == std::string::npos) {
			cleanup(first);
			std::cerr << "Bad string format: " << readed_line << std::endl;
			return nullptr; // Неправильный формат строки
		};
		std::string name_str, index_str;
		struct ListNode* current;
		try {
			name_str = readed_line.substr(0, sep_index);
			index_str = readed_line.substr(sep_index + 1); // substr() выделяет память и может выбросить исключение
			current = new ListNode{};
		}
		catch (std::bad_alloc &ba) {
			cleanup(first);
			std::cerr << "Cannot allocate memory: " << ba.what() << std::endl;
			return nullptr; // Не удалось выделить память
		};
		const int node_index = std::stoi(index_str);
		*prev_next = current;
		prev_next = &current->next;
		current->next = nullptr;
		current->data = std::move(name_str);
		current->prev = previous;
		current->rand = nullptr;
		previous = current;
		items.push_back(current);
		if (node_index >= 0) {
			rands[current] = node_index;
		};
	};
	if (f_stream.bad()) {
		cleanup(first);
		std::cerr << "Cannot read from file" << std::endl;
		return nullptr; // Ошибка при чтении из файла
	};
	for (std::pair<ListNode*, int> item : rands) {
		if (item.second < items.size()) {
			item.first->rand = items[item.second];
		} else {
			cleanup(first);
			std::cerr << "Item index is beyond array size: index is" << item.second << ", size is" << items.size() << std::endl;
			return nullptr; // Индекс за пределами массива
		};
	};
	return first;
};