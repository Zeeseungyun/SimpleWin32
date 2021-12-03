#pragma once
#include <core/core.h>
#include <interfaces/tickable.h>
#include <util/helper_macros.h>
#include <memory>

namespace zee {
	class toy;

	template<typename WorldClassT>
	struct world_traits {
		typedef WorldClassT world_class_type;
		static tstring world_name() { return world_class_type::world_name(); }
		static std::vector<std::shared_ptr<toy>> toys(const world_class_type& world_inst) {

		}
	};

	class world : public std::enable_shared_from_this<world> {
	public:
		static std::weak_ptr<world> current_world();

	public:
		void on_started();
		void on_destroied();
		
	protected:
		std::vector<std::shared_ptr<toy>> static_toys;
	};

namespace impl {
	class world_manager {
		ZEE_DEFINE_REMOVE_MOVE_AND_COPY_FUNCTION(world_manager);

	private:
		world_manager();
		static std::unique_ptr<world_manager> inst_;

	public:
		static world_manager& get() {
			if (!inst_) {
				inst_ = std::unique_ptr<world_manager>(new world_manager);
			}
			return *inst_;
		}

	};

}//namespace zee::impl

	template<typename WorldClassT>
	struct register_world {
		typedef WorldClassT world_class_type;
		static_assert(std::is_base_of<world, WorldClassT>::value, "");
		register_world() { 
			//impl::world_manager::get().add
		}

		static register_world inst_;
	};

}//namespace zee
