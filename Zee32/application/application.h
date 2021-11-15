#pragma once
#include "core/core.h"
#include "core/delegate.h"
#include "math/math.h"

namespace zee {
	//class device_context_dynamic;
	class application_impl;
	
	class application final {
		friend class zee::application_impl;

	public:
		struct global_delegates {
			static multicast_delegate<void(void)> on_window_created;
			static multicast_delegate<void(const math::vec2& /*new_size*/)> on_window_size_changed;
			static multicast_delegate<void(void)> on_window_destoried;
		};

	private:
		application();

		void on_window_create(handle_t new_window_handle, handle_t new_instance_handle) noexcept;
		void on_window_size_changed(const math::vec2& new_size) noexcept;
		void on_window_destory() noexcept;

	public:
		~application();

		void tick(float deltatime);

		math::vec2 get_window_client_size() const;

	public:
		handle_t window_handle() const noexcept;
		handle_t instance_handle() const noexcept;

	private:
		handle_t window_handle_ = NULL;
		handle_t instance_handle_ = NULL;

		//std::unique_ptr<device_context_dynamic> back_dc_;

	private:
		static std::unique_ptr<application> app_;

	public:
		static bool is_application_started();
		static application& get();
	};
}