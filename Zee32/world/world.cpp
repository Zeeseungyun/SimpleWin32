#include "world.h"
namespace zee {
    std::weak_ptr<world> world::current_world() {

        return std::weak_ptr<world>();
    }

    void world::tick(float delta_time) {

    }

}//namespace zee 