#pragma once
#include "../core/string.h"
#include "../file/path.h"
#include "../json/json.h"
#include "../util/helper_macros.h"

namespace zee {
namespace impl {
	template<typename DataT>
	struct has_config_name_impl : private zee::impl::yes_no_impl {
	private:
		template<typename T, 
			typename A = decltype(T::config_name()[0]), 
			std::enable_if_t<std::is_same<std::decay_t<A>, TCHAR>::value, int> = 0> 
		static yes& check(T*);
		template<typename T> 
		static no& check(...);

	public:
		static constexpr bool value = sizeof(check<DataT>(0)) == sizeof(yes);
	};

	template<typename DataT>
	struct has_config_dir_impl : private zee::impl::yes_no_impl {
	private:
		template<typename T, 
			typename A = decltype(T::config_dir()[0]), 
			std::enable_if_t<std::is_same<std::decay_t<A>, TCHAR>::value, int> = 0> 
		static yes& check(T*);

		template<typename T> 
		static no& check(...);

	public:
		static constexpr bool value = sizeof(check<DataT>(0)) == sizeof(yes);
	};

}//namespace zee::config::impl

	template<typename DataT>
	struct config_data_traits {
		typedef DataT data_type;

	private:
		template<typename T>
		static std::enable_if_t<impl::has_config_name_impl<T>::value, tstring>
			config_name_impl() noexcept {
			return T::config_name();
		}

		template<typename T>
		static std::enable_if_t<!impl::has_config_name_impl<T>::value, tstring>
			config_name_impl() noexcept {
			tstring ret = file::type_to_path<data_type>();
			size_t off = ret.find_last_of(TEXT('/'));
			if (off != tstring::npos) {
				ret.erase(begin(ret), begin(ret) + off + 1);
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
			size_t off = ret.find_last_of(TEXT('/'));
			if (off != tstring::npos) {
				ret.erase(off);
			}

			return ret;
		}

	public:
		static tstring config_name() noexcept {
			return config_name_impl<data_type>();
		}

		static tstring config_dir() noexcept {
			return file::paths::config_dir() + config_dir_impl<data_type>();
		}

		static tstring config_file_path() noexcept {
			return config_dir() + TEXT("/") + config_name() + TEXT(".ini");
		}
	};

	class config_base {
	protected:
		config_base(const tstring& config_file_path, const tstring& config_dir, const tstring& config_name) noexcept;

	public:
		const tstring& config_file_path() const noexcept { return config_file_path_; }
		const tstring& config_dir() const noexcept { return config_dir_; }
		const tstring& config_name() const noexcept { return config_name_; }
		const json& raw_json() const noexcept { return json_; }

	protected:
		bool load_impl() noexcept;
		void save_impl() noexcept;

	protected:
		json json_;

	private:
		tstring config_file_path_;
		tstring config_dir_;
		tstring config_name_;
	};

	template<typename DataT>
	class basic_config : public config_base {
	public:
		typedef DataT data_type;
		typedef config_data_traits<data_type> traits_type;

	private:
		basic_config() noexcept
			: config_base(traits_type::config_file_path(), traits_type::config_dir(), traits_type::config_name()) {
			load();
		}

		static std::unique_ptr<basic_config<data_type>> config_inst_;

	public:
		static basic_config<data_type>& get() noexcept {
			if (!config_inst_) {
				config_inst_ = std::unique_ptr<basic_config<data_type>>(new basic_config<data_type>);
			}

			return *config_inst_;
		}

		data_type& data() noexcept { return data_; }
		const data_type& data() const noexcept { return data_; }

	public:
		bool load() noexcept {
			if (config_base::load_impl()) {
				data() = raw_json().get<data_type>();
			} else {
				data() = data_type();
			}
			return true;
		}

		void save() noexcept {
			this->json_ = data();
			config_base::save_impl();
		}

	private:
		data_type data_;
	};

	template<typename ConfigT> std::unique_ptr<basic_config<ConfigT>> basic_config<ConfigT>::config_inst_;
	
}//namespace zee

//#define ZEE_QUICK_CONFIG_DEFINE()