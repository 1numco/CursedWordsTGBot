#include "test.hpp"

void SignalHandlerTest::TearDown() {
    
}

void SignalHandlerTest::test_callback() {
    std::unique_lock<std::mutex> lck(mtx);
    callback_counter++;
    lck.unlock();
    cv.notify_one();
}

TEST_F(SignalHandlerTest, CallbackIsInvokedOnSignal) {
    int expected_count = 1;
    std::vector<int> signals = {SIGINT, SIGTERM}; 
    SignalHandler signalHandler(signals, [&](){
        test_callback();
    });

    raise(SIGINT);

    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&]() { return callback_counter == expected_count; });

    ASSERT_EQ(callback_counter, 1);
}

TEST_F(SignalHandlerTest, ThrowExceptionOnSecondInitialization) {
    std::vector<int> signals = {SIGINT};

    SignalHandler signalHandler(signals, [&](){
        test_callback();
    });

    ASSERT_THROW(SignalHandler(signals, [&](){ test_callback(); }), std::runtime_error);
}

// // Тест на правильное поведение при нескольких сигналах
TEST_F(SignalHandlerTest, MultipleSignalsInvocation) {
    int expected_count = 2;
    std::vector<int> signals = {SIGINT, SIGTERM};
    SignalHandler signalHandler(signals, [&](){
        test_callback();
    });

    raise(SIGINT);
    raise(SIGTERM);
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&]() { return callback_counter == expected_count; });

    ASSERT_EQ(callback_counter, 2);
}