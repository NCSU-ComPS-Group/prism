#include <gtest/gtest.h>

int add(int a, int b) {
    return a + b;
}

TEST(IntFunctionTest, CheckAddition) {
    int expected = 5;
    int result = add(2, 3);

    EXPECT_EQ(result, expected);
}
