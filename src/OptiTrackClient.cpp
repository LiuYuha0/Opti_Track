
#include "OptiTrackClient.hpp"

//using opti_track::msg::TrackData;
//using opti_track::msg::Marker;
//using opti_track::msg::RigidBody;
//using geometry_msgs::msg::Point32;
//using geometry_msgs::msg::Pose;
//using geometry_msgs::msg::Point;
//using geometry_msgs::msg::Quaternion;


//std::shared_ptr<rclcpp::Node> node;
//std::shared_ptr<rclcpp::Publisher<TrackData>> publisher;
ros::Publisher publisher;
opti_track::TrackData trackData; // NOLINT(cert-err58-cpp)

//template<typename T>
//rclcpp::Parameter get_parameter(const std::shared_ptr<rclcpp::Node> & node, const std::string & name, T defaultValue) {
//    auto param = rclcpp::Parameter(name, defaultValue);
//    node->get_parameter_or(name, param, param);
//    return param;
//}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "opti_track_ros1");

	ros::NodeHandle nh("~");

//    node->declare_parameter("server_address");
//    node->declare_parameter("client_address");
//    auto serverAddress = get_parameter(node, "server_address", "192.168.1.12").as_string();
//    auto clientAddress = get_parameter(node, "client_address", "").as_string();
	std::string serverAddress;
	std::string clientAddress;
	nh.getParam("server_address", serverAddress);
	nh.getParam("client_address", clientAddress);
	ROS_INFO("serverAddress : %s", serverAddress.c_str());
	ROS_INFO("client_address : %s", clientAddress.c_str());

//    RCLCPP_INFO(node->get_logger(), "OptiTrackClient startups with: serverAddress: %s, clientAddress: %s", serverAddress.data(), clientAddress.data());

	publisher = nh.advertise<opti_track::TrackData>("opti_track_data", 1);
//    publisher = node->create_publisher<TrackData>("opti_track_data", 10);

    OptiTrackClient client([](sFrameOfMocapData *data) {
        trackData.header.frame_id = std::to_string(data->iFrame);
        trackData.header.stamp = ros::Time::now();
        trackData.marker_quantity = data->nLabeledMarkers;
        trackData.rigid_body_quantity = data->nRigidBodies;

        std::vector<opti_track::Marker> markers;
        for(int i=0; i < data->nLabeledMarkers; i++) {
            const auto &item = data->LabeledMarkers[i];
			opti_track::Marker marker;
			geometry_msgs::Point32 position;

            position.x = item.x;
            position.y = item.y;
            position.z = item.z;

            marker.id = item.ID;
            marker.position = position;
            marker.state = item.params;
            marker.residual = item.residual;

            markers.push_back(marker);
        }
        trackData.markers = markers;

        std::vector<opti_track::RigidBody> rigidBodies;
        for(int i=0; i < data->nRigidBodies; i++) {
            const auto &item = data->RigidBodies[i];
			opti_track::RigidBody rigidBody;
			geometry_msgs::Pose pose;
			geometry_msgs::Point position;
			geometry_msgs::Quaternion orientation;

            position.x = item.x;
            position.y = item.y;
            position.z = item.z;
            orientation.x = item.qx;
            orientation.y = item.qy;
            orientation.z = item.qz;
            orientation.w = item.qw;

            pose.position = position;
            pose.orientation = orientation;

            rigidBody.id = item.ID;
            rigidBody.pose = pose;
            rigidBody.mean_error = item.MeanError;
            rigidBody.tracking_flag = item.params;

            rigidBodies.push_back(rigidBody);
        }
        trackData.rigid_bodies = rigidBodies;

        publisher.publish(trackData);

    }, serverAddress.c_str(), clientAddress.c_str());

	ros::spin();

//    rclcpp::spin(node);
//    rclcpp::shutdown();

	return 0;
}