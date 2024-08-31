#ifndef TEST_H
#define TEST_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <optional>

#include "queue.hpp"

class QueueTest : public ::testing::Test{

protected:
    void SetUp() override;

    void TearDown() override;
	
	Queue<int> queue;
};


#endif