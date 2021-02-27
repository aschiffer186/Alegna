#include "test_framework.h"
#include "lexer/regex_parser.h"
#include <vector>
#include <sstream>

SET_UP_TESTS()

MAKE_TEST(regex_parser_1, Tests if parser can parse simple regex)
    std::string simple_regex = "ab*";
    std::vector<char> expected = {'a', 'b', '*', '?'};

    std::istringstream in(simple_regex);
    alegna::lexer::regex::regex_parser rp(in);
    auto parsed = rp.parse_regex();
    if(parsed)
        CONTENTS_TEST(expected, parsed.value());
    PASSED()
END_TEST()

int main(int argc, char** argv)
{
    TEST_SUMMARY()
    return 0;
}