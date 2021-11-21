#pragma once
#include "interfaces/clonable.h"
#include "core/string.h"
#include "image.h"
#include "math/math.h"
#include "util/idioms.h"
#include "color.h"
#include <vector>

namespace zee {
	typedef void* handle_t;
	typedef unsigned char byte;
namespace win32gdi {
	
	handle_t default_font_handle();
	handle_t default_brush_handle();
	handle_t default_pen_handle();

	//https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-bit_blt
	enum class bitblt_raster_op_type {
		blackness,
		whiteness,

		capture_blt,
		dest_invert,

		merge_copy,
		merge_paint,

		no_mirrior_bitmap,

		not_src_copy,
		not_src_erase,

		pattern_copy,
		pattern_invert,
		pattern_paint,

		src_and,
		src_copy,
		src_erase,
		src_paint,
	};

	enum class device_context_auto_type {
		none,
		paint,
		temp
	};

	/*
	* 디바이스 컨택스트 기반 클래스.
	* 그리기 연산에 대한 정의를 함.
	*/
	class device_context_base {
	private:
		REMOVE_COPY_FUNCTION_DEFINE(device_context_base);

	protected:
		struct handle_pair {
			DEFAULT_COPY_FUNCTION_DEFINE(handle_pair);
			handle_t old_handle = NULL, new_handle = NULL;
			
			handle_pair() = default;
			handle_pair(handle_pair && other) noexcept;
			handle_pair& operator=(handle_pair && other) noexcept;

			template<typename HandleT = handle_t>
			HandleT get_old() const noexcept {
				return (HandleT)old_handle;
			}

			template<typename HandleT = handle_t>
			HandleT get_new() const noexcept {
				return (HandleT)new_handle;
			}
		};

		device_context_base() noexcept;
		virtual ~device_context_base() noexcept;

	public:
		bool is_valid() const noexcept;
		template<typename HandleT = handle_t>
		HandleT dc_handle() const noexcept {
			return (HandleT)dc_handle_;
		}

	public:
		const math::size2& get_bitmap_size() const noexcept;
		const simple_bitmap_desc& get_bitmap_desc() const noexcept;

		//bitmap
	public:
		void bit_blt(device_context_base& dest_dc, const math::point& dest_pos, const math::point& src_pos, const math::size2& src_size,  bitblt_raster_op_type rop = bitblt_raster_op_type::src_copy) noexcept;
		void bit_blt(device_context_base& dest_dc, const math::point& dest_pos) noexcept;
		void transparent_blt(device_context_base& dest_dc, const math::point& dest_pos, const math::size2& dest_size, const math::point& src_pos, const math::size2& src_size, color transparent_color) noexcept;
		void transparent_blt(device_context_base& dest_dc, const math::point& dest_pos, color transparent_color) noexcept;
		void alphablend(device_context_base& dest_dc, const math::point& dest_pos, const math::size2& dest_size, const math::point& src_pos, const math::size2& src_size, float alpha) noexcept;
		void alphablend(device_context_base& dest_dc, const math::point& dest_pos, float alpha) noexcept;

		//font
	public:
		void print_text(const math::vec2& pos, const tstring& str) noexcept;

		//brush
	public:
		void rectangle(const math::rect& rt) noexcept;
		void circle(const math::circle& cc) noexcept;
		void ellipse(int32 left, int32 top, int32 right, int32 bottom) noexcept;

	protected:
		virtual void release_image_handle();
		virtual void release_font_handle();
		virtual void release_brush_handle();
		virtual void release_pen_handle();

	protected:
		static math::size2 calc_bitmap_size(handle_t bitmap_handle);

	protected:
		handle_t dc_handle_;
		math::size2 bitmap_size_;

		handle_pair image_handle_;
		handle_pair font_handle_;
		handle_pair brush_handle_;
		handle_pair pen_handle_;

		simple_bitmap_desc bitmap_desc_;
	};
	
	/* 
	* 임시로 사용되는 디바이스 컨택스트용 클래스.
	* BeginPaint 혹은 GetDC로 획득한 DC를 생성자에 전달하여 사용.
	* 소멸자에서 아무런 처리를 하지 않음.
	*/
	class device_context_auto final : public device_context_base {
	private:
		REMOVE_MOVE_AND_COPY_FUNCTION_DEFINE(device_context_auto);

	public:
		explicit device_context_auto(handle_t window_handle, device_context_auto_type new_type) noexcept;
		~device_context_auto() noexcept;

		device_context_auto_type get_type() const noexcept { return type_; }

	private:
		handle_t window_handle_ = NULL;
		device_context_auto_type type_;
		std::vector<byte> buffer_;
	};

	/*
	* 임의로 생성한 디바이스 컨택스트용 클래스.
	* 소멸자를 통해 사용된 핸들을 모두 반환하도록 해줌.
	* BMP파일을 불러오거나 이미지버퍼를 생성할 수 있음.
	*/
	class device_context_dynamic final : public device_context_base, public interfaces::clonable {
	private:
		REMOVE_COPY_FUNCTION_DEFINE(device_context_dynamic);
		typedef device_context_base parent_class;

	public:
		//~begin interfaces::clonable
		void clone(void* out_device_context_ptr) final;
		//~end interfaces::clonable

	public:
		device_context_dynamic();
		device_context_dynamic(device_context_dynamic&& other) noexcept;
		device_context_dynamic& operator=(device_context_dynamic&& other) noexcept;
		~device_context_dynamic();

		bool load_image(const tstring& str);

		// width != 0 && height != 0
		bool create_empty_image(const math::size2& new_size);
		void clear() noexcept;

	private:
		void move_from(device_context_dynamic&& other) noexcept;

		bool create_if_has_no_dc();
		void release_image_handle() final;
	};

	bool is_valid(const device_context_base& dc);
}//namespace zee::win32gdi
}//namespace zee 