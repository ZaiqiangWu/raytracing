//
// Created by wuzaiqiang on 19-2-9.
//

#ifndef DISPLAYIMAGE_VECTOR3_H
#define DISPLAYIMAGE_VECTOR3_H
//#include<algorithm>
#ifndef MIN
#define MIN(a,b)  (((a)<(b))?(a):(b))
#endif
#ifndef MAX
#define MAX(a,b)  (((a)>(b))?(a):(b))
#endif

template <typename scalar_t>
class vector3
{
public:
    scalar_t x;
    scalar_t y;
    scalar_t z;
    vector3(scalar_t x=0,scalar_t y=0,scalar_t z=0);
    vector3 operator+(const vector3& v2);
    vector3 operator+=(const vector3 &v2)
    {
        x=x+v2.x;
        y=y+v2.y;
        z=z+v2.z;
        return *this;
    }
    vector3 operator-=(const vector3 &v2)
    {
        x=x-v2.x;
        y=y-v2.y;
        z=z-v2.z;
        return *this;
    }
    vector3 clip(scalar_t low=0,scalar_t high=1)
    {
        vector3 tmp;
        tmp.x=MAX(low,MIN(x,high));
        tmp.y=MAX(low,MIN(y,high));
        tmp.z=MAX(low,MIN(z,high));
        return tmp;
    }
    vector3 negative()
    {
        vector3 tmp;
        tmp.x=-x;
        tmp.y=-y;
        tmp.z=-z;
        return tmp;
    }
    void clip_(scalar_t low=0,scalar_t high=1)
    {
        x=max(low,min(x,high));
        y=max(low,min(y,high));
        z=max(low,min(z,high));
    }

    vector3 operator-(const vector3& v2);
    vector3 operator*(scalar_t v) const;
    vector3 operator*(const vector3& v2) const
    {
        vector3 res;
        res.x=x*v2.x;
        res.y=y*v2.y;
        res.z=z*v2.z;
        return res;
    }
    template <typename scalar_t1>
    friend vector3<scalar_t1> operator*(const scalar_t1 &v,const vector3<scalar_t1> &vec);
    vector3 operator/(scalar_t v) const;
    scalar_t dot(const vector3& v2);
    vector3 cross(const vector3& v2);
    scalar_t length();
    scalar_t squareLength();
    vector3 normalize();
};

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
vector3<scalar_t> vector3<scalar_t>::operator*(scalar_t v) const //表示这个函数只有读的操作，没有写操作，不允许函数改变成员变量的值
{
    return vector3(v*x,v*y,v*z);
}

template <typename scalar_t1>
vector3<scalar_t1> operator*(const scalar_t1 &v,const vector3<scalar_t1> &vec)
{
    return vec*v;
}

template <typename scalar_t>
vector3<scalar_t> vector3<scalar_t>:: operator/(scalar_t v) const
{
    return vector3(x/v,y/v,z/v);
}

template <typename scalar_t>
scalar_t vector3<scalar_t>::dot(const vector3& v2)
{
    return this->x*v2.x+this->y*v2.y+this->z*v2.z;
}

template <typename scalar_t>
vector3<scalar_t> vector3<scalar_t>::cross(const vector3<scalar_t> &v2)
{
    vector3 res;
    res.x=this->y*v2.z-this->z*v2.y;
    res.y=this->z*v2.x-this->x*v2.z;
    res.z=this->x*v2.y-this->y*v2.x;
    return res;
}

template <typename scalar_t>
scalar_t vector3<scalar_t>::squareLength()
{
    return x*x+y*y+z*z;
}

template <typename scalar_t>
scalar_t vector3<scalar_t>::length()
{
    return sqrt(x*x+y*y+z*z);
}

template <typename scalar_t>
vector3<scalar_t> vector3<scalar_t>::normalize()
{
    scalar_t invlen=1/length();
    return *this*invlen;
}



#endif //DISPLAYIMAGE_VECTOR3_H
