
#include "source/lib/furnace.hpp"

namespace miu::svc {
furnace* create() {
    return new mktdump::furnace;
}
}    // namespace miu::svc
