#include "tick_manager.h"
namespace zee {
	tick_manager::tick_manager() {
		auto shared_this = shared_from_this();
		pre_tick.add_sp(shared_this, &tick_manager::on_pre_tick);
		tick.add_sp(shared_this, &tick_manager::on_tick);
		post_tick.add_sp(shared_this, &tick_manager::on_post_tick);
	}

	tick_manager& tick_manager::get() noexcept {
		static std::shared_ptr<tick_manager> inst;
		if (!inst) {
			inst = std::shared_ptr<tick_manager>();
		}
		return *inst;
	}

	void tick_manager::on_pre_tick(float delta_time) {
		tick_impl(pre_tickables_, delta_time);
	}

	void tick_manager::on_tick(float delta_time) {
		tick_impl(tickables_, delta_time);
	}

	void tick_manager::on_post_tick(float delta_time) {
		tick_impl(post_tickables_, delta_time);
	}

	void tick_manager::add_impl(std::vector<std::weak_ptr<interfaces::tickable>>& tickables, std::shared_ptr<interfaces::tickable>&& new_tickable) {
		for (auto iter = begin(tickables); iter != end(tickables);) {
			auto locked = iter->lock();
			if (locked == new_tickable) {
				return;
			}

			if (locked == nullptr) {
				iter = tickables.erase(iter);
			} else {
				++iter;
			}
		}

		tickables.push_back(std::move(new_tickable));
	}

	void tick_manager::remove_impl(std::vector<std::weak_ptr<interfaces::tickable>>& tickables, const std::shared_ptr<interfaces::tickable>& remove_tickable) {
		for (auto iter = begin(tickables); iter != end(tickables);) {
			auto locked = iter->lock();
			if (locked == remove_tickable) {
				iter = tickables.erase(iter);
				return;
			}

			if (locked == nullptr) {
				iter = tickables.erase(iter);
			} else {
				++iter;
			}
		}
	}

	void tick_manager::tick_impl(std::vector<std::weak_ptr<interfaces::tickable>>& tickables, float delta_time)	{ 
		for (auto iter = begin(tickables); iter != end(tickables);) {
			auto locked = iter->lock();
			if (locked) {
				locked->tick(delta_time);
				++iter;
			} else {
				iter = tickables.erase(iter);
			}
		}
	}

}//namespace zee 