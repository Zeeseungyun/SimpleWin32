#pragma once
#include <memory>
namespace zee {

	template<typename T>
	class singleton {
		friend class application;
		static T& get() {
			if (instance == nullptr) {
				instance = new T();
			}

			return *instance;
		}

	private:
		static std::unique_ptr<T> instance;
	};

	template<typename T>
	std::unique_ptr<T> singleton<T>::instance = nullptr;
}
