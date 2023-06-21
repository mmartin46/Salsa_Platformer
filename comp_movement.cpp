#include "game.hpp"

void GameState::computer_player_movement()
{
    Coordinates pc;
    pc.x_1 = this->get_player()->get_x();
    pc.x_2 = this->get_comp_player()->get_x();
    pc.y_1 = this->get_player()->get_y();
    pc.y_2 = this->get_comp_player()->get_y();

    double plyr_distance = get_distances(pc.x_1, pc.x_2, 
                                                      pc.y_1, pc.y_2);

    pair<double, double> cp_c;
    cp_c.first = pc.x_1;
    cp_c.second = pc.x_1;

    // Clear out the vector.
    if (this->not_moving.size() > 20)
    {
        this->not_moving.clear();
    }


    // If the computer player is too far from the screen.
    if (plyr_distance > 400)
    {
        this->get_comp_player()->set_x(this->get_player()->get_x());
        this->get_comp_player()->set_y(this->get_player()->get_y());
    }

    // If the player and computer player are too far
    // distant.
    if (plyr_distance > 30)
    {
        // Right
        double state_1 = get_distances(pc.x_1 + 10, pc.x_2, pc.y_1, pc.y_2);
        // Down
        double state_2 = get_distances(pc.x_1, pc.x_2, pc.y_1 + 10, pc.y_2);
        // Left
        double state_3 = get_distances(pc.x_1 - 10, pc.x_2, pc.y_1, pc.y_2);
        // Up
        double state_4 = get_distances(pc.x_1, pc.x_2, pc.y_1 - 10, pc.y_2);

        // Up-Right
        double state_5 = get_distances(pc.x_1 + 10, pc.x_2, pc.y_1 + 10, pc.y_2);
        // Up-Left
        double state_6 = get_distances(pc.x_1 - 10, pc.x_2, pc.y_1 + 10, pc.y_2);
        // Down-Right
        double state_7 = get_distances(pc.x_1 + 10, pc.x_2, pc.y_1 - 10, pc.y_2);
        // Down-Left
        double state_8 = get_distances(pc.x_1 - 10, pc.x_2, pc.y_1 - 10, pc.y_2);


        vector<double> states = { state_1, state_2, state_3, state_4, state_5, state_6,
                                 state_7, state_8 };

        using std::cout;
        using std::endl;

        // for (const auto &s : states) 
        // {
        //     cout << s << " ";
        // }
        // cout << endl;

        double min_distance = minimum(states); 


        // Left
        if (min_distance == state_1)
        {
            //cout << "LEFT\n";
            this->get_comp_player()->apply_left_movement(3);
        }
        // Up
        else if (min_distance == state_2)
        {
            //cout << "UP\n";
            this->get_comp_player()->apply_up_movement(4);
        }
        // Right
        else if (min_distance == state_3)
        {
            //cout << "RIGHT\n";
            this->get_comp_player()->apply_right_movement(3);
        }
        // Down
        else if (min_distance == state_4)
        {
            //cout << "DOWN\n";
            this->get_comp_player()->apply_down_movement();
        }
        // Up-Left
        else if (min_distance == state_5)
        {   
            //cout << "UP-LEFT\n";
            this->get_comp_player()->apply_up_movement(COMP_PLAYER_HEIGHT - (state_5 / 100));
            this->get_comp_player()->apply_left_movement(3);
        }
        // Up-Right
        else if (min_distance == state_6)
        {   
            //cout << "UP-RIGHT\n";
            this->get_comp_player()->apply_up_movement(COMP_PLAYER_HEIGHT - (state_6 / 100));
            this->get_comp_player()->apply_right_movement(3);
        }
        // Down-Left
        else if (min_distance == state_7)
        {   
            //cout << "DOWN-LEFT\n";
            this->get_comp_player()->apply_down_movement();
            this->get_comp_player()->apply_left_movement(3);
        }
        // Down-Right
        else if (min_distance == state_8)
        {   
            //cout << "DOWN-RIGHT\n";
            this->get_comp_player()->apply_down_movement();
            this->get_comp_player()->apply_right_movement(3);
        }
    }
    else
    {
        // If the player isn't performing any of the moves
        // slow his speed to zero.
        this->get_comp_player()->slow_movement();
        this->not_moving.push_back(cp_c);
    }
}
