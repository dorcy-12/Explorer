//
// Created by Dorcy Agape on 25.08.25.
//

#include "DrawPhysics.hpp"
#include "rlgl.h"
#include "raylib-cpp.hpp"


#define LINE_THICKNESS 3.0f
#define ALPHA_VAL 0.5f

// Helper function to turn b2HexColor to raylib::Color
static inline raylib::Color toRaylib(const b2HexColor color, float alpha = 1.0f) {
    // Clamp alpha to [0,255]
    unsigned int a = static_cast<unsigned int>(alpha * 255.0f) & 0xFFu;

    // Append alpha to the 24-bit RGB (color is enum int, promote to unsigned first)
    unsigned int rgba = (static_cast<unsigned int>(color) << 8) | a;

    return raylib::Color(rgba);
}

DrawPhysics::DrawPhysics() = default;

b2DebugDraw DrawPhysics::GetDebugDraw() {
    const b2DebugDraw debugDraw={
    .DrawPolygonFcn = DrawPolygonFcn,
    .DrawSolidPolygonFcn = DrawSolidPolygonFcn ,
    .DrawCircleFcn = DrawCircleFcn,
    .DrawSolidCircleFcn = DrawSolidCircleFcn,
    .DrawSegmentFcn = DrawSegmentFcn,
    .DrawTransformFcn = DrawTransformFcn,
    .DrawPointFcn = DrawPointFcn,
    .DrawStringFcn = DrawStringFcn,
    .context = this,
    };
    return debugDraw;
}

void DrawPhysics::DrawPolygonFcn(const b2Vec2 *vertices, int vertexCount, b2HexColor color, void *context) {
    static_cast<DrawPhysics*>(context)->drawPolygon(vertices, vertexCount, color);
}

void DrawPhysics::DrawSolidPolygonFcn(b2Transform transform, const b2Vec2 *vertices, int vertexCount, float radius,
    b2HexColor color, void *context) {
    static_cast<DrawPhysics*>(context)->drawSolidPolygon(transform, vertices, vertexCount,radius, color);
}

void DrawPhysics::DrawCircleFcn(b2Vec2 center, float radius, b2HexColor color, void *context) {
    static_cast<DrawPhysics*>(context)->drawCircle(center, radius , color);
}

void DrawPhysics::DrawSolidCircleFcn(b2Transform transform, float radius, b2HexColor color, void *context) {
    static_cast<DrawPhysics*>(context)->drawSolidCircle(transform, radius , color);
}

void DrawPhysics::DrawSolidCapsuleFcn(b2Vec2 p1, b2Vec2 p2, float radius, b2HexColor color, void *context) {
    static_cast<DrawPhysics*>(context)->drawSolidCapsule(p1, p2, radius , color);
}

void DrawPhysics::DrawSegmentFcn(b2Vec2 p1, b2Vec2 p2, b2HexColor color, void *context) {
    static_cast<DrawPhysics*>(context)->drawSegment(p1, p2, color);
}

void DrawPhysics::DrawTransformFcn(b2Transform transform, void *context) {
    static_cast<DrawPhysics*>(context)->drawTransform(transform);
}

void DrawPhysics::DrawPointFcn(b2Vec2 p, float size, b2HexColor color, void *context) {
    static_cast<DrawPhysics*>(context)->drawPoint(p, size, color);
}

void DrawPhysics::DrawStringFcn(b2Vec2 p, const char *s, b2HexColor color, void *context) {
    static_cast<DrawPhysics*>(context)->drawString(p,s,color);
}





// Implementations

void DrawPhysics::drawPolygon(const b2Vec2 *vertices, int vertexCount, b2HexColor color) {
    raylib::Color rColor = toRaylib(color);
    const auto *rVertices = reinterpret_cast<const Vector2*>(vertices);
    DrawLineStrip(rVertices, vertexCount, rColor);
    DrawLineEx(rVertices[vertexCount - 1], rVertices[0], LINE_THICKNESS, rColor);
}

void DrawPhysics::drawSolidPolygon(b2Transform transform, const b2Vec2 *vertices, int vertexCount, float radius,
    b2HexColor color) {
    // rlDisableBackfaceCulling();
    raylib::Color rColor = toRaylib(color, ALPHA_VAL);

    b2Vec2 world_vertices[vertexCount];
    for (int i = 0; i < vertexCount; i++) {
        b2Vec2 p = b2TransformPoint(transform, vertices[i]);
        world_vertices[i] = p;  //TODO: Check if i need to reverse the y.coordinate for Raylib
    }

    // Draw filled polygon using reinterpret_cast
    const auto* rVertices = reinterpret_cast<const Vector2*>(world_vertices);
    DrawTriangleFan(rVertices, vertexCount, rColor);

    drawPolygon(world_vertices, vertexCount, color);
}

void DrawPhysics::drawCircle(b2Vec2 center, float radius, b2HexColor color) {
    const raylib::Color rColor = toRaylib(color);

    // DrawCircleLines() uses integer coordinates, which isn't accurate with world coordinates, so...
    int numSides = 32;
    DrawPolyLinesEx(reinterpret_cast<const Vector2 &>(center), numSides, radius, 0.0f, LINE_THICKNESS, rColor);
}

void DrawPhysics::drawSolidCircle(b2Transform transform, float radius, b2HexColor color) {
    const raylib::Color rColor = toRaylib(color, ALPHA_VAL);

    const b2Vec2 center = transform.p;
    ::DrawCircleV(reinterpret_cast<const Vector2&>(center), radius, rColor);

    drawCircle(center, radius, color);

    // Draw a line, so rotation can be seen
    auto [x, y] = b2Rot_GetXAxis(transform.q);  // Get rotation direction
    const b2Vec2 lineEnd = {
        center.x + radius * x,  // End point at the circle's edge
        center.y + radius * y
    };

    drawSegment(center, lineEnd, color);

}

void DrawPhysics::drawSolidCapsule(b2Vec2 p1, b2Vec2 p2, float radius, b2HexColor color) {
    const raylib::Color rColor = toRaylib(color);
    const auto start = reinterpret_cast<Vector2&>(p1);
    const auto end =  reinterpret_cast<Vector2&>(p1);

    // Draw the rectangular body
    DrawLineEx(start, end, radius * 2, rColor);

    // Draw the circular end caps to make it a proper capsule
    DrawCircleV(start, radius, rColor);
    DrawCircleV(end, radius, rColor);
}

void DrawPhysics::drawSegment(b2Vec2 p1, b2Vec2 p2, b2HexColor color) {
    const raylib::Color rColor = toRaylib(color);
    DrawLineEx(reinterpret_cast<Vector2&>(p1),reinterpret_cast<Vector2&>(p2), LINE_THICKNESS, rColor );
}

void DrawPhysics::drawTransform(b2Transform transform) {
    constexpr float lineLength = 15.0f;
    const auto origin = reinterpret_cast<Vector2&>(transform.p);
    const b2Vec2 axisX = b2Rot_GetXAxis(transform.q);
    auto endX = lineLength * axisX;
    DrawLineEx(origin, reinterpret_cast<Vector2&>(endX), LINE_THICKNESS, RED); // Draw x-axis line

    const b2Vec2 axisY = b2Rot_GetYAxis(transform.q);
    auto endY = lineLength * axisY;
    DrawLineEx(origin, reinterpret_cast<Vector2&>(endY), LINE_THICKNESS , GREEN); // Draw y-axis line
}

void DrawPhysics::drawPoint(b2Vec2 p, float size, b2HexColor color) {
    const raylib::Color rColor = toRaylib(color);
    constexpr float radius = 2.0f;
    DrawCircleV(reinterpret_cast<Vector2&>(p), radius, rColor);
}

void DrawPhysics::drawString(b2Vec2 p, const char *s, b2HexColor color) {
    const raylib::Color rColor = toRaylib(color);
    const auto [x,y] = p;
    DrawTextEx(GetFontDefault(), s, {x,y}, 12.0f, 1.0f, rColor);

}





