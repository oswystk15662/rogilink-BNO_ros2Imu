#include "Rogilink_BNO.hpp"

Rogilink_BNO::Rogilink_BNO(uint8_t _id, PinName _tx, PinName _rx)
    : queue(32 * EVENTS_QUEUE_SIZE)
    , uart(USBTX, USBRX, 115200, nullptr, 0)
    , sub(uart, 2)
    , pub(uart, 2)
    , bno_id(_id)
    , tx(_tx)
    , rx(_rx)

{
    sub.set_callback(callback(this, &Rogilink_BNO::sub_callback));
}

void Rogilink_BNO::sub_callback(int _length, char* _cmd){
    //split
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

    // process
    if(cmds[0] == "Rate"){
        rate = std::stoi(cmds[1]);
        is_ready = true;
    }
    else if (cmds[0] == "Reset")
    {
        reset_flag = true;
    }
    else if(cmds[0] == "StartIMU"){
        bno = std::make_unique<IMU::BNO055>(tx, rx, std::chrono::milliseconds(1000 / rate));
        
        queue.call_every(std::chrono::milliseconds(1000 / rate), callback(this, &Rogilink_BNO::pub_callback));
        queue.dispatch_forever();
    }
    else{
        return;
    }
}

void Rogilink_BNO::pub_callback(){
    if(is_ready){
        IMU::Euler<double> eul = bno->getEulerRadians();
        IMU::Quaternion<double> qua = bno->getQuaternion();
        IMU::LinearAcceleration<double> lia = bno->getLinearAcc();

        std::unique_ptr<sensor_msgs::msg::Imu> msg;
        msg->header.frame_id = "base_link";
        
        msg->orientation.w = qua.w;
        msg->orientation.x = qua.x;
        msg->orientation.y = qua.y;
        msg->orientation.z = qua.z;

        msg->set_OrientationCovariance({
            0, 0, 0,
            0, 0, 0,
            0, 0, 0.05
        });

        msg->linear_acceleration.x = lia.x;
        msg->linear_acceleration.y = lia.y;
        msg->linear_acceleration.z = lia.z;
        msg->set_LinearAccelCovariance({
            0.01, 0, 0,
            0, 0.01, 0,
            0, 0, 0.01
        });

        // angle velはeulerから取るつもりだったがquaternionからとったほうが楽か？
        msg->angular_velocity.x = eul.roll;
        msg->angular_velocity.y = eul.pitch;
        msg->angular_velocity.z = eul.yaw;
        msg->set_AngleVelCovariance({
            0, 0, 0,
            0, 0, 0,
            0, 0, 0.05
        });

        pub.publish(this->bno_id, *msg);
    }
}

void Rogilink_BNO::reset_watchdog(){
    if(reset_flag){
        bno->reset({
            0
        });

        reset_flag = false;
    }
}
