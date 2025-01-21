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
                    bool can_pass_right = (this + 1)->type == Air;
                    bool left = (below - 1)->type == Air;
                    bool can_pass_left = (this - 1)->type == Air;
                    if(pos.x == (WIDTH/CELL_SIZE) - 1){
                        right = false;
                    }
                    if(pos.x == 0){
                        left = false;
                    }
                    switch ((right && can_pass_right) | (left && can_pass_left) << 1)
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
    // printf("%d %d\n", pos.x, pos.y);
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
                bool right_is_air;
                bool left_is_air;
                if(pos.x == 0) left_is_air = false; else {left_is_air = (this - 1)->type == Air;}
                if(pos.x == WIDTH/CELL_SIZE - 1) right_is_air = false; else {right_is_air = (this + 1)->type == Air;}
                switch (left_is_air | right_is_air << 1)
                {
                    case 1:
                        case_1:
                        if(TouchingGround(this, pos)){
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
                        if(TouchingGround(this, pos)){
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

bool TouchingGround(Object *obj, Vector2D pos)
{
    int increment = 1;
    while(pos.y + increment < HEIGHT/CELL_SIZE){
        switch ((obj + (increment * (WIDTH / CELL_SIZE)))->type)
        {
            case Air:
                return false;
            case Stone:
                return true;
            case Sand:
                increment++;
                break;
        }
    }
    return true;
}
