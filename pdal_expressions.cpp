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

namespace qi = boost::spirit::qi;

template <typename Iterator>
struct pdal_grammar : qi::grammar<Iterator, std::string(), qi::ascii::space_type>
{
    pdal_grammar() : pdal_grammar::base_type(assignment)
    {
        using qi::lit;
        using qi::char_;
        using qi::alpha;
        using qi::int_;
        using qi::double_;
        using qi::repeat;

        assignment = name >> char_('=') >> expression;

        expression = term   >> *(positive_ | negative_);
        term       = factor >> *(times_    | divides_);
        factor     = number | field | (char_('(') >> expression >> char_(')')) | minus_ | plus_;

        positive_ = char_('+') >> term;
        negative_ = char_('-') >> term;
        minus_   = char_('-') >> factor;
        plus_    = char_('+') >> factor;
        times_   = char_('*') >> factor;
        divides_ = char_('/') >> factor;

        number = char_("0-9") >> *char_("0-9"); // TODO: textual int_ replacement, use int_ and doble_ once semantic evaluation is added
        field = name | guid; // TODO: Add support for URN-prefixed GUIDs with have namespace identifier  urn:uuid:
                             //       or even { here Base64-based text of GUID  }

        name = char_("a-zA-Z_") >> *char_("a-zA-Z_0-9");
        guid = char_('{') >> guid_data >> char_('}');
        guid_data = guid_data1 >> guid_delim >> guid_data2 >> guid_delim >> guid_data3 >> guid_delim >> guid_data4;
        guid_data1 = repeat(8)[guid_digit];
        guid_data2 = repeat(4)[guid_digit];
        guid_data3 = repeat(4)[guid_digit];
        guid_data4 = repeat(4)[guid_digit] >> guid_delim >> repeat(12)[guid_digit];
        guid_delim = char_('-');
        guid_digit = char_("0-9a-fA-F");        
        
        BOOST_SPIRIT_DEBUG_NODE(assignment);
    }

    qi::rule<Iterator, std::string(), qi::ascii::space_type> assignment, expression, term, factor, field, name, number;
    qi::rule<Iterator, std::string(), qi::ascii::space_type> guid, guid_data, guid_delim, guid_data1, guid_data2, guid_data3, guid_data4;
    qi::rule<Iterator, char()> guid_digit;
    qi::rule<Iterator, std::string(), qi::ascii::space_type> minus_, plus_, times_, divides_, positive_, negative_;

};

template <typename T>
void test(T begin, T end, pdal_grammar<T>& pdal_parser)
{
    std::string out;
    bool r = qi::phrase_parse(begin, end, pdal_parser, qi::ascii::space, out);
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
    pdal_grammar<std::string::const_iterator> pdal_parser;

    std::string input;

    input = "Field1 = Field2 + 10";
    std::cout << "======" << input << "============================================\n";
    test(input.cbegin(), input.cend(), pdal_parser);
    
    input = "Field1=7*(Field2+10)";
    std::cout << "======" << input << "============================================\n";
    test(input.cbegin(), input.cend(), pdal_parser);

    input = "Field1 = -(3 * Field2) + (7 * (Field2 + 10)) / (Field3 - 5)";
    std::cout << "======" << input << "============================================\n";
    test(input.cbegin(), input.cend(), pdal_parser);

    input = "Field1 = {F32504E0-4F89-11D3-9A0C-0305E82C3301} + 10";
    std::cout << "======" << input << "============================================\n";
    test(input.cbegin(), input.cend(), pdal_parser);

    input = "Field1=7*({9F740E48-F56E-4105-900E-93B8E6E5ADCC}+10)";
    std::cout << "======" << input << "============================================\n";
    test(input.cbegin(), input.cend(), pdal_parser);

    input = "Field1 = -(3 * {F32504E0-4F89-11D3-9A0C-0305E82C3301}) + (7 * ({9F740E48-F56E-4105-900E-93B8E6E5ADCC} + 10)) / ({DD3534C6-C40B-4DF1-A67B-ED26387EBE84} - 5)";
    std::cout << "======" << input << "============================================\n";
    test(input.cbegin(), input.cend(), pdal_parser);


    return 0;
}