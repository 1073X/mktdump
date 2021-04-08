#pragma once

#include <log/log.hpp>
#include <mkt/source.hpp>

namespace miu::mktdump {

class source {
  public:
    source(std::string_view name)
        : _raw { name }
        , _topics(_raw.num_of_instrument(), mkt::topic {}) {}

    auto seek() {
        for (auto i = 0U; i < _raw.num_of_instrument(); i++) {
            if (_topics[i]) {
                continue;
            }

            _topics[i] = _raw.get(i);

            if (_topics[i]) {
                log::debug(+"mktdump NEW TOP",
                           _raw.name(),
                           +"IDX:",
                           _topics[i].index(),
                           _topics[i].id(),
                           _topics[i].symbol());
            }
        }
    }

    template<typename T>
    auto dump(T* t) {
        for (auto& topic : _topics) {
            if (topic && topic.refresh() > 0) {
                t->write(topic);
            }
        }
    }

  private:
    mkt::source _raw;
    std::vector<mkt::topic> _topics;
};

}    // namespace miu::mktdump
