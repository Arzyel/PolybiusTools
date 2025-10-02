#pragma once
#ifndef EU4_MAIN_PARSER_H
#define EU4_MAIN_PARSER_H
#include <string>
#include <iostream>
#include <fstream>
#include <tao/pegtl.hpp>
#include <unordered_map>
#include <cstdint>

namespace pegtl = tao::pegtl;
//#include "FileParser.h"
struct Node {
    std::unordered_map<uint16_t, Node> children;
    std::string value;
};

class Eu4MainParser{
public:
    Eu4MainParser();
    ~Eu4MainParser() = default;
    void parseFile(const std::string& filePath);

    std::unordered_map<std::string, uint16_t> stringToIntID;
    std::vector<std::string> intToStringID;
    Node rootNode = Node();
    std::vector<Node*> nodeStack;
    int counterID = -1;
protected:

// -----------------------
// Basic PEGTL rules
// -----------------------
    struct ws_space : pegtl::star<pegtl::space> {};
    struct lbrace : pegtl::one<'{'> {};
    struct rbrace : pegtl::one<'}'> {};
    struct eq : pegtl::one<'='> {};
    struct identifier : pegtl::plus<pegtl::sor<
        pegtl::alnum,
        pegtl::one<'_'>,
        pegtl::range<0x80, 0xFF>  // Accept ANSI extended characters
        >> {};
    struct comment : pegtl::seq<pegtl::one<'#'>, pegtl::until<pegtl::eolf>> {};
    struct ws : pegtl::star<pegtl::sor<pegtl::space, comment>> {};

    struct key : pegtl::seq<ws, identifier, ws, eq, ws> {};
    struct key_lbrace : pegtl::seq<key, lbrace> {};
    struct key_value : pegtl::seq<key, identifier> {};

    struct com_key_lbrace_com : pegtl::seq<ws, key_lbrace, ws> {};
    struct com_key_value_com : pegtl::seq<ws, key_value, ws> {};


// -----------------------
// Names
// -----------------------
    struct name : pegtl::plus<pegtl::sor<
        pegtl::alnum,
        pegtl::one<'_', '-','\''>,
        pegtl::range<0x80, 0xFF>  // ANSI extended characters
        >> {};

    // Match anything inside quotes except the quote itself
    struct quoted_content : pegtl::plus<pegtl::not_one<'"'>> {};

    struct complex_name : pegtl::seq<
        pegtl::one<'"'>,
        quoted_content,
        pegtl::one<'"'>
    > {
    };

    struct eval_name : pegtl::sor<complex_name, name> {};  // Try quoted names FIRST, then regular names



    // -----------------------
    // Blocks
    // -----------------------
    struct block;
    struct block_body : pegtl::star<pegtl::sor<
        block,                         // nested block
        com_key_value_com,             // normal key-value
        pegtl::seq<ws, eval_name, ws>  // individual names
        >> {};

    struct block : pegtl::seq<com_key_lbrace_com, block_body, ws, rbrace, ws> {};

    struct grammar : pegtl::must<pegtl::plus<block>> {};


    // Actions
    template<typename Rule> struct action : pegtl::nothing<Rule> {};
    template<>
    struct action<eval_name> {
        template<typename Input>
        static void apply(const Input& in, Eu4MainParser& parser) {
            std::string text = in.string();

            parser.intToStringID.push_back(text);
            int intID = ++parser.counterID;
            parser.stringToIntID[text] = intID;
            Node& child = parser.nodeStack.back()->children[intID];
            parser.nodeStack.back()->children[intID].value = text;

        }
    };
    template<>
    struct action<key_lbrace> {
        template<typename Input>
        static void apply(const Input& in, Eu4MainParser& parser) {
            std::string text = in.string();

            auto pos = text.find('=');
            if (pos != std::string::npos) {
                std::string key = text.substr(0, pos);

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
        static void apply(const Input& in, Eu4MainParser& parser) {
            std::string text = in.string();

            auto pos = text.find('=');
            if (pos != std::string::npos) {
                std::string key = text.substr(0, pos);
                std::string val = text.substr(pos + 1);

                key.erase(0, key.find_first_not_of(" \t\n\r"));
                key.erase(key.find_last_not_of(" \t\n\r") + 1);

                val.erase(0, val.find_first_not_of(" \t\n\r"));
                val.erase(val.find_last_not_of(" \t\n\r") + 1);


                parser.intToStringID.push_back(key);
                int intID = ++parser.counterID;
                parser.stringToIntID[key] = intID;
                Node& child = parser.nodeStack.back()->children[intID];
                parser.nodeStack.back()->children[intID].value = val;


            }
        }
    };

    template<>
    struct action<rbrace> {
        template<typename Input>
        static void apply(const Input& in, Eu4MainParser& parser) {
            parser.nodeStack.pop_back();
        }
    };
private:

};

#endif // EU4_MAIN_PARSER_H
