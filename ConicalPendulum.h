#pragma once
#include "Vector3.h"

struct ConicalPendulum
{
    // アンカーポイント 固定された端の位置
    Vector3 anchor;
    // 紐の長さ
    float length;
    // 円錐の頂角の半分
    float halfApexAngle;
    // 現在の角度
    float angle;
    // 角速度
    float angularVelocity;
};