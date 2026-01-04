#pragma once
#ifndef SIMPLE_PARSER_H
#define SIMPLE_PARSER_H
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <cctype>
#include <vector>
#include <regex>
#include <filesystem>

class SimpleParser {
public:
    static std::unordered_map<std::string, std::string> parseKeyValueFile(const std::string& filename) {
        std::unordered_map<std::string, std::string> data;
        std::ifstream file(filename);
        if (!file.is_open())
            return data;

        std::string line;
        while (std::getline(file, line)) {
            // Remove comments
            auto commentPos = line.find('#');
            if (commentPos != std::string::npos)
                line = line.substr(0, commentPos);

            // Remove all whitespace safely
            line.erase(std::remove_if(line.begin(), line.end(),
                [](unsigned char c) { return std::isspace(c); }),
                line.end());

            if (line.empty())
                continue;

            auto eqPos = line.find('=');
            if (eqPos == std::string::npos)
                continue; // skip invalid lines

            std::string key = line.substr(0, eqPos);
            std::string value = line.substr(eqPos + 1);

            data[key] = value;
        }

        return data;
    }

    static void getNumberedTxtFiles(std::vector<std::tuple<uint16_t, std::string, std::filesystem::path>>& files,
        const std::string& directory = ".")
    {
        files.clear();
        files.reserve(60000);

        char temp_buf[256];

        for (const auto& entry : std::filesystem::directory_iterator(directory)) {
            if (!entry.is_regular_file()) continue;

            std::string filename_str = entry.path().filename().string();
            const char* filename = filename_str.c_str();
            size_t len = filename_str.size();

            if (len < 4 || memcmp(filename + len - 4, ".txt", 4) != 0) {
                continue;
            }

            // Parse digits
            const char* p = filename;
            const char* name_end = filename + len - 4;
            while (p < name_end && *p >= '0' && *p <= '9') p++;

            if (p == filename) continue;

            // Manual number conversion
            uint16_t number = 0;
            for (const char* d = filename; d < p; d++) {
                number = number * 10 + (*d - '0');
            }

            // Filter name
            char* dest = temp_buf;
            for (const char* src = p; src < name_end; ++src) {
                if (*src != '-' && *src != ' ') {
                    *dest++ = *src;
                }
            }
            *dest = '\0';

            files.emplace_back(number, std::string(temp_buf, dest - temp_buf), entry.path());
        }
    }

    static std::string getValueOrDefault(
        const std::unordered_map<std::string, std::string>& map,
        const std::string& key,
        const std::string& defaultValue = "")
    {
        auto it = map.find(key);
        if (it != map.end()) {
            return it->second;
        }
        return defaultValue;
    }


};



#endif // !SIMPLE_PARSER_H
