#pragma once
#include "core/core.h"
#include "math/vec/vec.h"
#include "util/helper_macros.h"
#include "zconfig/zconfig.h"

namespace zee {
	class application;

	class application {
		ZEE_DEFINE_REMOVE_MOVE_AND_COPY_FUNCTION(application)

	protected:
		class application_config : public config::ini_base {
		public:
			application_config();//app.ini
		};

		application();
		~application() { /*do nothing..*/ }

		void clear() {
			window_handle_ = NULL;
			instance_handle_ = NULL;
			window_size_ = math::vec2i::constants::zero;
		}

	public:
		static application& get() noexcept;

	public:
		const math::vec2i& window_size() const noexcept { return window_size_; }
		template<typename HandleT = handle_t>
		HandleT window_handle() const noexcept { return (HandleT)window_handle_; }

		template<typename HandleT = handle_t>
		HandleT instance_handle() const noexcept { return (HandleT)instance_handle_; }

		bool is_started() const noexcept { return window_handle() != nullptr; }
		
	protected:
		application_config config_;

		handle_t window_handle_ = NULL;
		handle_t instance_handle_ = NULL;
		math::vec2i window_size_;
	};

}//namespace zee