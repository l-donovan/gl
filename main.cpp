#include <glm/glm.hpp>

#include <iostream>
#include <vector>

#include "utils/app.h"
#include "utils/geometry.h"
#include "utils/mesh.h"
#include "utils/scene.h"

Mesh suzanne("models/suzanne.obj", "shaders/simple.v.glsl", "shaders/simple.f.glsl");
Mesh cactus("models/succulent.obj", "shaders/simple.v.glsl", "shaders/simple.f.glsl");
Mesh skybox("models/uv_sphere.obj", "shaders/simple.v.glsl", "shaders/simple.f.glsl");

App app("My Engine", 800, 600);
Scene scene;

struct camera_t camera = {
    .position = glm::vec3(0.0, 2.0, 4.0),
    .target   = glm::vec3(0.0, 0.2, 0.0),
    .up       = glm::vec3(0.0, 1.0, 0.0),
    .vfov     = 45.0
};

void logic() {
    suzanne.set_translation(glm::vec3(2.0, 0.0, 0.0));
    suzanne.rotate(glm::vec3(0.0, -360.0 / 1000.0, 0.0), LOCAL);
    suzanne.rotate(glm::vec3(0.0, 180.0 / 1000.0, 0.0), GLOBAL);

    cactus.rotate(glm::vec3(0.0, 180.0 / 1000.0, 0.0));
    cactus.set_translation(glm::vec3(0.0, 0.1 * glm::sin(2.0 * glfwGetTime()), 0.0));
}

void cursor_pos(double x_pos, double y_pos) {
    camera.position = glm::vec3(4.0 * glm::sin(x_pos * 6.283), 2.0, 4.0 * glm::cos(x_pos * 6.283));
}

void window_size(int width, int height) {
}

int main(int argc, char** argv) {
    scene.set_camera(&camera);
    scene.register_mesh(&cactus);
    scene.register_mesh(&suzanne);
    scene.register_mesh(&skybox);

    app.set_active_scene(&scene);
    app.register_logic_fn(logic);
    app.register_cursor_pos_fn(cursor_pos);
    app.register_window_size_fn(window_size);
    app.startup();
    app.main_loop();

    return EXIT_SUCCESS;
}
