//
// Created by wuzaiqiang on 19-1-31.
//
#include "raytracing.h"



template <typename scalar_t>
vector3<scalar_t>::vector3(scalar_t x, scalar_t y, scalar_t z) {
    this->x=x;
    this->y=y;
    this->z=z;
}

template <typename scalar_t>
vector3<scalar_t> vector3<scalar_t>::operator+(const vector3<scalar_t>& v2)
{
    vector3 res;
    res.x=this->x+v2.x;
    res.y=this->y+v2.y;
    res.z=this->z+v2.z;
    return res;
}

template <typename scalar_t>
vector3<scalar_t> vector3<scalar_t>::operator-(const vector3<scalar_t>& v2)
{
vector3 res;
res.x=this->x-v2.x;
res.y=this->y-v2.y;
res.z=this->z-v2.z;
return res;
}

template <typename scalar_t>
Image<scalar_t>::Image(const int h, const int w, const int c) {
    this->height = h;
    this->width = w;
    this->channels = c;
    if(channels>1) channels=3;
    this->img_p = new scalar_t[h * w * c];
}

template<typename scalar_t>
Image<scalar_t>::Image(const Image<scalar_t> &img_b) {
    this->height = img_b.height;
    this->width = img_b.width;
    this->channels = img_b.channels;
    this->img_p = new scalar_t[height * width * channels];
    for (int i = 0; i < height * width * channels; i++) {
        (this->img_p)[i] = (img_b.img_p)[i];
    }
}

template <typename scalar_t>
cv::Mat Image<scalar_t>::GetOpencvMat() {
    cv::Mat cvMat;
    if(1==channels) cvMat.create(height,width,CV_32FC1);
    else cvMat.create(height,width,CV_32FC3);
    int c = cvMat.channels();
    int rows = cvMat.rows; //通道数 * 每个通道包含的列数
    int cols = c * cvMat.cols;

    //判断是否是连续存储
    if (cvMat.isContinuous())
    {
        cols *= rows;
        rows = 1;
    }

    float* p;
    for(int i = 0; i < rows; i++)
    {
        p = cvMat.ptr<float>(i);
        for (int j = 0; j < cols; j++)
        {
            //rgb bgr
            if(3==c)
            {
                p[j-j%3+2]=(float)img_p[i*cols+j-j%3];
                p[j-j%3+1]=(float)img_p[i*cols+j-j%3+1];
                p[j-j%3]=(float)img_p[i*cols+j-j%3+2];
            }
            else
                p[j]=(float)img_p[i*cols+j];
        }
    }
    return cvMat;
}

template<typename scalar_t>
Image<scalar_t>::~Image() {
    delete [] img_p;
}

template <typename scalar_t>
Image<scalar_t> Scence<scalar_t>::render()
{
    //cv::Mat img(cv::Size(size,size),CV_32FC1);
    Image<scalar_t> img(size,size,1);
    scalar_t *data=img.ptr();
    for(int u=0;u<size;u++)
    {
        for(int v=0;v<size;v++)
        {
            //cout<<"here0"<<endl;
            Ray<scalar_t> ray=camera.generateRay(u,v,size);
            //cout<<"here1"<<endl;
            IntersectionResult<scalar_t> result;
            //cout<<"here2"<<endl;
            bool ishit;
            ishit=objs->hit(ray.origin,ray.direction,0,100,result);
            //cout<<"here3"<<endl;
            if(ishit)
            {
                //cout<<"hit"<<endl;
                data[u*size+v]=min(1.0,this->light.ambient+max(0.0,0.9*result.normal.dot(light.direction.normalize())));
            }
            else
            {
                data[u*size+v]=0.0;
            }

        }
    }
    return img;
}