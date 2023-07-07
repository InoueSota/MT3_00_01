#include "OBB.h"
#include <Novice.h>
#include "Matrix4x4.h"
#include "AABB.h"

void OBB::MakeOrientations(OBB& obb, const Vector3& rotate)
{
    // 回転行列を生成
    Matrix4x4 rotateMatrix = Matrix4x4::MakeRotateMatrix(rotate);

    // 回転行列から軸を抽出
    obb.orientations[0] = Vector3::GetXaxis(rotateMatrix);
    obb.orientations[1] = Vector3::GetYaxis(rotateMatrix);
    obb.orientations[2] = Vector3::GetZaxis(rotateMatrix);
}

Matrix4x4 OBB::MakeRotationFromOrientations(const Vector3 orientations[3])
{
    return {
        orientations[0].x, orientations[0].y, orientations[0].z, 0.0f,
        orientations[1].x, orientations[1].y, orientations[1].z, 0.0f,
        orientations[2].x, orientations[2].y, orientations[2].z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
}

Matrix4x4 OBB::MakeInverse(const Matrix4x4& rotation, const Vector3& translate)
{
    Matrix4x4 transposeRotation = Matrix4x4::Transpose(rotation);
    Vector3 inverseTranslate = Vector3::Transform(translate, transposeRotation) * -1;
    transposeRotation.m[3][0] = inverseTranslate.x;
    transposeRotation.m[3][1] = inverseTranslate.y;
    transposeRotation.m[3][2] = inverseTranslate.z;
    return transposeRotation;
}

void OBB::MakeVertex(Vector3 base[8], const OBB& obb)
{
    Vector3 vertices[] = {
        { -obb.size.x,  obb.size.y, -obb.size.z },
        {  obb.size.x,  obb.size.y, -obb.size.z },
        {  obb.size.x, -obb.size.y, -obb.size.z },
        -obb.size,

        { -obb.size.x,  obb.size.y, obb.size.z },
        obb.size,
        {  obb.size.x, -obb.size.y, obb.size.z },
        { -obb.size.x, -obb.size.y, obb.size.z }
    };

    Matrix4x4 tmpMatrix = {
        obb.orientations[0].x, obb.orientations[0].y, obb.orientations[0].z, 0.0f,
        obb.orientations[1].x, obb.orientations[1].y, obb.orientations[1].z, 0.0f,
        obb.orientations[2].x, obb.orientations[2].y, obb.orientations[2].z, 0.0f,
        obb.center.x, obb.center.y, obb.center.z, 1.0f
    };

    for (uint32_t i = 0; i < 8; i++)
    {
        base[i] = Vector3::Transform(vertices[i], tmpMatrix);
    }
}

void OBB::Draw(Renderer& renderer, const OBB& obb)
{
    Vector3 vertices[] = {
            { -obb.size.x,  obb.size.y, -obb.size.z },
            {  obb.size.x,  obb.size.y, -obb.size.z },
            {  obb.size.x, -obb.size.y, -obb.size.z },
            -obb.size,

            { -obb.size.x,  obb.size.y, obb.size.z },
            obb.size,
            {  obb.size.x, -obb.size.y, obb.size.z },
            { -obb.size.x, -obb.size.y, obb.size.z }
    };
    
    Matrix4x4 tmpMatrix = {
        obb.orientations[0].x, obb.orientations[0].y, obb.orientations[0].z, 0.0f,
        obb.orientations[1].x, obb.orientations[1].y, obb.orientations[1].z, 0.0f,
        obb.orientations[2].x, obb.orientations[2].y, obb.orientations[2].z, 0.0f,
        obb.center.x, obb.center.y, obb.center.z, 1.0f
    };

    for (auto& vertex : vertices)
    {
        vertex = Vector3::Transform(vertex, tmpMatrix);
    }

    for (uint32_t i = 0; i < 4; i++)
    {
        uint32_t j = (i + 1) % 4;
        renderer.ScreenLine(vertices[i], vertices[j], obb.color);
        renderer.ScreenLine(vertices[i], vertices[i + 4], obb.color);
        renderer.ScreenLine(vertices[i + 4], vertices[j + 4], obb.color);
    }
}
