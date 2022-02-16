#pragma once
#include "interfaces/clonable.h"
#include "core/string.h"
#include "image.h"
#include "math/math.h"
#include "shape/shape.h"
#include "util/helper_macros.h"
#include "color.h"
#include <vector>

namespace yjj {
	typedef void* handle_t;
	typedef unsigned char byte;
namespace win32gdi {
	
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
	* 디바이스 컨택스트 기반 클래스.
	* 그리기 연산에 대한 정의를 함.
	*/
	class device_context_base {
	private:
		ZEE_DEFINE_REMOVE_COPY_FUNCTION(device_context_base);

	protected:
		struct handle_pair {
			handle_t old_handle = NULL, new_handle = NULL;

			template<typename HandleT = handle_t> HandleT get_old() const noexcept { return (HandleT)old_handle; }
			template<typename HandleT = handle_t> HandleT get_new() const noexcept { return (HandleT)new_handle; }

			void clear(handle_t handle_dc);
			void select(handle_t handle_dc, handle_t select_handle);
		};

		device_context_base() noexcept;
		virtual ~device_context_base() noexcept;

	public:
		bool is_valid() const noexcept;
		template<typename HandleT = handle_t> HandleT handle_dc() const noexcept { return (HandleT)handle_dc_; }

	public:
		math::vec2i get_bitmap_size() const noexcept;

		//bitmap
	public:
		void bit_blt(device_context_base& dest_dc, const math::vec2i& dest_pos, const math::vec2i& src_pos, const math::vec2i& src_size,  bitblt_raster_op_type rop = bitblt_raster_op_type::src_copy) noexcept;
		void bit_blt(device_context_base& dest_dc, const math::vec2i& dest_pos) noexcept;

		void transparent_blt(device_context_base& dest_dc, const math::vec2i& dest_pos, const math::vec2i& dest_size, const math::vec2i& src_pos, const math::vec2i& src_size, color transparent_color) noexcept;
		void transparent_blt(device_context_base& dest_dc, const math::vec2i& dest_pos, color transparent_color) noexcept;
		
		void alphablend(device_context_base& dest_dc, const math::vec2i& dest_pos, const math::vec2i& dest_size, const math::vec2i& src_pos, const math::vec2i& src_size, float alpha) noexcept;
		void alphablend(device_context_base& dest_dc, const math::vec2i& dest_pos, float alpha) noexcept;

		//@param points 좌상 우상 좌하 배열 필요
		void plg_blt(device_context_base& dest_dc, math::vec2f point, const float angle, const math::vec2i& src_pos, const math::vec2i& src_size) noexcept;
		void plg_blt(device_context_base& dest_dc, math::vec2f point, const float angle, const math::vec2i& src_size) noexcept;

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
		
		void change_brush_color(const color new_brush_color) noexcept;

		//pen
	public:
		void move_to(const math::vec2i& dot) noexcept;
		void line_to(const math::vec2i& line) noexcept;

		template<typename VecElemT>
		void draw_line(const math::basic_vec<2, VecElemT>* begin, const math::basic_vec<2, VecElemT>* end) noexcept {
			move_to(*begin++);
			while (begin != end) {
				line_to(*begin++);
			}
		}

		void change_pen_color(const color new_pen_color) noexcept;

	protected:
		static math::vec2i calc_bitmap_size(handle_t handle_bitmap);

	protected:
		handle_t handle_dc_;
		handle_pair handle_image_;

	};

	enum class device_context_auto_type {
		none,
		paint,
		temp
	};

	/* 
	* 임시로 사용되는 디바이스 컨택스트용 클래스.
	* BeginPaint 혹은 GetDC로 획득한 DC를 생성자에 전달하여 사용.
	* 소멸자에서 아무런 처리를 하지 않음.
	*/
	class device_context_auto : public device_context_base {
	private:
		ZEE_DEFINE_REMOVE_MOVE_AND_COPY_FUNCTION(device_context_auto);

	public:
		explicit device_context_auto(handle_t window_handle, device_context_auto_type new_type) noexcept;
		~device_context_auto() noexcept override;

		device_context_auto_type get_type() const noexcept { return type_; }

	protected:
		handle_t window_handle_ = NULL;
		device_context_auto_type type_;
		std::vector<byte> buf_;
	};

	/*
	* 임의로 생성한 디바이스 컨택스트용 클래스.
	* 소멸자를 통해 사용된 핸들을 모두 반환하도록 해줌.
	* BMP파일을 불러오거나 이미지버퍼를 생성할 수 있음.
	*/
	class device_context_dynamic : 
		public device_context_base, 
		public interfaces::cloneable<device_context_dynamic> {
	private:
		ZEE_DEFINE_REMOVE_COPY_FUNCTION(device_context_dynamic);

	public:
		//~begin interfaces::cloneable<device_context_dynamic>
		virtual bool clone(device_context_dynamic& out_object) override;
		//~end interfaces::cloneable<device_context_dynamic>

	public:
		device_context_dynamic();
		device_context_dynamic(device_context_dynamic&& other) noexcept;
		device_context_dynamic& operator=(device_context_dynamic&& other) noexcept;
		~device_context_dynamic();

		const simple_bitmap_desc& get_bitmap_desc() const noexcept { return bitmap_desc_; }

		bool load_image(const tstring& str);
		bool create_empty_image(const math::vec2i& new_size);
		void clear() noexcept;
		void resize(const math::vec2i& new_size);

	protected:
		void move_from(device_context_dynamic&& other) noexcept;

		bool create_if_has_no_dc();

	private:
		simple_bitmap_desc bitmap_desc_;
	};

	bool is_valid(const device_context_base& dc);
}//namespace yjj::win32gdi
}//namespace yjj 