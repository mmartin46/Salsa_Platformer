#include "utilities.hpp"


double minimum(const vector<double> &v)
{
    double min = v.at(0);
    typename vector<double>::const_pointer p, end = v.data() + v.size();
    for (p = v.data(); p < end; ++p)
    {
        if (min > *p)
        {
            min = *p;
        }
    }
    return min;
}

int collide2d(float x1, float y1, float x2, float y2, float ht1, float wt1, float wt2, float ht2)
{
    return (!((x1 > (x2+wt2)) || (x2 > (x1+wt1)) || (y1 > (y2+ht2)) || (y2 > (y1+ht1))));
}

SDL_Surface* get_surface(const char *file, const char *error_msg)
{
    SDL_Surface *surface = NULL;
    
    surface = IMG_Load(file);
    if (surface == NULL)
    {
        printf(error_msg);
        SDL_Quit();
        exit(1);
    } 
    return surface;   
}

double get_distances(double x_1, double x_2, double y_1, double y_2)
{
    double val = sqrt(pow(x_1 - x_2, 2) + pow(y_1 - y_2, 2));
    return val;
}

void modify_block_textures(vector<pair<string,string> > &surface_args, string idx, vector<string>& dont_include)
{    
    // Converts the strings in the dont_include
    // vector to lowercase.
    for (auto &it : dont_include)
    {
        std::transform(it.begin(), it.end(), it.begin(), ::tolower);
    }

    // Modifies the file names and error messages.
    vector<pair<string,string> >::pointer ptr, end = surface_args.data() + surface_args.size();
    for (ptr = surface_args.data(); ptr < end; ++ptr)
    {
        string starter = ptr->first;

        // If the file contains taco_soil, break.
        if ( starter.find("taco_soil") != string::npos )
        {
            ptr->first = "img\\taco_soil" + idx + ".png";
            ptr->second = "Cannot find taco_soil" + idx + ".png!\n\n";
            break;
        }

        // If the string doesn't contain any of the dont_include
        // elements, proceed.
        if ( starter.find(dont_include.at(0)) == string::npos &&
             starter.find(dont_include.at(1)) == string::npos &&
             starter.find(dont_include.at(2)) == string::npos )
        {
            string request = ptr->first;
            string period = {"."};
            std::size_t index = request.find(period);
            request.insert(index, idx);
            ptr->first = request;

            request = ptr->second;
            index = request.find(period);
            request.insert(index, idx);
            ptr->second = request;
        }
    }

}

void modify_player_textures(shared_ptr<Player> plyr, SDL_Renderer *r, const char *s1, const char *s2, int size, SDL_Surface *surface)
{
   string req, err;
   using std::to_string;

   for (int i = 0; i < size; ++i)
   {
       req = s1 + to_string(i + 1) + ".png";
       err = s2 + to_string(i + 1) + ".png!\n\n";

       surface = get_surface(req.c_str(), err.c_str());
       plyr->set_player_frame(i, SDL_CreateTextureFromSurface(r, surface));
       SDL_FreeSurface(surface);  
   }     
}


