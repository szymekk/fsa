#include "gtest/gtest.h"
#include "fsa/fsa.h"

// fixture class
class FsaTest : public ::testing::Test {
protected:
    // virtual void SetUp() {}

    // virtual void TearDown() {}

    const int offset = 1; // for the end state
    //Fsa::Arc(unsigned char label, int target, bool is_final, bool is_last);
    const std::vector<Fsa::Arc> arcs = {
        Fsa::Arc('T',        1337, true,  true), // end state
        Fsa::Arc('c', -1 + offset, true,  true),
        Fsa::Arc('k', -1 + offset, true,  true),
        Fsa::Arc('b',  0 + offset, false, false),
        Fsa::Arc('z',  1 + offset, false, true),
        Fsa::Arc('y', -1 + offset, true,  false),
        Fsa::Arc('z',  0 + offset, false, true),
        Fsa::Arc('a',  2 + offset, true,  false), // root
        Fsa::Arc('z',  4 + offset, true,  true) };

    const int root_index = 6 + offset;

    const Fsa::Fsa automaton = Fsa::Fsa(arcs, root_index);

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
    
    printAcceptedLanguage(automaton, fsa_ss);
    for (const auto & str : language) {
        lang_ss << str << '\n';
    }
    EXPECT_EQ(lang_ss.str(), fsa_ss.str());
}

TEST_F(FsaTest, OutputsANodesRightLanguage) {

    std::stringstream lang_ss, state_ss;

    const auto root_idx = automaton.root_;
    const auto arc = automaton.arcs_[root_idx + 1];
    const auto state_idx = arc.target_;
    const auto label = arc.label_;

    auto iter = automaton.getIterator();
    const auto advanced = iter.advance(label);
    EXPECT_NE(advanced, 0);
    //printRightLanguage(automaton, state_idx, state_ss);
    printRightLanguage(iter, state_ss);
    for (const auto & str : language) {
        if (1 < str.length() && label == str[0]) {
            lang_ss << str.substr(1) << '\n';
        }
    }
    EXPECT_EQ(lang_ss.str(), state_ss.str());
}

TEST_F(FsaTest, AcceptsWordsWithinTheLanguage) {
    for (const auto& word : language) {
        EXPECT_TRUE(accepts(automaton.getIterator(), word));
    }
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

    const auto language_iterator = automaton.getIterator();
    for (auto const & word : word_list) {
        EXPECT_FALSE(accepts(automaton.getIterator(), word));
    }

    EXPECT_FALSE(accepts(language_iterator, "xyzz"));
    EXPECT_FALSE(accepts(language_iterator, "xyzz"));
    EXPECT_FALSE(accepts(language_iterator, "zzca"));
    EXPECT_FALSE(accepts(language_iterator, "abcdff"));
    EXPECT_FALSE(accepts(language_iterator, "aazkqpk"));
    
}

TEST(FsaConstructorTests, ThrowsOnInvalidRootArg) {
    const std::vector<Fsa::Arc> arcs = {
        Fsa::Arc('E', 1337, true , true),
        Fsa::Arc('x', 10  , true , false),
        Fsa::Arc('Z', 1000, false, true),
        Fsa::Arc('c', 1337, false, false)
    };

    ASSERT_THROW(Fsa::Fsa(arcs, -2), std::invalid_argument);
    ASSERT_THROW(Fsa::Fsa(arcs, -1), std::invalid_argument);

    ASSERT_NO_THROW(Fsa::Fsa(arcs, 0));
    ASSERT_NO_THROW(Fsa::Fsa(arcs, 1));
    ASSERT_NO_THROW(Fsa::Fsa(arcs, 2));
    ASSERT_NO_THROW(Fsa::Fsa(arcs, 3));
    
    ASSERT_THROW(Fsa::Fsa(arcs, 4), std::invalid_argument);
    ASSERT_THROW(Fsa::Fsa(arcs, 5), std::invalid_argument);
}

