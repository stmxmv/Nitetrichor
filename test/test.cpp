/*!
* Created by Molybdenum on 8/15/21.
*
*/
#include "gtest/gtest.h"
class WNTest : public ::testing::Test {
protected:
    virtual void SetUp() override {

    }

    virtual void TearDown() override {

    }


};

namespace WN {
    int add(int a, int b) {
        return a + b;
    }
}


TEST_F(WNTest, aTest){

    EXPECT_EQ(3, WN::add(1, 2));

}
