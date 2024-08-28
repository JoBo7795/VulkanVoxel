#include "Physics.h"



Physics::Ray::Ray(glm::vec3 in_origin, glm::vec3 in_direction, float in_length)
{
	origin = in_origin;
	direction = in_direction;
	length = in_length;
}

float  Physics::Ray::CalculateGradient(float x1,float x2, float y1, float y2) {

	return (y2 - y1) / (x2 - x1);

}

void  Physics::BoundingBox::CreateBoundingBox(std::vector<Vertex>& vertexList)
{
	glm::vec3 currPos;
	int listSize = vertexList.size();
	for (int i = 0; i < listSize; i++) {

		currPos = vertexList[i].pos;

		if (currPos.x < xMin)
			xMin = currPos.x;
		if (currPos.x > xMax)
			xMax = currPos.x;

		if (currPos.y < xMin)
			yMin = currPos.y;
		if (currPos.y > xMax)
			yMax = currPos.y;

		if (currPos.z < zMin)
			zMin = currPos.z;
		if (currPos.z > zMax)
			zMax = currPos.z;
	}


}

bool Physics::CheckRayBoxCollision(Ray& ray, BoundingBox& bb){



	return(CheckRayOnPlane(ray, glm::abs(bb.zMin - (ray.origin.z + ray.direction.z)), bb.xMin, bb.yMin, bb.xMax, bb.yMax) ||			// Front 
		CheckRayOnPlane(ray, glm::abs(bb.zMax - (ray.origin.z + ray.direction.z)), bb.xMin, bb.yMin, bb.xMax, bb.yMax));	// Back


}

bool Physics::CheckRayOnPlane(Ray& ray, float rayBoxDiff, float xMin, float yMin, float xMax, float yMax) {

	//  ray.direction.x ,ray.direction.y,
	return (CheckPointOnPlane(ray.origin.z, ray.origin.z + ray.direction.z * rayBoxDiff, ray.origin.x, xMin, yMin, xMax, yMax, ray.CalculateGradient(ray.direction.z, ray.direction.z * 2.0, ray.direction.x, ray.direction.x * 2.0)) &&
		CheckPointOnPlane(ray.origin.z, ray.origin.z + ray.direction.z * rayBoxDiff, ray.origin.y, xMin, yMin, xMax, yMax, ray.CalculateGradient(ray.direction.z, ray.direction.z * 2.0, ray.direction.y, ray.direction.y * 2.0)));

}

bool Physics::CheckPointOnPlane(float x1, float x2, float y1, float xMin, float yMin,float xMax, float yMax, float m){

	float my;

	// m = y2 - y1/ x2 - x1 
	// y2 = m * (x2 - x1) + y1
	// x2 = ((y2 - y1) / m) - x1

	my = m * (x2 - x1) + y1;
	//mx = ((y2 - y1) / m) - x1;

	if ( my < yMin || my > yMax )
		return false;

	return true;
}
