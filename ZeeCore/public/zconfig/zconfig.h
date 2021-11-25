#pragma once
#include "../core/string.h"
#include "../file/path.h"
#include "../json/json.h"
#include "../util/helper_macros.h"

namespace zee {
namespace config {
namespace impl {
	class config_base_impl {
	protected:
		config_base_impl(const tstring& ini_file_name, const tstring& config_dir, const tstring& config_name) noexcept;

	public:
		const tstring& ini_file_name() const noexcept { return ini_file_name_; }
		const tstring& config_dir() const noexcept { return config_dir_; }
		const tstring& config_name() const noexcept { return config_name_; }

	protected:
		bool load_impl() noexcept;
		void save_impl() noexcept;

	protected:
		bool is_loaded_successfully_ = false;
		json json_;

	private:
		tstring ini_file_name_;
		tstring config_dir_;
		tstring config_name_;
	};

	template<typename ConfigT>
	struct has_config_name_impl {
	private:
		typedef char yes[2];
		typedef char no[1];

		template<typename T, 
			typename A = decltype(T::config_name()), 
			std::enable_if_t<std::is_same<A, const TCHAR*>::value, int> = 0> 
		static yes& check(T*);
		template<typename T> 
		static no& check(...);

	public:
		static constexpr bool value = sizeof(check<ConfigT>(0)) == sizeof(yes);
	};

	template<typename ConfigT>
	struct has_config_dir_impl {
	private:
		typedef char yes[2];
		typedef char no[1];

		template<typename T, 
			typename A = decltype(T::config_dir()), 
			std::enable_if_t<std::is_same<A, const TCHAR*>::value, int> = 0> 
		static yes& check(T*);

		template<typename T> 
		static no& check(...);

	public:
		static constexpr bool value = sizeof(check<ConfigT>(0)) == sizeof(yes);
	};

}//namespace zee::config::impl

	template<typename ConfigT>
	struct ini_traits {
		typedef ConfigT config_type;
	private:
		template<typename T>
		static std::enable_if_t<impl::has_config_name_impl<T>::value, tstring>
			config_name_impl() noexcept {
			return T::config_name();
		}

		template<typename T>
		static std::enable_if_t<!impl::has_config_name_impl<T>::value, tstring>
			config_name_impl() noexcept {
			tstring ret = file::type_to_path<config_type>();
			size_t off = ret.find_last_of(TEXT("/"));
			if (off != tstring::npos) {
				ret.erase(begin(ret), begin(ret) + off);
			}

			return ret;
		}

		template<typename T>
		static std::enable_if_t<impl::has_config_dir_impl<T>::value, tstring>
			config_dir_impl() noexcept {
			return T::config_dir();
		}

		template<typename T>
		static std::enable_if_t<!impl::has_config_dir_impl<T>::value, tstring>
			config_dir_impl() noexcept {
			tstring ret = file::type_to_path<T>();
			size_t off = ret.find_last_of(TEXT("/"));
			if (off != tstring::npos) {
				ret.erase(off);
			}

			return file::paths::config_dir() + ret;
		}

	public:
		static tstring config_name() noexcept {
			return config_name_impl<config_type>();
		}

		static tstring config_dir() noexcept {
			return config_dir_impl<config_type>();
		}

		static tstring config_file_path() noexcept {
			return config_dir() + config_name() + TEXT(".ini");
		}
	};

	template<typename ConfigT>
	class config_base : public impl::config_base_impl {
	private:
		typedef impl::config_base_impl base_type;

	public:
		typedef ConfigT config_type;
		typedef ini_traits<config_type> traits_type;

	private:
		config_base() noexcept
			: base_type(traits_type::config_file_path(), traits_type::config_dir(), traits_type::config_name()) {
			load();
		}

		static std::unique_ptr<config_base<config_type>> config_inst_;

	public:
		static config_base<config_type>& get() noexcept {
			if (!config_inst_) {
				config_inst_ = std::unique_ptr<config_base<config_type>>(new config_base<config_type>);
			}

			return *config_inst_;
		}

		config_type& config() noexcept { return config_; }
		const config_type& config() const noexcept { return config_; }

	public:
		bool load() noexcept {
			if (base_type::load_impl()) {
				config() = this->json_;
			} else {
				config() = config_type();
			}
			return true;
		}

		void save() noexcept {
			this->json_ = config();
			base_type::save_impl();
		}

	private:
		config_type config_;
	};

	template<typename ConfigT> std::unique_ptr<config_base<ConfigT>> config_base<ConfigT>::config_inst_;
	
	template<typename ConfigT>
	using config = config_base<ConfigT>;

}//namespace zee::config 
}//namespace zee