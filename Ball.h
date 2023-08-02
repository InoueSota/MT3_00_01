#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include <Novice.h>
#include "Renderer.h"

struct Ball
{
    // ボールの位置
    Vector3 position;
    // ボールの速度
    Vector3 velocity;
    // ボールの加速度
    Vector3 acceleration;
    // ボールの質量
    float mass;
    // ボールの半径
    float radius;
    // ボールの色
    unsigned int color;

    /// <summary>
    /// 反射
    /// </summary>
    static Vector3 Reflect(const Vector3& input, const Vector3& normal);

    /// <summary>
    /// 球描画
    /// </summary>
    static void Draw(Renderer& renderer, const Ball& ball);
};