
#include <vector>
#include <string>
#include <stdio.h>
#include <tgbot/tgbot.h>
#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "queue.hpp"
#include "test.h"
#include "server.h"

using namespace std;

// int main(int argc, char **argv) {
//   ::testing::InitGoogleMock(&argc, argv);
//   return RUN_ALL_TESTS();
// }

int main(int argc, char *argv[]) {
    Server server;
    server.start();

  return 0;
}

