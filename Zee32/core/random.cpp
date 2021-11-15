#include "random.h"
#include "core/core.h"

namespace zee {
namespace impl {
	class default_random_engine_constructor {
	public:
		default_random_engine_constructor() : rd_(), engine_(rd_()){}
		std::default_random_engine& get() { return engine_; }

	private:
		std::random_device rd_;
		std::default_random_engine engine_;
	};

}//namespace impl
	thread_local static impl::default_random_engine_constructor random_engine_ctor;
	std::default_random_engine& get_engine() {
		return random_engine_ctor.get();
	}
}
