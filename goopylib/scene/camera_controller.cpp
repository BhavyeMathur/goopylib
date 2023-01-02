#include "camera_controller.h"
#include "src/goopylib/scene/CameraController.h"

#include "goopylib/core/window_module.h"
#include "goopylib/core/window_object.h"

#include "config.h"

#if (GP_LOG_CAMERA_CONTROLLER != true) and (GP_LOG_CAMERA_CONTROLLER <= GP_LOGGING_LEVEL)
#undef GP_LOGGING_LEVEL
#define GP_LOGGING_LEVEL GP_LOG_CAMERA_CONTROLLER
#endif

#if !GP_VALUE_CHECK_CAMERA_CONTROLLER
#undef GP_VALUE_CHECKING
#undef GP_TYPE_CHECKING
#undef GP_ERROR_CHECKING
#endif

#include "goopylib/debug.h"


struct CameraControllerObject {
    PyObject_HEAD
    Ref<gp::CameraController> controller;
};

// Camera Controller Core
namespace controller {
    static PyObject *new_(PyTypeObject *type, PyObject *Py_UNUSED(args), PyObject *Py_UNUSED(kwds)) {
        GP_PY_DEBUG("gp.camera_controller.CameraController.__new__()");

        CameraControllerObject *self;
        self = (CameraControllerObject *) type->tp_alloc(type, 0);

        if (self != nullptr) {

        }
        return (PyObject *) self;
    }

    static int init(CameraControllerObject *self, PyObject *args, PyObject *Py_UNUSED(kwds)) {
        GP_PY_INFO("gp.camera_controller.CameraController()");

        PyObject *window;
        if (!PyArg_ParseTuple(args, "O!", WindowType, &window)) {
            return -1;
        }

        auto win = ((WindowObject *) window)->window.get();
        self->controller = std::shared_ptr<gp::CameraController>(new gp::CameraController(win));

        return 0;
    }

    static PyObject *repr(CameraControllerObject *Py_UNUSED(self)) {
        GP_PY_TRACE("gp.camera_controller.CameraController.__repr__()");
        return PyUnicode_FromString("CameraController()");
    }

    static int traverse(CameraControllerObject *Py_UNUSED(self), visitproc Py_UNUSED(visit), void *Py_UNUSED(arg)) {
        return 0;
    }

    static int clear(CameraControllerObject *Py_UNUSED(self)) {
        GP_PY_TRACE("gp.camera_controller.CameraController.clear()");
        return 0;
    }

    static void dealloc(CameraControllerObject *self) {
        GP_PY_DEBUG("gp.camera_controller.CameraController.__dealloc__()");

        self->controller.reset();

        PyObject_GC_UnTrack(self);
        clear(self);
        Py_TYPE(self)->tp_free((PyObject *) self);
    }
}

// Camera Controller methods
namespace controller {
    static PyObject *update(CameraControllerObject *self, PyObject *Py_UNUSED(args)) {
        self->controller->update();
        Py_RETURN_NONE;
    }

    static PyObject *enable_movement(CameraControllerObject *self, PyObject *arg) {
        #if GP_TYPE_CHECKING
        if (!PyBool_Check(arg)) {
            RAISE_TYPE_ERROR(nullptr, "bool", arg);
        }
        #endif

        self->controller->enableMovement(arg == Py_True);
        Py_RETURN_NONE;
    }

    static PyObject *enable_rotation(CameraControllerObject *self, PyObject *arg) {
        #if GP_TYPE_CHECKING
        if (!PyBool_Check(arg)) {
            RAISE_TYPE_ERROR(nullptr, "bool", arg);
        }
        #endif

        self->controller->enableRotation(arg == Py_True);
        Py_RETURN_NONE;
    }

    static PyObject *enable_zoom(CameraControllerObject *self, PyObject *arg) {
        #if GP_TYPE_CHECKING
        if (!PyBool_Check(arg)) {
            RAISE_TYPE_ERROR(nullptr, "bool", arg);
        }
        #endif

        self->controller->enableZoom(arg == Py_True);
        Py_RETURN_NONE;
    }

    static PyObject *invert_movement(CameraControllerObject *self, PyObject *arg) {
        #if GP_TYPE_CHECKING
        if (!PyBool_Check(arg)) {
            RAISE_TYPE_ERROR(nullptr, "bool", arg);
        }
        #endif

        self->controller->invertMovement(arg == Py_True);
        Py_RETURN_NONE;
    }

    static PyObject *invert_rotation(CameraControllerObject *self, PyObject *arg) {
        #if GP_TYPE_CHECKING
        if (!PyBool_Check(arg)) {
            RAISE_TYPE_ERROR(nullptr, "bool", arg);
        }
        #endif

        self->controller->invertRotation(arg == Py_True);
        Py_RETURN_NONE;
    }

    static PyObject *use_wasd(CameraControllerObject *self, PyObject *Py_UNUSED(args)) {
        self->controller->useWASD();
        Py_RETURN_NONE;
    }

    static PyObject *use_arrows(CameraControllerObject *self, PyObject *Py_UNUSED(args)) {
        self->controller->useArrows();
        Py_RETURN_NONE;
    }

    static PyObject *set_movement_keys(CameraControllerObject *self, PyObject *args) {
        int up, left, down, right;
        if (!PyArg_ParseTuple(args, "iiii", &up, &left, &down, &right)) {
            return nullptr;
        }

        self->controller->setMovementKeys(up, left, down, right);
        Py_RETURN_NONE;
    }

    static PyObject *set_rotation_keys(CameraControllerObject *self, PyObject *args) {
        int clockwise, anticlockwise;
        if (!PyArg_ParseTuple(args, "ii", &clockwise, &anticlockwise)) {
            return nullptr;
        }

        self->controller->setRotationKeys(clockwise, anticlockwise);
        Py_RETURN_NONE;
    }

    static PyObject *set_zoom_keys(CameraControllerObject *self, PyObject *args) {
        int in, out;
        if (!PyArg_ParseTuple(args, "ii", &in, &out)) {
            return nullptr;
        }

        self->controller->setZoomKeys(in, out);
        Py_RETURN_NONE;
    }
}

// Camera Controller getters & setters
namespace controller {
    // Horizontal Speed
    static PyObject *get_horizontal_speed(CameraControllerObject *self, void *Py_UNUSED(closure)) {
        return PyFloat_FromDouble(self->controller->getHorizontalSpeed());
    }

    static int set_horizontal_speed(CameraControllerObject *self, PyObject *value, void *Py_UNUSED(closure)) {
        #if GP_TYPE_CHECKING
        if (!PyNumber_Check(value)) {
            RAISE_TYPE_ERROR(-1, "number", value);
        }
        #endif

        self->controller->setHorizontalSpeed((float) PyFloat_AsDouble(value));
        return 0;
    }

    // Vertical Speed
    static PyObject *get_vertical_speed(CameraControllerObject *self, void *Py_UNUSED(closure)) {
        return PyFloat_FromDouble(self->controller->getVerticalSpeed());
    }

    static int set_vertical_speed(CameraControllerObject *self, PyObject *value, void *Py_UNUSED(closure)) {
        #if GP_TYPE_CHECKING
        if (!PyNumber_Check(value)) {
            RAISE_TYPE_ERROR(-1, "number", value);
        }
        #endif

        self->controller->setVerticalSpeed((float) PyFloat_AsDouble(value));
        return 0;
    }

    // Rotation Speed
    static PyObject *get_rotate_speed(CameraControllerObject *self, void *Py_UNUSED(closure)) {
        return PyFloat_FromDouble(self->controller->getRotateSpeed());
    }

    static int set_rotate_speed(CameraControllerObject *self, PyObject *value, void *Py_UNUSED(closure)) {
        #if GP_TYPE_CHECKING
        if (!PyNumber_Check(value)) {
            RAISE_TYPE_ERROR(-1, "number", value);
        }
        #endif

        self->controller->setRotateSpeed((float) PyFloat_AsDouble(value));
        return 0;
    }

    // Zoom Speed
    static PyObject *get_zoom_speed(CameraControllerObject *self, void *Py_UNUSED(closure)) {
        return PyFloat_FromDouble(self->controller->getZoomSpeed());
    }

    static int set_zoom_speed(CameraControllerObject *self, PyObject *value, void *Py_UNUSED(closure)) {
        #if GP_TYPE_CHECKING
        if (!PyNumber_Check(value)) {
            RAISE_TYPE_ERROR(-1, "number", value);
        }
        #endif

        self->controller->setZoomSpeed((float) PyFloat_AsDouble(value));
        return 0;
    }
}

// Camera Controller Type
namespace controller {
    static PyMethodDef methods[] = {
            {"update",            (PyCFunction) update,            METH_NOARGS,
                    "Updates the camera controller"},

            {"enable_movement",   (PyCFunction) enable_movement,   METH_O,
                    "Enables camera movement"},
            {"enable_rotation",   (PyCFunction) enable_rotation,   METH_O,
                    "Enables camera rotation"},
            {"enable_zoom",       (PyCFunction) enable_zoom,       METH_O,
                    "Enables camera zoom"},

            {"invert_movement",   (PyCFunction) invert_movement,   METH_O,
                    "Inverts the camera movement"},
            {"invert_rotation",   (PyCFunction) invert_rotation,   METH_O,
                    "Inverts the camera rotation"},

            {"use_wasd",          (PyCFunction) use_wasd,          METH_NOARGS,
                    "Uses the WASD keys for camera movement"},
            {"use_arrows",        (PyCFunction) use_arrows,        METH_NOARGS,
                    "Uses the arrow keys for camera movement"},

            {"set_movement_keys", (PyCFunction) set_movement_keys, METH_NOARGS,
                    "Sets the keys used for camera movement"},
            {"set_rotation_keys", (PyCFunction) set_rotation_keys, METH_NOARGS,
                    "Sets the keys used for camera rotation"},
            {"set_zoom_keys",     (PyCFunction) set_zoom_keys,     METH_NOARGS,
                    "Sets the keys used for camera zooming"},

            {nullptr}
    };

    static PyGetSetDef getsetters[] = {
            {"horizontal_speed", (getter) get_horizontal_speed, (setter) set_horizontal_speed, "horizontal_speed", nullptr},
            {"vertical_speed", (getter) get_vertical_speed, (setter) set_vertical_speed, "vertical_speed", nullptr},
            {"rotate_speed", (getter) get_rotate_speed, (setter) set_rotate_speed, "rotate_speed", nullptr},
            {"zoom_speed", (getter) get_zoom_speed, (setter) set_zoom_speed, "zoom_speed", nullptr},

            {nullptr}
    };
}

static PyTypeObject CameraControllerType = {
        PyVarObject_HEAD_INIT(nullptr, 0)
        "goopylib.CameraController",
        sizeof(CameraControllerObject),
        0,
        (destructor) controller::dealloc,
        0,
        nullptr,
        nullptr,
        nullptr,
        (reprfunc) controller::repr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC,
        "",
        (traverseproc) controller::traverse,
        (inquiry) controller::clear,
        nullptr,
        0,
        nullptr,
        nullptr,
        controller::methods,
        nullptr,
        controller::getsetters,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        0,
        (initproc) controller::init,
        nullptr,
        controller::new_,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        0,
        nullptr,
        nullptr
};

static struct PyModuleDef CameraControllerModule = {
        PyModuleDef_HEAD_INIT,
        "camera_controller",
        "",
        -1,
        nullptr,
};

PyMODINIT_FUNC PyInit_camera_controller(void) {
    #if GP_LOGGING_LEVEL >= 5
    std::cout << "[--:--:--] PYTHON: PyInit_camera_controller()" << std::endl;
    #endif

    PyObject *m = PyModule_Create(&CameraControllerModule);
    if (m == nullptr) {
        return nullptr;
    }

    // Importing Window

    #if GP_LOGGING_LEVEL >= 6
    std::cout << "[--:--:--] PYTHON: PyInit_cameraController() - import_window()" << std::endl;
    #endif
    PyWindow_API = (void **) PyCapsule_Import("goopylib.ext.window._C_API", 0);
    if (PyWindow_API == nullptr) {
        return nullptr;
    }

    WindowType = Window_pytype();

    // Exposing class

    EXPOSE_PYOBJECT_CLASS(CameraControllerType, "CameraController");

    return m;
}
