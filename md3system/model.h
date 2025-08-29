// Model system for loading models
// By DREADCRAFT
//

#ifndef MODEL_H
#define MODEL_H

#include "base.h"

#include "GLFW/glfw3.h"
#include "stb_image.h"

#include "assimp/scene.h"
#include "assimp/cimport.h"
#include "assimp/postprocess.h"

/* Model structure */
typedef struct Model
{
    struct aiScene* scene;
    GLuint* textures;
    unsigned int numTextures;
    char* directory;
} Model;

/* Model entity for umap */
typedef struct
{
    float position[3];
    float rotation[3];
    float scale[3];
    Model* model;
    char* modelPath;
} ModelEntity;

/* Function declarations */
Model* loadModel(const char* path);
void renderModel(Model* model);
void freeModel(Model* model);

#endif // MODEL_H
