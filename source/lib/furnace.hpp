#pragma once

#include <svc/furnace.hpp>

namespace miu::mktdump {

class furnace : public svc::furnace {
  private:
    void ignite(cfg::settings const&) override {}

    void quench() override {}
};

}    // namespace miu::mktdump
