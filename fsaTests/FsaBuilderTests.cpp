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
    FsaBuilder builder;
    builder.build(in_ss);
    Fsa automaton = builder.getFsa();

    std::stringstream out_ss;
    automaton.printAllWords(out_ss);

    EXPECT_EQ(in_ss.str(), out_ss.str());
}
