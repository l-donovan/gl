#include <glm/glm.hpp>

#include "lgll/constants.h"
#include "lgll/app.h"
#include "lgll/entity.h"
#include "lgll/scene.h"
#include "lgll/shader.h"

Entity suzanne("engine/models/suzanne.obj", "engine/shaders/base.v.glsl", "engine/shaders/base.f.glsl");
Entity cactus("engine/models/succulent.obj", "engine/shaders/base.v.glsl", "engine/shaders/base.f.glsl");
Entity skybox("engine/models/uv_sphere.obj", "engine/shaders/base.v.glsl", "engine/shaders/base.f.glsl");
Entity sphere("engine/models/uv_sphere.obj", "engine/shaders/base.v.glsl", "engine/shaders/base.f.glsl");

struct attributes suzanne_mat = {
    .color = glm::vec3(0.6, 0.5, 0.6),
    .metallic = 1.0,
    .roughness = 1.0,
    .reflectance = 0.5,
    .clear_coat = 0.1,
    .clear_coat_roughness = 0.1
};

struct attributes cactus_mat = {
    .color = glm::vec3(0.6, 0.5, 0.6),
    .metallic = 0.0,
    .roughness = 0.1,
    .reflectance = 0.5,
    .clear_coat = 0.1,
    .clear_coat_roughness = 0.1
};

struct attributes skybox_mat = {
    .color = glm::vec3(0.6, 0.5, 0.6),
    .metallic = 0.0,
    .roughness = 0.0,
    .reflectance = 0.5,
    .clear_coat = 0.1,
    .clear_coat_roughness = 0.1
};

struct attributes sphere_mat = {
    .color = glm::vec3(0.8, 0.6, 0.4),
    .metallic = 1.0,
    .roughness = 1.0,
    .reflectance = 0.5,
    .clear_coat = 0.1,
    .clear_coat_roughness = 0.1
};

App app("LGLL Engine", 1200, 800);
Scene scene;

struct camera_t camera = {
    .position = glm::vec3(0.0, 2.0, 4.0),
    .target   = glm::vec3(0.0, 0.0, 0.0),
    .up       = glm::vec3(0.0, 1.0, 0.0),
    .vfov     = 45.0
};

void logic() {
    suzanne.rotate(glm::vec3(0.0, -360.0 / 1000.0, 0.0), LOCAL);
    suzanne.rotate(glm::vec3(0.0, 180.0 / 1000.0, 0.0), GLOBAL);

    cactus.rotate(glm::vec3(0.0, 180.0 / 1000.0, 0.0));
    cactus.set_translation(glm::vec3(0.0, 0.1 * glm::sin(2.0 * glfwGetTime()) - 0.5, 0.0));
    sphere.rotate(glm::vec3(0.0, 180.0 / 1000.0, 0.0));
}

void cursor_pos(double x_pos, double y_pos) {
    camera.position = glm::vec3(
        4.0 * glm::sin(x_pos * TAU),
        2.0 * glm::cos(y_pos * PI),
        4.0 * glm::cos(x_pos * TAU)
    );
}

void window_size(int width, int height) {
}

int main(int argc, char** argv) {
    suzanne.set_texture("engine/images/chinese_garden.png");
    cactus.set_texture("engine/images/SPIKEY_SUCCULENT_5K_u1_v1.jpg");
    skybox.set_texture("engine/images/chinese_garden.png");
    sphere.set_texture("engine/images/chinese_garden.png");

    suzanne.set_material_attributes(&suzanne_mat);
    cactus.set_material_attributes(&cactus_mat);
    skybox.set_material_attributes(&skybox_mat);
    sphere.set_material_attributes(&sphere_mat);

    suzanne.set_translation(glm::vec3(2.0, 0.0, 0.0));
    cactus.set_scale(glm::vec3(4.0));
    sphere.set_scale(glm::vec3(0.1));

    scene.set_camera(&camera);
    scene.register_entity(&cactus);

    scene.register_entity(&suzanne);
    scene.register_entity(&skybox);
    //scene.register_entity(&sphere);

    app.set_active_scene(&scene);
    app.register_logic_fn(logic);
    app.register_cursor_pos_fn(cursor_pos);
    app.register_window_size_fn(window_size);
    app.startup();
    app.main_loop();

    return EXIT_SUCCESS;
}
