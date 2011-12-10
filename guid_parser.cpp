//
// Copyright (C) 2011 Mateusz Loskot <mateusz@loskot.net>
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#define BOOST_SPIRIT_DEBUG 1
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_char_.hpp>
#include <boost/spirit/include/qi_char_class.hpp>
#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/qi_repeat.hpp>
#include <iostream>
#include <string>

#include "spirit_simple_trace.hpp"

namespace qi = boost::spirit::qi;

template <typename Iterator>
struct guid_grammar : qi::grammar<Iterator, std::string(), qi::ascii::space_type>
{
    guid_grammar() : guid_grammar::base_type(guid_enclosed)
    {
        using qi::char_; 
        using qi::lit;
        using qi::repeat;

        digit = char_("0-9a-fA-F");
        data1 = repeat(8)[digit];
        data2 = repeat(4)[digit];
        data3 = repeat(4)[digit];
        data4 = repeat(4)[digit] >> char_('-') >> repeat(12)[digit];
        guid = data1 >> char_('-') >> data2 >> char_('-') >> data3 >> char_('-') >> data4;
        guid_enclosed = -lit('{') >> guid >> -lit('}');

        BOOST_SPIRIT_DEBUG_NODE(guid_enclosed);
    }

    qi::rule<Iterator, std::string(), qi::ascii::space_type> guid_enclosed, guid, data1, data2, data3, data4;
    qi::rule<Iterator, char()> digit;
};

int main()
{
    std::string input("3F2504E0-4F89-11D3-9A0C-0305E82C3301");
    std::string out;

    typedef std::string::const_iterator iterator_type;
    guid_grammar<iterator_type> guid_parser;
    
    auto it = input.begin();
    auto end = input.end();
    bool r = qi::phrase_parse(it, end, guid_parser, qi::ascii::space, out);
    if (r && it == end)
    {
        std::cout << "-------------------------\n";
        std::cout << "Parsing succeeded: \n" << out << "\n";
        std::cout << "-------------------------\n";
    }
    else
    {
        std::string rest(it, end);
        std::cout << "-------------------------\n";
        std::cout << "Parsing failed\n";
        std::cout << "stopped at: \":" << rest << "\"\n";
        std::cout << "-------------------------\n";
        }

    return 0;
}