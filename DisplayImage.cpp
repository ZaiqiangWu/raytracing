#include <iostream>
#include <opencv2/opencv.hpp>
#include "raytracing.h"


using namespace cv;
using namespace std;

int main(int argc, char** argv )
{
    int batch=1;
    const int num_spheres=2;
    int size=512;

    Scence<float> scence(size,batch);

    float central_p[3*num_spheres]={-0.5,0.0,-0.0,0.3,0.0,0.0};
    float radius_p[num_spheres]={0.5,0.6};
    for(int i=0;i<batch;i++)
    {
        for(int j=0;j<num_spheres;j++)
        {
            scence.objs[i].append(vector3<float>(central_p[i*num_spheres*3+j*3],central_p[i*num_spheres*3+j*3+1],central_p[i*num_spheres*3+j*3+2]),radius_p[i*num_spheres+j]);
        }
    }
    cout<<scence.objs[0].len<<endl;
    cv::Mat image(cv::Size(size,size),CV_32FC1);
    //Image<float> img=scence.render();
    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", scence.render().GetOpencvMat());

    waitKey(0);

    return 0;
}
