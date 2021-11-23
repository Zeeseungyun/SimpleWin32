#pragma once
#include <cstdint>
#include <cassert>

#ifndef NOMINMAX //for windows.h
#define NOMINMAX
#endif

namespace zee {
	typedef void* handle_t;
	typedef std::int64_t  int64;
	typedef std::int32_t  int32;
	typedef std::int16_t  int16;
	typedef std::int8_t   int8;
	typedef std::uint64_t uint64;
	typedef std::uint32_t uint32;
	typedef std::uint16_t uint16;
	typedef std::uint8_t  uint8;
	typedef std::uint8_t  byte;
	typedef float default_floating_point_t;

}//namespace zee
