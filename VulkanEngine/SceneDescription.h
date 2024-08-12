#pragma once

#include "Includes.h"
#include "TextureManager.h"
#include "GameObjectManager.h"
#include "ModelManager.h"
#include "VoxelMesh.h"

class Scene {

public:
	void LoadRessources();
	void SceneDescription();
	void ChangeVoxelAtIndex(uint32_t index, uint8_t val);
	void ChangeVoxelAtIndex(glm::vec3 index, uint8_t val);
private:

	VoxelMesh voxelMesh;

};

