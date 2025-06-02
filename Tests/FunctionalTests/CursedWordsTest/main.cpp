#include <gtest/gtest.h>

int main(int argc, char **argv) {
    std::cout.setf(std::ios::unitbuf);
    
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}