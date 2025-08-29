// Model system for loading models
// By DREADCRAFT
//

#include "base.h"

#include "GLFW/glfw3.h"
#include "stb_image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "assimp/scene.h"
#include "assimp/cimport.h"
#include "assimp/postprocess.h"

#include "model.h"
#include "variables.h"

/* Global Assimp properties */
const struct aiLogStream stream = 
{
    NULL,
    NULL
};

/* Load texture for model */
static GLuint loadModelTexture(const char* filename, const char* directory)
{
    if (!filename || strlen(filename) == 0) 
    {
        Msg("Empty texture filename\n");

        return 0;
    }

    Msg("Loading model texture: %s\n", filename);

    const char* extensions[] = { ".png", "" };
    const char* textureDirs[] = {
        "textures/models/",
        "textures/",
        "models/",
        "" 
    };

    for (int dirIdx = 0; dirIdx < sizeof(textureDirs) / sizeof(textureDirs[0]); dirIdx++) 
    {
        for (int extIdx = 0; extIdx < sizeof(extensions) / sizeof(extensions[0]); extIdx++) 
        {
            
            char fullpath[512];
            
            if (strlen(textureDirs[dirIdx]) > 0) 
            {
                snprintf(fullpath, sizeof(fullpath), "%s%s%s", textureDirs[dirIdx], filename, extensions[extIdx]);
            } 
            else 
            {
                snprintf(fullpath, sizeof(fullpath), "%s%s", filename, extensions[extIdx]);
            }

            printf("Trying texture path: %s\n", fullpath);

            FILE* test = fopen(fullpath, "rb");
            if (test)
            {
                fclose(test);
                
                int width, height, channels;
                unsigned char* data = stbi_load(fullpath, &width, &height, &channels, 4);
                
                if (data) 
                {
                    Msg("Model texture loaded: %dx%d, %d channels from %s\n", width, height, channels, fullpath);

                    GLuint tex;
                    glGenTextures(1, &tex);
                    glBindTexture(GL_TEXTURE_2D, tex);

                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                    stbi_image_free(data);

                    Msg("Model texture created: ID %u\n", tex);
                    return tex;
                } 
                else
                {
                    Msg("STBI failed to load: %s\n", stbi_failure_reason());
                }
            }
        }
    }

    Msg("Failed to load model texture: %s (tried all paths and extensions)\n", filename);
    return 0;
}

/* Load all textures for a material */
static unsigned int loadMaterialTextures(const struct aiMaterial* mat, const char* directory)
{
    GLuint texture = 0;
    
    enum aiTextureType textureTypes[] = 
    {
        aiTextureType_DIFFUSE,
        aiTextureType_AMBIENT,
        aiTextureType_EMISSIVE,
        aiTextureType_SPECULAR
    };
    
    for (int i = 0; i < sizeof(textureTypes) / sizeof(textureTypes[0]); i++) 
    {
        if (aiGetMaterialTextureCount(mat, textureTypes[i]) > 0) 
        {
            struct aiString path;
            if (aiGetMaterialTexture(mat, textureTypes[i], 0, &path, NULL, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) 
            {
                Msg("Loading texture type %d: %s\n", textureTypes[i], path.data);
                texture = loadModelTexture(path.data, directory);
                if (texture) 
                {
                    break; 
                }
            }
        }
    }
    
    return texture;
}

/* Recursively process nodes for rendering */
static void processNode(struct aiNode* node, const struct aiScene* scene, GLuint* textures)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        struct aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        
        glPushMatrix();
        
        struct aiMatrix4x4 transform = node->mTransformation;
        GLfloat glMatrix[16] = {
            transform.a1, transform.b1, transform.c1, transform.d1,
            transform.a2, transform.b2, transform.c2, transform.d2,
            transform.a3, transform.b3, transform.c3, transform.d3,
            transform.a4, transform.b4, transform.c4, transform.d4
        };
        glMultMatrixf(glMatrix);
        
        if (mesh->mMaterialIndex < scene->mNumMaterials)
        {
            GLuint texture = textures[mesh->mMaterialIndex];
            if (texture)
            {
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, texture);
            }
            else
            {
                glDisable(GL_TEXTURE_2D);
            }
        }
        else
        {
            glDisable(GL_TEXTURE_2D);
        }
        
        for (unsigned int j = 0; j < mesh->mNumFaces; j++)
        {
            struct aiFace face = mesh->mFaces[j];
            
            glBegin(GL_TRIANGLES);
            for (unsigned int k = 0; k < face.mNumIndices; k++)
            {
                unsigned int index = face.mIndices[k];
                
                if (mesh->mNormals != NULL)
                {
                    glNormal3f(mesh->mNormals[index].x, 
                              mesh->mNormals[index].y, 
                              mesh->mNormals[index].z);
                }
                
                if (mesh->mTextureCoords[0] != NULL)
                {
                    glTexCoord2f(mesh->mTextureCoords[0][index].x, 
                                mesh->mTextureCoords[0][index].y);
                }
                
                glVertex3f(mesh->mVertices[index].x, 
                          mesh->mVertices[index].y, 
                          mesh->mVertices[index].z);
            }
            glEnd();
        }
        
        glPopMatrix();
    }
    
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, textures);
    }
}

/* Load model from file */
Model* loadModel(const char* path)
{
    /* Initialize Assimp if not already done */
    static int assimpInitialized = 0;
    if (!assimpInitialized)
    {
        aiEnableVerboseLogging(1);
        assimpInitialized = 1;
    }
    
    /* Load the scene */
    const struct aiScene* scene = aiImportFile(path, 
        aiProcess_Triangulate | 
        aiProcess_FlipUVs | 
        aiProcess_GenNormals |
        aiProcess_OptimizeMeshes);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        return NULL;
    }
    
    /* Create model structure */
    Model* model = malloc(sizeof(Model));
    model->scene = (struct aiScene*)scene;
    
    /* Extract directory from path */
    const char* lastSlash = strrchr(path, '/');
    if (!lastSlash) lastSlash = strrchr(path, '\\');
    
    if (lastSlash)
    {
        size_t dirLength = lastSlash - path + 1;
        model->directory = malloc(dirLength + 1);
        strncpy(model->directory, path, dirLength);
        model->directory[dirLength] = '\0';
    }
    else
    {
        model->directory = strdup("");
    }
    
    /* Load materials and textures */
    model->numTextures = scene->mNumMaterials;
    model->textures = malloc(sizeof(GLuint) * model->numTextures);
    
    for (unsigned int i = 0; i < scene->mNumMaterials; i++)
    {
        model->textures[i] = loadMaterialTextures(scene->mMaterials[i], model->directory);
    }
    
    return model;
}

/* Render model */
void renderModel(Model* model)
{
    if (!model || !model->scene) return;
    
    glPushMatrix();
    processNode(model->scene->mRootNode, model->scene, model->textures);
    glPopMatrix();
}

/* Free model resources */ 
void freeModel(Model* model)
{
    if (!model) return;
    
    for (unsigned int i = 0; i < model->numTextures; i++)
    {
        if (model->textures[i])
        {
            glDeleteTextures(1, &model->textures[i]);
        }
    }
    free(model->textures);
    
    free(model->directory);
    
    aiReleaseImport(model->scene);
    
    free(model);
}