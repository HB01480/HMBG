#include "camera.h"


RenderCamera renderCamera_init(vec3s position, vec3s worldUp, f32 pitch, f32 yaw, f32 movementSpeed, f32 mouseSensitivity) {
    RenderCamera camera; SDL_zero(camera);
    camera.position = position;
    camera.front = glms_vec3_zero();
    camera.right = glms_vec3_zero();
    camera.up = glms_vec3_zero();
    camera.worldUp = worldUp;

    camera.pitch = pitch;
    camera.yaw = yaw;

    camera.movementSpeed = movementSpeed;
    camera.mouseSensitivity = mouseSensitivity;

    camera.prevMousePosition = glms_vec2_zero();
    camera.firstMouse = true;

    renderCamera_updateCameraVectors(&camera);
    return camera;
}

mat4s renderCamera_calculateViewMatrix(RenderCamera *camera) {
    return glms_lookat(camera->position, glms_vec3_add(camera->position, camera->front), camera->worldUp);
}

void renderCamera_updateCameraVectors(RenderCamera *camera) {
    f32 pitchRadians = camera->pitch * DEGREES_TO_RADIANS;
    f32 yawRadians = camera->yaw * DEGREES_TO_RADIANS;
    
    vec3s front = glms_vec3_zero();
    front.x = SDL_cosf(yawRadians) * SDL_cosf(pitchRadians);
    front.y = SDL_sinf(pitchRadians);
    front.z = SDL_sinf(yawRadians) * SDL_cosf(pitchRadians);

    camera->front = glms_vec3_normalize(front);
    camera->right = glms_vec3_normalize(glms_cross(camera->front, camera->worldUp));
    camera->up = glms_vec3_normalize(glms_cross(camera->right, camera->front));
}

void renderCamera_pan(RenderCamera *camera, vec2s mousePosition, f32 dt) {
    if (camera->firstMouse) {
        camera->prevMousePosition = mousePosition;
        camera->firstMouse = false;
    }

    vec2s mouseDelta = glms_vec2_sub(mousePosition, camera->prevMousePosition);

    camera->yaw += mouseDelta.x * camera->mouseSensitivity * dt;
    camera->pitch -= mouseDelta.y * camera->mouseSensitivity * dt;

    if (camera->pitch > 89.0f)
        camera->pitch = 89.0f;
    if (camera->pitch < -89.0f)
        camera->pitch = -89.0f;

    camera->prevMousePosition = mousePosition;
    renderCamera_updateCameraVectors(camera);
}

void renderCamera_move(RenderCamera *camera, CameraDirection direction, f32 dt) {
    f32 velocity = camera->movementSpeed * dt;

    if (direction == CM_FORWARD)
        camera->position = glms_vec3_add(camera->position, glms_vec3_scale(camera->front, velocity));
    if (direction == CM_BACKWARD)
        camera->position = glms_vec3_sub(camera->position, glms_vec3_scale(camera->front, velocity));
    if (direction == CM_LEFTWARD)
        camera->position = glms_vec3_sub(camera->position, glms_vec3_scale(camera->right, velocity));
    if (direction == CM_RIGHTWARD)
        camera->position = glms_vec3_add(camera->position, glms_vec3_scale(camera->right, velocity));

    renderCamera_updateCameraVectors(camera);
}
