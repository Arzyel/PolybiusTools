#include "FileParser.h"

FileParser::FileParser()
{
	nodeStack.push_back(&rootNode);
}



void FileParser::parseFile(const std::string& filePath) {
    // Check file exists
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Cannot open file\n";
        return;
    }

    // Read whole file into a string
    std::string content((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());

    pegtl::file_input<> in(filePath);

    // Parse using our grammar and actions
    pegtl::parse<grammar, action>(in, *this);
}