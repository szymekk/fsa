#include "gtest/gtest.h"
#include "../fsa/fsa_builder.h"


TEST(FsaBuilderTest, BuildsAutomaton) {
    const std::vector<std::string> input_language = {
        "a",
        "abc",
        "azk",
        "z",
        "zy",
        "zzc",
    };
    std::stringstream in_ss;
    for (auto str : input_language) {
        in_ss << str << '\n';
    }
    Fsa::FsaBuilder builder;
    builder.build(in_ss);
    Fsa::Fsa automaton = builder.getFsa();

    std::stringstream out_ss;
    Fsa::printAcceptedLanguage(automaton, out_ss);

    EXPECT_EQ(in_ss.str(), out_ss.str());
}

TEST(DISABLED_FsaBuilderTest, HandlesEmptyString) {
    std::stringstream in_ss;
    in_ss << "" << '\n';
    Fsa::FsaBuilder builder;
    builder.build(in_ss);
    Fsa::Fsa automaton = builder.getFsa();

    std::stringstream out_ss;
    Fsa::printAcceptedLanguage(automaton, out_ss);

    EXPECT_EQ(in_ss.str(), out_ss.str());
}

TEST(DISABLED_FsaBuilderTest, HandlesEmptyInput) {
    std::stringstream empty_ss;
    Fsa::FsaBuilder builder;
    builder.build(empty_ss);
    Fsa::Fsa automaton = builder.getFsa();

    std::stringstream out_ss;
    Fsa::printAcceptedLanguage(automaton, out_ss);

    EXPECT_EQ(empty_ss.str(), out_ss.str());
}
