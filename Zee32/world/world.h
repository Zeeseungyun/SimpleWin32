#pragma once
#include <core/core.h>
#include <interfaces/tickable.h>
#include <util/helper_macros.h>
#include <memory>
#include <functional>

namespace zee {
	class toy;

	//forward declaration.
	template<typename ToyT>
	std::enable_if_t<std::is_base_of<toy, ToyT>::value, std::shared_ptr<ToyT>> spawn_toy();

	class world : public std::enable_shared_from_this<world>, public interfaces::tickable {
	public:
		template<typename ToyT>
		friend std::enable_if_t<std::is_base_of<toy, ToyT>::value, std::shared_ptr<ToyT>> spawn_toy();

	public:
		static std::weak_ptr<world> current_world();

	public:
		virtual void tick(float delta_time) override;

	protected:
		virtual void on_started();
		virtual void on_destroied();

	public:
		bool was_destoried() const noexcept { return was_destoried_; }

		const std::vector<std::shared_ptr<toy>>& toys() const { return toys_; }

	protected:
		template<typename ToyT>
		void register_toy() {

		}

	private:
		std::vector<std::shared_ptr<toy>> toys_;

	private:
		bool was_destoried_ = false;
	};

	template<typename WorldClassT>
	struct is_world_class : std::conditional_t<std::is_base_of<world, WorldClassT>::value, std::true_type, std::false_type> {

	};

namespace impl {
	template<typename WorldClassT>
	struct world_has_custom_name_impl {
	private:
		typedef char yes[2];
		typedef char no[1];

		template<typename T, bool IsWorldClass = is_world_class<T>::value, bool HasWorldName = std::is_same<TCHAR, std::decay_t<decltype(T::world_name()[0])>>::value>
		static yes& check(T*);

		template<typename T>
		static no& check(...);

	public:
		constexpr static bool value = sizeof(check<WorldClassT>(0)) == sizeof(yes);
	};

	class test_world : public world {
	public:
		//static const TCHAR* world_name() { return TEXT("asd"); }
	};

	void test_func() {
		constexpr bool bbb = world_has_custom_name_impl<test_world>::value;
	}

}//namespace impl 
	template<typename WorldClassT>
	struct world_traits {
		typedef WorldClassT world_class_type;

		static std::enable_if_t<impl::world_has_custom_name_impl<world_class_type>::value, tstring>
			world_name() { 
			return world_class_type::world_name(); 
		}

		static std::enable_if_t<!impl::world_has_custom_name_impl<world_class_type>::value, tstring>
			world_name() {
			return type_to_naming_string<world_class_type>();
		}

		static std::vector<std::shared_ptr<toy>> startup_toys(std::shared_ptr<world_class_type>& world_inst) {
			world_inst->
		}
	};

	template<typename WorldClassT>
	struct register_world;

	class world_manager {
		ZEE_DEFINE_REMOVE_MOVE_AND_COPY_FUNCTION(world_manager);
	private:
		template<typename WorldClassT> friend struct register_world;

	private:
		world_manager();
		static std::unique_ptr<world_manager> inst_;

	public:
		static world_manager& get() {
			if (!inst_) {
				inst_ = std::unique_ptr<world_manager>(new world_manager);
			}
			return *inst_;
		}

	private:
		std::map<tstring, std::function<void(world*&)>> constructors;
	};

	template<typename WorldClassT>
	struct register_world {
		typedef WorldClassT world_class_type;
		static_assert(std::is_base_of<world, WorldClassT>::value && !std::is_abstract<WorldClassT>::value, "invalid world class type.");
		register_world() { 
			world_manager::get().constructors[];
		}

		static register_world inst_;
	};

}//namespace zee
