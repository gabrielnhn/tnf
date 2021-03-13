#include "input.h"

unsigned char key[ALLEGRO_KEY_MAX]; // used to tell whether a key is being pressed

void input_setup(player* p1, player* p2)
{
    memset(key, 0, sizeof(key)); // clear it
    memset(p1->input_buffer, 0, sizeof(p1->input_buffer));
    memset(p2->input_buffer, 0, sizeof(p2->input_buffer));
    p1->buffer_length = 0;
    p2->buffer_length = 0;
    p1->last_input_frame = 0;
    p2->last_input_frame = 0;
}

int check_input(player *p1, player *p2, ALLEGRO_EVENT event, long frame_count, int p2_input_method)
// return the animation the player wants to execute
// (handles all the input)
{
    switch (event.type)
    {
        case ALLEGRO_EVENT_TIMER: // clock ticks
            // according to key[] and p->input_buffer[], decide which animation to play

            // different animations DO have priority!
            p1->wanted_animation = idle; // default animation
            p1->is_blocking = false; // by default no one is blocking

            p2->wanted_animation = idle; 
            p2->is_blocking = false; 

            // FOR PLAYER 1 // 
            // check for dash punch
            if ((p1->buffer_length > 2) &&
                (p1->input_buffer[p1->buffer_length - 1] == PUNCH) && // just pressed PUNCH
                (p1->input_buffer[p1->buffer_length - 2] == RIGHT) && // was pressing forward
                ((p1->input_buffer[p1->buffer_length - 3] == DOWN) || !p1->is_standing)) // pressed DOWN a short time ago
            {
                // ↓→+👊
                p1->wanted_animation = dash_punch;
            }

            // check for overhead
            else if ((p1->buffer_length > 0) &&
                     (p1->input_buffer[p1->buffer_length - 1] == KICK) &&// just pressed KICK
                     (key[ALLEGRO_KEY_D])) // is holding forward
            {
                // →+👟
                p1->wanted_animation = overhead;
            }
            
            // check for dash
            else if ((p1->buffer_length > 1) &&
                     (p1->input_buffer[p1->buffer_length - 1] == RIGHT) &&
                     (p1->input_buffer[p1->buffer_length - 2] == RIGHT)) // just pressed RIGHT twice
            {
                // →+→
                p1->wanted_animation = dash;
            }

            // check for backdash
            else if ((p1->buffer_length > 1) &&
                     (p1->input_buffer[p1->buffer_length - 1] == LEFT) &&
                     (p1->input_buffer[p1->buffer_length - 2] == LEFT)) // just pressed LEFT twice
            {
                // ←+←
                p1->wanted_animation = backdash;
            }

            // check for crMK
            else if ((p1->buffer_length > 0) &&
                     (p1->input_buffer[p1->buffer_length - 1] == KICK)) // just pressed KICK
            {
                // 👟
                p1->wanted_animation = crMK;
            }

            // check for crLP
            else if ((p1->buffer_length > 0) &&
                     (p1->input_buffer[p1->buffer_length - 1] == PUNCH)) // just pressed KICK
            {
                // 👊
                p1->wanted_animation = crLP;
            }

            // no special commands, just movement:

            else if(key[ALLEGRO_KEY_S])
            {
                p1->wanted_animation = crouching;
                if (key[ALLEGRO_KEY_A])
                    p1->is_blocking = true;
            }
            else if (key[ALLEGRO_KEY_A])
            {
                p1->is_blocking = true;
                p1->wanted_animation = walk_backwards;
            }
            else if (key[ALLEGRO_KEY_D])
            {
                if (p1->wanted_animation != crouching)
                    p1->wanted_animation = walk_forward;
            }

            if(p2_input_method == IS_CPU)
                get_autoplayer_input(p1, p2);
            else
            {
                p2->wanted_animation = walk_backwards;
                p2->is_blocking = true;
            }

            // Needed to keep the input system working

            for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
                key[i] &= KEY_SEEN;

            // reset player buffers if needed:
            if (frame_count > (p1->last_input_frame + BUFFER_TIMER))
            {
                p1->buffer_length = 0; // reset buffer
            }
            if (frame_count > (p2->last_input_frame + BUFFER_TIMER))
            {
                p2->buffer_length = 0; // reset buffer
            }

            break;

        case ALLEGRO_EVENT_KEY_DOWN:
            // store it in the array (useful for directional inputs)
            key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
            
            // proceed to store it in the buffer
            int input;
            int p;
            switch (event.keyboard.keycode)
            {

                case ALLEGRO_KEY_COMMA: // p1 kick
                    p = 1;
                    input = KICK;
                    break;

                case ALLEGRO_KEY_FULLSTOP: // p1 punch
                    p = 1;
                    input = PUNCH;
                    break;

                // case ALLEGRO_KEY_UP:
                //     p = 1;
                //     input = UP;
                //     break;

                case ALLEGRO_KEY_S:
                    p = 1;
                    input = DOWN;
                    break;

                case ALLEGRO_KEY_A:
                    p = 1;
                    input = LEFT;
                    break;

                case ALLEGRO_KEY_D:
                    p = 1;
                    input = RIGHT;
                    break;
            }
            if (p == 1)
            {
                p1->last_input_frame = frame_count;
                p1->input_buffer[p1->buffer_length] = input;
                p1->buffer_length++;
            }
            else // player 2
            {
                p2->last_input_frame = frame_count;
                p2->input_buffer[p2->buffer_length] = input;
                p2->buffer_length++;
            }

            break;

        case ALLEGRO_EVENT_KEY_UP:
            key[event.keyboard.keycode] &= KEY_RELEASED;
            break;
    }

    


    return 0;
}