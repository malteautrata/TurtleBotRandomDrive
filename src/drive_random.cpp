#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/LaserScan.h"

void callback();
ros::Publisher pub;
ros::Subscriber sub;
bool shouldDodge = false;
bool randomBool()
{
    return rand() % 2;
}

void callback(const sensor_msgs::LaserScan::ConstPtr& msg)
{
    geometry_msgs::Twist twist;

    // Herausfinden wieviele Daten im angle[] array gespeichert sind: von 0 bis 359
    //int num_readings = std::ceil( (msg->angle_max - msg->angle_min) / msg->angle_increment );
    //std::cout << num_readings << std::endl;
    //std::cout << "Range 350:" <<  msg->ranges[350] << std::endl;


    // Herausfinden welche bei der Ausweichung sinnvoll sind
    /*
    for (int i = 0; i <= 359; i++)
    {
        if (msg->ranges[i] <= 0.3 && msg->ranges[i] >= 0.00001)
        {
            std::cout << i << std::endl;
        } 
    }
    std::cout << "new" << std::endl;
    */

    shouldDodge = false;

    for(int i = 1; i <= 40; i++)
    {
        if (msg->ranges[i] <= 0.1 && msg->ranges[i] >= 0.0001)
        {
            shouldDodge = true;
        }
    }

    for(int i = 320; i <= 359; i++)
    {
        if (msg->ranges[i] <= 0.1 && msg->ranges[i] >= 0.0001)
        {
            shouldDodge = true;
        }
    }

    if (msg->ranges[0] <= 0.2)
    {
        shouldDodge = true;
    }

    if(shouldDodge)
    {
        twist.linear.x = 0.0; twist.linear.y = 0.0; twist.linear.z = 0.0;
        std::cout << randomBool() << std::endl;
        if(randomBool() == 1)
        {
            twist.angular.x = 0.0; twist.angular.y = 0.0; twist.angular.z = 10.0;

        }
        else {
            twist.angular.x = 0.0; twist.angular.y = 0.0; twist.angular.z = -10.0;

        }
    } else {
        twist.linear.x = 0.5; twist.linear.y = 0.0; twist.linear.z = 0.0;
    }
    pub.publish(twist);
}

int main(int argc, char** argv){
    time_t seconds;
    time(&seconds);
    srand((unsigned int) seconds);
    ros::init(argc, argv, "drive_random");
    ros::NodeHandle n;
    pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 10);
    sub = n.subscribe("/scan", 10, callback);
    while(ros::ok())
    {
        ros::spin();
    }
 }
