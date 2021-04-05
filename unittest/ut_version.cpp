#include <gtest/gtest.h>

#include <iostream>
#include <meta/info.hpp>

#include "mktdump/version.hpp"

TEST(ut_version, version) {
    std::cout << miu::mktdump::version() << std::endl;
    std::cout << miu::mktdump::build_info() << std::endl;

    std::cout << miu::meta::info() << std::endl;
}
