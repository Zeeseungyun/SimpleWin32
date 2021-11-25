#include "application.h"
#include "zlog/zlog.h"
namespace zee {
	application::application_config::application_config() 
		: config::ini_base(TEXT("app.ini")) {

		bool dirty = false;
		if (!is_loaded()) {
			dirty = true;
		} else { //��ȿ�� üũ.. ��Ű���������� ���߿� ã�ƺ�����..
			auto& This = *this;
			if (This.is_object()) {
				auto found = This.find("window_size");
				if (This.end() == found) {
					dirty = true;
				} else {
					if (!found->is_object()) {
						dirty = true;
					} else {
						found = This["window_size"].find("width");
						if (This["window_size"].end() == found) {
							dirty = true;
						}
					}
				}
			}
		}

		if (dirty) {
			json default_ini;
			default_ini["window_size"]["width"] = 1024;
			default_ini["window_size"]["height"] = 1024;
			default_ini["full_size"] = false;
			default_ini["name"] = "my app";

			swap(default_ini);
			save();
		}
	}

}//namespace zee