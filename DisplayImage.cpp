#include <iostream>
#include <opencv2/opencv.hpp>
#include "raytracing.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv )
{
    /*if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }*/
/*
    Mat image(Size(320,240),CV_32FC1);
    image.at<Vec3d>(0,0)[1]=1;
    int n1=image.rows;
    int nc=image.cols*image.channels();
    for (int j=0;j<n1;j++)
    {
        float* data=image.ptr<float>(j);
        for(int i=0;i<nc;i=i+3)
        {
            data[i+2]=0;
            data[i+1]=0;
            data[i]=1;

        }
    }*/
    //sphere<float> sphere0;
    //sphere0.center=vector3<float>(0,0,-2);
    //sphere0.radius=0.5;
    //Light<float> light();
    //Camera<float> cam(vector3<float>(0,0,2),vector3<float>(0,0,-1),vector3<float>(1,0,0),70*3.14/180.0,1.0);
    int batch=1;
    const int num_spheres=2;
    int size=512;

    Scence<float> scence(size,batch);

    float central_p[3*num_spheres]={-0.4,0.4,0.9,0.3,0.0,0.0};
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
    scence.render(&image);
    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", image);

    waitKey(0);

    return 0;
}
