#pragma once
#include <math/vec/vec.h>
#include <interfaces/tickable.h>
#include <memory>
#include "world.h"

namespace zee {
	class toy : public std::enable_shared_from_this<toy>, public interfaces::tickable {
	public:
		const math::vec2f& get_position() const {
			return position;
		}

		void set_position(const math::vec2f& new_position) noexcept {
			position = new_position;
		}

	protected:
		math::vec2f position;

	private:
		std::weak_ptr<world> owner_world;
	};

	template<typename ToyT>
	std::enable_if_t<std::is_base_of<toy, ToyT>::value, std::shared_ptr<ToyT>> 
		spawn_toy() {
		if (auto current_world = world::current_world().lock()) {
			current_world->
		}
	}

}//namespace zee