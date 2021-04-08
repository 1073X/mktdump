#pragma once

#include <filesystem>
#include <log/log.hpp>
#include <shm/tempfs.hpp>
#include <time/time.hpp>

#include "source.hpp"
#include "writer.hpp"

namespace miu::mktdump {

class seeker {
  public:
    seeker(std::string_view path)
        : _path { path } {}

    auto seek() {
        auto today = "_" + com::to_string(time::today());
        for (auto file : shm::tempfs::find(".*\\.mkt")) {
            auto src_name = file.stem().string();
            auto dmp_name = src_name + today;
            if (_writers.end() == _writers.find(src_name)) {
                auto src = source { src_name };
                auto wr  = writer { dmp_name };
                _writers.emplace(src_name, std::make_pair(std::move(src), std::move(wr)));
                log::debug(+"mktdump NEW MKT", src_name);
            }
        }
    }

    auto dump() {
        for (auto& [name, pair] : _writers) {
            pair.first.seek();
            pair.first.dump(&pair.second);
        }
    }

  private:
    std::filesystem::path _path;
    std::map<std::string, std::pair<source, writer>> _writers;
};

}    // namespace miu::mktdump
