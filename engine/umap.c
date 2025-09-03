// Map system for this engine
// By DREADCRAFT
//

#include "base.h"

#include "GLFW/glfw3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "umap.h"
#include "variables.h"

Entity* mapLoad = NULL;

/* Load the texture */
static GLuint loadTexture(const char* filename)
{
    const char* textures_pos = strstr(filename, "textures/");
    char fullpath[256];

    if (textures_pos) 
    {
        snprintf(fullpath, sizeof(fullpath), "%s", filename);
    } 
    else 
    {
        snprintf(fullpath, sizeof(fullpath), "textures/%s", filename);
    }

    int width, height, channels;
    unsigned char* data = stbi_load(fullpath, &width, &height, &channels, 4);
    
    if (!data) 
    {
        printf("Failed to load texture: %s\n", fullpath);
        return 0;
    }

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    return tex;
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

static int cmp_float(const void* a, const void* b)
{
    float fa = *(float*)a;
    float fb = *(float*)b;
    if (fa < fb) return -1;
    if (fa > fb) return 1;
    return 0;
}

/* Building BSP node */
BSPNode* buildBSP(Entity** brushes, int count, int depth)
{
    if (count == 0) return NULL;

    BSPNode* node = malloc(sizeof(BSPNode));
    node->front = NULL;
    node->back = NULL;
    node->brushes = NULL;
    node->brushCount = 0;

	if (count <= 4 || depth > 16)
	{
		node->brushCount = count;
		node->brushes = malloc(sizeof(Entity*) * count);
		memcpy(node->brushes, brushes, sizeof(Entity*) * count);
		return node;
	}

    /* pick axis to split (cycle x,y,z) */
    int axis = depth % 3;
    node->axis = axis;

    float* coords = malloc(sizeof(float)*count);
    for (int i = 0; i < count; i++)
        coords[i] = brushes[i]->brush.position[axis];

	qsort(coords, count, sizeof(float), cmp_float);
    node->splitPos = coords[count/2];
    free(coords);

    /* partition brushes into front and back */
    Entity** frontBrushes = malloc(sizeof(Entity*) * count);
    Entity** backBrushes  = malloc(sizeof(Entity*) * count);
    int frontCount = 0, backCount = 0;

    for (int i = 0; i < count; i++) {
        if (brushes[i]->brush.position[axis] > node->splitPos)
            frontBrushes[frontCount++] = brushes[i];
        else
            backBrushes[backCount++] = brushes[i];
    }

    node->front = buildBSP(frontBrushes, frontCount, depth+1);
    node->back  = buildBSP(backBrushes, backCount, depth+1);

    free(frontBrushes);
    free(backBrushes);

    return node;
}

/* Drawing brush faces */
static void drawBrushFaces(float sizeX, float sizeY, float sizeZ, GLuint textureId, float color[3])
{
    float hx = sizeX / 2.0f;
    float hy = sizeY / 2.0f;
    float hz = sizeZ / 2.0f;

    if(renderMode == 0)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_COLOR_MATERIAL);
        glDisable(GL_LIGHT0);
        
        glColor3f(wireframeRed, wireframeGreen, wireframeBlue);
    }
    else if(renderMode == 1)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        
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
    else if(renderMode == 2)
    {
        if (textureId)
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, textureId);

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
    }

    glBegin(GL_QUADS);
        // Front
        glNormal3f(0, 0, 1);
        glTexCoord2f(0, 0); glVertex3f(-hx, -hy, hz);
        glTexCoord2f(1, 0); glVertex3f(hx, -hy, hz);
        glTexCoord2f(1, 1); glVertex3f(hx, hy, hz);
        glTexCoord2f(0, 1); glVertex3f(-hx, hy, hz);

        // Back
        glNormal3f(0, 0, -1);
        glTexCoord2f(0, 0); glVertex3f(hx, -hy, -hz);
        glTexCoord2f(1, 0); glVertex3f(-hx, -hy, -hz);
        glTexCoord2f(1, 1); glVertex3f(-hx, hy, -hz);
        glTexCoord2f(0, 1); glVertex3f(hx, hy, -hz);

        // Left
        glNormal3f(-1, 0, 0);
        glTexCoord2f(0, 0); glVertex3f(-hx, -hy, -hz);
        glTexCoord2f(1, 0); glVertex3f(-hx, -hy, hz);
        glTexCoord2f(1, 1); glVertex3f(-hx, hy, hz);
        glTexCoord2f(0, 1); glVertex3f(-hx, hy, -hz);

        // Right
        glNormal3f(1, 0, 0);
        glTexCoord2f(0, 0); glVertex3f(hx, -hy, hz);
        glTexCoord2f(1, 0); glVertex3f(hx, -hy, -hz);
        glTexCoord2f(1, 1); glVertex3f(hx, hy, -hz);
        glTexCoord2f(0, 1); glVertex3f(hx, hy, hz);

        // Top
        glNormal3f(0, 1, 0);
        glTexCoord2f(0, 0); glVertex3f(-hx, hy, hz);
        glTexCoord2f(1, 0); glVertex3f(hx, hy, hz);
        glTexCoord2f(1, 1); glVertex3f(hx, hy, -hz);
        glTexCoord2f(0, 1); glVertex3f(-hx, hy, -hz);

        // Bottom
        glNormal3f(0, -1, 0);
        glTexCoord2f(0, 0); glVertex3f(-hx, -hy, -hz);
        glTexCoord2f(1, 0); glVertex3f(hx, -hy, -hz);
        glTexCoord2f(1, 1); glVertex3f(hx, -hy, hz);
        glTexCoord2f(0, 1); glVertex3f(-hx, -hy, hz);
    glEnd();

    if (textureId)
        glDisable(GL_TEXTURE_2D);
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
            GLfloat diffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};
            GLfloat ambient[]  = {0.3f, 0.3f, 0.3f, 1.0f};
            GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
            float radius = e->light.radius;
            float quadratic = 1.0f / (radius * radius);

            glLightfv(gl_light, GL_POSITION,  pos);
            glLightfv(gl_light, GL_AMBIENT,   ambient);
            glLightf (gl_light, GL_CONSTANT_ATTENUATION,  1.0f);
            glLightf (gl_light, GL_LINEAR_ATTENUATION,    0.0f);
            glLightf (gl_light, GL_QUADRATIC_ATTENUATION, quadratic);

            light_id++;
        }

		if (light_id < 8)
		{
			GLenum gl_light = GL_LIGHT0 + light_id;
			glEnable(gl_light);
			
			GLfloat dir_pos[4] = {0.0f, -1.0f, 0.0f, 0.0f};
			GLfloat dir_diffuse[] = {0.3f, 0.3f, 0.3f, 1.0f};
			
			glLightfv(gl_light, GL_POSITION, dir_pos);
			glLightfv(gl_light, GL_DIFFUSE, dir_diffuse);
		}
    }

    if (light_id == 0)
        glDisable(GL_LIGHTING);
}

static EntityType parseEntityType(const char* str)
{
    if (strcmp(str, "ent_brush") == 0) return ENTITY_BRUSH;
    if (strcmp(str, "ent_light") == 0) return ENTITY_LIGHT;
    return ENTITY_UNKNOWN;
}

void parseFloats(const char* str, float* out, int count)
{
    float temp[3];
    sscanf(str, "%f,%f,%f", &temp[0], &temp[1], &temp[2]);
    out[0] = temp[0]; /* x stays x */
    out[1] = temp[2]; /* y becomes z (height) */
    out[2] = temp[1]; /* z becomes -y */
}

/* Rendering the map */
void renderMap(BSPNode* node, float camPos[3])
{
    if (!node) return;

    if (node->brushCount > 0)
    {
        /* leaf node: render all brushes in this node */
        for (int i = 0; i < node->brushCount; i++)
        {
            Entity* e = node->brushes[i];
            glPushMatrix();
            glTranslatef(e->brush.position[0], e->brush.position[1], e->brush.position[2]);
            drawBrushFaces(e->brush.size[0], e->brush.size[1], e->brush.size[2], e->brush.textureId, e->brush.color);
            glPopMatrix();
        }
        return;
    }

    if (camPos[node->axis] > node->splitPos)
    {
        renderMap(node->front, camPos);
        renderMap(node->back, camPos);
    }
    else
    {
        renderMap(node->back, camPos);
        renderMap(node->front, camPos);
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
        /* remove newlines */
        line[strcspn(line, "\r\n")] = 0;

        if (strcmp(line, "[entity]") == 0)
		{
            current = calloc(1, sizeof(Entity));
            current->type = ENTITY_UNKNOWN;

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
                } 
                else if (strcmp(key, "size") == 0 && current->type == ENTITY_BRUSH)
				{
                    parseFloats(val, current->brush.size, 3);
                } 
                else if (strcmp(key, "radius") == 0 && current->type == ENTITY_LIGHT)
				{
                    current->light.radius = atof(val);
                }
				else if (strcmp(key, "texture") == 0 && current->type == ENTITY_BRUSH)
				{
					current->brush.textureId = loadTexture(val);
				}
            }
        }
    }

    fclose(f);

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