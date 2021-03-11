#include "logic.h"



void init_players(player* p1, player* p2)
// initial players state
{
    p1->x = SCR_MIN + 90;
    p2->x = SCR_MAX - 90;
    
    p1->last_animation = idle;
    p2->last_animation = idle;
    
    p1->current_animation = idle;
    p2->current_animation = idle;
    
    p1->animation_frame = 0;
    p2->animation_frame = 0;

    p1->is_player1 = true;
    p2->is_player1 = false;

    update_boxes(p1, p2);

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
// according to input/game state, decide which animation the player will perform
{
    // no collision, do what the input tells us to do:

    bool can_move = false;
    
    if (p->animation_frame >= frames_on_each_animation[p->current_animation])
    // just finished a move
    {
        can_move = true;
        p->animation_frame = 0;
    }

    if (is_neutral(p->current_animation))
        can_move = true;

    if (((p->current_animation == crouching) || is_attack(p->last_animation)) && can_move)
    {

        if (p->wanted_animation == crouching)
        // continue crouching
        {
            if(is_attack(p->last_animation))
            // if recovering from an attack, one should recover back in crouching position, not standing. 
                p->current_animation = crouching;

            p->is_standing = false;

            if ((p->animation_sprite_id < 5) && !is_attack(p->last_animation));
            // is starting to crouch, just let it be

            else if (p->animation_sprite_id == (5) || is_attack(p->last_animation))
            // has fully crouched OR recovering from attack
            {
                p->last_animation = idle;
                p->animation_frame = (6*INTERVAL); // hardcoded sprite when Yun is crouching
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
            // switch animations
            p->current_animation = p->wanted_animation;
            p->animation_frame = 0;
        }

    }

    if (p->is_neutral)
        p->is_neutral = true;

    if (p->current_animation == idle)
        p->is_standing = true;

    // this is important so when doing command moves (such as crMK and crLP)
    // the move won't repeat itself
    if (is_attack(p->current_animation) && (p->current_animation == p->wanted_animation))
    {
        if (p->is_standing){
            p->wanted_animation = idle;
        }
        else if (p->current_animation != dash_punch 
                && p->current_animation != overhead) // if you dash punch or overhead, you have to stand again
        {
            // used so when recovering from an attack, the player won't have to crouch again 
            p->last_animation = p->wanted_animation;
            p->wanted_animation = crouching;
        }
    }
}

int speed_for_animation(enum animation a)
{
    switch (a)
    {
    case walk_forward:
        return SPEED;

    case walk_backwards:
        return -SPEED;

    case dash:
    case dash_punch:
        return SPEED * 1.5;
    
    case backdash:
        return SPEED * -1.1;

    case overhead:
        return SPEED * 0.5;

    default:
        return 0;
    }
}

void check_movement(player* p1, player* p2)
{
    // p1:
    if (speed_for_animation(p1->current_animation) > 0)
    {
        if (!boxes_collide(p1->main_hurtbox, p2->main_hurtbox) && (p1->x < SCR_MAX))
            p1->x += speed_for_animation(p1->current_animation);

        else if (boxes_collide(p1->main_hurtbox, p2->main_hurtbox))
        {
            if (p2->x < SCR_MAX) // drag the other player along
            {
                p1->x += speed_for_animation(p1->current_animation)/2;
                p2->x += speed_for_animation(p1->current_animation)/2;
            }
        }
    }
    else
    {
        if (p1->x > SCR_MIN)
            p1->x += speed_for_animation(p1->current_animation);
    }
    // for p2
    if (speed_for_animation(p2->current_animation) < 0)
    {
        if (!boxes_collide(p2->main_hurtbox, p1->main_hurtbox) && (p1->x > SCR_MIN))
            p2->x += speed_for_animation(p2->current_animation);

        else if (boxes_collide(p1->main_hurtbox, p2->main_hurtbox))
        {
            if (p1->x > SCR_MIN) // drag the other player along
            {
                p1->x += speed_for_animation(p2->current_animation)/2;
                p2->x += speed_for_animation(p2->current_animation)/2;
            }
        }
    }
    else
    {
        if (p2->x < SCR_MAX)
            p2->x += speed_for_animation(p2->current_animation);
    }
}