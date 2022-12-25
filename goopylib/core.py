"""
Module defining core and miscellaneous functions
"""

from typing import Union
import goopylib.ext.core as __core


def init():
    """Initializes goopylib internally.

    This function is automatically called when goopylib is imported.

    Raises:
        RuntimeError: if the initialization fails
    """
    __core.init()


def terminate():
    """Terminates goopylib internally and destroys all open Windows.

    Note:
        goopylib can be reinitialized using :func:`gp.init()<goopylib.init>`
    """
    __core.terminate()


def update():
    """Updates goopylib and its windows.

    Examples:
        The following is a standard goopylib mainloop:

        .. code-block:: python

            window = gp.Window(500, 500)

            while window.is_open():
                gp.update()
    """
    __core.update()


def update_on_event():
    """Updates goopylib every time an event occurs.
    """
    __core.update_on_event()


def update_timeout(timeout: float = 0):
    """Updates goopylib after a duration has passed.

    Args:
        timeout: the duration (seconds) to wait between updates. Defaults to `0`.
    """
    __core.update_timeout(timeout)


def glfw_compiled_version() -> str:
    """Returns the version of GLFW that goopylib was compiled with.
    """
    return __core.glfw_compiled_version()


def glfw_current_version() -> str:
    """Returns the version of GLFW that is currently running.
    """
    return __core.glfw_current_version()


def get_refresh_rate() -> int:
    """Returns the refresh rate of the primary monitor.

    Returns:
        int: the refresh rate in Hertz

    Raises:
        RuntimeError: if goopylib has not been initialized
    """
    return __core.get_refresh_rate()


def get_screen_width() -> int:
    """Returns the width of the primary monitor.

    Returns:
        int: the screen width in screen coordinates

    Raises:
        RuntimeError: if goopylib has not been initialized
    """
    return __core.get_screen_width()


def get_screen_height() -> int:
    """Returns the height of the primary monitor.

    Returns:
        int: the screen height in screen coordinates

    Raises:
        RuntimeError: if goopylib has not been initialized
    """
    return __core.get_screen_height()


def number_of_monitors() -> int:
    """Returns the number of monitors connected.

    Raises:
        RuntimeError: if goopylib has not been initialized
    """
    return __core.number_of_monitors()


def set_buffer_swap_interval(interval: int):
    """Sets the rate of swapping Window buffers.

    Args:
        interval: the number of refreshes to wait before swapping buffers.

    Raises:
        TypeError: if a non-integer value is passed.
    """
    return __core.set_buffer_swap_interval(interval)


def opengl_version() -> str:
    """Returns the version of OpenGL being used.
    """
    return __core.opengl_version()
