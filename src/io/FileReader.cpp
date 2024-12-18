#include <string>
#include <fstream>
#include <iostream>

#include "FileReader.hpp"

namespace dlb {
	std::string FileReader::read(const std::string& path) {
		const auto read_size = std::size_t(4096);
		std::ifstream file{ path, std::ios::binary | std::ios::ate };

		if (!file.is_open()) {
			std::cerr << "[FILEREADER] File not found: " << path << std::endl;
			exit(-1);
		}

		std::streamsize file_size = file.tellg();
		if (file_size <= 0) {
			std::cerr << "[FILEREADER] File is empty or size can not be determined: " << path << std::endl;
			exit(-1);
		}

		file.seekg(0, std::ios::beg);
		auto out = std::string();
		auto buf = std::string(read_size, '\0');
		while (file.read(&buf[0], read_size)) {
			out.append(buf, 0, file.gcount());
		}
		out.append(buf, 0, file.gcount());
		return out;
	}
};