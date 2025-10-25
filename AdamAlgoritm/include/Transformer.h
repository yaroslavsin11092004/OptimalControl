#ifndef TRANSFORMER_H
#define TRANSFORMER_H
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;
struct StringConverter {
	template<typename T>
	std::string operator()(T value) const {
		return std::to_string(value);
	}
};
template<typename Iterator>
struct Transformer : qi::grammar<Iterator, std::string(), qi::space_type> {
	private:
		qi::rule<Iterator, std::string(), qi::space_type> atomic_expression;
		qi::rule<Iterator, std::string(), qi::space_type> term;
		qi::rule<Iterator, std::string(), qi::space_type> multiplic_expr;
		qi::rule<Iterator, std::string(), qi::space_type> expression;
	public:
		Transformer() : Transformer::base_type(expression) {
			using qi::_val;
			using qi::_1;
			using qi::_2;
			atomic_expression = qi::double_[_val = phx::bind(StringConverter(), _1)] | 
			('(' >> expression >> ')')[_val = _1] | 
			(qi::no_case["sin"] >> '(' >> expression >> ')')[_val = "sin(" + _1 + ")"] | 
			(qi::no_case["cos"] >> '(' >> expression >> ')')[_val = "cos(" + _1 + ")"];
			term = qi::as_string[qi::char_("x") >> qi::char_("0-9")][_val = _1] |
			qi::as_string[qi::char_("x") >> qi::char_("0-9") >> qi::char_("1-9")][_val = _1] |
			qi::as_string[qi::char_("u") >> qi::char_("0-9")][_val = _1] | 
			qi::as_string[qi::char_("u") >> qi::char_("0-9") >> qi::char_("1-9")][_val = _1] | 
			qi::as_string[qi::string("psi") >> qi::char_("0-9")][_val = _1] | 
			qi::as_string[qi::string("psi") >> qi::char_("0-9") >> qi::char_("1-9")][_val = _1] | 
			qi::as_string[qi::char_("t")][_val = _1] | 
			('-' >> atomic_expression)[_val = "(0-" + _1 + ")"];
			multiplic_expr = (term >> '*' >> multiplic_expr)[_val = "(" + _1 + "*" + _2 + ")"] | 
			(term >> '/' >> multiplic_expr)[_val = "(" + _1 + "/" + _2 + ")"] | 
			(term >> '^' >> multiplic_expr)[_val = "(" + _1 + "^" + _2 + ")"] | 
			term[_val = _1];
			expression = (multiplic_expr >> '+' >> expression)[_val = "(" + _1 + "+" + _2 + ")"] | 
			(multiplic_expr >> '-' >> expression)[_val = "(" + _1 + "-" + _2 + ")"] | 
			multiplic_expr[_val = _1];
		}
};
inline std::string transform_expression(std::string& input, Transformer<std::string::const_iterator>& transformer) {
	std::string result;
	std::string::const_iterator begin = input.begin();
	std::string::const_iterator end = input.end();
	bool ok = qi::phrase_parse(begin, end, transformer, qi::space, result);
	if (ok && begin == end) 
		return result;
	else throw std::runtime_error("Error of parsing row!");
}
#endif
