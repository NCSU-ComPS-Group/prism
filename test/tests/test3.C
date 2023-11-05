#include <gtest/gtest.h>

class MyObject {
public:
    bool isEqual(const MyObject& other) const {
        // Your logic to compare two objects
        return true;  // For the sake of this example, always return true
    }
};

TEST(ObjectEqualityTest, CheckEquality) {
    MyObject obj1;
    MyObject obj2;

    EXPECT_TRUE(obj1.isEqual(obj2));
}
