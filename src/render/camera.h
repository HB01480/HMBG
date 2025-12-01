#pragma once
#include "../common.h"


typedef enum CameraDirection {
    CM_FORWARD,
    CM_BACKWARD,
    CM_RIGHTWARD,
    CM_LEFTWARD,

} CameraDirection;

typedef struct RenderCamera {
    vec3s position;
    vec3s front, right, up;
    vec3s worldUp;

    f32 pitch;
    f32 yaw;

    f32 movementSpeed;
    f32 mouseSensitivity;

    vec2s prevMousePosition;
    bool firstMouse;

} RenderCamera;


RenderCamera renderCamera_init(vec3s position, vec3s worldUp, f32 pitch, f32 yaw, f32 movementSpeed, f32 mouseSensitivity);

mat4s renderCamera_calculateViewMatrix(RenderCamera *camera);
void renderCamera_updateCameraVectors(RenderCamera *camera);

void renderCamera_pan(RenderCamera *camera, vec2s mousePosition, f32 dt);
void renderCamera_move(RenderCamera *camera, CameraDirection direction, f32 dt);
