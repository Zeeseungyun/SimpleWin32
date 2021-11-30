#include "key_state.h"
#include <zlog/zlog.h>
#include "../win32helper/win32helper.h"
#include <memory>
#include <math/common.h>
#include <WinUser.h>
namespace zee {
	key_state key_state::inst_;
	
	void key_state::internal_use_update(float delta_time) noexcept {
		if (GetKeyboardState((PBYTE)keys_)) {
			for (size_t i = 0; i != key_count; ++i) {

				//current down?
				if (keys_[i].value & 0xF0) {
					//previously down?
					if (durations_[i] >= 0) { 
						if (durations_[i] < max_duration) {
							durations_[i] += delta_time;
						}
						
					//previously up?
					} else {
						durations_[i] = 0.0f;
					}

				//current up?
				} else {
					//previously down?
					if (durations_[i] > 0) {
						durations_[i] = 0.0f;

					//previously up?
					} else {
						if (durations_[i] > -max_duration) {
							durations_[i] -= delta_time;
						}
					}
				}
			}

		} else {
			ZEE_LOG(normal, TEXT("key"), TEXT("An error occurred in the call to GetKeyboardState. %s"),
				win32helper::last_error_to_tstring().c_str()
			);
		}
	}

}//namespace zee 