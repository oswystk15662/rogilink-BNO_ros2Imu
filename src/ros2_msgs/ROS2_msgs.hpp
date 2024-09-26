#pragma once

#include <math.h>
#include <time.h>
#include <string>

namespace std_msgs
{
    typedef struct
    {
        uint32_t seq;
        time_t stamp;
        std::string frame_id;
    }Header;
} // namespace std_msgs

namespace geometry_msgs
{
    namespace msg
    {
        typedef struct 
        {
            double x;
            double y;
            double z;
            double w;
        }Quaternion;
        
        typedef struct
        {
            double x;
            double y;
            double z;
        }Vector3;
        
    } // namespace msg
    
} // namespace geometry_msgs

