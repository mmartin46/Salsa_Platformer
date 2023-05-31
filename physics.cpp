#include "main.hpp"

// Are two rectangles colliding.
int collide2d(float x1, float y1, float x2, float y2, float ht1, float wt1, float wt2, float ht2)
{
    return (!((x1 > (x2+wt2)) || (x2 > (x1+wt1)) || (y1 > (y2+ht2)) || (y2 > (y1+ht1))));
}

void process(GameState *gameState)
{
    // add time
    gameState->set_time(gameState->get_time() + 1);

    // plyr movement
    Player *plyr = &gameState->plyr;
    plyr->set_x(plyr->get_x() + plyr->get_dx());
    plyr->set_y(plyr->get_y() + plyr->get_dy());

    if (plyr->get_dx() != 0 && plyr->get_onBlock() && !plyr->slowingDown)
    {
        if (gameState->get_time() % 8 == 0)
        {
            if (plyr->animFrame == 0)
            {
                plyr->animFrame = 1;
            }
            else
            {
                plyr->animFrame = 0;
            }
        }
    }

    plyr->apply_gravity();

    gameState->set_scrollX(-gameState->plyr.get_x() + 320);
    
    if (gameState->get_scrollX() > 0)
    {
        gameState->set_scrollX(0);
    }
    if (gameState->get_scrollX() < -38000+320)
    {
        gameState->set_scrollX(-38000+320);
    }
}


void collisionDetect(GameState *gameState)
{
    // Check for collision with any blocks (brick blocks)
    for (int i = 0; i < 100; i++)
    {
        float pw = PLAYER_WIDTH, ph = PLAYER_HEIGHT;
        float px = gameState->plyr.get_x(), py = gameState->plyr.get_y();
        float bx = gameState->blocks[i].x, by = gameState->blocks[i].y, bw = gameState->blocks[i].w, bh = gameState->blocks[i].h;
    
        if (px+pw/2 > bx && px+pw/2 < bx+bw)
        {
            // Head Bump
            if (py < by+bh && py>by && gameState->plyr.get_dy() < 0)
            {
                // correct y
                gameState->plyr.set_y(by+bh);
                py = by+bh;

                // bumped our head, stop any jump velocity
                gameState->plyr.set_dy(0);
                gameState->plyr.set_onBlock();
            }
        }
        if (px+pw > bx && px<bx+bw)
        {
            // Head bump
            if (py+ph > by && py < by && gameState->plyr.get_dy() > 0)
            {
                // correct y
                gameState->plyr.set_y(by-ph);
                py = by-ph;

                //landed on this ledge, stop any jump velocity
                gameState->plyr.set_dy(0);
                gameState->plyr.set_onBlock();
            }
        }
        if (py+ph > by && py<by+bh)
        {
            // Rubbing against right edge
            if (px < bx+bw && px+pw > bx+bw && gameState->plyr.get_dx() < 0)
            {
                // correct x
                gameState->plyr.set_x(bx+bw);
                px = bx+bw;

                gameState->plyr.set_dx(0);
            }
            // Rubbing against left edge
            else if (px+pw > bx && px < bx && gameState->plyr.get_dx() > 0)
            {
                // correct x
                gameState->plyr.set_x(bx-pw);
                px = bx-pw;

                gameState->plyr.set_dx(0);
            }
        }
    }
}
