#include <gtest/gtest.h>
#include "pseudo.hpp"

using namespace pseudo;

void verify(int slice, initializer_list<Square> expected)
{
    int head = heads[slice];
    int tail = tails[slice];

    ASSERT_EQ(tail - head, expected.size());

    int i = head;
    for (Square target: expected)
    {
        EXPECT_EQ(moves[i++], target);
    }
}

TEST(PseudoTest, Crawl)
{
    verify(CrawlShift(72, King),   {87, 86, 85, 73, 71, 59, 58, 57});
    verify(CrawlShift(72, Knight), {101, 99, 88, 84, 60, 56, 45, 43});
}

TEST(PseudoTest, Slide)
{
    verify(SlideShift(72, 0), {87, 102, 117, 132});
    verify(SlideShift(72, 1), {86, 100, 114, 128, 139, 147, 155});
    verify(SlideShift(72, 2), {85, 98, 111, 124});
    verify(SlideShift(72, 3), {73, 74, 75, 76, 77, 78, 79});
    verify(SlideShift(72, 4), {71, 70, 69, 68, 67, 66});
    verify(SlideShift(72, 5), {59, 46, 33, 23});
    verify(SlideShift(72, 6), {58, 44, 30, 19, 11, 3});
    verify(SlideShift(72, 7), {57, 42, 27});    
}

TEST(PseudoTest, Push)
{
    verify(PushShift(72, Red),    {86});
    verify(PushShift(72, Blue),   {73});
    verify(PushShift(72, Yellow), {58});
    verify(PushShift(72, Green),  {71});
}

TEST(PseudoTest, Advance)
{
    verify(AdvanceShift(72, Red),    {87, 85});
    verify(AdvanceShift(72, Blue),   {87, 59});
    verify(AdvanceShift(72, Yellow), {59, 57});
    verify(AdvanceShift(72, Green),  {85, 57}); 
}

TEST(PseudoTest, Attack)
{
    verify(AttackShift(72, Red),    {59, 57});
    verify(AttackShift(72, Blue),   {85, 57}); 
    verify(AttackShift(72, Yellow), {87, 85});
    verify(AttackShift(72, Green),  {87, 59});
}