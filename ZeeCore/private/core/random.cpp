#include "../../public/core/random.h"
#include "../../public/core/core.h"

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

	thread_local static default_random_engine_constructor random_engine_ctor;
	std::default_random_engine& get_engine() {
		return random_engine_ctor.get();
	}
}//namespace impl
}
