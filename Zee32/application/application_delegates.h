#pragma once
#include "core/delegate.h"
#include "math/vec/vec.h"
#include "util/helper_macros.h"
#include <memory>

namespace zee {

	class application;
	struct application_delegates;

	struct application_delegates {
		ZEE_DEFINE_REMOVE_MOVE_AND_COPY_FUNCTION(application_delegates)

	private:
		application_delegates();
		static application_delegates& inst() noexcept;

	public:
		static multicast_delegate<void(const std::weak_ptr<application>&)>& on_started() noexcept {
			return inst().on_started_;
		}

		static multicast_delegate<void(void)>& on_minimized() noexcept {
			return inst().on_minimized_;
		}

		static multicast_delegate<void(const math::vec2i&/*new_window_size*/)>& on_maximized() noexcept {
			return inst().on_maximized_;
		}

		static multicast_delegate<void(void)>& on_destroied() noexcept {
			return inst().on_destroied_;
		}

		static multicast_delegate<void(const math::vec2i&)>& on_window_size_changed() noexcept {
			return inst().on_window_size_changed_;
		}

	private:
		multicast_delegate<void(const std::weak_ptr<application>&)> on_started_;
		multicast_delegate<void(const math::vec2i&/*new_window_size*/)> on_window_size_changed_;
		multicast_delegate<void(void)> on_minimized_;
		multicast_delegate<void(const math::vec2i&/*new_window_size*/)> on_maximized_;
		multicast_delegate<void(void)> on_destroied_;
	};

}//namespace zee
