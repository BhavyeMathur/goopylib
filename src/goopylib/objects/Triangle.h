#pragma once

#include "Renderable.h"


namespace gp {
    /**
     * An object representing a triangle with 3 vertices.
     */
    class Triangle : public Renderable {

        friend class Renderer;

    public:
        /**
         * An object representing a triangle with 3 vertices.
         *
         * @param p1 1st coordinate (x, y) in world space
         * @param p2 2nd coordinate (x, y) in world space
         * @param p3 3rd coordinate (x, y) in world space
         */
        GPAPI Triangle(Point p1, Point p2, Point p3);

        GPAPI virtual ~Triangle() = default;

        /**
         * Sets the fill color of the triangle
         * @param color the color of the triangle
         */
        GPAPI void setColor(const Color& color);

        /**
         * Sets the fill color of the triangle
         *
         * @param color1 the color of the 1st vertex
         * @param color2 the color of the 2nd vertex
         * @param color3 the color of the 3rd vertex
         */
        GPAPI void setColor(const Color& color1, const Color& color2, const Color& color3);

        /**
         * Sets the fill color of the object
         *
         * @param color the color of the object
         * @param alpha alpha component of the color between 0-1
         *
         * @throws std::invalid_argument invalid hexstring
         * @throws std::invalid_argument alpha must be between 0-1
         */
        void GPAPI setColor(const char *hexstring, float alpha = 1);

        /**
         * Sets the fill color of the object
         *
         * @param color1 the color of the 1st vertex
         * @param color2 the color of the 2nd vertex
         * @param color3 the color of the 3rd vertex
         *
         * @throws std::invalid_argument invalid hexstring
         */
        void GPAPI setColor(const char *hex1, const char *hex2, const char *hex3);

        /**
         * Sets the fill color of the object
         *
         * @param red red component of the color between 0-255
         * @param green green component of the color between 0-255
         * @param blue blue component of the color between 0-255
         * @param alpha alpha component of the color between 0-1
         *
         * @throws std::invalid_argument RGB must be between 0-255
         * @throws std::invalid_argument alpha must be between 0-1
         */
        void GPAPI setColor(int red, int green, int blue, float alpha = 1);

        /**
         * Sets the transparency of the object
         *
         * @param value transparency of the object between 0-1
         *
         * @throws std::invalid_argument if values are not between 0 and 1
         */
        GPAPI void setTransparency(float value) override;

        /**
         * Sets the transparency of the triangle
         *
         * @param v1 transparency of the 1st vertex between 0-1
         * @param v2 transparency of the 2nd vertex between 0-1
         * @param v3 transparency of the 3rd vertex between 0-1
         *
         * @throws std::invalid_argument if values are not between 0 and 1
         */
        GPAPI void setTransparency(float v1, float v2, float v3);

    private:
        SolidVertexAttrib m_V1 = {{0.0f, 0.55f, 0.9f}};
        SolidVertexAttrib m_V2 = {{0.0f, 0.55f, 0.9f}};
        SolidVertexAttrib m_V3 = {{0.0f, 0.55f, 0.9f}};

        GPAPI uint32_t _draw(Window &window) override;

        GPAPI void _destroy() const override;

        GPAPI void _update() override;

        [[nodiscard]] GPAPI bool _contains(float x, float y) const override;
    };
}
