#include "Rogilink_BNO.hpp"

Rogilink_BNO::Rogilink_BNO()
    : uart(USBTX, USBRX, 115200, nullptr, 0)
    , sub(uart, 2)
    , pub(uart, 2)

{

}

void Rogilink_BNO::sub_callback(int _length, char* _cmd){

    vector<string> cmds;
    string item;
    for (int i = 0; i < _length; i++) {
        if (_cmd[i] == ',') {
            if (!item.empty())
                cmds.push_back(item);
            item.clear();
        }
        else {
            item += _cmd[i];
        }
    }
    if (!item.empty())
        cmds.push_back(item);

    if(cmds[0] == "Rate"){
        rate = std::stoi(cmds[1]);
        is_ready = true;
    }
    else if (cmds[0] == "Reset")
    {
        reset_flag = true;
    }
    else{
        return;
    }
}

void Rogilink_BNO::pub_callback(){
    if(is_ready){
        IMU::Euler<float> eul = bno->getEulerRadians();
        IMU::Quaternion<float> qua = bno->getQuaternion();
        IMU::LinearAcceleration<float> lia = bno->getLinearAcc();

        std::unique_ptr<sensor_msgs::msg::Imu> msg;
        msg->header.frame_id = "base_link";
        
        msg->orientation.w = qua.w;
        msg->orientation.x = qua.x;
        msg->orientation.y = qua.y;
        msg->orientation.z = qua.z;

        msg->orientation_covariance = {
            0, 0, 0,
            0, 0, 0,
            0, 0, 0.05
        };

        msg->linear_acceleration.x = lia.x;
        msg->linear_acceleration.y = lia.y;
        msg->linear_acceleration.z = lia.z;
        msg->linear_acceleration_covariance = {
            0, 0, 0,
            0, 0, 0,
            0, 0, 0
        };

        // angle velはeulerから取るつもりだったがquaternionからとったほうが楽か？
        msg->angular_velocity.x = eul.roll;
        msg->angular_velocity.y = eul.pitch;
        msg->angular_velocity.z = eul.yaw;
        msg->angular_velocity_covariance = {
            0, 0, 0,
            0, 0, 0,
            0, 0, 0
        };
    }
}
