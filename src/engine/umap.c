// Map system for Uranium (engine)
// By DREADCRAFT
//

#include "base.h"

#include "GLFW/glfw3.h"
#include "stb/stb_image.h"

#include "../materialsystem/materialsystem.h"
#include "umap.h"
#include "variables.h"

Entity* mapLoad = NULL;

/* Load the texture */
static GLuint loadTexture(const char* filename)
{
    Material* mat = loadMaterial(filename);

    if(mat && mat->texture)
    {
        return mat->texture->textureId;
    }

    return 0;
}

/* Counting */
int countBrushes(Entity* head)
{
    int count = 0;
    for (Entity* e = head; e != NULL; e = e->next)
        if (e->type == ENTITY_BRUSH) count++;
    return count;
}

/* Getting the brush array */
Entity** getBrushArray(Entity* head)
{
    int count = countBrushes(head);
    Entity** arr = malloc(sizeof(Entity*) * count);
    int i = 0;
    for (Entity* e = head; e != NULL; e = e->next)
        if (e->type == ENTITY_BRUSH) arr[i++] = e;
    return arr;
}

/* Drawing brush faces */
static void drawBrushFaces(float sizeX, float sizeY, float sizeZ, GLuint textureId, float color[3], int textureFit, bool ignoreLighting)
{
    float hx = sizeX / 2.0f;
    float hy = sizeY / 2.0f;
    float hz = sizeZ / 2.0f;

    if (ignoreLighting)
    {
        glDisable(GL_LIGHTING);
    }

    if (textureId)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureId);

        if (textureFit) 
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        } 
        else 
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
        
        GLfloat mat_diffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat mat_ambient[]  = {0.5f, 0.5f, 0.5f, 1.0f};
        GLfloat mat_specular[] = {0.1f, 0.1f, 0.1f, 1.0f};
        GLfloat mat_shininess  = 8.0f;

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   mat_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   mat_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  mat_specular);
        glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
    }
    else
    {
        glDisable(GL_TEXTURE_2D);
            
        GLfloat mat_diffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat mat_ambient[]  = {0.5f, 0.5f, 0.5f, 1.0f};
        GLfloat mat_specular[] = {0.1f, 0.1f, 0.1f, 1.0f};
        GLfloat mat_shininess  = 8.0f;

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   mat_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   mat_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  mat_specular);
        glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
    }

    float texScaleX = 1.0f, texScaleY = 1.0f;

    if (!textureFit) 
    {
        texScaleX = sizeX / 2.0f; 
        texScaleY = sizeY / 2.0f;
    }

    glBegin(GL_QUADS);

        glNormal3f(0, 0, 1);
        glTexCoord2f(0, texScaleY); glVertex3f(-hx, -hy, hz);
        glTexCoord2f(texScaleX, texScaleY); glVertex3f(hx, -hy, hz);
        glTexCoord2f(texScaleX, 0); glVertex3f(hx, hy, hz);
        glTexCoord2f(0, 0); glVertex3f(-hx, hy, hz);

        glNormal3f(0, 0, -1);
        glTexCoord2f(0, texScaleY); glVertex3f(hx, -hy, -hz);
        glTexCoord2f(texScaleX, texScaleY); glVertex3f(-hx, -hy, -hz);
        glTexCoord2f(texScaleX, 0); glVertex3f(-hx, hy, -hz);
        glTexCoord2f(0, 0); glVertex3f(hx, hy, -hz);

        glNormal3f(-1, 0, 0);
        glTexCoord2f(0, texScaleY); glVertex3f(-hx, -hy, -hz);
        glTexCoord2f(texScaleX, texScaleY); glVertex3f(-hx, -hy, hz);
        glTexCoord2f(texScaleX, 0); glVertex3f(-hx, hy, hz);
        glTexCoord2f(0, 0); glVertex3f(-hx, hy, -hz);

        glNormal3f(1, 0, 0);
        glTexCoord2f(0, texScaleY); glVertex3f(hx, -hy, hz);
        glTexCoord2f(texScaleX, texScaleY); glVertex3f(hx, -hy, -hz);
        glTexCoord2f(texScaleX, 0); glVertex3f(hx, hy, -hz);
        glTexCoord2f(0, 0); glVertex3f(hx, hy, hz);

        glNormal3f(0, 1, 0);
        glTexCoord2f(0, texScaleY); glVertex3f(-hx, hy, hz);
        glTexCoord2f(texScaleX, texScaleY); glVertex3f(hx, hy, hz);
        glTexCoord2f(texScaleX, 0); glVertex3f(hx, hy, -hz);
        glTexCoord2f(0, 0); glVertex3f(-hx, hy, -hz);

        glNormal3f(0, -1, 0);
        glTexCoord2f(0, texScaleY); glVertex3f(-hx, -hy, -hz);
        glTexCoord2f(texScaleX, texScaleY); glVertex3f(hx, -hy, -hz);
        glTexCoord2f(texScaleX, 0); glVertex3f(hx, -hy, hz);
        glTexCoord2f(0, 0); glVertex3f(-hx, -hy, hz);
    
    glEnd();

    if (textureId)
        glDisable(GL_TEXTURE_2D);

    if (ignoreLighting)
    {
        glEnable(GL_LIGHTING);
    }
}

/* Lights */
void setupLights(Entity* head)
{
    int light_id = 0;

    for (Entity* e = head; e != NULL; e = e->next)
	{
        if (e->type == ENTITY_LIGHT && light_id < 8)
		{
            GLenum gl_light = GL_LIGHT0 + light_id;
            
            glEnable(GL_LIGHTING);
            glEnable(gl_light);

            GLfloat pos[4] = { e->light.position[0], e->light.position[1], e->light.position[2], 1.0f };
            
            GLfloat diffuse[]  = { 
                e->light.color[0], 
                e->light.color[1], 
                e->light.color[2], 
                1.0f 
            };
            GLfloat ambient[]  = { 
                e->light.color[0] * 0.3f, 
                e->light.color[1] * 0.3f, 
                e->light.color[2] * 0.3f, 
                1.0f 
            };
            GLfloat specular[] = { 
                e->light.color[0], 
                e->light.color[1], 
                e->light.color[2], 
                1.0f 
            };
            
            float radius = e->light.radius;
            float quadratic = 1.0f / (radius * radius);

            glLightfv(gl_light, GL_POSITION, pos);
            glLightfv(gl_light, GL_DIFFUSE, diffuse);
            glLightfv(gl_light, GL_AMBIENT, ambient);
            glLightfv(gl_light, GL_SPECULAR, specular);
            glLightf (gl_light, GL_CONSTANT_ATTENUATION,  1.0f);
            glLightf (gl_light, GL_LINEAR_ATTENUATION,    0.0f);
            glLightf (gl_light, GL_QUADRATIC_ATTENUATION, quadratic);

            light_id++;
        }
    }

    if (light_id < 8)
    {
        GLenum gl_light = GL_LIGHT0 + light_id;
        
        glEnable(gl_light);
        
        GLfloat dir_pos[4] = {0.0f, -1.0f, 0.0f, 0.0f};
        GLfloat dir_diffuse[] = {0.3f, 0.3f, 0.3f, 1.0f};
        GLfloat dir_ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};
        
        glLightfv(gl_light, GL_POSITION, dir_pos);
        glLightfv(gl_light, GL_DIFFUSE, dir_diffuse);
        glLightfv(gl_light, GL_AMBIENT, dir_ambient);
        
        light_id++;
    }

    for (int i = light_id; i < 8; i++)
    {
        glDisable(GL_LIGHT0 + i);
    }

    if (light_id == 0)
        glDisable(GL_LIGHTING);
}

static EntityType parseEntityType(const char* str)
{
    if (strcmp(str, "ent_brush") == 0) return ENTITY_BRUSH;
    if (strcmp(str, "ent_light") == 0) return ENTITY_LIGHT;
    if (strcmp(str, "ent_pushable") == 0) return ENTITY_PUSHABLE;

    return ENTITY_UNKNOWN;
}

void parseFloats(const char* str, float* out, int count)
{
    float temp[3];
    sscanf(str, "%f,%f,%f", &temp[0], &temp[1], &temp[2]);
    out[0] = temp[0];
    out[1] = temp[2];
    out[2] = temp[1];
}

void parseColor(const char* str, float* out)
{
    float temp[3];
    sscanf(str, "%f,%f,%f", &temp[0], &temp[1], &temp[2]);
    out[0] = temp[0];
    out[1] = temp[1];
    out[2] = temp[2];
}

/* Rendering the map */
void renderMap(Entity* head, float camPos[3])
{
    for (Entity* e = head; e != NULL; e = e->next)
    {
        if (e->type == ENTITY_BRUSH)
        {
            glPushMatrix();
            
            glTranslatef(e->brush.position[0], e->brush.position[1], e->brush.position[2]);
            
            drawBrushFaces(e->brush.size[0], e->brush.size[1], e->brush.size[2], e->brush.textureId, e->brush.color, e->brush.textureFit, e->brush.ignoreLighting);
           
            glPopMatrix();
        }
        else if (e->type == ENTITY_PUSHABLE)
        {
            glPushMatrix();
            
            glTranslatef(e->pushable.position[0], e->pushable.position[1], e->pushable.position[2]);
                        
            drawBrushFaces(e->pushable.size[0], e->pushable.size[1], e->pushable.size[2], e->pushable.textureId, e->pushable.color, e->pushable.textureFit, e->pushable.ignoreLighting);
           
            glPopMatrix();
        }
    }
}

/* Load the map */
Entity* loadMap(const char* filename)
{
    FILE* f = fopen(filename, "r");

    if (!f)
	{
        return NULL;
    }

    Entity* head = NULL;
    Entity* tail = NULL;
    Entity* current = NULL;

    char line[256];

    while (fgets(line, sizeof(line), f))
	{
        line[strcspn(line, "\r\n")] = 0;

        if (strcmp(line, "[entity]") == 0)
		{
            current = calloc(1, sizeof(Entity));
            current->type = ENTITY_UNKNOWN;
            current->brush.textureFit = 1;
            current->brush.ignoreLighting = false;
            
            current->light.color[0] = 1.0f;
            current->light.color[1] = 1.0f;
            current->light.color[2] = 1.0f;

            if (!head) head = current;
            else tail->next = current;
            tail = current;

        } 
        else if (current)
		{
            char key[64], val[128];
            if (sscanf(line, "%63[^=]=%127s", key, val) == 2)
			{
                if (strcmp(key, "type") == 0)
				{
                    current->type = parseEntityType(val);

                } 
                else if (strcmp(key, "position") == 0)
				{
                    if (current->type == ENTITY_BRUSH)
                    {
						parseFloats(val, current->brush.position, 3);
                    }
                    else if (current->type == ENTITY_LIGHT)
                    {
                        parseFloats(val, current->light.position, 3);
                    }
                    else if (current->type == ENTITY_PUSHABLE)
                    {
                        parseFloats(val, current->pushable.position, 3);
                    }
                } 
                else if (strcmp(key, "size") == 0)
				{
                    if (current->type == ENTITY_BRUSH)
                    {
                        parseFloats(val, current->brush.size, 3);
                    }
                    else if (current->type == ENTITY_PUSHABLE)
                    {
                        parseFloats(val, current->pushable.size, 3);
                    }
                } 
                else if (strcmp(key, "radius") == 0 && current->type == ENTITY_LIGHT)
				{
                    current->light.radius = atof(val);
                }
				else if (strcmp(key, "texture") == 0)
				{
                    if (current->type == ENTITY_BRUSH)
                    {
                        current->brush.textureId = loadTexture(val);

                        Material* mat = getMaterial(val);
                        if (mat)
                        {
                            current->brush.ignoreLighting = mat->ignoreLighting;
                        }
                    }
                    else if (current->type == ENTITY_PUSHABLE)
                    {
                        current->pushable.textureId = loadTexture(val);

                        Material* mat = getMaterial(val);
                        if (mat)
                        {
                            current->pushable.ignoreLighting = mat->ignoreLighting;
                        }
                    }
				}
                else if (strcmp(key, "textureFit") == 0)
				{
                    if (current->type == ENTITY_BRUSH)
                    {
                        if (strcmp(val, "yes") == 0) 
                        {
                            current->brush.textureFit = 1;
                        }
                        else if (strcmp(val, "no") == 0) 
                        {
                            current->brush.textureFit = 0; 
                        }
                    }
                    else if (current->type == ENTITY_PUSHABLE)
                    {
                        if (strcmp(val, "yes") == 0) 
                        {
                            current->pushable.textureFit = 1;
                        }
                        else if (strcmp(val, "no") == 0) 
                        {
                            current->pushable.textureFit = 0; 
                        }
                    }
				}
                else if (strcmp(key, "color") == 0)
                {
                    if (current->type == ENTITY_LIGHT)
                    {
                        parseColor(val, current->light.color);
                    }
                    else if (current->type == ENTITY_BRUSH)
                    {
                        parseColor(val, current->brush.color);
                    }
                    else if (current->type == ENTITY_PUSHABLE)
                    {
                        parseColor(val, current->pushable.color);
                    }
                }
                else if (strcmp(key, "mass") == 0 && current->type == ENTITY_PUSHABLE)
                {
                    current->pushable.mass = atof(val);
                }
            }
        }
    }

    fclose(f);

    cameraX = 85.0f;
    cameraY = 0.0f;
    cameraZ = 0.0f;

    return head;
}

void freeMap(Entity* head)
{
    Entity* e = head;
    while (e)
	{
        Entity* next = e->next;
        free(e);
        e = next;
    }
}
