#pragma once

#include <iostream>
#include <vector>
#include <complex>
#include <numbers>
#include <direct.h>

#include "core/gpu/shader.h"
#include "components/simple_scene.h"
#include "core/gpu/frame_buffer.h"

#ifndef TEXT_SIZE
#define TEXT_SIZE 100
#endif // !TEXT_SIZE

#ifndef TEXT_DEF_PRECISION
#define TEXT_DEF_PRECISION 10
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // !M_PI

#ifndef GetCurrentDir
#define GetCurrentDir _getcwd
#endif // !#define GetCurrentDir _getcwd

#ifndef SURF_DEF_VERT
#define SURF_DEF_VERT 50
#endif
