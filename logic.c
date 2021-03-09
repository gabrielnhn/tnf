#include "logic.h"

int check_hitboxes(player* p1, player* p2)
{
    return 0;
}

void init_players(player* p1, player* p2)
{
    p1->x = 50;
    p2->x = WIDTH - 300;
    
    p1->last_animation = idle;
    p2->last_animation = idle;
    
    p1->current_animation = idle;
    p2->current_animation = idle;
    
    p1->animation_frame = 0;
    p2->animation_frame = 0;

    update_hurtboxes(p1, p2);

}

bool boxes_collide(box_t A, box_t B)
{
    if(A.x > (B.x + B.width)) return false;
    if((A.x + A.width) < B.x) return false;
    if(A.y > (B.y + B.height)) return false;
    if((A.y + A.height) < B.y) return false;

    return true;
}

void choose_animation(player* p)
{
    
    // int hitbox_retval = check_hitboxes(p1, p2);
    // if (hitbox_retval != 0)
    // {
    //     // histun/blocking animations
    //     return;
    // }



    // no one's hurt:

    bool can_move = false;
    
    if (p->animation_frame >= frames_on_each_animation[p->current_animation])
    // finished a move
    {
        can_move = true;
        p->animation_frame = 0;
    }

    if (is_neutral(p->current_animation))
        can_move = true;

    if (((p->current_animation == crouching) || is_command_attack(p->last_animation)) && can_move)
    {

        if (p->wanted_animation == crouching)
        // continue crouching
        {
            if(is_command_attack(p->last_animation))
                p->current_animation = crouching;

            p->is_standing = false;

            if ((p->animation_sprite_id < 5) && !is_command_attack(p->last_animation));
            // is starting to crouch, just let it be

            else if (p->animation_sprite_id == (5) || is_command_attack(p->last_animation))
            // has fully crouched OR recovering from attack
            {
                p->last_animation = idle;
                p->animation_frame = (6*INTERVAL) - 1; // hardcoded sprite when Yun is crouching
            }
            else
            // was standing up but decided to go back to crouching
            {
                // reset animation
                p->animation_frame = 0;
            }
        }
    }


    if (can_move)
    {

        // again, details when crouching
        if ((p->current_animation == crouching) && ((p->wanted_animation == idle)))
        {
            if (p->animation_frame == (frames_on_each_animation[crouching] - 1))
            // only switch to idle animation when "standing up" animation is done
            {
                p->current_animation = p->wanted_animation;
                p->animation_frame = 0;
            }
        }
        else if(p->current_animation != p->wanted_animation)
        {
            p->current_animation = p->wanted_animation;
            p->animation_frame = 0;
        }

        if (is_neutral(p->current_animation))
            p->is_neutral = true;

    }

    if (p->current_animation == idle)
        p->is_standing = true;

    // this is important so when doing command moves (such as crMK and crLP)
    // the move won't repeat itself
    if (is_command_attack(p->current_animation) && (p->current_animation == p->wanted_animation))
    {
        if (p->is_standing){
            p->wanted_animation = idle;
        }
        else
        {
            // used so when recovering from an attack, the player won't have to crouch again 
            p->last_animation = p->wanted_animation;
            p->wanted_animation = crouching;
        }
    }
}