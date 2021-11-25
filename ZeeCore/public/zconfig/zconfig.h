#pragma once
#include "../core/string.h"
#include "../file/path.h"
#include "../json/json.h"
#include "../util/helper_macros.h"

namespace zee {
namespace config {
namespace impl {
	class ini_base_impl {
	protected:
		ini_base_impl(const tstring& ini_file_name, const tstring& config_dir, const tstring& config_name) noexcept;

	public:
		const tstring& ini_file_name() const noexcept { return ini_file_name_; }
		const tstring& config_dir() const noexcept { return config_dir_; }
		const tstring& config_name() const noexcept { return config_name_; }

		virtual bool load() noexcept;
		virtual void save() noexcept;

	protected:
		bool is_loaded_successfully_ = false;
		json json_;

	private:
		tstring ini_file_name_;
		tstring config_dir_;
		tstring config_name_;
	};

	template<typename ConfigT>
	struct has_config_file_path_func {
	private:
		typedef char yes[2];
		typedef char no[1];

		template<typename T, typename A = decltype(T::config_path_name())> static yes& check(T*);
		template<typename T> static no& check(...);

	public:
		static constexpr bool value = sizeof(check<ConfigT>(0)) == sizeof(yes);
	};

}//namespace zee::config::impl

	template<typename ConfigT>
	struct ini_traits {
		typedef ConfigT config_type;

		static tstring config_name() noexcept {
			tstring ret = file::type_to_path<config_type>();
			size_t off = ret.find_last_of(TEXT("/"));
			if (off != tstring::npos) {
				ret.erase(begin(ret), begin(ret) + off);
			}

			return ret;
		}

		static tstring config_dir() noexcept {
			tstring ret = file::type_to_path<config_type>();
			size_t off = ret.find_last_of(TEXT("/"));
			if (off != tstring::npos) {
				ret.erase(off);
			}

			return file::paths::config_dir() + ret;
		}

		template<typename T>
		static std::enable_if_t<impl::has_config_file_path_func<T>::value, tstring>
			config_file_path_impl() noexcept {
			return to_tstring(T::config_path_name()) + TEXT(".ini");
		}

		template<typename T>
		static std::enable_if_t<!impl::has_config_file_path_func<T>::value, tstring>
			config_file_path_impl() noexcept {
			return to_tstring(config_name()) + TEXT(".ini");
		}

		static tstring
			config_file_path() noexcept {
			return config_file_path_impl<config_type>();
		}
	};

	template<typename ConfigT>
	class ini_base : public impl::ini_base_impl {
	private:
		typedef impl::ini_base_impl base_type;

	public:
		typedef ConfigT config_type;
		typedef ini_traits<config_type> traits_type;

	private:
		ini_base() noexcept
			: base_type(traits_type::config_file_path(), traits_type::config_dir(), traits_type::config_name()) {
			load();
		}

		static std::unique_ptr<ini_base<config_type>> config_inst;

	public:
		static ini_base<config_type>& get() noexcept {
			if (!config_inst) {
				config_inst = std::unique_ptr<ini_base<config_type>>(new ini_base<config_type>);
			}

			return *config_inst;
		}

		config_type& config() noexcept { return config_; }
		const config_type& config() const noexcept { return config_; }

	public:
		virtual bool load() noexcept override {
			if (base_type::load()) {
				config() = this->json_;
			} else {
				config() = config_type();
			}
			return true;
		}

		virtual void save() noexcept override {
			this->json_ = config();
			base_type::save();
		}

	private:
		config_type config_;
	};

	template<typename ConfigT>
	std::unique_ptr<ini_base<ConfigT>> ini_base<ConfigT>::config_inst;

}//namespace zee::config 
}//namespace zee

#define ZEE_GET_CONFIG(config_type) ::zee::config::ini_base<config_type>::get()
