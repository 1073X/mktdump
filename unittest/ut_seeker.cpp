#include <gtest/gtest.h>

#include <fstream>
#include <log/log.hpp>
#include <mkt/reg_var_str.hpp>
#include <stub/mkt.hpp>

#include "source/lib/seeker.hpp"

struct ut_seeker : public testing::Test {
    void SetUp() override {
        miu::mkt::reg_var_str();
        miu::log::reset(miu::log::severity::DEBUG, 1024);
    }
    void TearDown() override { miu::log::dump(); }

    miu::mkt::stub stub;
};

TEST_F(ut_seeker, seek) {
    miu::mktdump::seeker seeker { "." };
    seeker.seek();

    auto name = stub.marker() + "_" + miu::com::to_string(miu::time::today());
    auto file = miu::com::strcat(name, "dmp").str();
    EXPECT_TRUE(std::filesystem::exists(file));
}

TEST_F(ut_seeker, dump) {
    {
        miu::mktdump::seeker seeker { "." };
        seeker.seek();

        stub.subscribe(1);
        seeker.dump();    // seek the subscribed topic

        stub.renew(1, [](auto quote) {
            quote->set_bid(1.0);
            quote->set_ask(2.0);
        });

        seeker.dump();
    }

    auto name = stub.marker() + "_" + miu::com::to_string(miu::time::today());
    auto file = miu::com::strcat(name, "dmp").str();
    std::ifstream ss { file };

    uint32_t index;
    miu::time::stamp time;
    miu::ref::symbol symbol;
    miu::mkt::quote quote;

    //
    ss.read((char*)&index, sizeof(index));
    EXPECT_EQ(1U, index);

    ss.read((char*)&time, sizeof(time));

    ss.read((char*)&symbol, sizeof(symbol));
    EXPECT_EQ(stub.db()->find(1).symbol(), symbol);

    ss.read((char*)&quote, sizeof(quote));
    EXPECT_EQ(1.0, quote.bid());
    EXPECT_EQ(2.0, quote.ask());
}
