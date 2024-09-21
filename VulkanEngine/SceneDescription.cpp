#include "SceneDescription.h"

VoxelMesh Scene::voxelMesh;

void Scene::LoadRessources() {

    std::vector<Texture> textureArr;

    Texture texture;
    texture.CreateTextureRessources();
    TextureManager::GetInstance()->AppendTextureToQueue(texture);

    textureArr.push_back(texture);

    Texture texture2;
    texture2.texturePath = TEXTURE_PATH2;
    texture2.CreateTextureRessources();
    TextureManager::GetInstance()->AppendTextureToQueue(texture2);

    textureArr.push_back(texture2);

    Texture texture3;
    texture3.texturePath = TEXTURE_PATH2;
    texture3.CreateTextureRessources();
    TextureManager::GetInstance()->AppendTextureToQueue(texture3);
    
    textureArr.push_back(texture3);



    Model model;
    model.LoadModelFromObjFile(MODEL_PATH);    
    model.verticeBufferId = BufferManager::GetInstance()->CreateVertexBuffer(model.vertices);
    model.indexBufferId = BufferManager::GetInstance()->CreateIndexBuffer(model.indices);
    ModelManager::GetInstance()->AppendModelToMap(model,MODEL_AFFE);

    model.LoadModelFromObjFile(VIKING_MODEL_PATH);
    model.verticeBufferId = BufferManager::GetInstance()->CreateVertexBuffer(model.vertices);
    model.indexBufferId = BufferManager::GetInstance()->CreateIndexBuffer(model.indices);
    ModelManager::GetInstance()->AppendModelToMap(model, MODEL_VIKING);

    model.LoadModelFromObjFile(MARKER_SPHERE_MODEL_PATH);
    model.verticeBufferId = BufferManager::GetInstance()->CreateVertexBuffer(model.vertices);
    model.indexBufferId = BufferManager::GetInstance()->CreateIndexBuffer(model.indices);
    ModelManager::GetInstance()->AppendModelToMap(model, MARKER_SPHERE);

    model.LoadModelFromObjFile(DEFAULT_CUBE_MODEL_PATH);
    model.verticeBufferId = BufferManager::GetInstance()->CreateVertexBuffer(model.vertices);
    model.indexBufferId = BufferManager::GetInstance()->CreateIndexBuffer(model.indices);
    ModelManager::GetInstance()->AppendModelToMap(model, DEFAULT_CUBE);

    voxelMesh.LoadVoxelMesh();

    LightManager* lmRef = LightManager::GetInstance();

    Light light;
    light.position = glm::vec4(VOXEL_GRID_LENGTH/2, VOXEL_GRID_HEIGHT/2, VOXEL_GRID_DEPTH/2, 1);
    light.color = glm::vec4(1, 1, 0, 1);

    lmRef->AddLightToQueue(light);

    light.position = glm::vec4(-1, 20, 2, 1);
    light.color = glm::vec4(0, 1, 1, 1);

    lmRef->AddLightToQueue(light);

    light.position = glm::vec4(100, 10, 10, 1);
    light.color = glm::vec4(0, 1, 0, 1);

    lmRef->AddLightToQueue(light);
}

void Scene::SceneDescription() {



    
   // GameObject go;
   // 
   // go.modelId = DEFAULT_CUBE;
   // go.position = glm::vec3(glm::vec3(0,0,0));
   // GameObjectManager::GetInstance()->AppendGameObjectToQueue(go);
    //
    //
    //
    //go.modelId = MODEL_AFFE;
    //go.position = glm::vec3(glm::vec3(0, 3, 0));
    //GameObjectManager::GetInstance()->AppendGameObjectToQueue(go);

    
}

void Scene::ChangeVoxelAtIndex(uint32_t index, uint8_t val) {
    voxelMesh.ChangeVoxelAtIndex(index, val);
    std::cout << "changed index: " << index << " to value: " << val << std::endl;
}

void Scene::ChangeVoxelAtIndex(glm::vec3 index, uint8_t val) {   
    std::cout << "changed index: " << voxelMesh.ChangeVoxelAtIndex(index, val) << " to value: " << val << std::endl;
}

uint8_t Scene::GetVoxelAtIndex(glm::vec3 index) {
    uint8_t voxelVal = voxelMesh.GetVoxelAtIndex(index);
    //std::cout << "get voxel: " << glm::to_string(index) << " with value: " << voxelVal << std::endl;
    return voxelVal;
}