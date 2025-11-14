// Console commands for Uranium (on game)
// By DREADCRAFT
//

#include "base.h"

#include "commands.h"
#include "game_gameui.h"
#include "collision.h"
#include "player.h"
#include "../engine/console.h"

/* Commands for console */
int gameConsoleCommands(const char* commandLine)
{
    char command[256];
    char args[256] = {0};

    /* Console commands */
    if (sscanf(commandLine, "%255s %255[^\n]", command, args) >= 1)
    {
        /* NoClip <VALUE> */
        if (strcmp(command, "noclip") == 0)
        {
            int value;
            
            if (sscanf(args, "%d", &value) == 1)
            {
                enableNoClip = (value != 0);

                Msg("NoClip %s\n", enableNoClip ? "ON" : "OFF");

                return 1;
            }
            else 
            {
                Error("Usage: noclip <VALUE>\n");
            }

            return 1;
        }

        /* Collision <VALUE> */
        if (strcmp(command, "collision") == 0)
        {
            int value;

            if (sscanf(args, "%d", &value) == 1)
            {
                physicsEnabled = (value != 0);

                Msg("Collision %s\n", physicsEnabled ? "ON" : "OFF");

                return 1;
            }
            else
            {
                Error("Usage: collision <VALUE>\n");
            }

            return 1;
        }

        /* Collision wireframe <VALUE> */
        if (strcmp(command, "collision_wireframe") == 0)
        {
            int value;
            
            if (sscanf(args, "%d", &value) == 1)
            {
                collisionWireframe = (value != 0);

                Msg("Collision Wireframe %s\n", collisionWireframe ? "ON" : "OFF");

                return 1;
            }
            else
            {
                Error("Usage: collision_wireframe <VALUE>\n");
            }

            return 1;
        }

        /* Hud <VALUE> */
        if (strcmp(command, "hud") == 0)
        {
            int value;
            
            if (sscanf(args, "%d", &value) == 1)
            {
                hud = (value != 0);

                Msg("Hud %s\n", hud ? "ON" : "OFF");

                return 1;
            }
            else
            {
                Error("Usage: hud <VALUE>\n");
            }

            return 1;
        }

        /* Kill */
        if (strcmp(command, "kill") == 0)
        {
            health = 0;
            armor = 0;
            
            handlePlayerDeath();
            
            return 1;
        }

        /* Heal */
        if (strcmp(command, "heal") == 0)
        {
            if (isPlayerDead)
            {
                return 1;
            }

            const int healAmount = 10;
            
            int oldHealth = health;
            health += healAmount;
            
            if (health > 100) health = 100;
            
            int healthHealed = health - oldHealth;
            
            int oldArmor = armor;
            armor += healAmount;
            
            if (armor > 100) armor = 100;
            
            int armorHealed = armor - oldArmor;

            return 1;
        }

        /* Damage */
        if (strcmp(command, "damage") == 0)
        {
            if (isPlayerDead)
            {
                return 1;
            }

            const int damageAmount = 10;

            if (armor > 0)
            {
                int armorDamage = damageAmount;

                if (armorDamage > armor)
                {
                    armorDamage = armor;
                }
                
                armor -= armorDamage;
                int remainingDamage = damageAmount - armorDamage;
                
                if (remainingDamage > 0)
                {
                    health -= remainingDamage;
                    if (health < 0) health = 0;
                }
            }
            else
            {
                health -= damageAmount;

                if (health < 0) health = 0;
            }
            
            return 1;
        }
    }
    
    return 0; 
}
