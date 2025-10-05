#pragma once
#ifndef EU4_PARSER_H
#define EU4_PARSER_H
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include "MemoryMappingFile.h"

inline static const char* handleBracketLists(const char* ptr,const char* end,
	std::string& valueBuffer, std::vector<std::string>& keyStack) {
	const char* tempPtr = ptr;
	bool endLoop = false;
	++tempPtr;
	while (tempPtr < end && !endLoop) {
		switch (*tempPtr) {
		case '=': {
			valueBuffer.clear();
			endLoop = true;
			return ptr;
			break;
		}

		case '}': {
			valueBuffer.clear();
			endLoop = true;
			if (!keyStack.empty()) {
				keyStack.pop_back();
			}
			return tempPtr;
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
}


template<typename Callable>
static int parseEu4File(const std::string& filePath, Callable callable) {
	mmap::Handle handle;
	if (!mmap::open(filePath, handle)) {
		std::cerr << "Could not open file at : " << filePath << "\n";
		return 1;
	}

	const char* ptr = handle.data;
	const char* end = ptr + handle.size;

	std::vector<std::string> keyStack;
	std::string keyBuffer;
	std::string valueBuffer;
	bool captureKey = true;
	bool isList = false;


	while (ptr < end) {
		char c = *ptr;
		switch (c) {
		case '#': {
			while (ptr < end && *ptr != '\n') {
				++ptr;
			}
			break;
		}
		case '{': {
			captureKey = true;
			valueBuffer.clear();
			ptr = handleBracketLists(ptr, end, valueBuffer, keyStack);
			break;
		}
		case '=': {
			keyStack.push_back(keyBuffer);
			keyBuffer.clear();
			captureKey = false;
			break;
		}
		case '}':
			if (!keyStack.empty()) {
				keyStack.pop_back();
			}
			break;
		case ' ':
		case '\n':
		case '\t': {
			break;
		}

		default: {
			if (captureKey) {
				keyBuffer.push_back(c);
			}
			else {
				while (ptr < end) {
					char c = *ptr;
					if (c == '\n' || c == '\t' || c == '#' || c == '}' || c == ' ') {
						--ptr;
						break;
					}
					if ( c != '\"') {
						valueBuffer.push_back(c);
					}
					++ptr;
				}
				captureKey = true;
				//std::cout << "Value : " << valueBuffer << "\n";
				callable(keyStack, valueBuffer);
				if (!keyStack.empty()) {
					keyStack.pop_back();
				}
				valueBuffer.clear();
			}
		}
		}
		++ptr;
	}
	mmap::close(handle);

	return 0;
}




#endif // EU4_PARSER_H
