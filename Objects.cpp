#include "Objects.h"

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
            break;
    }   
}