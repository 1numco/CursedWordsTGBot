#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <optional>
#include "parser.hpp"

class ParserTest : public ::testing::Test{
protected:
    void SetUp() override;
    void TearDown() override;	
};