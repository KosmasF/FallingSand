#include "Objects.h"
#include <csignal>

void Object::Update(Vector2D pos)
{
    // if(pos.y == HEIGHT/CELL_SIZE - 1)
    //     return;
    switch(type)
    {
        case Sand:
        {
            Object* below = this + (WIDTH/CELL_SIZE);
            if(pos.y != (HEIGHT/CELL_SIZE) - 1){
                if(below->type == Air){
                    below->type = type;
                    below->color = color;
                    type = Air;
                }
                else{
                    bool right = (below + 1)->type == Air;
                    bool left = (below - 1)->type == Air;
                    if(pos.x == (WIDTH/CELL_SIZE) - 1){
                        right = false;
                    }
                    if(pos.x == 0){
                        left = false;
                    }
                    switch (right | left << 1)
                    {
                        case 0:
                            return;
                        case 1:
                            case_1:
                            (below + 1)->type = type;
                            (below + 1)->color = color;
                            type = Air;
                            return;
                        case 2:
                            case_2:
                            (below - 1)->type = type;
                            (below - 1)->color = color;
                            type = Air;
                            return;
                        case 3:
                            bool will_go_right = rand() > RAND_MAX / 2;
                            if(will_go_right){
                                goto case_1;
                            }
                            else{
                                goto case_2;
                            }
                    }
                }
            }
            return;
        }
        default:
            break;
    }    
}

void Object::Fall(Vector2D pos)
{
    switch(type)
    {
        case Sand:
        {
            Object* below = this + (WIDTH/CELL_SIZE);
            if(pos.y != (HEIGHT/CELL_SIZE) - 1){
                if(below->type == Air){
                    below->type = type;
                    below->color = color;
                    type = Air;
                }
            }
            return;
        }
        default:
            return;
    }  
}

void Object::Slide(Vector2D pos)
{
    switch(type)
    {
        case Sand:
        {
            Object* below = this + (WIDTH/CELL_SIZE);
            if(pos.y != (HEIGHT/CELL_SIZE) - 1){
                bool right = (below + 1)->type == Air;
                bool left = (below - 1)->type == Air;
                if(pos.x == (WIDTH/CELL_SIZE) - 1){
                    right = false;
                }
                if(pos.x == 0){
                    left = false;
                }
                switch (right | left << 1)
                {
                    case 0:
                        return;
                    case 1:
                        case_1:
                        (below + 1)->type = type;
                        (below + 1)->color = color;
                        type = Air;
                        return;
                    case 2:
                        case_2:
                        (below - 1)->type = type;
                        (below - 1)->color = color;
                        type = Air;
                        return;
                    case 3:
                        bool will_go_right = rand() > RAND_MAX / 2;
                        if(will_go_right){
                            goto case_1;
                        }
                        else{
                            goto case_2;
                        }
                }
            }
            return;
        }
        default:
            return;
    }   
}

void Object::Erosion(Vector2D pos)
{
    switch(type)
    {
        case Sand:
        {
            {
                bool right_is_air = (this + 1)->type == Air;
                bool left_is_air = (this - 1)->type == Air;
                if(pos.x == 0) left_is_air = false;
                if(pos.x == WIDTH/CELL_SIZE - 1) right_is_air = false;
                switch (left_is_air | right_is_air << 1)
                {
                    case 1:
                        case_1:
                        {
                            int sand_at_right = 0;
                            int increment = 1;
                            while (pos.x + increment < WIDTH/CELL_SIZE)
                            {
                                if((this + increment)->type != Air){
                                    sand_at_right++;
                                    increment++;
                                }else{
                                    break;
                                }
                            }
                            float probability_of_falling = 1.f/(EROSION_INVERSE_STRENGTH + (EROSION_INVERSE_STRENGTH*(sand_at_right+1)));
                            if(rand() < RAND_MAX * probability_of_falling){
                                (this - 1)->type = type;
                                (this - 1)->color = color;
                                type = Air;
                            }
                        }
                        return;
                    case 2:
                        case_2:
                        {
                            int sand_at_left = 0;
                            int increment = 1;
                            while (pos.x - increment > -1)
                            {
                                if((this - increment)->type != Air){
                                    sand_at_left++;
                                    increment++;
                                }else{
                                    break;
                                }
                            }
                            float probability_of_falling = 1.f/(EROSION_INVERSE_STRENGTH + (EROSION_INVERSE_STRENGTH*(sand_at_left+1)));
                            if(rand() < RAND_MAX * probability_of_falling){
                                (this + 1)->type = type;
                                (this + 1)->color = color;
                                type = Air;
                            }
                        }
                        return;
                    case 3:
                        switch (rand() > RAND_MAX/2)
                        {
                            case 1:
                                goto case_1;
                            case 0:
                                goto case_2;
                        }
                }
            }
            return;
        }
        default:
            return;
    }
}
