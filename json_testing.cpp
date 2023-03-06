// json_testing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "jsonutils.hpp"
#include <fstream>
#include <filesystem>


bool file_valid(const std::filesystem::path& path) {
	return std::filesystem::exists(path) && std::filesystem::is_regular_file(path);
}

bool file_readable(const std::filesystem::path& path) {
	std::filesystem::perms p = std::filesystem::status(path).permissions();

	return	(p & std::filesystem::perms::owner_read) != std::filesystem::perms::none ||
		(p & std::filesystem::perms::group_read) != std::filesystem::perms::none ||
		(p & std::filesystem::perms::others_read) != std::filesystem::perms::none;
}

bool file_writable(const std::filesystem::path& path) {
	std::filesystem::perms p = std::filesystem::status(path).permissions();

	return	(p & std::filesystem::perms::owner_write) != std::filesystem::perms::none ||
		(p & std::filesystem::perms::group_write) != std::filesystem::perms::none ||
		(p & std::filesystem::perms::others_write) != std::filesystem::perms::none;
}

std::string read_file(const std::filesystem::path& path) {
	std::ifstream file(path.native(), std::ios::binary | std::ios::in);
	if (!file.is_open()) {
		throw std::exception("failed to open file");
	}

	std::string s;
	s.resize(std::filesystem::file_size(path));

	file.read(s.data(), s.size());

	return s;
}










int main()
{
	std::filesystem::path p("json_test.json");
	std::string s;
	if (file_valid(p) && file_readable(p)) {
		s = read_file(p);
	}

	json_parser root_parser(s);

	std::string item1;
	int number;
	float myfloat;
	float optional;
	std::vector<
		std::vector<int>
	> arr;

	root_parser.get<std::string>(item1).get<int>(number).get<float>(myfloat).get_named_optional<float>(optional, "optional_float").get<std::vector<int>>(arr);

	for (const std::vector<int>& x : arr) {
		for (auto y : x) {
			std::cout << y << " ";
		}
		std::cout << "\n";
	}
}

