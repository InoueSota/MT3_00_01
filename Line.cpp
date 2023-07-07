#include "Line.h"
#include <Novice.h>
#include "Plane.h"
#include "Triangle.h"
#include "AABB.h"

Vector3 Line::ClosestPoint(const Vector3& point, const Line& line)
{
	return Vector3::Add(line.origin, Vector3::Project(Vector3::Subtract(point, line.origin), line.diff));
}

Vector3 Segment::ClosestPoint(const Vector3& point, const Segment& segment) {
	return Vector3::Add(segment.origin, Vector3::Project(Vector3::Subtract(point, segment.origin), segment.diff));
}

void Segment::Draw(Renderer& renderer, const Segment& segment)
{
	renderer.ScreenLine(segment.origin, segment.origin + segment.diff, segment.color);
}
