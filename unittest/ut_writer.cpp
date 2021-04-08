#include <gtest/gtest.h>

#include <time/time.hpp>

#include "source/lib/writer.hpp"

using namespace std::chrono_literals;

struct ut_writer : public testing::Test {
    void SetUp() override { std::filesystem::remove("ut_writer.dmp"); }
};

TEST_F(ut_writer, create) {
    miu::mktdump::writer writer { "ut_writer" };
    EXPECT_TRUE(std::filesystem::exists("ut_writer.dmp"));
}

TEST_F(ut_writer, quote) {
    auto local_time = miu::time::now();

    {
        miu::mktdump::writer writer { "ut_writer" };

        auto symbol = miu::ref::symbol { "NYSE/STOCK/abc" };
        auto quote  = miu::mkt::quote {};
        quote.set_bid(1.0);
        quote.set_ask(1.1);

        writer.write(1, local_time, symbol, &quote, nullptr);
        writer.write(2, local_time + 1s, symbol, &quote, nullptr);
    }

    std::ifstream ss { "ut_writer.dmp" };

    uint32_t index;
    miu::time::stamp time;
    miu::ref::symbol symbol;
    miu::mkt::quote quote;

    //
    ss.read((char*)&index, sizeof(index));
    EXPECT_EQ(1U, index);

    ss.read((char*)&time, sizeof(time));
    EXPECT_EQ(local_time, time);

    ss.read((char*)&symbol, sizeof(symbol));
    EXPECT_EQ(miu::ref::symbol { "NYSE/STOCK/abc" }, symbol);

    ss.read((char*)&quote, sizeof(quote));
    EXPECT_EQ(1.0, quote.bid());
    EXPECT_EQ(1.1, quote.ask());

    //
    ss.read((char*)&index, sizeof(index));
    EXPECT_EQ(2U, index);

    ss.read((char*)&time, sizeof(time));
    EXPECT_EQ(local_time + 1s, time);

    ss.read((char*)&symbol, sizeof(symbol));
    ss.read((char*)&quote, sizeof(quote));
}

TEST_F(ut_writer, depth) {
    auto local_time = miu::time::now();

    {
        miu::mktdump::writer writer { "ut_writer" };

        auto symbol = miu::ref::symbol { "NYSE/STOCK/abc" };

        auto quote = miu::mkt::quote {};
        quote.set_depth_id(1);

        auto depth = miu::mkt::depth {};
        depth.set_bid(1, 1.2);
        depth.set_ask(2, 2.0);

        writer.write(1, local_time, symbol, &quote, &depth);

        quote.set_depth_id(0);
        writer.write(2, local_time, symbol, &quote, &depth);
    }

    std::ifstream ss { "ut_writer.dmp" };

    uint32_t index;
    miu::time::stamp time;
    miu::ref::symbol symbol;
    miu::mkt::quote quote;
    miu::mkt::depth depth;

    //
    ss.read((char*)&index, sizeof(index));
    ss.read((char*)&time, sizeof(time));
    ss.read((char*)&symbol, sizeof(symbol));
    ss.read((char*)&quote, sizeof(quote));
    ss.read((char*)&depth, sizeof(depth));
    EXPECT_EQ(1.2, depth.bid(1));
    EXPECT_EQ(2.0, depth.ask(2));
}

