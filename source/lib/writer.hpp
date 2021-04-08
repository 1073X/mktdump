#pragma once

#include <com/strcat.hpp>
#include <filesystem>
#include <fstream>
#include <mkt/topic.hpp>

namespace miu::mktdump {

class writer {
  public:
    explicit writer(std::string_view name)
        : _ss { com::strcat(name, "dmp").str(), std::ios_base::app } {}

    writer(writer&& another)
        : _ss(std::move(another._ss)) {}

    auto& operator=(writer&& another) {
        std::swap(another._ss, _ss);
        return *this;
    }

    auto write(uint32_t index,
               time::stamp local_time,
               ref::symbol symbol,
               mkt::quote const* quote,
               mkt::depth const* depth) {
        _ss.write((const char*)&index, sizeof(index));
        _ss.write((const char*)&local_time, sizeof(local_time));
        _ss.write((const char*)&symbol, sizeof(symbol));
        _ss.write((const char*)quote, sizeof(mkt::quote));
        if (quote->depth_id()) {
            _ss.write((const char*)depth, sizeof(mkt::depth));
        }
    }

    auto write(mkt::topic const& topic) {
        assert(topic);
        write(topic.index(), topic.local_time(), topic.symbol(), topic.quote(), topic.depth());
    }

  private:
    std::ofstream _ss;
};

}    // namespace miu::mktdump
