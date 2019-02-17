//
// Created by wuzaiqiang on 19-2-16.
//

#ifndef DISPLAYIMAGE_OCTREE_H
#define DISPLAYIMAGE_OCTREE_H

#include "vector3.h"
#include "aabb.h"
#include "triangle.h"
#include "string"
#include <iostream>
using namespace std;
template <typename scalar_t>
class TriangleNode
{
public:
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
public:
    TriangleNode<scalar_t> *head;
    TriangleNode<scalar_t> *end;
    TriangleNode<scalar_t> *iter;
    int size;
    TriangleList()
    {
        head=NULL;
        end=NULL;
        size=0;
        iter=NULL;
    }
    TriangleNode<scalar_t>* iteration()
    {
        TriangleNode<scalar_t>* temp=iter;
        if(iter)
        {
            iter=iter->NextNode;
        }
        return temp;
    }
    void resetIteration()
    {
        iter=head;
    }
    void append(Triangle<scalar_t> *tri)
    {
        if(!head)
        {
            head=new TriangleNode<scalar_t>;
            head->triangle=tri;
            end=head;
            size++;
            iter=head;
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
    void cleardata()
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
    TriangleList<scalar_t> triangle_list;
    bool isleaf;
    int current_depth;
    AABB<scalar_t> aabb;
    OctreeNode<scalar_t> *children[8];
    OctreeNode()
    {
        isleaf=true;
        current_depth=0;
        for(int i=0;i<8;i++)
        {
            children[i]=NULL;
        }
    }
    bool ishit(vector3<scalar_t> e,vector3<scalar_t> d,scalar_t t0,scalar_t t1,IntersectionResult<scalar_t>& rec)
    {
        if(!isleaf||triangle_list.size==0)
        {
            return false;
        }
        else
        {
            triangle_list.resetIteration();
            IntersectionResult<scalar_t> res_tmp;
            scalar_t min_t=10000;
            bool flag=false;
            for(int i=0;i<triangle_list.size;i++)
            {
                if(triangle_list.iteration()->triangle->ishit(e,d,t0,t1,res_tmp))
                {
                    flag=true;
                    if(res_tmp.t<min_t)
                    {
                        rec=res_tmp;
                    }
                }
            }
            return flag;
        }
    }
};
template <typename scalar_t>
class Octree
{
public:
    OctreeNode<scalar_t>* root;
    unsigned int max_depth;
    int max_faces;
    Octree()
    {
        max_depth=8;
        max_faces=32;
        root=NULL;
        //
    }
    ~Octree()
    {
        free_mem(root);
    }
    void free_mem(OctreeNode<scalar_t>* node)
    {
        if(node)
        {
            for(int i=0;i<8;i++)
            {
                free_mem(node->children[i]);
            }
        }
    }
    void generate(Triangle<scalar_t> *tris,int num_tris,AABB<scalar_t> aabb1)
    {
        root=new OctreeNode<scalar_t>;
        root->mortonCode=string(max_depth,'F');
        root->aabb=aabb1;//
        for(int i=0;i<num_tris;i++)
        {
            root->triangle_list.append(&tris[i]);
        }
        //cout<<"root triangle num:"<<root.triangle_list.size<<endl;
        divide(root);
        cout<<"Finish generating octree!"<<endl;

    }
    void divide(OctreeNode<scalar_t> *node)
    {
        if(node->current_depth>=max_depth||node->triangle_list.size<=max_faces)
        {
            cout<<"current depth:"<<node->current_depth<<" num tris:"<<node->triangle_list.size<<" "<<node->mortonCode<<endl;
            return;
        }
        node->isleaf= false;
        OctreeNode<scalar_t> *node_p=new OctreeNode<scalar_t>[8];

        for(int i=0;i<8;i++)
        {
            node->children[i]=&node_p[i];
            node->children[i]->aabb=subAABB(node->aabb,(unsigned int)i);//todo
            node->children[i]->current_depth=node->current_depth+1;
            node->children[i]->mortonCode=node->mortonCode;
            node->children[i]->mortonCode[node->current_depth]='0'+(char)i;

        }
        node->triangle_list.resetIteration();
        Triangle<scalar_t> *test_tri_p=NULL;
        for(int i=0;i<node->triangle_list.size;i++)
        {
            test_tri_p=node->triangle_list.iteration()->triangle;
            for(int j=0;j<8;j++)
            {
                if(is_tri_in_aabb(test_tri_p,node->children[j]->aabb))
                {
                    //cout<<"tri in aabb"<<endl;
                    node->children[j]->triangle_list.append(test_tri_p);
                }

            }

        }
        //delete &(node->triangle_list);//info moved to leaf nodes,free memory todo
        node->triangle_list.cleardata();
        for(int i=0;i<8;i++)
        {
            divide(node->children[i]);
        }

    }
    bool is_tri_in_aabb(Triangle<scalar_t> *tri,AABB<scalar_t> aabb_test)
    {
        bool isin0=is_in_aabb(*tri->p0,aabb_test);
        bool isin1=is_in_aabb(*tri->p1,aabb_test);
        bool isin2=is_in_aabb(*tri->p2,aabb_test);
        bool isin=false;
        if(isin0)
            isin=true;
        if(isin1)
            isin=true;
        if(isin2)
            isin=true;
        return isin;
    }
    string coor2code(vector3<scalar_t> coordinate)
    {
        string code;
        if(!is_in_aabb(coordinate,root->aabb))
            return string(max_depth,'F');
        else
        {
            AABB<scalar_t> aabb_test=root->aabb;
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
        AABB<scalar_t> aabb_res=root->aabb;
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
    OctreeNode<scalar_t>* code2node(string code)
    {
        OctreeNode<scalar_t>* pointer=root;
        for(int i=0;i<code.length();i++)
        {
            if(code[i]!='F')
            {
                pointer=pointer->children[code[i]-'0'];
            }
            else
                break;
        }
        return pointer;
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
    bool ishit(vector3<scalar_t> e,vector3<scalar_t> d,scalar_t t0,scalar_t t1,IntersectionResult<scalar_t>& rec)
    {
        vector3<scalar_t> inpoint,outpoint;
        string code;
        bool flag=false;

        if(root->aabb.ishit(e,d,inpoint,outpoint))
        {
            code=coor2code(inpoint+(scalar_t)eps_t*d);
            while(!flag&&code!=string(max_depth,'F'))
            {
                if(code2node(code)->ishit(e,d,t0,t1,rec))
                {
                    flag=true;
                }
                else
                {
                    code=coor2code(outpoint+(scalar_t)eps_t*d);
                    code2aabb(code).ishit(e,d,inpoint,outpoint);
                }
            }

        }
        if(!flag)
        {
            cout<<"not hit octree"<<endl;
        }
        else
            cout<<"hit octree"<<endl;
        return flag;
    }
};
#endif //DISPLAYIMAGE_OCTREE_H
