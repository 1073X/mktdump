#include <gmock/gmock.h>

#include <stub/mkt.hpp>

#include "source/lib/source.hpp"

using testing::_;

struct ut_source : public testing::Test {
    MOCK_METHOD(void, write, (miu::mkt::topic const&));

    miu::mkt::stub stub;
    miu::mktdump::source source { stub.marker() };
};

TEST_F(ut_source, dump) {
    stub.subscribe(2);

    source.seek();

    source.dump(this);

    stub.renew(2, [](auto) {});
    EXPECT_CALL(*this, write(_));
    source.dump(this);
}
