#pragma once
#include "math/math.h"
#include "shape/rect.h"
namespace zee {
namespace render {
	class resource {
	public:

	private:
		
	};

	class renderer {
	public:
		shape::rectf render;

	private:
		std::weak_ptr<resource> resources;
	};

}//namespace zee::render 
}//namespace zee 