#pragma once
#include <core/core.h>
#include <math/vec/vec.h>
#include <shape/shape.h>
#include <util/helper_macros.h>
#include <zconfig/zconfig.h>

namespace zee {
	class application;
	class application {
		ZEE_DEFINE_REMOVE_MOVE_AND_COPY_FUNCTION(application)
	public:
		static application& get() noexcept;

	public:
		struct config_data {
			static const TCHAR* config_dir() noexcept { return TEXT("app"); }
			static const TCHAR* config_name() noexcept { return TEXT("app"); }

			tstring app_name = TEXT("my app");
			math::vec2i window_position = { 0, 0 };
			math::vec2i window_size = { 1024, 1024 };
			math::vec2i client_size = { 1024, 1024 };
			int frame_count = 60;
			bool vsync = true;
			bool maximize = true;
		};

	protected:
		application() = default;
		~application() { /*do nothing..*/ }

		void clear() {
			window_handle_ = NULL;
			instance_handle_ = NULL;
		}

	public:
		const zee::basic_config<config_data>& app_config() const noexcept { return zee::basic_config<config_data>::get(); }
		const config_data& config() const noexcept { return app_config().data(); }

	protected:
		zee::basic_config<config_data>& app_config_() noexcept { return zee::basic_config<config_data>::get(); }
		config_data& config_() noexcept { return app_config_().data(); }

	public:
		bool is_started() const noexcept { return is_started_; }
		template<typename HandleT = handle_t> HandleT window_handle() const noexcept { return (HandleT)window_handle_; }
		template<typename HandleT = handle_t> HandleT instance_handle() const noexcept { return (HandleT)instance_handle_; }

		void change_window_size(const math::vec2i& new_window_size) noexcept;
		void change_window_position(const math::vec2i& new_window_position) noexcept;
		
	protected:
		bool is_started_ = false;
		handle_t window_handle_ = NULL;
		handle_t instance_handle_ = NULL;
	};

}//namespace zee

template<>
struct nlohmann::adl_serializer<zee::application::config_data> {
	typedef zee::application::config_data data_type;

	static void to_json(json& j, const data_type& c) {
		j =
		{
			{"app_name"			, c.app_name},
			{"window_position"	, c.window_position},
			{"window_size"		, c.window_size},
			{"client_size"		, c.client_size},
			{"frame_count"		, c.frame_count},
			{"vsync"			, c.vsync},
			{"maximize"			, c.maximize}
		};
	}

	static void from_json(const json& j, data_type& c) {
		using namespace zee;
		bool is_valid = false;
		const auto list = json_helper::safety_get_members(is_valid, j
			, "app_name"
			, "window_position"
			, "window_size"
			, "client_size"
			, "frame_count"
			, "vsync"
			, "maximize"
		);

		if (is_valid) {
			c.app_name			= to_tstring(list[0].value());
			c.window_position	= list[1].value();
			c.window_size		= list[2].value();
			c.client_size		= list[3].value();
			c.frame_count		= list[4].value();
			c.vsync				= list[5].value();
			c.maximize			= list[6].value();
		} else {
			c = data_type{};
		}
	}
};