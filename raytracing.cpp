//
// Created by wuzaiqiang on 19-1-31.
//
#include "raytracing.h"
template <typename scalar_t>
Image<scalar_t>::Image(const int h,const int w,const int c)
{
    this->height=h;
    this->width=w;
    this->channels=c;
    this->img_p=new scalar_t[h*w*c];
}
template <typename scalar_t>
Image<scalar_t>::Image(const Image<scalar_t> &img_b)
{
    this->height=img_b.height;
    this->width=img_b.width;
    this->channels=img_b.channels;
    this->img_p=new scalar_t[height*width*channels];
    for(int i=0;i<height*width*channels;i++)
    {
        (this->img_p)[i]=(img_b.img_p)[i];
    }
}
template <typename scalar_t>
Image<scalar_t>::~Image()
{
    delete [] img_p;
}