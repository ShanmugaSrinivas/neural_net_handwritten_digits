#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <iostream>
#include <fstream>
#include <assert.h>

using namespace std;
template <class T>
class matrix
{
public:
    int m,n;
    vector <T> data;
    matrix()
    {
        m=n=0;
    }
    matrix(int ma,int na,T el=0)
    {
        resize(ma,na,el);
    }
    inline T &e(int i,int j)
    {
        return data[i*n+j];
    }
    void resize(int ma,int na,T el=0)
    {
        m=ma;
        n=na;
        data.resize(ma*na,el);
    }
    void read(fstream &f)
    {
        f.read((char *)&m,sizeof(m));
        f.read((char *)&n,sizeof(n));
        size_t size;
        f.read((char *)&size,sizeof(size));
        data.resize(size);
        f.read((char *)data.data(),sizeof(data[0])*size);
    }
    void read(string fname)
    {
        fstream f(fname,ios::in | ios::binary);
        read(f);
    }
    void write(fstream &f)
    {
        f.write((char *)&m,sizeof(m));
        f.write((char *)&n,sizeof(n));
        size_t size=data.size();
        f.write((char *)&size,sizeof(size));
        f.write((char *)data.data(),sizeof(data[0])*size);
    }
    void write(string fname)
    {
        fstream f(fname,ios::out | ios::binary);
        write(f);
    }
    void print_metadata(string name="")
    {
        cout << "\nmatrix \"" << name << "\" " << m << " x " << n;
    }
    void print()
    {
        print_metadata();
        cout << "\n[";
            cout << "[" << e(0,0);
            for(int j=1;j<n;j++)
                cout << "," << e(0,j);
            cout << "]";
        for(int i=1;i<m;i++)
        {
            cout << "\n [" << e(i,0);
            for(int j=1;j<n;j++)
                cout << "," << e(i,j);
            cout << "]";
        }
        cout << "]";
    }
    void apply(T (*func)(T arg))
    {
        for(int i=0;i<data.size();i++)
            data[i]=func(data[i]);
    }
    void equal(T c)
    {
        for(int i=0;i<data.size();i++)
            data[i]=c;
    }
    void multiply(T c)
    {
        for(int i=0;i<data.size();i++)
            data[i]*=c;
    }
    void add(T c)
    {
        for(int i=0;i<data.size();i++)
            data[i]+=c;
    }
    void sub(T c)
    {
        c*=-1;
        add(c);
    }
    void add(matrix &mat)
    {
        assert(m==mat.m && n==mat.n);
        for(int i=0;i<data.size();i++)
            data[i]+=mat.data[i];
    }
    void add(matrix &mat1,matrix &mat2)
    {
        assert(m==mat1.m && n==mat1.n);
        assert(mat2.m==mat1.m && mat2.n==mat1.n);
        for(int i=0;i<data.size();i++)
        data[i]=mat1.data[i]+mat2.data[i];
    }
    void sub(matrix &mat)
    {
        assert(m==mat.m && n==mat.n);
        for(int i=0;i<data.size();i++)
            data[i]-=mat.data[i];
    }
    void sub(matrix &mat1,matrix &mat2)
    {
        assert(m==mat1.m && n==mat1.n);
        assert(mat2.m==mat1.m && mat2.n==mat1.n);
        for(int i=0;i<data.size();i++)
            data[i]=mat1.data[i]-mat2.data[i];
    }
    void multiply(matrix &m1,matrix &m2)
    {
        assert(m1.m==m);
        assert(m1.n==m2.m);
        assert(m2.n==n);
        for(int i=0;i<m;i++)
            for(int j=0;j<n;j++)
            {
                T temp=0;
                for(int k=0;k<m1.n;k++)
                    temp+=m1.e(i,k)*m2.e(k,j);
                e(i,j)=temp;
            }
    }
};
#endif
