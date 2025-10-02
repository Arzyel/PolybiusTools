#include "Eu4MainParser.h"

Eu4MainParser::Eu4MainParser()
{
    nodeStack.push_back(&rootNode);
}

void Eu4MainParser::parseFile(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Cannot open file\n";
        return;
    }

    // Read whole file into a string
    std::string content((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());

    pegtl::memory_input in(content, filePath);

    try {
        pegtl::parse<grammar, action>(in, *this);
    }
    catch (const pegtl::parse_error& e) {
        auto positions = e.positions();
        if (!positions.empty()) {
            const auto& pos = positions.front();
            std::cerr << "Parse failed at line " << pos.line << ", column " << pos.column << "\n";
        }
        else {
            std::cerr << "Parse failed (no position info)\n";
        }
        throw;
    }


}
