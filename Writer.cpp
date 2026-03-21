#include <string>
#include <fstream>
#include <iostream>
#include <cstdint>
#include <array>
#include <bit>
#include <unordered_map>
#include "Headers.hpp"

uint32_t to_little_endian(const uint32_t x) {
	if (std::endian::native == std::endian::little) {
		return x;
	}
	else {
		return ((x & 255) << 24) | ((x & 65280) << 8) | ((x & 16711680) >> 8) | ((x & 4278190080ul) >> 16);
	};
};
bool write(struct ListNode* first, std::string path) {
	static const uint32_t magic = to_little_endian(1337); // TODO перенести преобразование в какой-то один порядок байтов, но через функцию не из сетевого стека
	std::unordered_map<ListNode*, int> indices{};
	struct ListNode* current = first;
	uint32_t i = 0;
	while (current) {
		indices[current] = i;
		current = current->next;
		i++;
	};
	i = to_little_endian(i);
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
			const int32_t length = to_little_endian(current->data.length());
			f_stream.write((const char*)&length, sizeof length); // Записываем длину пользовательских данных
			const int32_t index = to_little_endian(current->rand ? indices[current->rand] : -1);
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