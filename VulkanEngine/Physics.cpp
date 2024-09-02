#include "Physics.h"
#include "VoxelMesh.h"


Physics::Ray::Ray(glm::vec3 in_origin, glm::vec3 in_direction, float in_length)
{
	origin = in_origin;
	direction = in_direction;
	length = in_length;
}

float Physics::CalculateGradient(float x1,float x2, float y1, float y2) {

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


bool Physics::CheckRayBoxCollision(Ray& ray, BoundingBox& bb, glm::vec3& collPoint, uint8_t& cubeSide){

	bool first = true, hit = false;
	double minDist = -1, dist;

	if (CheckRayOnPlane(ray.origin.z, ray.direction.z, ray.origin.x, ray.direction.x, ray.origin.y, ray.direction.y, bb.zMin - ray.origin.z, bb.xMin, bb.yMin, bb.xMax, bb.yMax, collPoint)) {
		
		dist = glm::length(ray.origin-collPoint);

		if (first || (minDist > dist)) {
			minDist = dist;
			cubeSide = Cube::FRONT;
			first = false;
		}
		hit = true;
	}			
	if(CheckRayOnPlane(ray.origin.z, ray.direction.z, ray.origin.x, ray.direction.x, ray.origin.y, ray.direction.y, bb.zMax - ray.origin.z, bb.xMin, bb.yMin, bb.xMax, bb.yMax, collPoint)){
		
		dist = glm::length(ray.origin - collPoint);

		if (first || (minDist > dist)) {
			minDist = dist;
			cubeSide = Cube::BACK;
			first = false;
		}		
		hit = true;
	}
	if(CheckRayOnPlane(ray.origin.x, ray.direction.x, ray.origin.y, ray.direction.y, ray.origin.z, ray.direction.z, bb.xMin - ray.origin.x, bb.yMin, bb.zMin, bb.yMax, bb.zMax, collPoint)){
		
		dist = glm::length(ray.origin - collPoint);

		if (first || (minDist > dist)) {
			minDist = dist;
			cubeSide = Cube::LEFT;
			first = false;
		}
		hit = true;
	}
	if(CheckRayOnPlane(ray.origin.x, ray.direction.x, ray.origin.y, ray.direction.y, ray.origin.z, ray.direction.z, bb.xMax - ray.origin.x, bb.yMin, bb.zMin, bb.yMax, bb.zMax, collPoint)){
		
		dist = glm::length(ray.origin - collPoint);

		if (first || (minDist > dist)) {
			minDist = dist;
			cubeSide = Cube::RIGHT;
			first = false;
		}
		hit = true;
	}
	if (CheckRayOnPlane(ray.origin.y, ray.direction.y, ray.origin.x, ray.direction.x, ray.origin.z, ray.direction.z, bb.yMin - ray.origin.y, bb.xMin, bb.zMin, bb.xMax, bb.zMax, collPoint)) {
		
		dist = glm::length(ray.origin - collPoint);

		if (first || (minDist > dist)) {
			minDist = dist;
			cubeSide = Cube::TOP;
			first = false;
		}		

		hit = true;
	}
	if (CheckRayOnPlane(ray.origin.y, ray.direction.y, ray.origin.x, ray.direction.x, ray.origin.z, ray.direction.z, bb.yMax - ray.origin.y, bb.xMin, bb.zMin, bb.xMax, bb.zMax, collPoint)) {
		
		dist = glm::length(ray.origin - collPoint);

		if (first || (minDist > dist)) {
			minDist = dist;
			cubeSide = Cube::BOTTOM;
			first = false;
		}	

		hit = true;

	}

	return hit;
}


bool Physics::CheckRayOnPlane(float rayOriginZ,float raydirZ, float rayOriginX, float rayDirX, float rayOriginY, float rayDirY, float rayBoxDiff, float xMin, float yMin, float xMax, float yMax, glm::vec3& collPoint) {
	float x2 = 0.0, y2 = 0.0;

	if (CheckPointOnPlane(rayOriginZ, rayOriginZ + rayBoxDiff, rayOriginX, x2, xMin, xMax, CalculateGradient(rayOriginZ, rayOriginZ + raydirZ, rayOriginX, rayOriginX + rayDirX),collPoint) &&
		CheckPointOnPlane(rayOriginZ, rayOriginZ + rayBoxDiff, rayOriginY, y2, yMin, yMax, CalculateGradient(rayOriginZ, rayOriginZ + raydirZ, rayOriginY, rayOriginY + rayDirY), collPoint)){

		collPoint.x = x2;
		collPoint.y = y2;
		collPoint.z = rayOriginZ + raydirZ * rayBoxDiff;

		return true;

	}

	return false;
}

bool Physics::CheckPointOnPlane(float x1, float x2, float y1, float& y2, float xMin, float xMax, float m, glm::vec3& collPoint) {

	float my;

	// m = y2 - y1/ x2 - x1 
	// y2 = m * (x2 - x1) + y1
	// x2 = ((y2 - y1) / m) - x1

	my = m * (x2 - x1) + y1;
	//mx = ((y2 - y1) / m) - x1;

	if (my < xMin || my > xMax)
		return false;

	y2 = my;
	return true;
}
