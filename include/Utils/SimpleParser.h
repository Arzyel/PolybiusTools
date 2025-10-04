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

    static std::unordered_map<uint16_t, std::filesystem::path> getNumberedTxtFiles(const std::string& directory = ".") {
        std::unordered_map<uint16_t, std::filesystem::path> filesMap;

        for (const auto& entry : std::filesystem::directory_iterator(directory)) {
            if (!entry.is_regular_file()) continue;

            const auto filename = entry.path().filename().string();

            // Fast parse: read leading digits
            size_t i = 0;
            while (i < filename.size() && std::isdigit(static_cast<unsigned char>(filename[i]))) {
                ++i;
            }
            if (i == 0) continue; // no leading number

            uint16_t number = static_cast<uint16_t>(std::stoi(filename.substr(0, i)));
            if (filename.ends_with(".txt")) {
                filesMap[number] = entry.path();
            }
        }

        return filesMap;
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
