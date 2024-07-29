#include "SceneDescription.h"

void Scene::LoadRessources() {

    Texture texture;
    texture.CreateTextureRessources();
    TextureManager::GetInstance()->AppendTextureToQueue(texture);

    Texture texture2;
    texture2.texturePath = TEXTURE_PATH2;
    texture2.CreateTextureRessources();
    TextureManager::GetInstance()->AppendTextureToQueue(texture2);

    Model model;
    model.LoadModelFromObjFile(MODEL_PATH);    
    model.verticeBufferId = BufferManager::GetInstance()->CreateVertexBuffer(model.vertices);
    model.indexBufferId = BufferManager::GetInstance()->CreateIndexBuffer(model.indices);
    ModelManager::GetInstance()->AppendModelToMap(model,MODEL_AFFE);


    model.LoadModelFromObjFile(VIKING_MODEL_PATH);
    model.verticeBufferId = BufferManager::GetInstance()->CreateVertexBuffer(model.vertices);
    model.indexBufferId = BufferManager::GetInstance()->CreateIndexBuffer(model.indices);
    ModelManager::GetInstance()->AppendModelToMap(model, MODEL_VIKING);

     VoxelMesh().LoadVoxelMesh();
}

void Scene::SceneDescription() {


     GameObject gameObject;
     
     gameObject.textureId = 0;
     gameObject.position = glm::vec3(-0.5, 1.5, 0.0);
     gameObject.modelId = MODEL_AFFE;
     //GameObjectManager::GetInstance()->AppendGameObjectToQueue(gameObject);


}