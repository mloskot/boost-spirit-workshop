// Source: http://boost-spirit.com/home/articles/doc-addendum/debugging/
#ifndef WORKSHOP_BOOST_SPIRIT_SIMPLE_TRACE_HPP
#define WORKSHOP_BOOST_SPIRIT_SIMPLE_TRACE_HPP

struct simple_trace
{
    void print_indent(int n) const
    {
        n *= BOOST_SPIRIT_DEBUG_INDENT;
        for (int i = 0; i != n; ++i)
            BOOST_SPIRIT_DEBUG_OUT << ' ';
    }
 
    template <typename Iterator>
    void print_some(
        char const* tag
      , int indent
      , Iterator first, Iterator const& last) const
    {
        print_indent(indent);
        BOOST_SPIRIT_DEBUG_OUT << '<' << tag << '>';
        int const n = BOOST_SPIRIT_DEBUG_PRINT_SOME;
        for (int i = 0; first != last && i != n; ++i)
            BOOST_SPIRIT_DEBUG_OUT << *first++;
        BOOST_SPIRIT_DEBUG_OUT << "</" << tag << '>' << std::endl;
    }
 
    template <typename Iterator, typename Context, typename State>
    void operator()(
        Iterator const& first
      , Iterator const& last
      , Context const& context
      , State state
      , std::string const& rule_name) const
    {
        int static indent = 0;
 
        switch (state)
        {
            case pre_parse:
                print_indent(indent++);
                BOOST_SPIRIT_DEBUG_OUT
                    << '<' << rule_name << '>'
                    << std::endl;
                print_some("try", indent, first, last);
                break;
            case successful_parse:
                print_some("success", indent, first, last);
                print_indent(indent);
                BOOST_SPIRIT_DEBUG_OUT
                    << "<attributes>" <<
                        context.attributes << "</attributes>";
                if (!fusion::empty(context.locals))
                    BOOST_SPIRIT_DEBUG_OUT << "<locals>"
                        << context.locals << "</locals>";
                BOOST_SPIRIT_DEBUG_OUT << std::endl;
                print_indent(--indent);
                BOOST_SPIRIT_DEBUG_OUT << "</" << rule_name << '>'
                    << std::endl;
                break;
            case failed_parse:
                print_indent(indent);
                BOOST_SPIRIT_DEBUG_OUT
                    << "<fail/>" << std::endl;
                print_indent(--indent);
                BOOST_SPIRIT_DEBUG_OUT << "</" << rule_name << '>'
                    << std::endl;
                break;
        }
    }
};

#endif
