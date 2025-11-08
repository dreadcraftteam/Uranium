// Light entity for Uranium (engine)
// By DREADCRAFT
//

#include "base.h"
#include "GLFW/glfw3.h"
#include "light.h"
#include "../../engine/umap.h"

/* Initialization */
void lightInit(Light* light)
{
    light->position[0] = 0.0f;
    light->position[1] = 0.0f;
    light->position[2] = 0.0f;
    
    light->radius = 10.0f;
    
    light->color[0] = 1.0f;
    light->color[1] = 1.0f;
    light->color[2] = 1.0f;
}

/* Setup lights */
void lightSetup(Entity* head)
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

/* Parsing entity data */
void lightParseData(Light* light, const char* key, const char* val)
{
    if (strcmp(key, "position") == 0)
    {
        float temp[3];
        sscanf(val, "%f,%f,%f", &temp[0], &temp[1], &temp[2]);
        light->position[0] = temp[0];
        light->position[1] = temp[2];
        light->position[2] = temp[1];
    }
    else if (strcmp(key, "radius") == 0)
    {
        light->radius = atof(val);
    }
    else if (strcmp(key, "color") == 0)
    {
        float temp[3];
        sscanf(val, "%f,%f,%f", &temp[0], &temp[1], &temp[2]);
        light->color[0] = temp[0];
        light->color[1] = temp[1];
        light->color[2] = temp[2];
    }
}
