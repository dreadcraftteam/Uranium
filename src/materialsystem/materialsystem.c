// This is a main file in materialsystem project 
// By DREADCRAFT
//

#include "base.h"

#include "GLFW/glfw3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "materialsystem.h"

#define MAX_MATERIALS 256
#define MAX_TEXTURES 256

static Material materials[MAX_MATERIALS];
static Texture textures[MAX_TEXTURES];

static int materialCount = 0;
static int textureCount = 0;

int textureQuality = 3;

/* Scale down image data */
static unsigned char* scaleImageData(const unsigned char* data, int width, int height, int channels, int scaleFactor, int* newWidth, int* newHeight)
{
    if (scaleFactor <= 1) 
    {
        *newWidth = width;
        *newHeight = height;
        
        // Return a copy of original data
        unsigned char* copy = malloc(width * height * channels);
        memcpy(copy, data, width * height * channels);

        return copy;
    }
    
    *newWidth = width / scaleFactor;
    *newHeight = height / scaleFactor;
    
    if (*newWidth < 1) *newWidth = 1;
    if (*newHeight < 1) *newHeight = 1;
    
    unsigned char* scaledData = malloc((*newWidth) * (*newHeight) * channels);
    
    for (int y = 0; y < *newHeight; y++) 
    {
        for (int x = 0; x < *newWidth; x++) 
        {
            int srcX = x * scaleFactor;
            int srcY = y * scaleFactor;
            
            for (int c = 0; c < channels; c++) 
            {
                scaledData[(y * (*newWidth) + x) * channels + c] = 
                    data[(srcY * width + srcX) * channels + c];
            }
        }
    }
    
    return scaledData;
}

/* Reload texture with current quality settings */
static void reloadTexture(Texture* tex)
{
    if (!tex || tex->textureId == 0) return;

    char fullpath[256];
    snprintf(fullpath, sizeof(fullpath), "textures/%s", tex->name);

    int width, height, channels;
    unsigned char* data = stbi_load(fullpath, &width, &height, &channels, 4); // Force 4 channels

    if(!data)
    {
        Error("Failed to reload texture: %s\n", fullpath);

        return;
    }

    int quality = getTextureQuality();
    int scaleFactor = 1;
    
    // Determine scale factor based on quality
    switch(quality)
    {
        case 0: // LOW 
            scaleFactor = 8; 
            break;
        case 1: // MEDIUM
            scaleFactor = 4;
            break;
        case 2: // HIGH
            scaleFactor = 2;
            break;
        case 3: // VERY HIGH
            scaleFactor = 1;
            break;
    }

    int newWidth, newHeight;
    unsigned char* scaledData = scaleImageData(data, width, height, 4, scaleFactor, &newWidth, &newHeight);

    tex->width = newWidth;
    tex->height = newHeight;

    glBindTexture(GL_TEXTURE_2D, tex->textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, newWidth, newHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, scaledData);

    switch(quality)
    {
        case 0: // LOW
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
            break;
            
        case 1: // MEDIUM
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 500);
            break;
            
        case 2: // HIGH
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 2000);
            break;
        case 3: // VERY HIGH
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
    }
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_image_free(data);
    free(scaledData);
}

/* Reload all textures with current quality settings */
void reloadAllTextures(void)
{
    for (int i = 0; i < textureCount; i++)
    {
        if (textures[i].textureId != 0)
        {
            reloadTexture(&textures[i]);
        }
    }
}

/* Load texture from file */
static Texture* loadTextureFromFile(const char* filename)
{
    // Check if texture already loaded
    for (int i = 0; i < textureCount; i++)
    {
        if (strcmp(textures[i].name, filename) == 0)
        {
            return &textures[i];
        }
    }

    if (textureCount >= MAX_TEXTURES)
    {
        Warning("Maximum textures reached!\n");
        
        return NULL;
    }

    // Check if file has .png extension
    const char* ext = strrchr(filename, '.');
    if (!ext || strcasecmp(ext, ".png") != 0)
    {
        Error("Only PNG textures are supported: %s\n", filename);

        return NULL;
    }

    char fullpath[256];
    snprintf(fullpath, sizeof(fullpath), "textures/%s", filename);

    int width, height, channels;
    unsigned char* data = stbi_load(fullpath, &width, &height, &channels, 4); // Force 4 channels

    if(!data)
    {
        Error("Failed to load texture: %s\n", fullpath);

        return NULL;
    }

    Texture* tex = &textures[textureCount];
    
    strncpy(tex->name, filename, sizeof(tex->name) - 1);
    tex->bumpmapId = 0;

    int quality = getTextureQuality();
    int scaleFactor = 1;
    
    // Determine scale factor based on quality
    switch(quality)
    {
        case 0: // LOW 
            scaleFactor = 8; 
            break;
        case 1: // MEDIUM
            scaleFactor = 4;
            break;
        case 2: // HIGH
            scaleFactor = 2;
            break;
        case 3: // VERY HIGH
            scaleFactor = 1;
            break;
    }

    int newWidth, newHeight;
    unsigned char* scaledData = scaleImageData(data, width, height, 4, scaleFactor, &newWidth, &newHeight);

    tex->width = newWidth;
    tex->height = newHeight;

    glGenTextures(1, &tex->textureId);
    glBindTexture(GL_TEXTURE_2D, tex->textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, newWidth, newHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, scaledData);

    switch(quality)
    {
        case 0: // LOW
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
            break;
            
        case 1: // MEDIUM
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 500);
            break;
            
        case 2: // HIGH
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1000);
            break;
        case 3: // VERY HIGH
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
    }
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_image_free(data);
    free(scaledData);

    textureCount++;

    return tex;
}

/* Parse .umf material file */
Material* loadMaterial(const char* materialPath)
{
    // Check if material already loaded
    for (int i = 0; i < materialCount; i++)
    {
        if (strcmp(materials[i].name, materialPath) == 0)
        {
            return &materials[i];
        }
    }

    if (materialCount >= MAX_MATERIALS)
    {
        Warning("Maximum materials reached!\n");

        return NULL;
    }

    char fullpath[256];
    snprintf(fullpath, sizeof(fullpath), "textures/%s", materialPath);

    FILE* f = fopen(fullpath, "r");
    if(!f)
    {
        Error("Failed to open material file: %s\n", fullpath);

        // Try to load error material instead
        if (strcmp(materialPath, "engine/error.umf") != 0)
        {
            return loadMaterial("engine/error.umf");
        }
        
        return NULL;
    }

    Material* mat = &materials[materialCount];

    strncpy(mat->name, materialPath, sizeof(mat->name) - 1);
    mat->texture = NULL;
    mat->bumpmap = NULL;
    mat->ignoreLighting = false;

    char line[256];
    char texturePath[256] = {0};
    char bumpmapPath[256] = {0};

    int inTextureSection = 0;
    int inBraces = 0;

    while (fgets(line, sizeof(line), f))
    {
        line[strcspn(line, "\r\n")] = 0;

        // Skip comments and empty lines 
        if(line[0] == '/' || line[0] == '#' || line[0] == '\0')
        {
            continue;
        }

        // Check for [texture] section
        if (strstr(line, "[texture]") != NULL)
        {
            inTextureSection = 1;

            continue;
        }

        // If not in texture section, skip
        if (!inTextureSection)
        {
            continue;
        }

        // Check for opening brace
        if (strstr(line, "{") != NULL)
        {
            inBraces = 1;

            continue;
        }

        // Check for closing brace
        if (strstr(line, "}") != NULL)
        {
            inBraces = 0;
            break;
        }

        // Only parse inside braces
        if (!inBraces)
        {
            continue;
        }

        // Parse key value pairs in format: "key" "value"
        char key[64], value[192];

        // Try parsing
        if (sscanf(line, " \"%63[^\"]\" \"%191[^\"]\"", key, value) == 2)
        {
            if (strcmp(key, "texture") == 0)
            {
                strncpy(texturePath, value, sizeof(texturePath) - 1);
            }
            else if (strcmp(key, "ignoreLighting") == 0)
            {
                mat->ignoreLighting = (strcmp(value, "true") == 0);
            }
        }
    }

    fclose(f);

    if(texturePath[0] != '\0')
    {
        mat->texture = loadTextureFromFile(texturePath);

        if(!mat->texture) 
        {
            // Free the current material slot before loading error material
            Material* errorMat = loadMaterial("engine/error.umf");
            if (errorMat)
            {
                // Copy error material properties to current slot
                memcpy(mat, errorMat, sizeof(Material));
                strncpy(mat->name, materialPath, sizeof(mat->name) - 1); // Keep original name for reference
            }
            else
            {
                return NULL;
            }
        }
    }
    else
    {
        return NULL;
    }

    materialCount++;
    
    return mat;
}

/* Get texture by name */
Texture* getTexture(const char* textureName)
{
    for (int i = 0; i < textureCount; i++)
    {
        if(strcmp(textures[i].name, textureName) == 0)
        {
            return &textures[i];
        }
    }

    // For direct texture loading, just return error texture
    Texture* errorTex = loadTextureFromFile("engine/error.png");
    return errorTex;
}

/* Get material by name */
Material* getMaterial(const char* materialName)
{
    for (int i = 0; i < materialCount; i++)
    {
        if(strcmp(materials[i].name, materialName) == 0)
        {
            return &materials[i];
        }
    }

    Material* errorMat = loadMaterial("engine/error.umf");
    if (errorMat)
    {
        return errorMat;
    }
    
    return NULL;
}

/* Free material */
void freeMaterial(Material* material)
{
    if(material && material->texture)
    {
        material->texture = NULL;
    }
}

/* Free texture */
void freeTexture(Texture* texture)
{
    if(texture && texture->textureId != 0)
    {
        glDeleteTextures(1, &texture->textureId);

        texture->textureId = 0;
    }
}

/* Initialization */
void materialInit(void)
{
    materialCount = 0;
    textureCount = 0;

    memset(materials, 0, sizeof(materials));
    memset(textures, 0, sizeof(textures));
}

/* Shutdown */
void materialShutdown(void)
{
    for (int i = 0; i < textureCount; i++)
    {
        if(textures[i].textureId != 0)
        {
            glDeleteTextures(1, &textures[i].textureId);
        }
    }
    
    materialCount = 0;
    textureCount = 0;
}

void applyTextureQuality(int quality)
{ 
    textureQuality = quality;
    
    reloadAllTextures();
}

int getTextureQuality(void)
{
    return textureQuality;
}
