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


RenderCamera RenderCamera_init(vec3s position, vec3s worldUp, f32 pitch, f32 yaw, f32 movementSpeed, f32 mouseSensitivity);

mat4s RenderCamera_calculateViewMatrix(RenderCamera *camera);
void RenderCamera_updateCameraVectors(RenderCamera *camera);

void RenderCamera_pan(RenderCamera *camera, vec2s mouseDelta, f32 dt);
void RenderCamera_moveForward(RenderCamera *camera, f32 dt);
void RenderCamera_moveBackward(RenderCamera *camera, f32 dt);
void RenderCamera_moveLeftward(RenderCamera *camera, f32 dt);
void RenderCamera_moveRightward(RenderCamera *camera, f32 dt);
