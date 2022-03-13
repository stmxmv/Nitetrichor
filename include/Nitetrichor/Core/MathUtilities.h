// -*- Nitetrichor -*-
//===------------------------------ `AN` ---------------------------------===//
//
// MathUtilities.h
//
// Created by Molybdenum on 11/27/21.
// Copyright (c) 2021 Nitetrichor. All rights reserved.
//===----------------------------------------------------------------------===//
#ifndef NITETRICHOR_MATHUTILITIES_H
#define NITETRICHOR_MATHUTILITIES_H

#include <simd/simd.h>


#ifdef __cplusplus
extern "C" {
#endif


static inline matrix_float3x3 matrix_float3x3_translation(float tx, float ty) {
    return (matrix_float3x3){{{1, 0, 0},
                              {0, 1, 0},
                              {tx, ty, 1}}};
}


static inline matrix_float4x4 matrix_float4x4_translation(float tx, float ty, float tz) {
    return (matrix_float4x4){{{1, 0, 0, 0},
                              {0, 1, 0, 0},
                              {0, 0, 1, 0},
                              {tx, ty, tz, 1}}};
}


static inline matrix_float4x4 matrix_float4x4_uniform_scale(float scale) {
    return (matrix_float4x4){{{scale, 0, 0, 0},
                              {0, scale, 0, 0},
                              {0, 0, scale, 0},
                              {0, 0, 0, 1}}};
}

static inline matrix_float4x4 matrix_float4x4_rotation(float radians, vector_float3 axis) {
    axis     = vector_normalize(axis);
    float ct = cosf(radians);
    float st = sinf(radians);
    float ci = 1 - ct;
    float x = axis.x, y = axis.y, z = axis.z;
    return (matrix_float4x4){{{ct + x * x * ci, y * x * ci + z * st, z * x * ci - y * st, 0},
                              {x * y * ci - z * st, ct + y * y * ci, z * y * ci + x * st, 0},
                              {x * z * ci + y * st, y * z * ci - x * st, ct + z * z * ci, 0},
                              {0, 0, 0, 1}}};
}


static inline matrix_float4x4 matrix_float4x4_perspective_right_hand(float fovyRadians, float aspect, float nearZ, float farZ) {
    float ys = 1 / tanf(fovyRadians / 2);
    float xs = ys / aspect;
    float zs = farZ / (nearZ - farZ);

    return (matrix_float4x4){{{xs, 0, 0, 0},
                              {0, ys, 0, 0},
                              {0, 0, zs, -1},
                              {0, 0, nearZ * zs, 0}}};
}

#ifdef __cplusplus
}
#endif

#endif//NITETRICHOR_MATHUTILITIES_H
