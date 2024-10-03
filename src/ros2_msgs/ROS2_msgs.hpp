#pragma once

#include <math.h>
#include <time.h>
#include <string>

constexpr size_t ONEBYTE = sizeof(uint8_t);
constexpr size_t TWOBYTE = sizeof(uint16_t);
constexpr size_t THREEBYTE = ONEBYTE + TWOBYTE;
constexpr size_t FOURBYTE = sizeof(uint32_t);

namespace std_msgs
{
    typedef struct
    {
        uint32_t seq;
        time_t stamp;
        char frame_id[16]; // 16文字以上は不可

        const size_t SIZE_HEADER = 20U + sizeof(time_t);

        void serialize(uint8_t* data){
            memcpy(data, &seq, 4U);
            memcpy(data + 4U, &stamp, sizeof(time_t));
            memcpy(data + 4U + sizeof(time_t), &frame_id, 16U);
        }

        void deserialize(uint8_t* data, size_t* size){
            memcpy(&seq, data, 4U);
            memcpy(&stamp, data + 4U, sizeof(time_t));
            memcpy(&frame_id, data + 4U + sizeof(time_t), 16U);
            *size = SIZE_HEADER;
        }
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

