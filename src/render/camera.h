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

} RenderCamera;


RenderCamera renderCamera_init(vec3s position, vec3s worldUp, f32 pitch, f32 yaw, f32 movementSpeed, f32 mouseSensitivity);

mat4s renderCamera_calculateViewMatrix(RenderCamera *camera);
void renderCamera_updateCameraVectors(RenderCamera *camera);

void renderCamera_pan(RenderCamera *camera, vec2s mouseDelta, f32 dt);
void renderCamera_moveForward(RenderCamera *camera, f32 dt);
void renderCamera_moveBackward(RenderCamera *camera, f32 dt);
void renderCamera_moveLeftward(RenderCamera *camera, f32 dt);
void renderCamera_moveRightward(RenderCamera *camera, f32 dt);
