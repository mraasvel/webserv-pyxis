#include "catch.hpp"
#include "parser/ContentParser.hpp"
#include <iostream>

TEST_CASE("Basic Content Parsing", "[parser]")
{
	ContentParser parser;

	std::string input = "12345";


	parser.setContentLength(100);
	while (parser.getState() == ContentParser::PARSING) {
		std::size_t index = 0;
		parser.parse(input, index);
	}

	REQUIRE(parser.getContent().length() == 100);
	REQUIRE(parser.getState() == ContentParser::COMPLETE);
}

TEST_CASE("ContentParser Error", "[parser]")
{
	ContentParser parser;
	std::string input = "12345";

	parser.setMaxSize(1000);
	parser.setContentLength(std::numeric_limits<std::size_t>::max());
	while (parser.getState() == ContentParser::PARSING) {
		std::size_t index = 0;
		parser.parse(input, index);
	}

	REQUIRE(parser.getState() == ContentParser::ERROR);
	REQUIRE(parser.getStatus() == StatusCode::PAYLOAD_TOO_LARGE);
}
