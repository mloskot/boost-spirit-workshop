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
#include <boost/spirit/include/qi_raw.hpp>
#include <boost/spirit/include/qi_repeat.hpp>
#include <iostream>
#include <string>
namespace qi = boost::spirit::qi;

template <typename T, typename P>
void my_test_parse(T begin, T end, P const& p)
{
    std::string out;
    bool r = qi::parse(begin, end, p, out);
    if (r && begin == end)
    {
        std::cout << "-------------------------\n";
        std::cout << "Parsing succeeded: \n" << out << "\n";
        std::cout << "-------------------------\n";
    }
    else
    {
        std::string rest(begin, end);
        std::cout << "-------------------------\n";
        std::cout << "Parsing failed\n";
        std::cout << "stopped at: \":" << rest << "\"\n";
        std::cout << "-------------------------\n";
    }
}


template <typename T, typename P>
void my_test_phrase_parse(T begin, T end, P const& p)
{
    std::string out;
    bool r = qi::phrase_parse(begin, end, p, qi::ascii::space, out);
    if (r && begin == end)
    {
        std::cout << "-------------------------\n";
        std::cout << "Parsing succeeded: \n" << out << "\n";
        std::cout << "-------------------------\n";
    }
    else
    {
        std::string rest(begin, end);
        std::cout << "-------------------------\n";
        std::cout << "Parsing failed\n";
        std::cout << "stopped at: \":" << rest << "\"\n";
        std::cout << "-------------------------\n";
    }
}

int main()
{
    using qi::lit;
    using qi::char_;
    using qi::alpha;
    using qi::int_;
    using qi::double_;
    using qi::repeat;
 
    typedef std::string::const_iterator const_iterator;
    typedef qi::rule<const_iterator, std::string()> rule_type;
    rule_type guid_delim = char_('-');
    rule_type guid_digit = char_("0-9a-fA-F");
    rule_type guid_data1 = repeat(8)[guid_digit];
    rule_type guid_data2 = repeat(4)[guid_digit];
    rule_type guid_data3 = repeat(4)[guid_digit];
    rule_type guid_data4 = repeat(4)[guid_digit] >> guid_delim >> repeat(12)[guid_digit];
    rule_type guid_data = guid_data1 >> guid_delim >> guid_data2 >> guid_delim >> guid_data3 >> guid_delim >> guid_data4;
    rule_type guid = -char_('{') >> guid_data >> -char_('}');

    rule_type name = qi::char_("a-zA-Z_") >> *qi::char_("a-zA-Z_");
    rule_type op = char_('+') | char_('-') | char_('*') | char_('/');
    rule_type term = guid | (qi::raw[qi::int_] > qi::omit[!qi::char_("a-zA-Z")]);
    rule_type expression = term >> *(op >> term);
    rule_type assignment = name >> qi::char_('=') >> expression;
    
    BOOST_SPIRIT_DEBUG_NODE(assignment);
 
    std::string input;
    input = "A=5-8";
    std::cout << "======" << input << "============================================\n";
    my_test_parse(input.cbegin(), input.cend(), assignment);
    std::cout << "======" << input << "============================================\n";
    my_test_phrase_parse(input.cbegin(), input.cend(), assignment);

    input = "A=5+F52504E0-4F89-11D3-9A0C-0305E82C3301";
    std::cout << "======" << input << "============================================\n";
    my_test_parse(input.cbegin(), input.cend(), assignment);
    std::cout << "======" << input << "============================================\n";
    my_test_phrase_parse(input.cbegin(), input.cend(), assignment);

    //input = "Field1 = 3F2504E0-4F89-11D3-9A0C-0305E82C3301";
    //std::cout << "======" << input << "============================================\n";
    //test(input.cbegin(), input.cend(), p);

    return 0;
}