#include <string>
#include <fstream>
#include <iostream>
#include <cstdint>
#include <array>
#include <unordered_map>
#include "Headers.hpp"

bool write(struct ListNode* first, std::string path) {
	static const uint32_t magic = 1337;
	std::unordered_map<ListNode*, int> indices{};
	struct ListNode* current = first;
	uint32_t i = 0;
	while (current) {
		indices[current] = i;
		current = current->next;
		i++;
	};
	std::ofstream f_stream{ path, std::ios::binary | std::ios::trunc };
	if (!f_stream.good()) {
		std::cerr << "Cannot open file" << std::endl;
		return true;
	}
	f_stream.exceptions(std::ios::badbit);
	try {
		f_stream.write((const char*)&magic, sizeof magic); // Записываем магическое число
		f_stream.write((const char*)&i, sizeof i); // Записываем общее количество элементов
		current = first;
		while (current) {
			static const std::array<char, 8> zero_pad{};
			const int32_t length = current->data.length();
			f_stream.write((const char*)&length, sizeof length); // Записываем длину пользовательских данных
			const int32_t index = current->rand ? indices[current->rand] : (-1);
			f_stream.write((const char*)&index, sizeof index); // Записываем индекс элемента, куда указывает rand
			f_stream.write((const char*)current->data.data(), current->data.length()); // Записываем данные
			f_stream.write(zero_pad.data(), (8 - (current->data.length() % 8)) & 7); // Записываем набивку до кратности в 8 байтов
			current = current->next;
		};
	} catch (std::ios_base::failure& failure) {
		std::cerr << "Cannot write file: " << failure.what() << std::endl;
		return true;
	}
	return false;
};