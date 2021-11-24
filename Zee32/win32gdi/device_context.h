#pragma once
#include "interfaces/clonable.h"
#include "core/string.h"
#include "image.h"
#include "math/math.h"
#include "shape/shape.h"
#include "util/helper_macros.h"
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


	/*
	* ����̽� ���ý�Ʈ ��� Ŭ����.
	* �׸��� ���꿡 ���� ���Ǹ� ��.
	*/
	class device_context_base {
	private:
		ZEE_DEFINE_REMOVE_COPY_FUNCTION(device_context_base)

	protected:
		struct handle_pair {
			ZEE_DEFINE_DEFAULT_COPY_FUNCTION(handle_pair)

		public:
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
		const math::vec2i& get_bitmap_size() const noexcept;
		const simple_bitmap_desc& get_bitmap_desc() const noexcept;

		//bitmap
	public:
		void bit_blt(device_context_base& dest_dc, const math::vec2i& dest_pos, const math::vec2i& src_pos, const math::vec2i& src_size,  bitblt_raster_op_type rop = bitblt_raster_op_type::src_copy) noexcept;
		void bit_blt(device_context_base& dest_dc, const math::vec2i& dest_pos) noexcept;

		void transparent_blt(device_context_base& dest_dc, const math::vec2i& dest_pos, const math::vec2i& dest_size, const math::vec2i& src_pos, const math::vec2i& src_size, color transparent_color) noexcept;
		void transparent_blt(device_context_base& dest_dc, const math::vec2i& dest_pos, color transparent_color) noexcept;
		
		void alphablend(device_context_base& dest_dc, const math::vec2i& dest_pos, const math::vec2i& dest_size, const math::vec2i& src_pos, const math::vec2i& src_size, float alpha) noexcept;
		void alphablend(device_context_base& dest_dc, const math::vec2i& dest_pos, float alpha) noexcept;

		//font
	public:
		void print_text(const math::vec2& pos, const tstring& str) noexcept;

		//brush
	public:
		void rectangle(const shape::recti& rt) noexcept;
		void rectangle(const shape::recti& rt, const math::vec2i& pt) noexcept;

		void circle(const shape::circlei& cc) noexcept;
		void circle(const shape::circlei& cc, const math::vec2i& pt) noexcept;

		void ellipse(int32 left, int32 top, int32 right, int32 bottom) noexcept;

	protected:
		virtual void release_image_handle();
		virtual void release_font_handle();
		virtual void release_brush_handle();
		virtual void release_pen_handle();

	protected:
		static math::vec2i calc_bitmap_size(handle_t bitmap_handle);

	protected:
		handle_t dc_handle_;
		math::vec2i bitmap_size_;

		handle_pair image_handle_;
		handle_pair font_handle_;
		handle_pair brush_handle_;
		handle_pair pen_handle_;

		simple_bitmap_desc bitmap_desc_;
	};

	enum class device_context_auto_type {
		none,
		paint,
		temp
	};

	/* 
	* �ӽ÷� ���Ǵ� ����̽� ���ý�Ʈ�� Ŭ����.
	* BeginPaint Ȥ�� GetDC�� ȹ���� DC�� �����ڿ� �����Ͽ� ���.
	* �Ҹ��ڿ��� �ƹ��� ó���� ���� ����.
	*/
	class device_context_auto : public device_context_base {
	private:
		ZEE_DEFINE_REMOVE_MOVE_AND_COPY_FUNCTION(device_context_auto)

	public:
		explicit device_context_auto(handle_t window_handle, device_context_auto_type new_type) noexcept;
		~device_context_auto() noexcept;

		device_context_auto_type get_type() const noexcept { return type_; }

	protected:
		handle_t window_handle_ = NULL;
		device_context_auto_type type_;
		std::vector<byte> buf_;
	};

	/*
	* ���Ƿ� ������ ����̽� ���ý�Ʈ�� Ŭ����.
	* �Ҹ��ڸ� ���� ���� �ڵ��� ��� ��ȯ�ϵ��� ����.
	* BMP������ �ҷ����ų� �̹������۸� ������ �� ����.
	*/
	class device_context_dynamic : 
		public device_context_base, 
		public interfaces::clonable<device_context_dynamic> {
	private:
		ZEE_DEFINE_REMOVE_COPY_FUNCTION(device_context_dynamic)
		typedef device_context_base parent_class;

	public:
		//~begin interfaces::clonable
		virtual device_context_dynamic clone() override;
		//~end interfaces::clonable

	public:
		device_context_dynamic();
		device_context_dynamic(device_context_dynamic&& other) noexcept;
		device_context_dynamic& operator=(device_context_dynamic&& other) noexcept;
		~device_context_dynamic();

		bool load_image(const tstring& str);

		// width != 0 && height != 0
		bool create_empty_image(const math::vec2i& new_size);
		void clear() noexcept;

	protected:
		void move_from(device_context_dynamic&& other) noexcept;

		bool create_if_has_no_dc();
		void release_image_handle() final;
	};

	bool is_valid(const device_context_base& dc);
}//namespace zee::win32gdi
}//namespace zee 