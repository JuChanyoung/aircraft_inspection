#include "component_test/occlusion_culling.h"
#include <geometry_msgs/PoseArray.h>



int main(int argc, char **argv)
{

    ros::init(argc, argv, "test");
    ros::NodeHandle n;
    ros::Publisher visible_pub = n.advertise<sensor_msgs::PointCloud2>("occlusion_free_cloud", 100);
    ros::Publisher original_pub = n.advertise<sensor_msgs::PointCloud2>("original_point_cloud", 10);
    ros::Publisher vector_pub = n.advertise<geometry_msgs::PoseArray>("pose", 10);

    pcl::PointCloud<pcl::PointXYZ>::Ptr test (new pcl::PointCloud<pcl::PointXYZ>);
     pcl::PointCloud<pcl::PointXYZ> test3, test4, combined;
     geometry_msgs::PoseArray vec;
     OcclusionCulling obj(n,"scaled_desktop.pcd");

     geometry_msgs::Pose location;
    //example 1 from searchspace file
     location.position.x=0.0; location.position.y=2.0; location.position.z=1.0; location.orientation.x=0.649369; location.orientation.y=-0.27985;location.orientation.z=-0.649369;location.orientation.w=0.279856;
     test3 = obj.extractVisibleSurface(location);
     test->points = test3.points;
     vec.poses.push_back(location);
     float covpercent1 = obj.calcCoveragePercent(test);

     //example 2 from searchspace file
//     OcclusionCulling obj1(n,"scaled_desktop.pcd");
//     location.position.x=6; location.position.y=1; location.position.z=-36; location.orientation.x=-0.198723; location.orientation.y=-0.7181;location.orientation.z=0.198723;location.orientation.w=0.636672;
//     test3 = obj.extractVisibleSurface(location);

//     combined=test3;
//     combined+=test4;

//     float covpercent = obj.calcCoveragePercent(location);

//     float combinedcoverage = covpercent + covpercent1;

//     std::cout<<"coverage percentage: "<<combinedcoverage<<"%\n";
//     vec.poses.push_back(location);
     ros::Rate loop_rate(10);
     while (ros::ok())
     {
         std::cout<<"filtered original point cloud: "<<obj.filtered_cloud->size()<<"\n";
         sensor_msgs::PointCloud2 cloud1;
         pcl::toROSMsg(*obj.filtered_cloud, cloud1);
         cloud1.header.frame_id = "base_point_cloud";
         cloud1.header.stamp = ros::Time::now();
         original_pub.publish(cloud1);

         sensor_msgs::PointCloud2 cloud2;
         pcl::toROSMsg(test3, cloud2);
         cloud2.header.frame_id = "base_point_cloud";
         cloud2.header.stamp = ros::Time::now();

         vec.header.frame_id= "base_point_cloud";
         vec.header.stamp = ros::Time::now();
         vector_pub.publish(vec);

         visible_pub.publish(cloud2);
         ROS_INFO("Publishing Marker");

         ros::spinOnce();
         loop_rate.sleep();
     }


    return 0;
}