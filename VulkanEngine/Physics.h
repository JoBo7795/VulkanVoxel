#pragma once
#include "Defs.h"


namespace Physics
{
	struct Ray {

		Ray(glm::vec3 in_origin, glm::vec3 in_direction, float in_length = 1.0f);
		glm::vec3 origin, direction;
		float length;
		float gradient;		

		float CalculateGradient(float x1, float x2, float y1, float y2);
	};

	struct BoundingBox {

		float xMin = 0.0, xMax = 0.0, yMin = 0.0, yMax = 0.0, zMin = 0.0, zMax = 0.0;

		void CreateBoundingBox(std::vector<Vertex>& vertexList);
	};

	bool CheckRayBoxCollision(Ray& ray, BoundingBox& bb,glm::vec3& collPoint);
	bool CheckRayOnPlane(Ray& ray, float rayBoxDiff, float xMin, float yMin, float xMax, float yMax, glm::vec3& collPoint);
	bool CheckPointOnPlane(float x1, float x2, float y1, float& y2, float xMin,  float xMax, float m, glm::vec3& collPoint);


};

