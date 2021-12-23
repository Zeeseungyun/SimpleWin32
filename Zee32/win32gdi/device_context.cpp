#include "device_context.h"
#include "core/core.h"
#include "zlog/zlog.h"
#include "../win32helper/windows_with_macro.h"
#include "../win32helper/win32helper.h"
#include "../application/application.h"
#include "math/matrix.h"

namespace zee {
	namespace win32gdi {

		void device_context_base::handle_pair::clear(handle_t handle_dc) {
			if (handle_dc) {
				if (new_handle) {
					SelectObject((HDC)handle_dc, get_old<HGDIOBJ>());
					DeleteObject(get_new<HGDIOBJ>());
					new_handle = NULL;
					old_handle = NULL;
				}
			}
		}

		void device_context_base::handle_pair::select(handle_t handle_dc, handle_t select_handle) {
			if (handle_dc) {
				if (select_handle) {
					new_handle = select_handle;
					old_handle = SelectObject((HDC)handle_dc, get_new<HGDIOBJ>());
				}
			}
		}

		device_context_base::device_context_base() noexcept
			: handle_dc_(NULL) {

		}

		device_context_base::~device_context_base() {

		}

		bool device_context_base::is_valid() const noexcept {
			return handle_dc() != nullptr;
		}

		math::vec2i device_context_base::get_bitmap_size() const noexcept {
			if (handle_dc()) {
				return calc_bitmap_size(GetCurrentObject(handle_dc<HDC>(), OBJ_BITMAP));
			}
			return {};
		}

		static DWORD type_to_raster_op(bitblt_raster_op_type type) {
			switch (type)
			{
			case bitblt_raster_op_type::blackness: return BLACKNESS; break;
			case bitblt_raster_op_type::whiteness: return WHITENESS; break;
			case bitblt_raster_op_type::capture_blt: return CAPTUREBLT; break;
			case bitblt_raster_op_type::dest_invert: return DSTINVERT; break;
			case bitblt_raster_op_type::merge_copy: return MERGECOPY; break;
			case bitblt_raster_op_type::merge_paint: return MERGEPAINT; break;
			case bitblt_raster_op_type::no_mirrior_bitmap: return NOMIRRORBITMAP; break;
			case bitblt_raster_op_type::not_src_copy: return NOTSRCCOPY; break;
			case bitblt_raster_op_type::not_src_erase: return NOTSRCERASE; break;
			case bitblt_raster_op_type::pattern_copy: return PATCOPY; break;
			case bitblt_raster_op_type::pattern_invert: return PATINVERT; break;
			case bitblt_raster_op_type::pattern_paint: return PATPAINT; break;
			case bitblt_raster_op_type::src_and: return SRCAND; break;
			case bitblt_raster_op_type::src_copy: return SRCCOPY; break;
			case bitblt_raster_op_type::src_erase: return SRCERASE; break;
			case bitblt_raster_op_type::src_paint: return SRCPAINT; break;
			default:
				break;
			}

			//default.
			return SRCCOPY;
		}

		void device_context_base::bit_blt(device_context_base& dest_dc, const math::vec2i& dest_pos, const math::vec2i& src_pos, const math::vec2i& src_size, bitblt_raster_op_type rop) noexcept {
			if (this == &dest_dc) {
				ZEE_LOG(warning, TEXT("device_context"), TEXT("dest dc is equal to this dc."));
				return;
			}

			if (!dest_dc.handle_dc()) {
				ZEE_LOG(warning, TEXT("device_context"), TEXT("dest dc is null handle."));
				return;
			}

			if (!handle_dc()) {
				ZEE_LOG(warning, TEXT("device_context"), TEXT("this dc is null handle."));
				return;
			}

			if (!BitBlt(
				dest_dc.handle_dc<HDC>(),
				(int)dest_pos.x, (int)dest_pos.y,
				(int)src_size.x, (int)src_size.y,
				handle_dc<HDC>(),
				(int)src_pos.x, (int)src_pos.y,
				type_to_raster_op(rop)
			)) {
				ZEE_LOG_DETAIL(warning, TEXT("device_context"), TEXT("device_context_base::bitblt is failed. detail: [%s]"),
					win32helper::last_error_to_tstring().c_str()
				);
			}
		}

		void device_context_base::bit_blt(device_context_base& dest_dc, const math::vec2i& dest_pos) noexcept {
			bit_blt(dest_dc, dest_pos, math::vec2i::constants::zero, get_bitmap_size());
		}

		void device_context_base::transparent_blt(device_context_base& dest_dc, const math::vec2i& dest_pos, const math::vec2i& dest_size, const math::vec2i& src_pos, const math::vec2i& src_size, color transparent_color) noexcept {
			if (this == &dest_dc) {
				ZEE_LOG(warning, TEXT("device_context"), TEXT("dest dc is equal to this dc."));
				return;
			}

			if (!dest_dc.handle_dc()) {
				ZEE_LOG(warning, TEXT("device_context"), TEXT("dest dc is null handle."));
				return;
			}

			if (!handle_dc()) {
				ZEE_LOG(warning, TEXT("device_context"), TEXT("this dc is null handle."));
				return;
			}

			if (!GdiTransparentBlt(
				dest_dc.handle_dc<HDC>(),
				(int)dest_pos.x, (int)dest_pos.y,
				(int)dest_size.x, (int)dest_size.y,
				handle_dc<HDC>(),
				(int)src_pos.x, (int)src_pos.y,
				(int)src_size.x, (int)src_size.y,
				(UINT)transparent_color)) {
				ZEE_LOG_DETAIL(warning, TEXT("device_context"), TEXT("device_context_base::transparent_blt is failed. detail: [%s]"),
					win32helper::last_error_to_tstring().c_str()
				);
			}
		}

		void device_context_base::transparent_blt(device_context_base& dest_dc, const math::vec2i& dest_pos, color transparent_color) noexcept {
			transparent_blt(dest_dc, dest_pos, get_bitmap_size(), math::vec2i::constants::zero, get_bitmap_size(), transparent_color);
		}

		static BLENDFUNCTION make_blend_func(float alpha) noexcept {
			BLENDFUNCTION ret;
			ret.BlendOp = AC_SRC_OVER;
			ret.AlphaFormat = 0;	//0이면 알파 채널 무시. AC_SRC_ALPHA면 알파 소스 사용.
			ret.BlendFlags = 0;
			ret.SourceConstantAlpha = (BYTE)(math::clamp(alpha, 0.0f, 1.0f) * 255);
			return ret;
		}

		void device_context_base::alphablend(device_context_base& dest_dc, const math::vec2i& dest_pos, const math::vec2i& dest_size, const math::vec2i& src_pos, const math::vec2i& src_size, float alpha) noexcept {
			if (this == &dest_dc) {
				ZEE_LOG(warning, TEXT("device_context"), TEXT("dest dc is equal to this dc."));
				return;
			}

			if (!dest_dc.handle_dc()) {
				ZEE_LOG(warning, TEXT("device_context"), TEXT("dest dc is null handle."));
				return;
			}

			if (!handle_dc()) {
				ZEE_LOG(warning, TEXT("device_context"), TEXT("this dc is null handle."));
				return;
			}

			if (!GdiAlphaBlend(
				dest_dc.handle_dc<HDC>(),
				(int)dest_pos.x, (int)dest_pos.y,
				(int)dest_size.x, (int)dest_size.y,
				handle_dc<HDC>(),
				(int)src_pos.x, (int)src_pos.y,
				(int)src_size.x, (int)src_size.y,
				make_blend_func(alpha)
			)) {
				ZEE_LOG_DETAIL(warning, TEXT("device_context"), TEXT("device_context_base::alphablend is failed. detail: [%s]")
					, win32helper::last_error_to_tstring().c_str()
				);
			}
		}

		void device_context_base::alphablend(device_context_base& dest_dc, const math::vec2i& dest_pos, float alpha) noexcept {
			alphablend(dest_dc, dest_pos, get_bitmap_size(), math::vec2i::constants::zero, get_bitmap_size(), alpha);
		}

		void device_context_base::plg_blt(device_context_base& dest_dc, math::vec2f point, const float& angle, const math::vec2i& src_pos, const math::vec2i& src_size) noexcept {
			if (this == &dest_dc) {
				ZEE_LOG(warning, TEXT("device_context"), TEXT("dest dc is equal to this dc"));
				return;
			}
			if (!dest_dc.handle_dc()) {
				ZEE_LOG(warning, TEXT("device_context"), TEXT("dest dc is null handle."));
				return;
			}
			if (!handle_dc()) {
				ZEE_LOG(warning, TEXT("device_context"), TEXT("this dc is null handle."));
				return;
			}

			static math::vec2f t = { 200, 200 };
			static float x_scale = 1.0f;
			static float y_scale = 1.0f;
			math::vec2f m0 = { cos(angle) * x_scale, -sin(angle) * x_scale };
			math::vec2f m1 = { sin(angle) * y_scale, cos(angle) * y_scale };
			math::vec2f m2 = { t.x, t.y };


			math::vec2f v0 = { point.x - src_size.x / 2, point.y - src_size.y / 2 };
			math::vec2f v1 = { point.x + src_size.x / 2, point.y - src_size.y / 2 };
			math::vec2f v2 = { point.x - src_size.x / 2, point.y + src_size.y / 2 };

			auto m_mul_v = [](const math::vec2f& v, const math::vec2f& m0, const math::vec2f& m1, const math::vec2f& m2)
			{
				math::vec2f ret;
				ret.x =
					m0.x * v.x +
					m1.x * v.y +
					m2.x * 1.0f;
				ret.y =
					m0.y * v.x +
					m1.y * v.y +
					m2.y * 1.0f;
				return ret;
			};
			v0 = m_mul_v(v0, m0, m1, m2);
			v1 = m_mul_v(v1, m0, m1, m2);
			v2 = m_mul_v(v2, m0, m1, m2);

			std::vector<POINT> v_point;
			v_point.push_back({ (int)v0.x, (int)v0.y });
			v_point.push_back({ (int)v1.x, (int)v1.y });
			v_point.push_back({ (int)v2.x, (int)v2.y });

			math::matrix m;

			if (!PlgBlt(
				dest_dc.handle_dc<HDC>(),
				v_point.data(),
				handle_dc<HDC>(),
				(int)src_pos.x, (int)src_pos.y,
				(int)src_size.x, (int)src_size.y,
				0,
				0, 0
			)) {
				ZEE_LOG_DETAIL(warning, TEXT("device_context"), TEXT("device_context_base::PlgBlt is failed. detail: [%s]")
					, win32helper::last_error_to_tstring().c_str()
				);
			}
		}
		void device_context_base::plg_blt(device_context_base& dest_dc, math::vec2f point, const float& angle, const math::vec2i& src_size) noexcept {
			plg_blt(dest_dc, point, angle, {}, src_size);
		}


		void device_context_base::print_text(const math::vec2& pos, const tstring& str) noexcept {
			if (!TextOut(handle_dc<HDC>(), (int)pos.x, (int)pos.y, str.c_str(), (int)str.size())) {
				ZEE_LOG_DETAIL(warning, TEXT("device_context"), TEXT("device_context_base::print is failed. detail: [%s]"),
					zee::win32helper::last_error_to_tstring().c_str()
				);
			}
		}

		void device_context_base::rectangle(const shape::recti& rt) noexcept {
			rectangle(rt, math::vec2i::constants::zero);
		}

		void device_context_base::rectangle(const shape::recti& rt, const math::vec2i& pt) noexcept {
			if (!Rectangle(handle_dc<HDC>(), (int)rt.left + pt.x, (int)rt.top + pt.y, (int)rt.right + pt.x, (int)rt.bottom + pt.y)) {
				ZEE_LOG_DETAIL(warning, TEXT("device_context"), TEXT("device_context_base::rectangle is failed. detail: [%s]"),
					zee::win32helper::last_error_to_tstring().c_str()
				);
			}
		}

		void device_context_base::circle(const shape::circlei& cc) noexcept {
			circle(cc, math::vec2i::constants::zero);
		}

		void device_context_base::circle(const shape::circlei& cc, const math::vec2i& pt) noexcept {
			ellipse(
				(int)(cc.origin.x + pt.x - cc.radius), (int)(cc.origin.y + pt.y - cc.radius),
				(int)(cc.origin.x + pt.x + cc.radius), (int)(cc.origin.y + pt.y + cc.radius)
			);
		}

		void device_context_base::ellipse(int32 left, int32 top, int32 right, int32 bottom) noexcept {
			if (!Ellipse(handle_dc<HDC>(), (int)left, (int)top, (int)right, (int)bottom)) {
				ZEE_LOG_DETAIL(warning, TEXT("device_context"), TEXT("device_context_base::ellipse is failed. detail: [%s]"),
					zee::win32helper::last_error_to_tstring().c_str()
				);
			}
		}

		void device_context_base::change_brush_color(const color new_color) noexcept {
			assert(handle_dc());
			SelectObject(handle_dc<HDC>(), GetStockObject(DC_BRUSH));
			SetDCBrushColor(handle_dc<HDC>(), (COLORREF)new_color);
		}

		void device_context_base::move_to(const math::vec2i& dot) noexcept {
			assert(handle_dc());
			MoveToEx(handle_dc<HDC>(), (int)dot.x, (int)dot.y, NULL);
		}

		void device_context_base::line_to(const math::vec2i& line) noexcept {
			assert(handle_dc());
			LineTo(handle_dc<HDC>(), (int)line.x, (int)line.y);
		}

		void device_context_base::change_pen_color(const color new_color) noexcept {
			assert(handle_dc());
			SelectObject(handle_dc<HDC>(), GetStockObject(DC_PEN));
			SetDCPenColor(handle_dc<HDC>(), (COLORREF)new_color);
		}

		math::vec2i device_context_base::calc_bitmap_size(handle_t handle_bitmap) {
			math::vec2i ret;
			if (GetObjectType(handle_bitmap) == OBJ_BITMAP) {
				BITMAP bmp;
				GetObject(handle_bitmap, sizeof(BITMAP), &bmp);
				ret.x = (math::vec2i::element_type)bmp.bmWidth;
				ret.y = (math::vec2i::element_type)bmp.bmHeight;
			}

			return ret;
		}

		device_context_auto::device_context_auto(handle_t window_handle, device_context_auto_type new_type) noexcept
			: window_handle_(window_handle), type_(new_type) {
			switch (get_type())
			{
			case device_context_auto_type::paint:
			{
				buf_.resize(sizeof(PAINTSTRUCT));
				handle_dc_ = BeginPaint((HWND)window_handle, (LPPAINTSTRUCT)buf_.data());
			}
			break;

			case device_context_auto_type::temp:
			{
				handle_dc_ = GetDC((HWND)window_handle);
			}
			break;

			case device_context_auto_type::none:
			default:
				break;
			}

			assert(handle_dc_ && window_handle_ && get_type() != device_context_auto_type::none);
		}

		device_context_auto::~device_context_auto() noexcept {
			switch (get_type())
			{
			case device_context_auto_type::paint:
			{
				const BOOL success = EndPaint((HWND)window_handle_, (LPPAINTSTRUCT)buf_.data());
				assert(success);
				buf_.clear();
				handle_dc_ = NULL;
			}
			break;

			case device_context_auto_type::temp:
			{
				const int success = ReleaseDC((HWND)window_handle_, (HDC)handle_dc_);
				assert(success);
			}
			break;

			default:
				break;
			}
		}

		bool device_context_dynamic::clone(device_context_dynamic& out_object) {
			assert(this != &out_object);
			out_object.clear();
			if (is_valid()) {
				const auto bitmap_size = get_bitmap_size();
				if (!bitmap_size.is_zero()) {
					out_object.create_empty_image(bitmap_size);
					bit_blt(out_object, {});
				}
			}
			return true;
		}

		device_context_dynamic::device_context_dynamic() {

		}

		device_context_dynamic::device_context_dynamic(device_context_dynamic&& other) noexcept
			: device_context_dynamic() {
			move_from(std::move(other));
		}

		device_context_dynamic& device_context_dynamic::operator=(device_context_dynamic&& other) noexcept {
			if (this != &other) {
				move_from(std::move(other));
			}
			return *this;
		}

		device_context_dynamic::~device_context_dynamic() {
			clear();
		}

		bool device_context_dynamic::load_image(const tstring& str) {
			if (!create_if_has_no_dc()) {
				return false;
			}

			handle_t new_image_handle = NULL;
			image_load_result ret = win32gdi::load_image(new_image_handle, str);
			if (ret == image_load_result::success) {
				bitmap_desc_.size = calc_bitmap_size(new_image_handle);
				bitmap_desc_.load_type = image_load_type::file;
				bitmap_desc_.type = image_type::bmp;
				bitmap_desc_.file_path = str;
				handle_image_.clear(handle_dc());
				handle_image_.select(handle_dc(), new_image_handle);
				return true;
			}

			return false;
		}

		bool device_context_dynamic::create_empty_image(const math::vec2i& new_size) {
			assert(new_size.x > 0 && new_size.y > 0);
			if (application::get().is_started()) {
				if (create_if_has_no_dc()) {
					device_context_auto temp_dc(application::get().window_handle<HWND>(), device_context_auto_type::temp);
					handle_t new_image_handle = CreateCompatibleBitmap(temp_dc.handle_dc<HDC>(), (int)new_size.x, (int)new_size.y);
					if (!new_image_handle) {
						ZEE_LOG_DETAIL(warning, TEXT("device_context"), TEXT("device_context_dynamic::create_empty_image is failed. detail: [%s]"),
							zee::win32helper::last_error_to_tstring().c_str()
						);
						return false;
					}

					bitmap_desc_.size = new_size;
					bitmap_desc_.load_type = image_load_type::buffer;
					bitmap_desc_.type = image_type::bmp;
					handle_image_.clear(handle_dc());
					handle_image_.select(handle_dc(), new_image_handle);
					return true;
				}
			}

			return false;
		}

		void device_context_dynamic::clear() noexcept {
			if (handle_dc_) {
				handle_image_.clear(handle_dc());
				DeleteObject(handle_dc_);
				handle_dc_ = NULL;
			}
		}

		void device_context_dynamic::resize(const math::vec2i& new_size) {
			if (new_size.x > 0 && new_size.y > 0) {
				if (get_bitmap_size() != new_size) {
					create_empty_image(new_size);
				}
			}
		}

		void device_context_dynamic::move_from(device_context_dynamic&& other) noexcept {
			if (this != &other) {
				clear();
				handle_image_ = other.handle_image_;
				bitmap_desc_ = std::move(other.bitmap_desc_);
				handle_dc_ = other.handle_dc_;

				other.handle_image_ = handle_pair{};
				other.bitmap_desc_.clear();
				other.handle_dc_ = NULL;
			}
		}

		bool device_context_dynamic::create_if_has_no_dc() {
			if (application::get().is_started()) {
				if (!handle_dc()) {
					device_context_auto temp_dc(application::get().window_handle(), device_context_auto_type::temp);
					handle_dc_ = CreateCompatibleDC(temp_dc.handle_dc<HDC>());
					if (!handle_dc()) {
						ZEE_LOG_DETAIL(warning, TEXT("device_context"), TEXT("device_context_dynamic::create_if_has_no_dc is failed. detail: [%s]"),
							zee::win32helper::last_error_to_tstring().c_str()
						);
						return false;
					}

					return true;

				}
				else {
					ZEE_LOG(warning, TEXT("device_context"), TEXT("Device context was already created."));
					return true;
				}

			}
			else {
				ZEE_LOG_DETAIL(warning, TEXT("device_context"), TEXT("Application is not started."));
				return false;
			}
		}

		bool is_valid(const device_context_base& dc) {
			return dc.is_valid();
		}

		void change_color(const color new_brush_color) noexcept {
		}

	}//namespace zee::win32gdi
}//namespace zee