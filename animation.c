#include "animation.h"

char *animation_enum_to_folder(enum animation a)
// get the folder of sprites for a given animation
{
    switch (a)
    {
    case idle:
        return "animation/1-idle/";
    case walk_forward:
        return "animation/2-walk-forward/";

    case walk_backwards:
        return "animation/3-walk-backwards/";

    case dash:
        return "animation/4-dash/";

    case backdash:
        return "animation/5-backdash/";

    case crouching:
        return "animation/6-crouching/";

    case crLP:
        return "animation/7-crLP/";

    case crMK:
        return "animation/8-crMK/";

    case dash_punch:
        return "animation/9-dash-punch/";

    case overhead:
        return "animation/10-overhead/";

    case block_high:
        return "animation/11-block_high/";

    case block_low:
        return "animation/12-block_low/";

    case fall:
        return "animation/13-fall/";

    case high_hitstun:
        return "animation/14-high-hitstun/";

    case low_hitstun:
        return "animation/15-low-hitstun/";

    case rise:
        return "animation/16-rise/";

    default:
        fprintf(stderr, "No such animation %d\n", a);
        exit(1);
    }
}

int sprites_on_each_animation[ANIMATIONS_N]; // array used only to free memory

ALLEGRO_BITMAP ***load_sprites()
// get all the sprites from the drive
{
    // store them all in a matrix of BITMAP*
    ALLEGRO_BITMAP ***animations = malloc(sizeof(ALLEGRO_BITMAP **) * ANIMATIONS_N);
    must_init(animations, "animations array");

    DIR *animation_d;
    struct dirent *dir;

    char path[STR_MAX]; // store the path of each sprite

    for (int i = 0; i < ANIMATIONS_N; i++)
    // for each animation
    {
        // open its folder
        animation_d = opendir(animation_enum_to_folder(i));
        must_init(animation_d, "animation dir");
        rewinddir(animation_d);

        // determine how many sprites there are for the animation
        int sprites = 0;
        while ((dir = readdir(animation_d)) != NULL)
        {
            if (strcmp(dir->d_name, ".") && strcmp(dir->d_name, ".."))
                sprites++;
        }
        rewinddir(animation_d);

        // get memory in order to store each sprite
        animations[i] = malloc(sizeof(ALLEGRO_BITMAP **) * sprites);
        must_init(animations[i], "sprites");

        // get them sprites
        int j = 0;
        while ((dir = readdir(animation_d)) != NULL)
        {
            if (strcmp(dir->d_name, ".") && strcmp(dir->d_name, ".."))
            {
                sprintf(path, "%s%d.png", animation_enum_to_folder(i), j);
                animations[i][j] = al_load_bitmap(path);
                // printf("%s\n", path);
                must_init(animations[i][j], "sprite");

                j += 1;
            }
        }
        sprites_on_each_animation[i] = j;
    }
    return animations;
}

void destroy_sprites(ALLEGRO_BITMAP ***animations)
{
    for (int i = 0; i < ANIMATIONS_N; i++)
    {
        for (int j = 0; j < sprites_on_each_animation[i]; j++)
        {
            al_destroy_bitmap(animations[i][j]);
        }
    }
}


// FRAME DATA
int frames_on_each_animation[ANIMATIONS_N];

void animation_setup()
{
    frames_on_each_animation[idle] = 11 * INTERVAL;
    frames_on_each_animation[walk_forward] = 15 * INTERVAL;
    frames_on_each_animation[walk_backwards] = 15 * INTERVAL;
    frames_on_each_animation[dash] = 8 * INTERVAL;
    frames_on_each_animation[backdash] = 21 * INTERVAL;
    // frames_on_each_animation[crouching] = 6 * INTERVAL;
    frames_on_each_animation[crouching] = 11 * INTERVAL;
    frames_on_each_animation[crLP] = 8 * INTERVAL;
    frames_on_each_animation[crMK] = 13 * INTERVAL;
    frames_on_each_animation[dash_punch] = 15 * INTERVAL;
    frames_on_each_animation[overhead] = 20 * INTERVAL;
    frames_on_each_animation[block_high] = 11 * INTERVAL;
    frames_on_each_animation[block_low] =  11 * INTERVAL;
    frames_on_each_animation[fall] = 19 * INTERVAL;
    frames_on_each_animation[high_hitstun] = 5 * INTERVAL;
    frames_on_each_animation[low_hitstun] = 7 * INTERVAL;
    frames_on_each_animation[rise] = 9 * INTERVAL;
}

int sprite_for_frame(enum animation a, int frame)
// return the index of the sprite for a given frame
{
    if (frame >= frames_on_each_animation[a])
        // no such frame
        return (-1); 
        // or maybe return the last sprite? // return (sprites_on_each_animation[a] -1);


    if (sprites_on_each_animation[a] == (frames_on_each_animation[a]/INTERVAL))
        // animation doesn't go 'back and forward'
    {
        return (frame/INTERVAL);
    }
    else
    {
        if(frame/INTERVAL < (sprites_on_each_animation[a]))
        // animation going "forward"
        {
            return (frame/INTERVAL);
        }
        else
        // animation going "backwards"
        {
            // mirror the value based on the amount of sprites of the animation
            // so it stays in the range of the sprites array

            return ((frame/INTERVAL* (-1)) + (sprites_on_each_animation[a] * 2)) -1;
        }
        
    }
}

bool is_command_attack(enum animation a)
{
    switch (a)
    {
    case crLP:
    case crMK:
    case dash_punch:
    case overhead:
        return true;
    
    default:
        return false;
    }
}