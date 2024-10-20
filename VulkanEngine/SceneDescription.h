#pragma once

#include "Includes.h"
#include "TextureManager.h"
#include "GameObjectManager.h"
#include "ModelManager.h"
#include "VoxelMesh.h"
#include "LightManager.h"
#include "ChunkManager.h"

class Scene {

public:
	void LoadRessources();
	void SceneDescription();
	void ChangeVoxelAtIndex(uint32_t index, uint8_t val);
	static void ChangeVoxelAtIndex(glm::vec3 index, uint8_t val);
	static void ChangeVoxelAtIndex(Chunk& chunk, glm::vec3 index, uint8_t val);
	static uint8_t GetVoxelAtIndex(Chunk& chunk, glm::vec3 index);
	static VoxelMesh voxelMesh;
};

