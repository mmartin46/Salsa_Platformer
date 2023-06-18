#include <vector>

using std::vector;

// Returns the minimum in a vector.
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