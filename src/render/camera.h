#pragma once
#include "../common.h"


typedef struct Camera {
    vec3 position;
    vec3 worldUp;

    f32 pitch, yaw;

} Camera;


Camera camera_init();
void camera_free(Camera *camera);
