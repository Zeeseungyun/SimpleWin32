#pragma once
#include <core/core.h>
#include <interfaces/tickable.h>
#include <util/helper_macros.h>
#include <memory>
#include <vector>

namespace yjj {
	class tick_manager final : public std::enable_shared_from_this<tick_manager> {
	private:
		ZEE_DEFINE_REMOVE_MOVE_AND_COPY_FUNCTION(tick_manager);
		tick_manager();

	public:
		static tick_manager& get() noexcept;

	public:
		multicast_delegate<void(float)> pre_tick;
		multicast_delegate<void(float)> tick;
		multicast_delegate<void(float)> post_tick;

		void add_pre(std::shared_ptr<interfaces::tickable> new_tickable) {
			add_impl(pre_tickables_, std::move(new_tickable));
		}

		void remove_pre(std::shared_ptr<interfaces::tickable> remove_tickable) {
			remove_impl(pre_tickables_, remove_tickable);
		}

		void add(std::shared_ptr<interfaces::tickable> new_tickable) {
			add_impl(tickables_, std::move(new_tickable));
		}

		void remove(std::shared_ptr<interfaces::tickable> remove_tickable) {
			remove_impl(tickables_, remove_tickable);
		}

		void add_post(std::shared_ptr<interfaces::tickable> new_tickable) {
			add_impl(post_tickables_, std::move(new_tickable));
		}

		void remove_post(std::shared_ptr<interfaces::tickable> remove_tickable) {
			remove_impl(post_tickables_, remove_tickable);
		}

	private:
		void on_pre_tick(float delta_time);
		void on_tick(float delta_time);
		void on_post_tick(float delta_time);

		void add_impl(std::vector<std::weak_ptr<interfaces::tickable>>& tickables, std::shared_ptr<interfaces::tickable>&& new_tickable);
		void remove_impl(std::vector<std::weak_ptr<interfaces::tickable>>& tickables, const std::shared_ptr<interfaces::tickable>& remove_tickable);
		void tick_impl(std::vector<std::weak_ptr<interfaces::tickable>>& tickables, float delta_time);

	private:
		std::vector<std::weak_ptr<interfaces::tickable>> pre_tickables_;
		std::vector<std::weak_ptr<interfaces::tickable>> tickables_;
		std::vector<std::weak_ptr<interfaces::tickable>> post_tickables_;
	};

}//namespace yjj 