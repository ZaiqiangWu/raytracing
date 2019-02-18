//
// Created by wuzaiqiang on 19-2-15.
//

#ifndef DISPLAYIMAGE_AABB_H
#define DISPLAYIMAGE_AABB_H


#include "vector3.h"

template <typename scalar_t>
class AABB
{
public:
    scalar_t x_min;
    scalar_t x_max;
    scalar_t y_min;
    scalar_t y_max;
    scalar_t z_min;
    scalar_t z_max;
    AABB()
    {
        x_max=0;
        x_min=0;
        y_min=0;
        y_max=0;
        z_max=0;
        z_min=0;
    }
    bool ishit(vector3<scalar_t> e,
               vector3<scalar_t> d)
    {
        scalar_t txmax,txmin,tymax,tymin,tzmax,tzmin;
        scalar_t den;
        if(fabs(d.x)<eps_t)
        {
            den=d.x>0?eps_t:-eps_t;
        }
        else
        {
            den=d.x;
        }
        txmax=MAX((x_min-e.x)/den,(x_max-e.x)/den);
        txmin=MIN((x_min-e.x)/den,(x_max-e.x)/den);
        if(fabs(d.y)<eps_t)
        {
            den=d.y>0?eps_t:-eps_t;
        }
        else
        {
            den=d.y;
        }
        tymax=MAX((y_min-e.y)/den,(y_max-e.y)/den);
        tymin=MIN((y_min-e.y)/den,(y_max-e.y)/den);
        if(fabs(d.z)<eps_t)
        {
            den=d.z>0?eps_t:-eps_t;
        }
        else
        {
            den=d.z;
        }
        tzmax=MAX((z_min-e.z)/den,(z_max-e.z)/den);
        tzmin=MIN((z_min-e.z)/den,(z_max-e.z)/den);
        if(MAX(txmin,MAX(tymin,tzmin))>MIN(txmax,MIN(tymax,tzmax)))
        {
            //cout<<0<<endl;
            return false;
        }
        else if(MIN(txmax,MIN(tymax,tzmax))<0)
        {
            //cout<<1<<endl;
            return false;
        }
        else
        {
            //cout<<2<<endl;
            return true;
        }
    }
    bool ishit(vector3<scalar_t> e,vector3<scalar_t> d,vector3<scalar_t> &inPoint,vector3<scalar_t> &outPoint)
    {
        scalar_t txmax,txmin,tymax,tymin,tzmax,tzmin;
        scalar_t den;
        if(fabs(d.x)<eps_t)
        {
            den=d.x>0?eps_t:-eps_t;
        }
        else
        {
            den=d.x;
        }
        txmax=MAX((x_min-e.x)/den,(x_max-e.x)/den);
        txmin=MIN((x_min-e.x)/den,(x_max-e.x)/den);
        if(fabs(d.y)<eps_t)
        {
            den=d.y>0?eps_t:-eps_t;
        }
        else
        {
            den=d.y;
        }
        tymax=MAX((y_min-e.y)/den,(y_max-e.y)/den);
        tymin=MIN((y_min-e.y)/den,(y_max-e.y)/den);
        if(fabs(d.z)<eps_t)
        {
            den=d.z>0?eps_t:-eps_t;
        }
        else
        {
            den=d.z;
        }
        tzmax=MAX((z_min-e.z)/den,(z_max-e.z)/den);
        tzmin=MIN((z_min-e.z)/den,(z_max-e.z)/den);
        if(MAX(txmin,MAX(tymin,tzmin))>MIN(txmax,MIN(tymax,tzmax)))
        {
            //cout<<0<<endl;
            return false;
        }
        else if(MIN(txmax,MIN(tymax,tzmax))<0)
        {
            //cout<<1<<endl;
            return false;
        }
        else
        {
            //cout<<2<<endl;
            inPoint=MAX(0,MAX(txmin,MAX(tymin,tzmin)))*d+e;
            outPoint=MIN(txmax,MIN(tymax,tzmax))*d+e;
            return true;
        }
    }
    bool IsIntersect(const AABB<scalar_t> aabb1)
    {
        bool flag=true;
        if(x_min>aabb1.x_max||x_max<aabb1.x_min)
            flag= false;
        if(y_min>aabb1.y_max||y_max<aabb1.y_min)
            flag= false;
        if(z_min>aabb1.z_max||z_max<aabb1.z_min)
            flag= false;
        return flag;
    }
};
#endif //DISPLAYIMAGE_AABB_H
