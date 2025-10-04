#pragma once
#ifndef EU4_PARSER_H
#define EU4_PARSER_H
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>

struct keyToken {
	const char* start;
	size_t length;
};

static int parse(const std::string& filePath) {
	std::ifstream file(filePath);
	if (!file.is_open()) {
		std::cerr << "Could not open file at : " << filePath << "\n";
		return 1;
	}

	std::string line;
	while (std::getline(file, line)) {
		//std::cout << line << std::endl;
	}

	file.close();

	return 0;
}
static int parseWithView(const std::string& filePath) {
	std::ifstream file(filePath, std::ios::binary | std::ios::ate);
	if (!file.is_open()) {
		std::cerr << "Could not open file at : " << filePath << "\n";
		return 1;
	}

	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	std::vector<char> buffer(size);
	if (!file.read(buffer.data(), size)) {
		std::cerr << "Failed to read file\n";
		return 1;
	}

	const char* ptr = buffer.data();
	const char* end = ptr + buffer.size();
	
	while (ptr < end) {
		const char* line_start = ptr;

		while (ptr < end && *ptr != '\n') ++ptr;

		std::string_view line(line_start, ptr - line_start);
		//std::cout << line << '\n';
		for (char c : line) {
			switch (c) {
			case '#':
				goto skip_rest_of_line;
				break;
			}
			//std::cout << c;
		}
		//std::cout << '\n';

		skip_rest_of_line:
		if (ptr < end && *ptr == '\n') ++ptr;
	}

	file.close();

	return 0;
}
static int parseWithView2(const std::string& filePath) {
	std::ifstream file(filePath, std::ios::binary | std::ios::ate);
	if (!file.is_open()) {
		std::cerr << "Could not open file at : " << filePath << "\n";
		return 1;
	}

	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	std::vector<char> buffer(size);
	if (!file.read(buffer.data(), size)) {
		std::cerr << "Failed to read file\n";
		return 1;
	}

	const char* ptr = buffer.data();
	const char* end = ptr + buffer.size();

	std::vector<keyToken> keyStack;
	const char* token_start = nullptr;
	size_t token_length = 0;
	uint8_t depth = 0;
	
	while (ptr < end) {
		// Use memchr to find next newline
		const char* line_end = static_cast<const char*>(memchr(ptr, '\n', end - ptr));
		if (!line_end) line_end = end;

		// Handle Windows \r\n endings
		size_t line_length = line_end - ptr;
		if (line_length > 0 && ptr[line_length - 1] == '\r')
			--line_length;

		// Create string_view without copying
		std::string_view line(ptr, line_length);

		// Process the line (example: just printing)
		

		for (char c : line) {
			switch (c) {
			case '{':
				++depth;
				break;
			case '}':
				--depth;
				break;
			case '=':
				keyStack.push_back(keyToken(token_start, token_length));
				break;
			case '#':
				goto skip_rest_of_line2;
				break;
			default:
				token_start = line.data();
				token_length = &c - line.data();
			}
			//std::cout << c;
		}
		//std::cout << '\n';

		skip_rest_of_line2:
		// Move pointer past the newline
		ptr = (line_end < end) ? line_end + 1 : line_end;
	}

	file.close();

	return 0;
}
static int parseWithView3(const std::string& filePath) {
	std::ifstream file(filePath, std::ios::binary | std::ios::ate);
	if (!file.is_open()) {
		std::cerr << "Could not open file at : " << filePath << "\n";
		return 1;
	}

	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	std::vector<char> buffer(size);
	if (!file.read(buffer.data(), size)) {
		std::cerr << "Failed to read file\n";
		return 1;
	}

	const char* ptr = buffer.data();
	const char* end = ptr + buffer.size();

	/*std::vector<keyToken> keyStack;
	const char* token_start = nullptr;
	size_t token_length = 0;*/
	uint8_t depth = 0;
	std::vector<std::string> keyStack;
	std::string keyBuffer;
	std::string valueBuffer;
	bool captureKeyorValue = true;
	bool isList = false;


	while (ptr < end) {
		const char* line_end = static_cast<const char*>(memchr(ptr, '\n', end - ptr));
		if (!line_end) line_end = end;

		size_t line_length = line_end - ptr;
		if (line_end < end && line_end[-1] == '\r') --line_length; // optional \r trim

		// include \n if it's actually in the buffer
		if (line_end < end) ++line_length;

		std::string_view line(ptr, line_length);


		for (auto it = line.begin(); it != line.end(); ++it) {
			char c = *it;
			switch (c) {
			case '\t':
				if (!valueBuffer.empty()) {
					std::cout << "Value : " << valueBuffer << '\n';
					valueBuffer.clear();
					captureKeyorValue = true;
				}
				break;
			case '\n':
				if (!valueBuffer.empty()) {
					std::cout << "Value : " << valueBuffer << '\n';
					valueBuffer.clear();
					captureKeyorValue = true;
				}
				break;
			case ' ':
				if (!valueBuffer.empty()) {
					std::cout << "Value : " << valueBuffer << '\n';
					valueBuffer.clear();
					captureKeyorValue = true;
				}
				break;
			case '{': {
				// finish key or value
				if (!keyBuffer.empty()) keyStack.push_back(keyBuffer);
				keyBuffer.clear();
				if (!valueBuffer.empty()) {
					// store value somewhere
					valueBuffer.clear();
				}
				++depth;
				captureKeyorValue = true; // reset for next key

				// ✅ Calculate actual position in buffer
				const char* currentPosInBuffer = line.data() + std::distance(line.begin(), it);

				const char* tempPtr = currentPosInBuffer;
				while (tempPtr < end) {
					if (*tempPtr == '=') {
						isList = false;
						break;
					}
					else if (*tempPtr == '}') {
						isList = true;
						break;
					}

					++tempPtr;
				}
				break;
			}

			case '}':
				if (!keyBuffer.empty()) keyStack.push_back(keyBuffer);
				keyBuffer.clear();
				if (!valueBuffer.empty()) {
					// store value
					valueBuffer.clear();
				}
				--depth;
				if (!keyStack.empty()) keyStack.pop_back(); // exit scope
				break;

			case '=':
				if (!keyBuffer.empty()) keyStack.push_back(keyBuffer);
				std::cout << keyBuffer << '\n';
				keyBuffer.clear();
				captureKeyorValue = false; // start capturing value
				break;

			case '#':
				goto skip_rest_of_line; // comment
				break;

			default:
				if (captureKeyorValue)
					keyBuffer.push_back(c);
				else
					valueBuffer.push_back(c);
			}
		}

	skip_rest_of_line:
		ptr = (line_end < end) ? line_end + 1 : line_end;
	}


	file.close();

	return 0;
}
static int parseWithView4(const std::string& filePath) {
	std::ifstream file(filePath, std::ios::binary | std::ios::ate);
	if (!file.is_open()) {
		std::cerr << "Could not open file at : " << filePath << "\n";
		return 1;
	}

	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	std::vector<char> buffer(size);
	if (!file.read(buffer.data(), size)) {
		std::cerr << "Failed to read file\n";
		return 1;
	}

	const char* ptr = buffer.data();
	const char* end = ptr + buffer.size();

	uint8_t depth = 0;
	std::vector<std::string> keyStack;
	std::string keyBuffer;
	std::string valueBuffer;
	bool captureKeyorValue = true;
	bool isList = false;


	while (ptr < end) {
		char c = *ptr;
		switch (c) {
		case '#': {
			while (*ptr != '\n')
				++ptr;
			break;
		}
		case '{': {
			const char* tempPtr = ptr;
			bool endLoop = false;
			++tempPtr;
			while (tempPtr < end && !endLoop) {
				switch (*tempPtr) {
					case '=': {
						valueBuffer.clear();
						endLoop = true;
						break;
					}

					case '}': {
						std::cout << valueBuffer << '\n';
						valueBuffer.clear();
						ptr = tempPtr;
						endLoop = true;
						break;
					}

					case '\n':
					case '\t': {
						// Only add space if buffer is not empty AND last char isn't already a space
						if (!valueBuffer.empty() && valueBuffer.back() != ' ') {
							valueBuffer.push_back(' ');
						}
						break;
					}

					default: {
						valueBuffer.push_back(*tempPtr);
						break;
					}
				}
				++tempPtr;
			}
			break;
		}
		}
		++ptr;
	}


	file.close();

	return 0;
}




#endif // EU4_PARSER_H
