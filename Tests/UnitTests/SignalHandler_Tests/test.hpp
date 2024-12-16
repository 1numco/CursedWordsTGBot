#pragma once

#include <gtest/gtest.h>
#include <condition_variable>
#include <mutex>
#include "signalhandler.hpp"

class SignalHandlerTest : public ::testing::Test {
protected:
    void test_callback();
    void TearDown() override;
    int callback_counter = 0;
    std::mutex mtx;
    std::condition_variable cv;
};