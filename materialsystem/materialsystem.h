// This is a main file in materialsystem project 
// By DREADCRAFT
//

#ifndef MATERIALSYSTEM_H
#define MATERIALSYSTEM_H

#include "base.h"

#include "GLFW/glfw3.h"

extern int textureQuality;

typedef struct 
{
    char name[256];
    GLuint textureId;
    GLuint bumpmapId;
    int width;
    int height;
} Texture;

typedef struct 
{
    char name[256];
    Texture* texture;
    Texture* bumpmap;
    bool ignoreLighting;
} Material;

/* Initialization */
void materialInit(void);

/* Shutdown */
void materialShutdown(void);

/* Load material from .umf file */
Material* loadMaterial(const char* materialPath);

/* Get texture by name */
Texture* getTexture(const char* textureName);

/* Get material by name */
Material* getMaterial(const char* materialName);

/* Remove texture from memory */
void freeTexture(Texture* texture);

/* Remove material from memory */
void freeMaterial(Material* material);

/* Texture quality functions */
void applyTextureQuality(int quality);
int getTextureQuality(void);

/* Reload all textures with current quality settings */
void reloadAllTextures(void);

/* Reload all materials */
void reloadAllMaterials(void);

#endif // MATERIALSYSTEM_H
