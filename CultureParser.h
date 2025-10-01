#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <tao/pegtl.hpp>

namespace pegtl = tao::pegtl;

class CultureParser {
public:
    void parseFile(const std::string& filePath);

private:
    static std::vector<std::string> key_stack;
    // Grammar rules
    struct identifier : pegtl::plus<pegtl::alnum> {};
    struct ws : pegtl::star<pegtl::space> {};
    struct eq : pegtl::one<'='> {};
    struct lbrace : pegtl::one<'{'>{};
    struct rbrace : pegtl::one<'}'>{};
    

    struct value; // forward declaration
    struct key_value;


    struct key : pegtl::seq<ws, identifier, ws, eq, ws>{};



    struct inner_block : pegtl::seq<
        lbrace,
        ws,
        pegtl::star<key_value>, // key-values or nested blocks
        ws,
        rbrace
    > {
    };

    struct value : pegtl::sor<identifier, inner_block> {};

    struct key_value : pegtl::seq<
        key,
        value,
        ws
    > {
    };

    struct outer_block : pegtl::seq<
        ws,
        identifier,
        ws,
        eq,
        ws,
        inner_block,
        ws
    > {
    };

    struct outer_assignment : pegtl::seq<
        pegtl::star<pegtl::seq<ws, identifier, ws, eq, ws, inner_block, ws>>
    > {
    };

    struct grammar : pegtl::must<pegtl::star<outer_block>> {};





    // Actions

    template<typename Rule> struct action : pegtl::nothing<Rule> {};

    // Print every key-value
    //template<>
    //struct action<key_value> {
    //    template<typename Input>
    //    static void apply(const Input& in, void*) {
    //        std::string text = in.string();

    //        // find '='
    //        auto pos = text.find('=');
    //        if (pos != std::string::npos) {
    //            std::string key = text.substr(0, pos);
    //            std::string val = text.substr(pos + 1);

    //            // trim spaces
    //            key.erase(0, key.find_first_not_of(" \t\n\r"));
    //            key.erase(key.find_last_not_of(" \t\n\r") + 1);

    //            val.erase(0, val.find_first_not_of(" \t\n\r"));
    //            val.erase(val.find_last_not_of(" \t\n\r") + 1);

    //            std::cout << key << " = " << val << "\n";
    //        }
    //    }
    //};

    // Every key_value (key = value) prints the full path
    template<>
    struct action<key_value> {
        template<typename Input>
        static void apply(const Input& in, void*) {
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

                // Print full path using key stack
                if (!key_stack.empty())
                    std::cout << key_stack.back() << "." << key << " = " << val << "\n";
                else
                    std::cout << key << " = " << val << "\n";
            }
        }
    };

    template<>
    struct action<outer_block> {
        template<typename Input>
        static void apply(const Input& in, void*) {
            std::string text = in.string();

            // Find '='
            auto pos = text.find('=');
            if (pos != std::string::npos) {
                std::string key = text.substr(0, pos);
                key.erase(0, key.find_first_not_of(" \t\n\r"));
                key.erase(key.find_last_not_of(" \t\n\r") + 1);

                std::cout << "Outer key: " << key << "\n";
                key_stack.push_back(key);
            }
        }
    };

    // Push key when entering a new block
    template<>
    struct action<identifier> {
        template<typename Input>
        static void apply(const Input& in, void*) {
            // Only push if the next token is a block
            // In PEGTL you might need to push from outer_assignment or inner_block
        }
    };

    // Pop key when block ends
    template<>
    struct action<rbrace> {
        template<typename Input>
        static void apply(const Input&, void*) {
            if (!key_stack.empty())
                key_stack.pop_back();
        }
    };
};
