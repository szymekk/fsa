#include "gtest/gtest.h"
#include "../fsa/arc.h"

TEST(ArcHashTests, SameHashForTheSameArc) {
    Fsa::Arc first('c', 11, false, true);
    Fsa::Arc second('c', 11, false, true);
    EXPECT_EQ(hash_value(first), hash_value(second));
}

TEST(ArcHashTests, SameHashForUnequalIsLast) {
    Fsa::Arc is_last('c', 11, false, true);
    Fsa::Arc is_not_last('c', 11, false, false);
    EXPECT_EQ(hash_value(is_last), hash_value(is_not_last));
}

TEST(ArcHashTests, DifferentHashForDifferentArc) {
    Fsa::Arc first = { 'a', 99, false, true };
    Fsa::Arc second = { 'X', 99, false, true };
    EXPECT_NE(hash_value(first), hash_value(second));

    first = { 'a', 99, false, true };
    second = { 'a', 10, false, true };
    EXPECT_NE(hash_value(first), hash_value(second));

    first = { 'a', 99, false, true };
    second = { 'a', 99, true, true };
    EXPECT_NE(hash_value(first), hash_value(second));
}