#pragma once
#include "../win32gdi/device_context.h"

namespace zee {
	//¿ÃπÃ¡ˆ
	struct image_data {
		image_data() noexcept;
		virtual ~image_data() noexcept;

		void init_load_images();
		
		win32gdi::device_context_dynamic back;
		win32gdi::device_context_dynamic dig;
		win32gdi::device_context_dynamic monster_spawn;
		win32gdi::device_context_dynamic monster_die;
	};
}
