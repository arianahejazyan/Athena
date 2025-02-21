#include <gtest/gtest.h>
#include "definitions.h"

using namespace definitions;

class DefinitionsTest: public ::testing::Test
{
    protected:

};

TEST_F(DefinitionsTest, Rank)
{
    EXPECT_EQ(rank(0), 0);
    EXPECT_EQ(rank(10), 1);
    EXPECT_EQ(rank(60), 5);
    EXPECT_EQ(rank(140), 11);
    EXPECT_EQ(rank(159), 13);
}

TEST_F(DefinitionsTest, File)
{
    EXPECT_EQ(file(0), 3);
    EXPECT_EQ(file(10), 5);
    EXPECT_EQ(file(60), 8);
    EXPECT_EQ(file(140), 7);
    EXPECT_EQ(file(159), 10);
}

TEST_F(DefinitionsTest, Spot)
{
    EXPECT_EQ(spot(0, 3), 0);
    EXPECT_EQ(spot(1, 5), 10);
    EXPECT_EQ(spot(5, 8), 60);
    EXPECT_EQ(spot(11, 7), 140);
    EXPECT_EQ(spot(13, 10), 159);
}

TEST_F(DefinitionsTest, ValidSquare)
{
    EXPECT_TRUE(valid_square(3,0));
    EXPECT_TRUE(valid_square(6,10));
    EXPECT_TRUE(valid_square(13,10));

    EXPECT_FALSE(valid_square(0,0));
    EXPECT_FALSE(valid_square(-1,5));
    EXPECT_FALSE(valid_square(15,8));
}