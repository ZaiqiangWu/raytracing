#include <iostream>
#include <opencv2/opencv.hpp>
#include "raytracing.h"
//#include "color.h"


//using namespace cv;
using namespace std;

int main(int argc, char** argv )
{
    const int num_spheres=1;
    int size=1024;

    Scence<float> scence(size);

    float central_p[3*num_spheres]={-0.9,0.0,-0.0};
    float radius_p[num_spheres]={0.6};
    sphere<float> sp0(vector3<float>(-0.9,0.0,-0.0),0.6,1);
    rectangle<float> rec0(1,2);
    rec0.rotationY(-0.6);

    scence.objs.append(&sp0);
    scence.objs.append(&rec0);


    cout<<scence.objs.len<<endl;
    //cv::Mat image(cv::Size(size,size),CV_32FC1);
    //Image<float> img=scence.render();
    cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
    cv::imshow("Display Image", scence.render().GetOpencvMat());

    cv::waitKey(0);

    return 0;
}
