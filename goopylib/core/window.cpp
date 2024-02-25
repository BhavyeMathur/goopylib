#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <map-macro/map.h>

#include "window/Window.h"

namespace py = pybind11;
using namespace pybind11::literals;

#define GP_PYOBJ const py::object &
#define GP_INT_SUFFIX(arg) arg##_int
#define GP_KWARG(kwarg) py::arg(#kwarg)
#define GP_RETHROW_TYPE_ERROR(code, type) try { code } catch (std::runtime_error &e) { throw py::type_error("Expected " #type " argument"); };

#define GP_OPTIONAL_TO_INT(arg) GP_INT_SUFFIX(arg) = (arg.is_none() ? -1 : arg.cast<int>());
#define GP_SAFE_OPTIONAL_TO_INT(arg) int GP_INT_SUFFIX(arg); GP_RETHROW_TYPE_ERROR(GP_OPTIONAL_TO_INT(arg), int)
#define GP_SAFE_OPTIONALS_TO_INT(...) MAP(GP_SAFE_OPTIONAL_TO_INT, __VA_ARGS__)

#define GP_SIZE_LIMIT_SETTER_BODY(func, ...) GP_SAFE_OPTIONALS_TO_INT(__VA_ARGS__) self.set##func(MAP_LIST(GP_INT_SUFFIX, __VA_ARGS__))
#define GP_SIZE_LIMIT_SETTER(func, ...) [](gp::Window &self, MAP_LIST(GP_PYOBJ, __VA_ARGS__)) {GP_SIZE_LIMIT_SETTER_BODY(func, __VA_ARGS__);}
#define GP_SET_SIZE_LIMIT(func, ...) GP_SIZE_LIMIT_SETTER(func, __VA_ARGS__), MAP_LIST(GP_KWARG, __VA_ARGS__)

PYBIND11_MODULE(window, m) {
    py::class_<gp::Window>(m, "Window")
            .def(py::init<int, int, const char *>(), "width"_a, "height"_a, "title"_a = "goopylib Window")
            .def("__repr__", &gp::Window::toString)

            .def("is_closed", &gp::Window::isClosed)
            .def("is_open", &gp::Window::isOpen)
            .def("is_destroyed", &gp::Window::isDestroyed)

            .def("update", &gp::Window::update)
            .def("destroy", &gp::Window::destroy)

            .def("set_size", &gp::Window::setSize, "width"_a, "height"_a)
            .def("set_size_limits", GP_SET_SIZE_LIMIT(SizeLimits, min_width, min_height, max_width, max_height))
            .def("set_min_size", GP_SET_SIZE_LIMIT(MinSize, min_width, min_height))
            .def("set_max_size", GP_SET_SIZE_LIMIT(MaxSize, max_width, max_height))

            .def("set_aspect_ratio", &gp::Window::setAspectRatio, "numerator"_a, "denominator"_a)
            .def("get_aspect_ratio", &gp::Window::getAspectRatio)
            .def("get_frame_size", &gp::Window::getFrameSize)
            .def("get_content_scale", &gp::Window::getContentScale)
            .def("get_framebuffer_size", &gp::Window::getFramebufferSize)

            .def("restore", &gp::Window::restore)
            .def("fullscreen", &gp::Window::fullscreen)
            .def("is_fullscreen", &gp::Window::isFullscreen)
            .def("minimize", &gp::Window::minimize)
            .def("is_minimized", &gp::Window::isMinimized)
            .def("maximize", &gp::Window::maximize)
            .def("is_maximized", &gp::Window::isMaximized)
            .def("show", &gp::Window::show)
            .def("hide", &gp::Window::hide, "hide"_a = py::bool_(true))
            .def("is_visible", &gp::Window::isVisible)
            .def("focus", &gp::Window::focus)
            .def("has_focus", &gp::Window::hasFocus)
            .def("request_attention", &gp::Window::requestAttention)

            .def("is_mouse_hovering", &gp::Window::isMouseHovering)
            .def("get_mouse_position", &gp::Window::getMousePosition)
            .def("set_cursor_mode", &gp::Window::setCursorMode, "mode"_a)
            .def("check_mouse_button", &gp::Window::checkMouseButton, "button"_a)
            .def("check_left_click", &gp::Window::checkLeftClick)
            .def("check_middle_click", &gp::Window::checkMiddleClick)
            .def("check_right_click", &gp::Window::checkRightClick)

            .def("check_shift_key", &gp::Window::checkShiftKey)
            .def("check_control_key", &gp::Window::checkControlKey)
            .def("check_alt_key", &gp::Window::checkAltKey)
            .def("check_super_key", &gp::Window::checkSuperKey)
            .def("check_key", &gp::Window::checkKey, "keycode"_a)

            .def_property("resize_callback", py::cpp_function(), &gp::Window::setResizeCallback)
            .def_property("close_callback", py::cpp_function(), &gp::Window::setCloseCallback)
            .def_property("destroy_callback", py::cpp_function(), &gp::Window::setDestroyCallback)
            .def_property("position_callback", py::cpp_function(), &gp::Window::setPositionCallback)
            .def_property("minimize_callback", py::cpp_function(), &gp::Window::setMinimizeCallback)
            .def_property("maximize_callback", py::cpp_function(), &gp::Window::setMaximizeCallback)
            .def_property("focus_callback", py::cpp_function(), &gp::Window::setFocusCallback)
            .def_property("refresh_callback", py::cpp_function(), &gp::Window::setRefreshCallback)
            .def_property("content_scale_callback", py::cpp_function(), &gp::Window::setContentScaleCallback)
            .def_property("framebuffer_size_callback", py::cpp_function(), &gp::Window::setFramebufferSizeCallback)
            .def_property("mouse_motion_callback", py::cpp_function(), &gp::Window::setMouseMotionCallback)
            .def_property("mouse_enter_callback", py::cpp_function(), &gp::Window::setMouseEnterCallback)
            .def_property("scroll_callback", py::cpp_function(), &gp::Window::setScrollCallback)

            .def("set_key_callback", &gp::Window::setKeyCallback)
            .def("set_mouse_button_callback", &gp::Window::setMouseButtonCallback)
            .def_property("left_click_callback", py::cpp_function(), &gp::Window::setLeftClickCallback)
            .def_property("middle_click_callback", py::cpp_function(), &gp::Window::setMiddleClickCallback)
            .def_property("right_click_callback", py::cpp_function(), &gp::Window::setRightClickCallback)

            .def("get_camera", &gp::Window::getCamera)
            .def("to_world", [](const gp::Window &self, int x, int y) {
                auto p = self.toWorld({x, y});
                return py::make_tuple(p.x, p.y);
            }, "x"_a, "y"_a)

            .def("to_screen", [](const gp::Window &self, int x, int y) {
                auto p = self.toScreen({x, y});
                return py::make_tuple(p.x, p.y);
            }, "x"_a, "y"_a)

            .def_property("title", &gp::Window::getTitle, &gp::Window::setTitle)
            .def_property("background", &gp::Window::getBackground, &gp::Window::setBackground)
            .def_property("xpos", &gp::Window::getXPos, &gp::Window::setXPos)
            .def_property("ypos", &gp::Window::getYPos, &gp::Window::setYPos)
            .def_property("position", &gp::Window::getPosition, &gp::Window::setPosition)

            .def_property("width", &gp::Window::getWidth, &gp::Window::setWidth)
            .def_property("height", &gp::Window::getHeight, &gp::Window::setHeight)

            .def_property("min_width", &gp::Window::getMinWidth, GP_SIZE_LIMIT_SETTER(MinWidth, value))
            .def_property("min_height", &gp::Window::getMinHeight, GP_SIZE_LIMIT_SETTER(MinHeight, value))
            .def_property("max_width", &gp::Window::getMaxWidth, GP_SIZE_LIMIT_SETTER(MaxWidth, value))
            .def_property("max_height", &gp::Window::getMaxHeight, GP_SIZE_LIMIT_SETTER(MaxHeight, value))

            .def_property("resizable", &gp::Window::isResizable, &gp::Window::setResizable)
            .def_property("decorated", &gp::Window::isDecorated, &gp::Window::setDecorated)
            .def_property("floating", &gp::Window::isFloating, &gp::Window::setFloating)
            .def_property("auto_minimized", &gp::Window::isAutoMinimized, &gp::Window::setAutoMinimized)
            .def_property("focused_on_show", &gp::Window::isFocusedOnShow, &gp::Window::setFocusedOnShow)

            .def_static("update_all", &gp::Window::updateAll)
            .def_static("destroy_all", &gp::Window::destroyAll);
}
