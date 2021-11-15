#include "application.h"
#include "core/core.h"
#include "core/delegate.h"
#include "core/input/keys.h"
#include "render/win32gdi/device_context.h"
#include <Windows.h>

namespace zee {
	//begin global delegates 

	multicast_delegate<void(void)> application::global_delegates::on_window_created;
	multicast_delegate<void(const math::vec2& /*new_size*/)> application::global_delegates::on_window_size_changed;
	multicast_delegate<void(void)> application::global_delegates::on_window_destoried;

	//end global delegates

	application::application() {

	}

	application::~application()	{

	}

	void application::on_window_create(handle_t new_window_handle, handle_t new_instance_handle) noexcept {
		window_handle_ = new_window_handle;
		instance_handle_ = new_instance_handle;
		//back_dc_ = std::unique_ptr<device_context_dynamic>(new device_context_dynamic());
		//back_dc_->create_empty_image(get_window_size());
		global_delegates::on_window_created.broadcast();
	}

	void application::on_window_size_changed(const math::vec2& new_size) noexcept {
		//back_dc_->create_empty_image(new_size);
		global_delegates::on_window_size_changed.broadcast(new_size);
	}

	void application::on_window_destory() noexcept {
		//back_dc_ = NULL;
		window_handle_ = NULL;
		instance_handle_ = NULL;
		delegate<void(void)> del;
		global_delegates::on_window_destoried.broadcast();
	}

	handle_t application::window_handle() const noexcept {
		return window_handle_;
	}

	handle_t application::instance_handle() const noexcept {
		return instance_handle_;
	}

	bool application::is_application_started() {
		return app_.get() != nullptr;
	}
	
	std::unique_ptr<application> application::app_;

	application& application::get() {
		assert(app_ != nullptr);
		return *app_;
	}

	void application::tick(float deltatime) {

	}

	math::vec2 application::get_window_client_size() const	{
		math::vec2 ret;
		if (is_application_started()) {
			RECT rc = {};
			if (GetClientRect((HWND)window_handle(), &rc)) {
				ret.x = (float)(rc.right - rc.left);
				ret.y = (float)(rc.bottom - rc.top);
			} else {
				//ZEE_LOG
			}
		}
		return ret;
	}
}
