#include "gtest/gtest.h"
#include "../fsa/fsa.h"

#include <boost/functional/hash/hash.hpp>

// fixture class
class FsaTest : public ::testing::Test {
protected:
    // virtual void SetUp() {}

    // virtual void TearDown() {}

    const int offset = 1; // for the end state
    //Arc(unsigned char label, int target, bool is_final, bool is_last);
    const std::vector<Arc> arcs = {
        Arc('E',        1337, true,  true), // end state
        Arc('c', -1 + offset, true,  true),
        Arc('k', -1 + offset, true,  true),
        Arc('b',  0 + offset, false, false),
        Arc('z',  1 + offset, false, true),
        Arc('y', -1 + offset, true,  false),
        Arc('z',  0 + offset, false, true),
        Arc('a',  2 + offset, true,  false), // root
        Arc('z',  4 + offset, true,  true) };

    const int root_index = 6 + offset;

    Fsa automaton = Fsa(arcs, root_index);

    const std::vector<std::string> language = {
        "a",
        "abc",
        "azk",
        "z",
        "zy",
        "zzc",
    };
};


TEST_F(FsaTest, OutputsExactlyTheInputLanguage) {
    /*
    for (const auto& word : language)
        EXPECT_TRUE(automaton.accepts(word));*/
    std::stringstream lang_ss, fsa_ss;
    
    automaton.printAllWords(fsa_ss);
    for (const auto & str : language) {
        lang_ss << str << '\n';
    }
    EXPECT_EQ(lang_ss.str(), fsa_ss.str());
}

TEST_F(FsaTest, AcceptsWordsWithinTheLanguage) {

    for (const auto& word : language)
        EXPECT_TRUE(automaton.accepts(word));

}

TEST_F(FsaTest, RejectsWordsNotInTheLanguage) {
    // list of all one, two and three letter words
    std::vector<std::string> word_list;
    std::string temp = "a";
    for (char a = 'a'; a <= 'z'; a++) {
        temp.resize(1);
        temp[0] = a;
        word_list.push_back(temp);
        for (char b = 'a'; b <= 'z'; b++) {
            temp.resize(2);
            temp[1] = b;
            word_list.push_back(temp);
            temp.resize(3);
            for (char c = 'a'; c <= 'z'; c++) {
                temp[2] = c;
                word_list.push_back(temp);
            }
        }
    }
    
    // remove words present in the language from the list
    std::vector<std::string> t;
    std::set_difference(std::make_move_iterator(word_list.begin()),
        std::make_move_iterator(word_list.end()),
        language.begin(), language.end(),
        std::inserter(t, t.begin()));
    word_list.swap(t);
    
    // number of one, two and three letter words
    EXPECT_EQ(26*(1 + 26*(1 + 26)) - language.size(), word_list.size());

    for (auto const & word : word_list)
        EXPECT_FALSE(automaton.accepts(word));

    EXPECT_FALSE(automaton.accepts("xyzz"));
    EXPECT_FALSE(automaton.accepts("xyzz"));
    EXPECT_FALSE(automaton.accepts("zzca"));
    EXPECT_FALSE(automaton.accepts("abcdff"));
    EXPECT_FALSE(automaton.accepts("aazkqpk"));
    
}

TEST(FsaConstructorTests, ThrowsOnInvalidRootArg) {
    const std::vector<Arc> arcs = {
        Arc('E', 1337, true , true),
        Arc('x', 10  , true , false),
        Arc('Z', 1000, false, true),
        Arc('c', 1337, false, false)
    };

    ASSERT_THROW(Fsa(arcs, -2), std::invalid_argument);
    ASSERT_THROW(Fsa(arcs, -1), std::invalid_argument);

    ASSERT_NO_THROW(Fsa(arcs, 0));
    ASSERT_NO_THROW(Fsa(arcs, 1));
    ASSERT_NO_THROW(Fsa(arcs, 2));
    ASSERT_NO_THROW(Fsa(arcs, 3));
    
    ASSERT_THROW(Fsa(arcs, 4), std::invalid_argument);
    ASSERT_THROW(Fsa(arcs, 5), std::invalid_argument);

}

