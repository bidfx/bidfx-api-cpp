/**
 * Copyright 2018 2018 BidFX Systems Ltd. All rights reserved.
 */

#include "include/price/price_field.h"
#include "gtest/gtest.h"

namespace bidfx_public_api::price::pixie
{
using bidfx_public_api::price::PriceField;

TEST(PriceFieldTest, TestString)
{
    std::string value = "Hello, World!";
    PriceField field = PriceField(value);
    EXPECT_EQ(PriceField::STRING, field.GetType());
    EXPECT_EQ(value, field.GetString());
    EXPECT_THROW(field.GetInt(), std::logic_error);
    EXPECT_THROW(field.GetLong(), std::logic_error);
    EXPECT_THROW(field.GetDouble(), std::logic_error);
}

TEST(PriceFieldTest, TestInt)
{
    int32_t value = 42;
    PriceField field = PriceField(value);
    EXPECT_EQ(PriceField::INTEGER, field.GetType());
    EXPECT_EQ(value, field.GetInt());
    EXPECT_THROW(field.GetLong(), std::logic_error);
    EXPECT_THROW(field.GetDouble(), std::logic_error);
    EXPECT_THROW(field.GetString(), std::logic_error);
}

TEST(PriceFieldTest, TestLong)
{
    int64_t value = 575000333223;
    PriceField field = PriceField(value);
    EXPECT_EQ(PriceField::LONG, field.GetType());
    EXPECT_EQ(value, field.GetLong());
    EXPECT_THROW(field.GetInt(), std::logic_error);
    EXPECT_THROW(field.GetDouble(), std::logic_error);
    EXPECT_THROW(field.GetString(), std::logic_error);
}

TEST(PriceFieldTest, TestDouble)
{
    double value = 200.47229;
    PriceField field = PriceField(value);
    EXPECT_EQ(PriceField::DOUBLE, field.GetType());
    EXPECT_EQ(value, field.GetDouble());
    EXPECT_THROW(field.GetInt(), std::logic_error);
    EXPECT_THROW(field.GetLong(), std::logic_error);
    EXPECT_THROW(field.GetString(), std::logic_error);
}

TEST(PriceFieldTest, TestNothing)
{
    PriceField field = PriceField();
    EXPECT_EQ(PriceField::NOTHING, field.GetType());
    EXPECT_THROW(field.GetInt(), std::logic_error);
    EXPECT_THROW(field.GetLong(), std::logic_error);
    EXPECT_THROW(field.GetDouble(), std::logic_error);
    EXPECT_THROW(field.GetString(), std::logic_error);
}

TEST(PriceFieldTest, TestOStreamInteger)
{
    int32_t value = 42;
    PriceField field = PriceField(value);
    std::stringstream ss;
    ss << field;
    EXPECT_EQ("42", ss.str());
}

TEST(PriceFieldTest, TestOStreamLong)
{
    int64_t value = 575000333223;
    PriceField field = PriceField(value);
    std::stringstream ss;
    ss << field;
    EXPECT_EQ("575000333223", ss.str());
}

TEST(PriceFieldTest, TestOStreamDouble)
{
    double value = 200.47229;
    PriceField field = PriceField(value);
    std::stringstream ss;
    ss << std::fixed << std::setprecision(6) << field;
    EXPECT_EQ("200.472290", ss.str());
}

TEST(PriceFieldTest, TestOStreamString)
{
    std::string value = "Hello, World!";
    PriceField field = PriceField(value);
    std::stringstream ss;
    ss << field;
    EXPECT_EQ(value, ss.str());
}
}