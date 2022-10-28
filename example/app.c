
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>

#include <canvas.h>

#include "app.h"

float2 getFloat2(float x, float y)
{
    float2 f = { x, y };
    return f;
}

void appInit(App* app)
{
    *app = (App){0};
    for (int i = 0; i < sizeof(app->fractals) / sizeof(Fractal); i++)
        app->fractals[i].size = 5;
    app->fractals[3].polygonSegments = 5;
    app->fractals[3].polygonReduction = 0.85f;
}

void appUpdate(App* app)
{
    ImGuiIO* io = igGetIO();
    int width = io->DisplaySize.x;
    int height = io->DisplaySize.y;

    cvSetCoordinateSystemFromScreenSpace(
            width / 2.f, height / 2.f, // origin
            50.f, 0.f,                 // right
            0.f, -50.f                 // top
    );

    // Camera movement
    if (io->MouseDown[1]) {
        if (io->MouseClicked[1])
            app->cameraStart = app->camera;

        ImVec2 delta = { 0 };
        igGetMouseDragDelta(&delta, ImGuiMouseButton_Right, 5.0f);

        app->camera.x = app->cameraStart.x + delta.x / 40;
        app->camera.y = app->cameraStart.y + delta.y / -40;
    }

    for (int i = 0; i < sizeof(app->fractals) / sizeof(Fractal); i++) {
        Fractal* f = &app->fractals[i];
        char checkBoxStr[20];
        sprintf(checkBoxStr, "Draw fractal %d", i);
        igCheckbox(checkBoxStr , &f->display);

        if (f->display) {
            igPushID_Ptr(f);
            igSliderAngle("Angle", &f->angle, 0.0f, 360.0f, "%.2f", 0);
            igSliderInt("Depth", &f->depth, 0, 20, "%d", 0);
            igSliderFloat("Size", &f->size, 0, 10, "%.1f", 0);

            igInputFloat("Position x", &f->position.x, 0.5f, 1, "%.2f", 0);
            igInputFloat("Position y", &f->position.y, 0.5f, 1, "%.2f", 0);
            igPopID();

            // Get render position
            float2 pos = { f->position.x + app->camera.x, f->position.y + app->camera.y };

            switch (i) {
                case 0:
                    squareFractal(pos, pos, f->size, f->angle, f->depth, CV_COL32_WHITE);
                    break;
                case 1:
                    drawSierpinskiTriangle(pos, pos, f->size, f->angle, f->depth, CV_COL32_WHITE);
                    break;
                case 2:
                    drawPythagorasTree(pos, getFloat2(pos.x + f->size, pos.y), f->angle, f->depth, CV_COL32_WHITE);
                    break;
                case 3:
                    igSliderInt("Segments", &f->polygonSegments, 3, 6, "%d", 0);
                    igSliderFloat("Reduction", &f->polygonReduction, 0.01, 1, "%.2f", 0);

                    drawPolygonSpiral(f->polygonSegments, f->size, f->angle, pos, CV_COL32_WHITE, f->polygonReduction, f->depth);
                    break;
                case 4:
                    break;
            }
        }
    }
}

void appShutdown(App* app)
{
    (void)app; // TOREMOVE: Silence unused parameter ‘app’ warning
}

float2 rotatePoint(float2 point, float2 center, float angle)
{
    float s = sin(angle);
    float c = cos(angle);
    point.x -= center.x;
    point.y -= center.y;
    return (float2){ point.x * c - point.y * s + center.x, point.y * c + point.x * s + center.y };
}

void drawSquareUsingCenter(float2 pos, float width, float2 rotationCenter, float angle, unsigned int color)
{
    float d = width / 2;

    float2 points[4] = {
        [0] = { // Top right
            .x = pos.x + d,
            .y = pos.y + d,
        },
        [1] = { // Bottom right
            .x = pos.x + d,
            .y = pos.y - d
        },
        [2] = { // Bottom left
            .x = pos.x - d,
            .y = pos.y - d
        },
        [3] = { // Top left
            .x = pos.x - d,
            .y = pos.y + d
        }
    };

    for (int i = 0; i < 4; i++) {
        points[i] = rotatePoint(points[i], rotationCenter, angle);
        //TRANSFORM_POINT(points[0]);
    }
    drawSquare(points[0], points[1], points[2], points[3], color);
}

void drawSquare(float2 p1, float2 p2, float2 p3, float2 p4, unsigned int color)
{
    cvAddLine(p1.x, p1.y, p2.x, p2.y, color);
    cvAddLine(p2.x, p2.y, p3.x, p3.y, color);
    cvAddLine(p3.x, p3.y, p4.x, p4.y, color);
    cvAddLine(p4.x, p4.y, p1.x, p1.y, color);
}

void drawCircle(float x, float y, float radius, int segments, unsigned int color, bool filled)
{
    float angle = 2 * M_PI / segments;
    for (int i = 0; i < segments; i++) {
        float2 point = { 0 };
        point.x = x + cos(angle * i) * radius;
        point.y = y + sin(angle * i) * radius;

        cvAddPoint(point.x, point.y, color);
    }
}

void drawTriangle(float2 p1, float2 p2, float2 p3, unsigned int color)
{
    cvAddLine(p1.x, p1.y, p2.x, p2.y, color);
    cvAddLine(p2.x, p2.y, p3.x, p3.y, color);
    cvAddLine(p3.x, p3.y, p1.x, p1.y, color);
}

void drawEquilateralTriangle(float2 pos, float sideLength, float2 rotationCenter, float angle, unsigned int color)
{
    float h = sideLength * sqrt(3) / 2;
    float radius = h * 2 / 3;

    float2 a = { pos.x, pos.y + radius };

    float yDistance = fabs(sqrt(radius * radius - (sideLength / 2) * (sideLength / 2)));
    float2 b = { pos.x - sideLength / 2, pos.y - yDistance};
    float2 c = { pos.x + sideLength / 2, pos.y - yDistance};

    a = rotatePoint(a, rotationCenter, angle);
    b = rotatePoint(b, rotationCenter, angle);
    c = rotatePoint(c, rotationCenter, angle);

    drawTriangle(a, b, c, color);
}

void drawPolygon(int nGones, float radius, float angleOffset, float2 pos, unsigned int color)
{
    float angle = 2 * M_PI / nGones;
    for (int i = 0; i < nGones; i++) {
        float2 point = { 0 };
        point.x = pos.x + cos(angle * i + angleOffset) * radius;
        point.y = pos.y + sin(angle * i + angleOffset) * radius;

        cvPathLineTo(point.x, point.y);
    }

    cvPathStroke(color, true); 
}

void squareFractal(float2 pos, float2 startingPos, float width, float angle, int depth, unsigned int color)
{
    drawSquareUsingCenter(pos, width, startingPos, angle, color);
    if (depth == 0)
        return;

    float halfWidth = width / 2;
    squareFractal(getFloat2(pos.x - halfWidth, pos.y - halfWidth), startingPos, halfWidth, angle, depth - 1, color);
    squareFractal(getFloat2(pos.x + halfWidth, pos.y - halfWidth), startingPos, halfWidth, angle, depth - 1, color);
    squareFractal(getFloat2(pos.x + halfWidth, pos.y + halfWidth), startingPos, halfWidth, angle, depth - 1, color);
    squareFractal(getFloat2(pos.x - halfWidth, pos.y + halfWidth), startingPos, halfWidth, angle, depth - 1, color);
}

void drawSierpinskiTriangle(float2 pos, float2 startingPos, float sideLength, float angle, int depth, unsigned int color)
{
    drawEquilateralTriangle(pos, sideLength, startingPos, angle, color);
    if (depth == 0)
        return;

    float h = sideLength * sqrt(3) / 2;
    float radius = h * 2 / 3;
    float yDistance = fabs(sqrt(radius * radius - (sideLength / 2) * (sideLength / 2)));

    drawSierpinskiTriangle(getFloat2(pos.x, pos.y + radius / 2), startingPos, sideLength / 2, angle, depth - 1, color);
    drawSierpinskiTriangle(getFloat2(pos.x - sideLength / 2 / 2, pos.y - yDistance / 2), startingPos, sideLength / 2, angle, depth - 1, color);
    drawSierpinskiTriangle(getFloat2(pos.x + sideLength / 2 / 2, pos.y - yDistance / 2), startingPos, sideLength / 2, angle, depth - 1, color);
}

void drawPythagorasTree(float2 p1, float2 p2, float angle, int depth, unsigned int color)
{
    // Square
    float2 p3 = rotatePoint(p1, p2, -M_PI / 2);
    float2 p4 = rotatePoint(p2, p1, M_PI / 2);
    drawSquare(p1, p2, p3, p4, color);

    // Triangle
    float2 middle = { (p3.x + p4.x) / 2, (p3.y + p4.y) / 2 };
    float2 p5 = rotatePoint(p4, middle, -angle);
    drawTriangle(p3, p4, p5, color);

    if (depth == 0)
        return;

    drawPythagorasTree(p5, p3, M_PI - angle, depth - 1, color);
    drawPythagorasTree(p4, p5, angle, depth - 1, color);
}

void drawPolygonSpiral(int nGones, float radius, float angleOffset, float2 pos, unsigned int color, float kReduction, int depth)
{
    drawPolygon(nGones, radius, angleOffset, pos, color);

    if (depth == 0)
        return;

    /*float stepAngle = 2 * M_PI / nGones;
    float angle2 = fabsf(stepAngle / 2 - M_PI / 4);

    float adj1 = (radius * sinf(kReduction)) / (tanf(angle2));
    float adj2 = radius * cosf(kReduction);
    float deltaL = fabsf(radius - adj2);

    float dRadius = fabsf(adj1 - deltaL);
    igText("Step angle : %f", stepAngle);
    igText("Angle2 : %f", angle2);
    igText("Adj 1 : %f", adj1);
    igText("Adj 2 : %f", adj2);
    igText("Delta L : %f", deltaL);
    igText("Delta radius : %f", dRadius);*/

    drawPolygonSpiral(nGones, radius * kReduction/*+ dRadius*/, angleOffset * kReduction /*+ stepAngle*/, pos, color, kReduction, depth - 1);
}
