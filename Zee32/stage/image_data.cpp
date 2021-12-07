#include "image_data.h"

namespace zee {
	image_data::image_data() noexcept {}
	image_data::~image_data() noexcept {}

	void image_data::init_load_images() {
		if (!back.is_valid()) { 
			back.load_image(TEXT("./assets/back.bmp")); 
		}
		if (!dig.is_valid()) { 
			dig.load_image(TEXT("./assets/dig.bmp")); 
		}
		if (!monster_spawn.is_valid()) { 
			monster_spawn.load_image(TEXT("./assets/monster_spawn.bmp"));
		}
		if (!monster_die.is_valid()) { 
			monster_die.load_image(TEXT("./assets/monster_die.bmp"));
		}
	}
}