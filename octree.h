//
// Created by wuzaiqiang on 19-2-16.
//

#ifndef DISPLAYIMAGE_OCTREE_H
#define DISPLAYIMAGE_OCTREE_H

#include "vector3.h"
#include "aabb.h"
#include "triangle.h"
#include "string"
template <typename scalar_t>
class TriangleNode
{
    Triangle<scalar_t> *triangle;
    TriangleNode<scalar_t> *NextNode;
    TriangleNode()
    {
        triangle=NULL;
        NextNode=NULL;
    }
};
template <typename scalar_t>
class TriangleList
{
    TriangleNode<scalar_t> *head;
    TriangleNode<scalar_t> *end;
    int size;
    TriangleList()
    {
        head=NULL;
        end=NULL;
        size=0;
    }
    void append(Triangle<scalar_t> *tri)
    {
        if(!head)
        {
            head=new TriangleNode<scalar_t>;
            head->triangle=tri;
            end=head;
            size++;
        }
        else
        {
            end->NextNode=new TriangleNode<scalar_t>;
            end=end->NextNode;
            end->triangle=tri;
            size++;
        }
    }
    ~TriangleList()
    {
        free_mem(head);
    }
    void free_mem(TriangleNode<scalar_t> *node_p)
    {
        if(node_p)
        {
            if(node_p->NextNode)
            {
                free_mem(node_p->NextNode);
            }
            else
            {
                delete node_p;
            }
        }
    }
};

template <typename scalar_t>
class OctreeNode
{
public:
    string mortonCode;
    Triangle<scalar_t> *triangles;
    bool isleaf;
    int num_triangles;
    AABB<scalar_t> aabb;
    OctreeNode<scalar_t> *children[8];
    OctreeNode()
    {
        triangles=NULL;
        isleaf=true;
        num_triangles=0;
        for(int i=0;i<8;i++)
        {
            children[i]=NULL;
        }
    }
};
template <typename scalar_t>
class Octree
{
public:
    OctreeNode<scalar_t> root;
    unsigned int max_depth;
    int max_faces;
    Octree()
    {
        max_depth=8;
        root.mortonCode=string(max_depth,'F');
    }
    void divide(OctreeNode<scalar_t> node)
    {
        OctreeNode<scalar_t> *node_p=new OctreeNode<scalar_t>[8];
        for(int i=0;i<8;i++)
        {
            node.children[i]=&node_p[i];
            node.children[i]->aabb=subAABB(node.aabb,i);//todo
        }
        if(node.num_triangles>max_faces)
        {
            ;
        }
    }
    string coor2code(vector3<scalar_t> coordinate)
    {
        string code;
        if(!is_in_aabb(coordinate,root.aabb))
            return string(max_depth,'F');
        else
        {
            AABB<scalar_t> aabb_test=root.aabb;
            while(code.length()<max_depth)
            {
                for(unsigned char i=0;i<8;i++)
                {
                    if(is_in_aabb(coordinate,subAABB(aabb_test,i)))
                    {
                        code.push_back('0'+i);
                        aabb_test=subAABB(aabb_test,i);
                        break;
                    }
                }
            }
            return code;
        }
    }
    AABB<scalar_t> code2aabb(string code)
    {
        AABB<scalar_t> aabb_res=root.aabb;
        for(unsigned char i=0;i<max_depth;i++)
        {
            if(code[i]!='F')
            {
                aabb_res=subAABB(aabb_res,code[i]-'0');
            }
            else
                break;
        }
        return aabb_res;
    }

    AABB<scalar_t> subAABB(AABB<scalar_t> aabb, unsigned int code)
    {
        AABB<scalar_t> aabb1;
        int x_c=code%2;
        code/=2;
        int y_c=code%2;
        code/=2;
        int z_c=code%2;
        if(1==x_c)
        {
            aabb1.x_min=(aabb.x_min+aabb.x_max)/2;
            aabb1.x_max=aabb.x_max;
        }
        else
        {
            aabb1.x_min=aabb.x_min;
            aabb1.x_max=(aabb.x_min+aabb.x_max)/2;
        }
        if(1==y_c)
        {
            aabb1.y_min=(aabb.y_min+aabb.y_max)/2;
            aabb1.y_max=aabb.y_max;
        }
        else
        {
            aabb1.y_min=aabb.y_min;
            aabb1.y_max=(aabb.y_min+aabb.y_max)/2;
        }
        if(1==z_c)
        {
            aabb1.z_min=(aabb.z_min+aabb.z_max)/2;
            aabb1.z_max=aabb.z_max;
        }
        else
        {
            aabb1.z_min=aabb.z_min;
            aabb1.z_max=(aabb.z_min+aabb.z_max)/2;
        }
        return aabb1;
    }
    bool is_in_aabb(vector3<scalar_t> coordinate,AABB<scalar_t> aabb)
    {
        bool flag=true;
        if(coordinate.x<aabb.x_min||coordinate.x>aabb.x_max)
        {
            flag= false;
        }
        if(coordinate.y<aabb.y_min||coordinate.y>aabb.y_max)
        {
            flag= false;
        }
        if(coordinate.z<aabb.z_min||coordinate.z>aabb.z_max)
        {
            flag= false;
        }
        return flag;
    }
};
#endif //DISPLAYIMAGE_OCTREE_H
