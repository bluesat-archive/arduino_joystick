#include "ros_video_components/ros_video_component.hpp"

ROS_Video_Component::ROS_Video_Component(QQuickItem * parent) :
    QQuickPaintedItem(parent), current_image(NULL), current_buffer(NULL)  {

}

void ROS_Video_Component::setup(ros::NodeHandle * nh) {

    image_transport::ImageTransport img_trans(*nh);
    //TODO: make these parameters of the component
    image_sub = img_trans.subscribe(
          "/cam0",
          1,
          &ROS_Video_Component::receive_image,
          this,
          image_transport::TransportHints("compressed")
    );


    ROS_INFO("Setup of video component complete");
}

void ROS_Video_Component::receive_image(const sensor_msgs::Image::ConstPtr &msg) {
    // check to see if we already have an image frame, if we do then we need to
    // delete it to avoid memory leaks
    if( current_image ) {
        delete current_image;
    }

    // allocate a buffer of sufficient size to contain our video frame
    uchar * temp_buffer = (uchar *) malloc(sizeof(uchar) * msg->data.size());

    // and copy the message into the buffer
    // we need to do this because QImage api requires the buffer we pass in to
    // continue to exist whilst the image is in use, but the msg and it's data will
    // be lost once we leave this context
    current_image = new QImage(
          temp_buffer,
          msg->width,
          msg->height,
          QImage::Format_RGB888 // TODO: detect the type from the message
    );

    ROS_INFO("Recieved Message");

    // We then swap out of bufer to avoid memory leaks
    if(current_buffer) {
        delete current_buffer;
        current_buffer = temp_buffer;
    }

    // finally we need to re-render the component to display the new image
    update();
}

void ROS_Video_Component::paint(QPainter * painter) {
    if(current_image) {
        painter->drawImage(QPoint(0,0), *(this->current_image));
    }
}
