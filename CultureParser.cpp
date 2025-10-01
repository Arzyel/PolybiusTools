#include "CultureParser.h"
#include <tao/pegtl/file_input.hpp>


std::vector<std::string> CultureParser::key_stack; // definition + default initialization

void CultureParser::parseFile(const std::string& filePath) {
    // Check file exists
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Cannot open file\n";
        return;
    }

    // Read whole file into a string
    std::string content((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());

    pegtl::memory_input in(content, filePath);

    // Parse using our grammar and actions
    pegtl::parse<grammar, action>(in, nullptr);
}
