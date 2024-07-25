#include "SceneDescription.h"

void Scene::LoadRessources() {

    Texture texture;
    texture.CreateTextureRessources();
    TextureManager::GetInstance()->AppendTextureToQueue(texture);

    Texture texture2;
    texture2.texturePath = TEXTURE_PATH2;
    texture2.CreateTextureRessources();
    TextureManager::GetInstance()->AppendTextureToQueue(texture2);


    // TODO: models sollen nur einmal geladen werden müssen

    Model model;
    model.LoadModelFromObjFile(MODEL_PATH);    
    model.verticeBufferId = BufferManager::GetInstance()->CreateVertexBuffer(model.vertices);
    model.indexBufferId = BufferManager::GetInstance()->CreateIndexBuffer(model.indices);
    ModelManager::GetInstance()->AppendModelToMap(model,MODEL_1);


    Model model2;

    model.LoadModelFromObjFile(VIKING_MODEL_PATH);
    model.verticeBufferId = BufferManager::GetInstance()->CreateVertexBuffer(model.vertices);
    model.indexBufferId = BufferManager::GetInstance()->CreateIndexBuffer(model.indices);
    ModelManager::GetInstance()->AppendModelToMap(model, MODEL_2);

    Model model3;
    model.LoadModelFromObjFile(MODEL_PATH);
    model.verticeBufferId = BufferManager::GetInstance()->CreateVertexBuffer(model.vertices);
    model.indexBufferId = BufferManager::GetInstance()->CreateIndexBuffer(model.indices);
    ModelManager::GetInstance()->AppendModelToMap(model, MODEL_3);

    Model model4;
    model.LoadModelFromObjFile(MODEL_PATH);
    model.verticeBufferId = BufferManager::GetInstance()->CreateVertexBuffer(model.vertices);
    model.indexBufferId = BufferManager::GetInstance()->CreateIndexBuffer(model.indices);
    ModelManager::GetInstance()->AppendModelToMap(model, MODEL_4);
}

void Scene::SceneDescription() {



    GameObject gameObject;

    gameObject.textureId = 0;
    gameObject.position = glm::vec3(-0.5, 1.5, 0.0);
    gameObject.modelId = MODEL_1;
    GameObjectManager::GetInstance()->AppendGameObjectToQueue(gameObject);



    gameObject.modelId = MODEL_2;
    gameObject.textureId = 1;
    gameObject.position = glm::vec3(1.0, -1.0, 0.0);

    GameObjectManager::GetInstance()->AppendGameObjectToQueue(gameObject);



    gameObject.modelId = MODEL_2;
    gameObject.textureId = 0;
    gameObject.position = glm::vec3(-1.0, -1.0, 0.0);

    GameObjectManager::GetInstance()->AppendGameObjectToQueue(gameObject);



    gameObject.modelId = MODEL_2;
    gameObject.position = glm::vec3(0.0, 0.0, 0.0);

    GameObjectManager::GetInstance()->AppendGameObjectToQueue(gameObject);

}