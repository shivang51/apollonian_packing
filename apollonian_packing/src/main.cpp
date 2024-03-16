#include "complex_number.h"
#include <array>
#include <chrono>
#include <cmath>
#include <iostream>
#include <locale>
#include <queue>
#include <raylib.h>
#include <thread>
#include <utility>
#include <vector>

#include "raymath.h"
#include "rlgl.h"

int WIDTH = 800, HEIGHT = 600;

int currentGen = 1;

struct Circle {
    ComplexNumber center;
    float k;
    int gen;

    Circle(){};

    Circle(float x, float y, float k, int gen) {
        this->center = ComplexNumber(x, y);
        this->k = k;
        this->gen = gen;
    }

    float radius() { return std::fabs(1 / this->k); }

    Vector2 vec() { return {this->center.getReal(), this->center.getImag()}; }

    void draw() {
        unsigned char r = 128 + (8 * gen);
        unsigned char g = (128 - (8 * gen)) * (gen % 2);
        unsigned char b = r * (gen % 2 == 0);
        Color color = {r, g, b, 255};
        // DrawCircleLines(this->center.getReal(), this->center.getImag(),
        //                 this->radius(), WHITE);

        // if (gen != 0)
        DrawCircle(this->center.getReal(), this->center.getImag(),
                   this->radius(), color);
    }
};

std::vector<Circle> circles = {};

typedef std::array<Circle, 3> TriCircle;
std::queue<TriCircle> circlesQueue = {};

std::pair<float, float> descartes(float k1, float k2, float k3) {
    float sum = k1 + k2 + k3;
    float prod = k1 * k2 + k2 * k3 + k1 * k3;
    prod = std::fabs(prod);
    float root = std::sqrt(prod);
    root *= 2;
    return {sum + root, sum - root};
}

std::vector<Circle> complex_descartes(TriCircle triCircles) {
    auto c1 = triCircles[0];
    auto c2 = triCircles[1];
    auto c3 = triCircles[2];

    float k1 = c1.k, k2 = c2.k, k3 = c3.k;
    auto k4 = descartes(k1, k2, k3);

    auto z1 = c1.center;
    auto z2 = c2.center;
    auto z3 = c3.center;

    auto zk1 = z1.scale(k1);
    auto zk2 = z2.scale(k2);
    auto zk3 = z3.scale(k3);

    auto sum = zk1.add(zk2).add(zk3);
    auto root = zk1.multiply(zk2).add(zk2.multiply(zk3)).add(zk1.multiply(zk3));
    root = root.sqrt().scale(2);

    auto z4p = sum.add(root).scale(1 / k4.first);
    auto z4p1 = sum.add(root).scale(1 / k4.second);
    auto z4n = sum.subtract(root).scale(1 / k4.first);
    auto z4n1 = sum.subtract(root).scale(1 / k4.second);
    return {
        Circle(z4p.getReal(), z4p.getImag(), k4.first, currentGen + 1),
        Circle(z4p1.getReal(), z4p1.getImag(), k4.second, currentGen + 1),
        Circle(z4n.getReal(), z4n.getImag(), k4.first, currentGen + 1),
        Circle(z4n1.getReal(), z4n1.getImag(), k4.second, currentGen + 1),
    };
}

bool is_valid_circle(TriCircle &triCircles, Circle c4) {
    if (c4.radius() < 4)
        return false;

    for (auto &circle : circles) {
        if (circle.center.distance(c4.center) < 0.1) {
            return false;
        }
    }

    for (auto &circle : triCircles) {
        float rSum = circle.radius() + c4.radius();
        float rDiff = std::abs(circle.radius() - c4.radius());

        auto p1 = circle.center;
        auto p2 = c4.center;

        float d = p1.distance(p2);
        float e = 1.01f;
        if (std::fabs(d - rSum) >= e && std::abs(d - rDiff) >= e) {
            return false;
        }
    }
    return true;
}

void add_inner_circles(Circle circle) {
    if (circle.radius() < 60)
        return;
    float r = circle.radius();
    float k = circle.k;

    float x = circle.center.getReal();
    float y = circle.center.getImag();

    float r1 = GetRandomValue(60, r);
    float k1 = 1 / r1;

    float r2 = r - r1;
    float k2 = 1 / r2;

    auto c1 = Circle(x - (r - r1), y, k1, circle.gen + 1);
    auto c2 = Circle(x + (r - r2), y, k2, circle.gen + 1);

    circles.push_back(c1);
    circles.push_back(c2);

    auto tricircle = TriCircle{c1, c2, circle};
    circlesQueue.push(tricircle);

    c1.k *= -1;
    add_inner_circles(c1);
    c2.k *= -1;
    add_inner_circles(c2);
}

void add_cricle(Circle &circle, const TriCircle &c) {
    circles.push_back(circle);
    circlesQueue.push({c[0], c[1], circle});
    circlesQueue.push({c[1], c[2], circle});
    circlesQueue.push({c[2], c[0], circle});

    circle.k *= -1;
    add_inner_circles(circle);
}

void draw_next_gen() {
    auto len = circlesQueue.size();
    for (int i = 0; i < len; i++) {
        auto c = circlesQueue.front();
        circlesQueue.pop();

        auto newCircles = complex_descartes(c);

        for (auto &circle : newCircles) {
            if (!is_valid_circle(c, circle)) {
                continue;
            }

            add_cricle(circle, c);
        }
    }
    currentGen++;
}

void handle_input(Camera2D &camera) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        draw_next_gen();
    }

    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
        // Get the world point that is under the mouse
        Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

        // Set the offset to where the mouse is
        camera.offset = GetMousePosition();

        // Set the target to match, so that the camera maps the world space
        // point under the cursor to the screen space point under the cursor at
        // any zoom
        camera.target = mouseWorldPos;

        // Zoom increment
        const float zoomIncrement = 0.125f;

        camera.zoom += (wheel * zoomIncrement);
        if (camera.zoom < zoomIncrement)
            camera.zoom = zoomIncrement;
    }
}

void setup() {
    circles = {};
    circlesQueue = {};
    currentGen = 2;

    WIDTH = GetRenderWidth(), HEIGHT = GetRenderHeight();

    float x = (float)WIDTH / 2, y = (float)HEIGHT / 2;

    float r = std::min(WIDTH, HEIGHT) * 0.35;
    float k = 1 / r;

    auto c1 = Circle(x, y, k, 0);
    circles.push_back(c1);
    c1.k *= -1;
    add_inner_circles(c1);
}

void draw() {
    WIDTH = GetRenderWidth(), HEIGHT = GetRenderHeight();

    for (auto &circle : circles) {
        circle.draw();
    }
}

int main() {
    SetConfigFlags(ConfigFlags::FLAG_WINDOW_RESIZABLE |
                   ConfigFlags::FLAG_MSAA_4X_HINT);
    InitWindow(WIDTH, HEIGHT, "Apollonian Packing");
    SetTargetFPS(60);
    SetRandomSeed(std::chrono::system_clock::now().time_since_epoch().count());
    Camera2D camera = {0};
    camera.zoom = 1.0f;

    while (!WindowShouldClose()) {

        if (IsWindowResized()) {
            setup();
        }

        handle_input(camera);
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);

        // Draw the 3d grid, rotated 90 degrees and centered around 0,0
        // just so we have something in the XY plane
        rlPushMatrix();
        rlTranslatef(0, 25 * 50, 0);
        rlRotatef(90, 1, 0, 0);
        rlPopMatrix();
        draw();
        EndMode2D();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
