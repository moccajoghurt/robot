/*
 * Sobald node gestartet wird, wird ein neuer Ordner mit dem aktuellem Datum + Uhrzeit
 * im records Ordner erstellt.
 * Im Ordner werden alle Bilddateien mit einem Zähler als Dateinamen gespeichert.
 * Zusätzlich wird eine timestamp.txt Datei mit folgender Form erstellt:
 * "[Bilddatei-Nr] [sekunden.nanosekunden]"
 * 
 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/image_encodings.h>
#include <cv_bridge/cv_bridge.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

int im_count = 0;
string data_path;
fstream timestamp_file;

void save_img(const sensor_msgs::ImageConstPtr& msg);
string get_zero_name(int im_count);
void create_timestamp(string path);

int main(int argc, char** argv) {
	
	//erzeuge neuen Ordner mit Datum als Name im Ordner records
	time_t t = time(0);
    struct tm* now = localtime( &t );
    char buf[80];
    strftime(buf, sizeof(buf), "%d-%m-%Y_%X", now);
    data_path = "./records/" + string(buf) + "/";
    string cmd = "mkdir -p " + data_path;
    system( cmd.c_str() );
    
    //erstelle timestamp file
    string timestamp_path = data_path + "timestamp.txt";
    timestamp_file.open(timestamp_path.c_str(), ios::out);
    
    
    //starte node
	ros::init(argc, argv, "im_sync");
	ros::NodeHandle nh;
	image_transport::ImageTransport it(nh);
	image_transport::Subscriber image_sub = it.subscribe("/usb_cam/image_raw", 1, save_img);
	ros::spin();
}

void save_img(const sensor_msgs::ImageConstPtr& msg) {
	
	cv_bridge::CvImagePtr cv_ptr;
	try {
		cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
		
	} catch (cv_bridge::Exception& e) {
		ROS_ERROR("cv_bridge exception: %s", e.what());
	}
	
	string img_name = data_path + get_zero_name(im_count) + ".png";
	imwrite(img_name, cv_ptr->image);
	
	stringstream ss;
	ss << get_zero_name(im_count) << " " << msg->header.stamp << endl;
	timestamp_file << ss.str().c_str();
	
	im_count++;
}

string get_zero_name(int im_count) {
	stringstream ss;
	string name;
	ss << im_count;
	ss >> name;
	int len = name.length();
	for (int i = 0; i < 7 - len; i++) {
		name = "0" + name;
	}
	return name;
}

