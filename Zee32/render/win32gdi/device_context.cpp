#include "device_context.h"
#include "application/application.h"
#include "core/core.h"
#include "log/log.h"
#include <Windows.h>

namespace zee {
namespace win32gdi {
	
	handle_t default_font_handle() {
		return GetStockObject(DEFAULT_GUI_FONT);
	}

	handle_t default_brush_handle() {
		return GetStockObject(WHITE_BRUSH);
	}

	handle_t default_pen_handle() {
		return GetStockObject(BLACK_PEN);
	}

	device_context_base::device_context_base() noexcept
		: dc_handle_(NULL) {

	}

	device_context_base::~device_context_base() {

	}

	bool device_context_base::is_valid() const noexcept {
		return dc_handle() != nullptr;
	}

	const math::size2& device_context_base::get_bitmap_size() const noexcept {
		return bitmap_size_;
	}

	const simple_bitmap_desc& device_context_base::get_bitmap_desc() const noexcept {
		return bitmap_desc_;
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

	void device_context_base::bit_blt(device_context_base& dest_dc, const math::point& dest_pos, const math::point& src_pos, const math::size2& src_size,  bitblt_raster_op_type rop) noexcept {
		if (this == &dest_dc) {
			ZEE_LOG(log::verbose::warning, _T("dest dc is equal to this dc."));
			return;
		}

		if (!dest_dc.dc_handle()) {
			ZEE_LOG(log::verbose::warning, _T("dest dc is null handle."));
			return;
		}

		if (!dc_handle()) {
			ZEE_LOG(log::verbose::warning, _T("this dc is null handle."));
			return;
		}

		if (!BitBlt(
			dest_dc.dc_handle<HDC>(), 
			(int)dest_pos.x, (int)dest_pos.y, 
			(int)src_size.width, (int)src_size.height,
			dc_handle<HDC>(), 
			(int)src_pos.x, (int)src_pos.y,
			type_to_raster_op(rop)
		)) {
			ZEE_LOG_DETAIL(log::verbose::warning, TEXT("device_context_base::bitblt is failed. detail: [%s]"), log::last_error_to_string().c_str());
		}
	}

	void device_context_base::bit_blt(device_context_base& dest_dc, const math::point& dest_pos) noexcept {
		bit_blt(dest_dc, dest_pos, math::point::constants::zero, get_bitmap_size());
	}

	void device_context_base::transparent_blt(device_context_base& dest_dc, const math::point& dest_pos, const math::size2& dest_size, const math::point& src_pos, const math::size2& src_size,  color transparent_color) noexcept {
		if (this == &dest_dc) {
			ZEE_LOG(log::verbose::warning, _T("dest dc is equal to this dc."));
			return;
		}

		if (!dest_dc.dc_handle()) {
			ZEE_LOG(log::verbose::warning, _T("dest dc is null handle."));
			return;
		}

		if (!dc_handle()) {
			ZEE_LOG(log::verbose::warning, _T("this dc is null handle."));
			return;
		}

		if (!GdiTransparentBlt(
			dest_dc.dc_handle<HDC>(),
			(int)dest_pos.x     , (int)dest_pos.y,
			(int)dest_size.width, (int)dest_size.height,
			dc_handle<HDC>(),
			(int)src_pos.x      , (int)src_pos.y,
			(int)src_size.width , (int)src_size.height,
			(UINT)transparent_color.rgba)) {
			ZEE_LOG_DETAIL(log::verbose::warning, TEXT("device_context_base::transparent_blt is failed. detail: [%s]"), log::last_error_to_string().c_str());
		}
	}

	void device_context_base::transparent_blt(device_context_base& dest_dc, const math::point& dest_pos, color transparent_color) noexcept {
		transparent_blt(dest_dc, dest_pos, get_bitmap_size(), math::point::constants::zero, get_bitmap_size(), transparent_color);
	}

	static BLENDFUNCTION make_blend_func(float alpha) noexcept {
		BLENDFUNCTION ret;
		ret.BlendOp = AC_SRC_OVER;
		ret.AlphaFormat = 0;
		ret.BlendFlags = 0;
		ret.SourceConstantAlpha = (BYTE)(math::clamp(alpha, 0.0f, 1.0f) * 255);
		return ret;
	}

	void device_context_base::alphablend(device_context_base& dest_dc, const math::point& dest_pos, const math::size2& dest_size, const math::point& src_pos, const math::size2& src_size, float alpha) noexcept {
		if (this == &dest_dc) {
			ZEE_LOG(log::verbose::warning, _T("dest dc is equal to this dc."));
			return;
		}

		if (!dest_dc.dc_handle()) {
			ZEE_LOG(log::verbose::warning, _T("dest dc is null handle."));
			return;
		}

		if (!dc_handle()) {
			ZEE_LOG(log::verbose::warning, _T("this dc is null handle."));
			return;
		}

		if (!GdiAlphaBlend(
			dest_dc.dc_handle<HDC>(),
			(int)dest_pos.x, (int)dest_pos.y,
			(int)dest_size.width, (int)dest_size.height,
			dc_handle<HDC>(),
			(int)src_pos.x, (int)src_pos.y,
			(int)src_size.width, (int)src_size.height,
			make_blend_func(alpha)
		)) {
			ZEE_LOG_DETAIL(log::verbose::warning, TEXT("device_context_base::alphablend is failed. detail: [%s]"), log::last_error_to_string().c_str());
		}
	}

	void device_context_base::alphablend(device_context_base& dest_dc, const math::point& dest_pos, float alpha) noexcept {
		alphablend(dest_dc, dest_pos, get_bitmap_size(), math::point::constants::zero, get_bitmap_size(), alpha);
	}

	void device_context_base::print_text(const math::vec2& pos, const tstring& str) noexcept {
		if (!TextOut(dc_handle<HDC>(), (int)pos.x, (int)pos.y, str.c_str(), (int)str.size())) {
			ZEE_LOG_DETAIL(log::verbose::warning, TEXT("device_context_base::print is failed. detail: [%s]"), log::last_error_to_string().c_str());
		}
	}

	void device_context_base::rectangle(const math::rect& rt) noexcept {
		if(!Rectangle(dc_handle<HDC>(), (int)rt.left, (int)rt.top, (int)rt.right, (int)rt.bottom)) {
			ZEE_LOG_DETAIL(log::verbose::warning, TEXT("device_context_base::rectangle is failed. detail: [%s]"), log::last_error_to_string().c_str());
		}
	}

	void device_context_base::circle(const math::circle& cc) noexcept {
		if (!Ellipse(dc_handle<HDC>(),
			(int)(cc.pt.x - cc.radius), (int)(cc.pt.y - cc.radius),
			(int)(cc.pt.x + cc.radius), (int)(cc.pt.y + cc.radius)
		)) {
			ZEE_LOG_DETAIL(log::verbose::warning, TEXT("device_context_base::circle is failed. detail: [%s]"), log::last_error_to_string().c_str());
		}
	}

	void device_context_base::ellipse(int32 left, int32 top, int32 right, int32 bottom) noexcept {
		if (!Ellipse(dc_handle<HDC>(), (int)left, (int)top, (int)right, (int)bottom)) {
			ZEE_LOG_DETAIL(log::verbose::warning, TEXT("device_context_base::ellipse is failed. detail: [%s]"), log::last_error_to_string().c_str());
		}
	}

#define RELEASE_HANDLE_DEFINE(handle_name) \
	if (is_valid()) { \
		if (handle_name##_handle_.new_handle) { \
			SelectObject((HDC)dc_handle_, (HGDIOBJ)handle_name##_handle_.old_handle); \
			DeleteObject((HGDIOBJ)handle_name##_handle_.new_handle); \
		} \
	}

	void device_context_base::release_image_handle() {
		RELEASE_HANDLE_DEFINE(image);
	}

	void device_context_base::release_font_handle() {
		RELEASE_HANDLE_DEFINE(font);
	}

	void device_context_base::release_brush_handle() {
		RELEASE_HANDLE_DEFINE(brush);
	}

	void device_context_base::release_pen_handle() {
		RELEASE_HANDLE_DEFINE(pen);
	}

#undef RELEASE_HANDLE_DEFINE

	math::size2 device_context_base::calc_bitmap_size(handle_t bitmap_handle) {
		math::size2 ret;
		if (GetObjectType(bitmap_handle) == OBJ_BITMAP){
			SIZE size = {0, 0};
			BITMAP bmp;
			GetObject(bitmap_handle, sizeof(BITMAP), &bmp);
			ret.width = (math::size2::element_type)bmp.bmWidth;
			ret.height = (math::size2::element_type)bmp.bmHeight;
		}

		return ret;
	}

	device_context_auto::device_context_auto(handle_t window_handle, device_context_auto_type new_type) noexcept 
		: window_handle_(NULL), type_(device_context_auto_type::none) {
		
		type_ = new_type;
		window_handle_ = window_handle;

		switch (get_type())
		{
		case device_context_auto_type::paint:
		{
			buffer_.resize(sizeof(PAINTSTRUCT));
			dc_handle_ = BeginPaint((HWND)window_handle, (LPPAINTSTRUCT)buffer_.data());
		}
		break;

		case device_context_auto_type::temp:
		{
			dc_handle_ = GetDC((HWND)window_handle);
		}
		break;

		default:
			break;
		}

		bitmap_size_ = calc_bitmap_size(GetCurrentObject((HDC)dc_handle_, OBJ_BITMAP));
		assert(dc_handle_ && window_handle_ && get_type() != device_context_auto_type::none);
	}

	device_context_auto::~device_context_auto() noexcept {
		switch (get_type())
		{
		case device_context_auto_type::paint:
		{
			BOOL success = EndPaint((HWND)window_handle_, (LPPAINTSTRUCT)buffer_.data());
			assert(success);
			buffer_.clear();
			dc_handle_ = NULL;
		}
		break;

		case device_context_auto_type::temp:
		{
			int success = ReleaseDC((HWND)window_handle_, (HDC)dc_handle_);
			assert(success);
		}
		break;

		default:
			break;
		}
	}

	void device_context_dynamic::clone(void* out_device_context_ptr) {
		if (device_context_dynamic* clone_target = static_cast<device_context_dynamic*>(out_device_context_ptr)) {

		}
	}

	device_context_dynamic::device_context_dynamic() {

		if (GetObjectType(dc_handle_) != OBJ_DC) {
			dc_handle_ = NULL;
		}

		if (dc_handle_) {
			handle_t current_handle = NULL;
			current_handle = GetCurrentObject((HDC)dc_handle_, OBJ_BRUSH);
			if (current_handle != default_brush_handle()) {
				brush_handle_.old_handle = default_brush_handle();
				brush_handle_.new_handle = current_handle;
			}

			current_handle = GetCurrentObject((HDC)dc_handle_, OBJ_FONT);
			if (current_handle != default_font_handle()) {
				font_handle_.old_handle = default_font_handle();
				font_handle_.new_handle = current_handle;
			}

			current_handle = GetCurrentObject((HDC)dc_handle_, OBJ_PEN);
			if (current_handle != default_pen_handle()) {
				pen_handle_.old_handle = default_pen_handle();
				pen_handle_.new_handle = current_handle;
			}

		}
	}

	device_context_dynamic::device_context_dynamic(device_context_dynamic&& other) noexcept 
		: device_context_dynamic()
	{
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

#define ZEE_SELECT_GDIOBJ(handle_name, new_handle_object)  \
		handle_name##_handle_.new_handle = new_handle_object; \
		handle_name##_handle_.old_handle = SelectObject((HDC)dc_handle_, handle_name##_handle_.new_handle);  \

	bool device_context_dynamic::load_image(const tstring& str) {
		if (!create_if_has_no_dc()) {
			return false;
		}

		handle_t new_image_handle = NULL;
		image_load_result ret = win32gdi::load_image(new_image_handle, str);
		if (ret == image_load_result::success) {
			release_image_handle();
			bitmap_size_ = calc_bitmap_size(new_image_handle);
			bitmap_desc_.size = bitmap_size_;
			bitmap_desc_.load_type = image_load_type::buffer;
			bitmap_desc_.type = image_type::bmp;
			bitmap_desc_.file_path = str;
			ZEE_SELECT_GDIOBJ(image, new_image_handle);
			return true;
		}

		return false;
	}

	bool device_context_dynamic::create_empty_image(const math::size2& new_size) {
		assert(new_size.width > 0 && new_size.height > 0);
		if (application::is_application_started()) {
			if (create_if_has_no_dc()) {
				handle_t new_image_handle = CreateCompatibleBitmap((HDC)dc_handle_, (int)new_size.width, (int)new_size.height);
				if (!new_image_handle) {
					ZEE_LOG_DETAIL(log::verbose::warning, TEXT("device_context_dynamic::create_empty_image is failed. detail: [%s]"), log::last_error_to_string().c_str());
					return false;
				}

				release_image_handle();
				bitmap_desc_.size = new_size;
				bitmap_desc_.load_type = image_load_type::buffer;
				bitmap_desc_.type = image_type::bmp;
				bitmap_size_ = new_size;
				ZEE_SELECT_GDIOBJ(image, new_image_handle);
				return true;
			}
		}

		return false;
	}

	void device_context_dynamic::clear() noexcept {
		if (dc_handle_) {
			release_image_handle();
			release_font_handle();
			release_brush_handle();
			release_pen_handle();
			DeleteObject(dc_handle_);
			dc_handle_ = NULL;
		}
	}

	void device_context_dynamic::move_from(device_context_dynamic&& other) noexcept {
		if (this != &other) {
			clear();
			image_handle_ = std::move(other.image_handle_);
			font_handle_ = std::move(other.font_handle_);
			brush_handle_ = std::move(other.brush_handle_);
			pen_handle_ = std::move(other.pen_handle_);
			bitmap_desc_ = std::move(other.bitmap_desc_);
			dc_handle_ = other.dc_handle_;
			other.bitmap_desc_.clear();
			other.dc_handle_ = NULL;
		}
	}

	bool device_context_dynamic::create_if_has_no_dc() {
		if (application::is_application_started()) {
			if (!dc_handle()) {
				device_context_auto temp_dc(application::get().window_handle(), device_context_auto_type::temp);
				dc_handle_ = CreateCompatibleDC(temp_dc.dc_handle<HDC>());
				if (!dc_handle()) {
					ZEE_LOG_DETAIL(log::verbose::warning, TEXT("device_context_dynamic::create_if_has_no_dc is failed. detail: [%s]"), log::last_error_to_string().c_str());
					return false;
				}

				return true;

			} else {
				ZEE_LOG(zee::log::verbose::warning, TEXT("Device context was already created."));
				return true;
			}

		} else {
			ZEE_LOG_DETAIL(zee::log::verbose::warning, TEXT("Application is not started."));
			return false;
		}
	}

	void device_context_dynamic::release_image_handle() { 
		parent_class::release_image_handle();
		bitmap_desc_.clear();
	}

	bool is_valid(const device_context_base& dc) {
		return dc.is_valid();
	}

	device_context_dynamic::handle_pair::handle_pair(handle_pair&& other) noexcept 
		: old_handle(other.old_handle), new_handle(other.new_handle) {
		other.new_handle = other.old_handle = NULL;
	}

	device_context_dynamic::handle_pair& device_context_dynamic::handle_pair::operator=(handle_pair&& other) noexcept {
		if (this != &other) {
			new_handle = other.new_handle; old_handle = other.old_handle;
			other.new_handle = other.old_handle = NULL;
		}

		return *this;
	}
}//namespace zee::win32gdi
}//namespace zee