#include "test.hpp"

void ParserTest::SetUp() {

}

void ParserTest::TearDown() {

}

TEST_F(ParserTest, Size_of_one_argument) {
    Parser parser{{"-token"}};
    auto str_parse = parser.parse_string("-token +24rf$dsvcx");
    EXPECT_EQ(str_parse.size(), 1);
}

TEST_F(ParserTest, Size_of_some_arguments) {
    Parser parser{{"-token", "-list", "-number"}};
    auto str_parse = parser.parse_string("-token 23fsf3 -list se_df35=_14 -number +24rf$dsvcx");
    EXPECT_EQ(str_parse.size(), 3);
}

TEST_F(ParserTest, ExeptionInvalidArgumentsString_1) {
    Parser parser{{"-token", "-list", "-number"}};
    try {
        ASSERT_THROW(parser.parse_string("-token-list-number"), std::invalid_argument);
    } catch (const std::invalid_argument& e) {
        ASSERT_STREQ("Unknow tokens: -token-list-number", e.what());
    }
}

TEST_F(ParserTest, ExeptionInvalidArgumentsString_2) {
    Parser parser{{"-token", "-list", "-number"}};
    try {
        ASSERT_THROW(parser.parse_string("-token -list #4lkdsd"), std::invalid_argument);
    } catch (const std::invalid_argument& e) {
        ASSERT_STREQ("Empty token: -token", e.what());
    }
}

TEST_F(ParserTest, ExeptionInvalidArgumentsString_3) {
    Parser parser{{"-token", "-list", "-number"}};
    try {
        ASSERT_THROW(parser.parse_string("-token 23f84 -list #4lkdsd -number ds234 -key 7dsf023"), std::invalid_argument);
    } catch (const std::invalid_argument& e) {
        ASSERT_STREQ("Unknow tokens: -key", e.what());
    }
}

TEST_F(ParserTest, ExeptionInvalidArgumentsString_4) {
    Parser parser{{"-token", "-list"}};
    try {
        ASSERT_THROW(parser.parse_string("-token 23f84 -list #4lkdsd -number ds234"), std::invalid_argument);
    } catch (const std::invalid_argument& e) {
        ASSERT_STREQ("Unknow tokens: -number", e.what());
    }
}

TEST_F(ParserTest, ExeptionInvalidArgumentsString_5) {
    Parser parser{{"-token", "-list"}};
    try {
        ASSERT_THROW(parser.parse_string("23f84 -list #4lkdsd -number ds234"), std::invalid_argument);
    } catch (const std::invalid_argument& e) {
        ASSERT_STREQ("Unknow tokens: 23f84", e.what());
    }
}

TEST_F(ParserTest, ExeptionInvalidArgumentsString_6) {
    Parser parser{{"-token", "-list"}};
    try {
        ASSERT_THROW(parser.parse_string(" token 23f84 -list #4lkdsd -number ds234"), std::invalid_argument);
    } catch (const std::invalid_argument& e) {
        ASSERT_STREQ("Unknow tokens: ", e.what());
    }
}

TEST_F(ParserTest, ExeptionInvalidArgumentsString_7) {
    Parser parser{{"-token", "-list"}};
    try {
        ASSERT_THROW(parser.parse_string("-token 23f84 -list #4lk dsd -number ds234"), std::invalid_argument);
    } catch (const std::invalid_argument& e) {
        ASSERT_STREQ("Unknow tokens: dsd", e.what());
    }
}

TEST_F(ParserTest, EqualOfArguments) {
    Parser parser{{"-token", "-list", "-number"}};
    auto map_of_tokens = parser.parse_string("-token 3jfd0s34f_fdsf -list 74322342 -number _234kfsfe");
    std::unordered_map<std::string, std::string> map_of_compare{{"-token", "3jfd0s34f_fdsf"},{"-list","74322342"},{"-number", "_234kfsfe"}};
    EXPECT_EQ(map_of_tokens, map_of_compare);
}