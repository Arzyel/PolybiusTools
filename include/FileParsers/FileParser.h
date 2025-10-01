#pragma once
#ifndef FILE_PARSER_H
#define FILE_PARSER_H
#include <string>
#include <iostream>
#include <fstream>
#include <tao/pegtl.hpp>
#include <unordered_map>
#include <cstdint>

namespace pegtl = tao::pegtl;

struct Node {
    std::unordered_map<uint16_t, Node> children;
    std::string value;
};

class FileParser {
public:
    FileParser();
    ~FileParser() = default;
    void parseFile(const std::string& filePath);

    std::unordered_map<std::string, uint16_t> stringToIntID;
    std::vector<std::string> intToStringID;
    Node rootNode = Node();
    std::vector<Node*> nodeStack;
    int counterID = -1;
protected:

    // PEGTL
    // Rules
    struct identifier : pegtl::plus<pegtl::sor<pegtl::alnum,pegtl::one<'_'>>> {};
    struct eq : pegtl::one<'='> {};
    //struct ws : pegtl::star<pegtl::blank> {};
    //struct endl : pegtl::seq<ws, pegtl::one<'\n'>> {};
    /*struct mendl : pegtl::star<endl> {};*/
    struct ws : pegtl::star<pegtl::space> {};
    struct lbrace : pegtl::one<'{'> {};
    struct rbrace : pegtl::one<'}'> {};

    struct comment : pegtl::seq < pegtl::one<'#'>, pegtl::until<pegtl::eolf> > {};


    struct key : pegtl::seq<ws, identifier, ws, eq, ws> {};
    struct key_lbrace : pegtl::seq<ws, key, lbrace> {};
    struct key_value : pegtl::seq<ws, key, identifier, ws> {};

    struct block;
    struct block_body : pegtl::star<pegtl::sor<key_value, block>> {};
    struct block : pegtl::seq<key_lbrace, block_body, rbrace> {};

    struct grammar : pegtl::must<pegtl::star<pegtl::seq<ws, block>>> {};


    //Actions
    template<typename Rule> struct action : pegtl::nothing<Rule> {};

    template<>
    struct action<key_lbrace> {
        template<typename Input>
        static void apply(const Input& in, FileParser& parser) {
            std::string text = in.string();

            // Find '='
            auto pos = text.find('=');
            if (pos != std::string::npos) {
                std::string key = text.substr(0, pos);

                // Trim spaces
                key.erase(0, key.find_first_not_of(" \t\n\r"));
                key.erase(key.find_last_not_of(" \t\n\r") + 1);

                parser.intToStringID.push_back(key);
                int intID = ++parser.counterID;
                parser.stringToIntID[key] = intID;
                Node& child = parser.nodeStack.back()->children[intID];
                parser.nodeStack.push_back(&child);
            }
        }
    };

    template<>
    struct action<key_value> {
        template<typename Input>
        static void apply(const Input& in, FileParser& parser) {
            std::string text = in.string();

            // Find '='
            auto pos = text.find('=');
            if (pos != std::string::npos) {
                std::string key = text.substr(0, pos);
                std::string val = text.substr(pos + 1);

                // Trim spaces
                key.erase(0, key.find_first_not_of(" \t\n\r"));
                key.erase(key.find_last_not_of(" \t\n\r") + 1);

                val.erase(0, val.find_first_not_of(" \t\n\r"));
                val.erase(val.find_last_not_of(" \t\n\r") + 1);


                parser.intToStringID.push_back(key);
                int intID = ++parser.counterID;
                parser.stringToIntID[key] = intID;
                Node& child = parser.nodeStack.back()->children[intID];
                // add the value
                parser.nodeStack.back()->children[intID].value = val;


            }
        }
    };

    template<>
    struct action<rbrace> {
        template<typename Input>
        static void apply(const Input& in, FileParser& parser) {
            parser.nodeStack.pop_back();
        }
    };

private:

};

#endif // FILE_PARSER_H
