
#include "OptiTrackClient_test.hpp"

opti_track::TrackData trackData; // NOLINT(cert-err58-cpp)

struct Position
{
    double x;
    double y;
    double z;
};
 
struct Orientation
{
    double x;
    double y;
    double z;
    double w;
};

struct Pose
{
    Position position;
    Orientation orientation;
};

struct Marker
{
    int id;
    Position position;
    int state;
    float residual;
};

struct RigidBody
{
    int id;
    Pose pose;
    float mean_error;
    int tracking_flag;
};


int main(int argc, char **argv)
{

	std::string serverAddress = "192.168.1.13";
	std::string clientAddress = "192.168.1.23";

    OptiTrackClient client([](sFrameOfMocapData *data) {

        int marker_quantity = data->nLabeledMarkers;
        int rigid_body_quantity = data->nRigidBodies;

        std::vector<Marker> markers;
        for(int i=0; i < data->nLabeledMarkers; i++) {
            const auto &item = data->LabeledMarkers[i];
            struct Position position;
			struct Marker marker;

            position.x = item.x;
            position.y = item.y;
            position.z = item.z;
            std::cout << "x:" << position.x << std::endl;
            std::cout << "y:" << position.y << std::endl;

            marker.id = item.ID;
            marker.position = position;
            marker.state = item.params;
            marker.residual = item.residual;

            markers.push_back(marker);
        }

        std::vector<RigidBody> rigidBodies;
        for(int i=0; i < data->nRigidBodies; i++) {
            const auto &item = data->RigidBodies[i];
            struct Position position;
            struct Orientation orientation;
            struct RigidBody rigidBody;
            struct Pose pose;
            
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


    }, serverAddress.c_str(), clientAddress.c_str());


	return 0;
}