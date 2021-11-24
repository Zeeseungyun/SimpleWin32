#include "application.h"
#include "zlog/zlog.h"
namespace zee {
	application::application_config::application_config() 
		: config::ini_base(TEXT("app.ini")) {

		json default_ini;
		default_ini["window_size"]["x"] = 1024;
		default_ini["window_size"]["y"] = 1024;
		default_ini["full_size"] = false;
		default_ini["name"] = "my app";

		if (!is_loaded()) {
			swap(default_ini);
		} else { //유효성 체크.. 스키마가능한진 나중에 찾아보도록..
			bool dirty = false;
			auto& This = *this;
			if (This.is_object()) {
				auto found = This.find("window_size");
				if (This.end() == found) {
					dirty = true;
				} else {
					if (!found->is_object()) {
						dirty = true;
					} else {
						found = This["window_size"].find("x");
						if (This.end() == found) {
							dirty = true;
						}
					}
				}
			}
		}
	}

}//namespace zee