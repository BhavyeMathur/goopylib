#include "Triangle.h"

// Core Methods
namespace gp {
    Triangle::Triangle(Point p1, Point p2, Point p3)
            : RenderableObject({(p1.x + p2.x + p3.x) / 3.0f,
                                (p1.y + p2.y + p3.y) / 3.0f}),
              m_V1({p1, {0, 0.55, 0.9}}),
              m_V2({p2, {0, 0.55, 0.9}}),
              m_V3({p3, {0, 0.55, 0.9}}) {
        GP_CORE_DEBUG("Initializing Triangle ({0}, {1}), ({2}, {3}), ({4}, {5})", p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
    }

    void Triangle::_calculateCenter() {
        m_Position.x = (m_V1.point.x + m_V2.point.x + m_V3.point.x) / 3.0f;
        m_Position.y = (m_V1.point.y + m_V2.point.y + m_V3.point.y) / 3.0f;
    }

    uint32_t Triangle::_draw(Window *window) const {
        return window->m_Renderer.drawTriangle(m_V1, m_V2, m_V3);
    }

    void Triangle::_destroy() const {
        m_Window->m_Renderer.destroyTriangle(m_RendererID);
    }

    void Triangle::_update() const {
        m_Window->m_Renderer.updateTriangle(m_RendererID, m_V1, m_V2, m_V3);
    }

    void Triangle::_move(float dx, float dy) {
        m_V1.point.x += dx;
        m_V1.point.y += dy;

        m_V2.point.x += dx;
        m_V2.point.y += dy;

        m_V3.point.x += dx;
        m_V3.point.y += dy;
    }

    void Triangle::_rotate(float sin, float cos) {
        m_V1.point = {m_V1.point.x * cos - m_V1.point.y * sin,
                      m_V1.point.x * sin + m_V1.point.y * cos};

        m_V2.point = {m_V2.point.x * cos - m_V2.point.y * sin,
                      m_V2.point.x * sin + m_V2.point.y * cos};

        m_V3.point = {m_V3.point.x * cos - m_V3.point.y * sin,
                      m_V3.point.x * sin + m_V3.point.y * cos};
    }

    void Triangle::_scale(float xfactor, float yfactor) {
        m_V1.point.x *= xfactor;
        m_V1.point.y *= yfactor;

        m_V2.point.x *= xfactor;
        m_V2.point.y *= yfactor;

        m_V3.point.x *= xfactor;
        m_V3.point.y *= yfactor;
    }
}

// Triangle Methods
namespace gp {
    void Triangle::setColor(const Color &color) {
        m_V1.color = {color.getRedf(), color.getGreenf(), color.getBluef()};
        m_V2.color = {color.getRedf(), color.getGreenf(), color.getBluef()};
        m_V3.color = {color.getRedf(), color.getGreenf(), color.getBluef()};

        update();
    }

    void Triangle::setColor(const Color &color1, const Color &color2, const Color &color3) {
        m_V1.color = {color1.getRedf(), color1.getGreenf(), color1.getBluef()};
        m_V2.color = {color2.getRedf(), color2.getGreenf(), color2.getBluef()};
        m_V3.color = {color3.getRedf(), color3.getGreenf(), color3.getBluef()};

        update();
    }
}
