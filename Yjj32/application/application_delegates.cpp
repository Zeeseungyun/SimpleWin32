#include "application_delegates.h"
namespace yjj {

	application_delegates& application_delegates::inst() noexcept {
		static std::unique_ptr<application_delegates> instance_;
		if (!instance_) {
			instance_ = std::unique_ptr<application_delegates>(new application_delegates);
		}
		return *instance_;
	}

}//namespace yjj 
