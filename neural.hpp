#ifndef NEURAL_HPP
#define NEURAL_HPP

#include <vector>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <math.h>
#include "matrix.hpp"
#include "dataset.hpp"

using namespace std;

template <class T>
class layer
{
public:
    int n;
    matrix <T> w;
    matrix <T> b;
    matrix <T> y;
    matrix <T> change_w;
    matrix <T> change_b;
    matrix <T> dE_by_dy;
    T error;
    layer *prev,*next;
    layer()
    {
        n=0;
        prev=next=nullptr;
        error=0;
    }
    void init(int na,layer *preva,layer *nexta)
    {
        n=na;
        prev=preva;
        next=nexta;
        if(prev==nullptr)//first
        {
            //w
            //b
            y.resize(n,1);
            //change_w
            //change_b
            //dE_by_dy
        }
        else
        {
            w.resize(n,prev->n);
            for(int i=0;i<w.data.size();i++)
                w.data[i]=(rand()%1000)/1000.0;
            for(int i=0;i<w.m;i++)
            {
                T temp=0;
                for(int j=0;j<w.n;j++)
                    temp+=w.e(i,j);
                for(int j=0;j<w.n;j++)
                    w.e(i,j)/=temp;
            }
            b.resize(n,1);
            for(int i=0;i<b.data.size();i++)
                b.data[i]=(rand()%1000)/1000.0;
            y.resize(n,1);
            change_w.resize(n,prev->n);
            change_b.resize(n,1);
            dE_by_dy.resize(n,1);
        }
    }
    void write(string fname)
    {
        fstream f(fname,ios::binary | ios::binary);
        write(f);
    }
    void write(fstream &f)
    {
        f.write((char *)&n,sizeof(n));
        w.write(f);
        b.write(f);
        y.write(f);
    }
    void read(string fname)
    {
        fstream f(fname,ios::binary | ios::binary);
        read(f);
    }
    void read(fstream &f)
    {
        f.read((char *)&n,sizeof(n));
        w.read(f);
        b.read(f);
        y.read(f);
    }
    static T activation_func(T arg)
    {
        return 1.0/(1.0+exp(-arg));
    }
    void in(matrix <T> &in)
    {
        assert(in.m==y.m && in.n==y.n);
        y=in;
    }
    void evaluate()
    {
        y.multiply(w,prev->y);
        y.sub(b);
        y.apply(&activation_func);
    }
    void calc_dE_by_dy_output(matrix <T> &t)
    {
        dE_by_dy.sub(y,t);
    }
    void calc_dE_by_dy()
    {
        for(int j=0;j<dE_by_dy.m;j++)
        {
            T temp=0;
            for(int k=0;k<next->y.m;k++)
            {
                T y_next=next->y.e(k,0);
                temp+=next->dE_by_dy.e(k,0)*y_next*(1-y_next)*next->w.e(k,j);
            }
            dE_by_dy.e(j,0)=temp;
        }
    }
    void calc_dE_by_dw()
    {
        for(int i=0;i<w.m;i++)
            for(int j=0;j<w.n;j++)
            {
                T y_temp=y.e(i,0);
                change_w.e(i,j)+=dE_by_dy.e(i,0)*y_temp*(1-y_temp)*prev->y.e(j,0);
            }
    }
    void calc_dE_by_db()
    {
        for(int i=0;i<w.m;i++)
        {
            T y_temp=y.e(i,0);
            change_b.e(i,0)+=dE_by_dy.e(i,0)*y_temp*(1-y_temp)*(-1);
        }
    }
    void calc_error(matrix <T> &t)
    {
        T temp=0;
        for(int i=0;i<n;i++)
        {
            T temp1=y.e(i,0)-t.e(i,0);
            temp+=temp1*temp1;
            //cout << "\ntemp1=" << temp1;
        }
        error+=temp;
    }
    void set_error_zero()
    {
        error=0;
    }
    void get_final_error(int batch_size)
    {
        error/=batch_size;
    }
    void set_change_zero()
    {
        change_w.equal(0);
        change_b.equal(0);
    }
    void change(T heta, int batch_size)
    {
        change_w.multiply(heta/batch_size);
        change_b.multiply(heta/batch_size);
        w.sub(change_w);
        b.sub(change_b);
    }
};

template <class T>
class neural
{
public:
    int no_layer;
    vector <int> layer_n;
    vector <layer<T>> l;
    neural(vector <int> &layer_na)
    {
        layer_n=layer_na;
        no_layer=layer_n.size();
        l.resize(no_layer,layer<T>());
        l[0].init(layer_n[0],nullptr,&l[1]);
        for(int i=0;i<no_layer-1;i++)
            l[i].init(layer_n[i],&l[i-1],&l[i+1]);
        l[no_layer-1].init(layer_n[no_layer-1],&l[no_layer-1-1],nullptr);
    }
    void write(string fname)
    {
        fstream f(fname,ios::binary | ios::out);
        write(f);
    }
    void write(fstream &f)
    {
        f.write((char *)&no_layer,sizeof(no_layer));
        f.write((char *)layer_n.data(),sizeof(layer_n[0])*no_layer);
        for(int i=0;i<no_layer;i++)
            l[i].write(f);
    }
    void read(string fname)
    {
        fstream f(fname,ios::binary | ios::out);
        read(f);
    }
    void read(fstream &f)
    {
        f.read((char *)&no_layer,sizeof(no_layer));
        layer_n.resize(no_layer);
        f.read((char *)layer_n.data(),sizeof(layer_n[0])*no_layer);
        l.resize(no_layer,layer<T>());
        l[0].init(layer_n[0],nullptr,&l[1]);
        for(int i=0;i<no_layer-1;i++)
            l[i].init(layer_n[i],&l[i-1],&l[i+1]);
        l[no_layer-1].init(layer_n[no_layer-1],&l[no_layer-1-1],nullptr);
        for(int i=0;i<no_layer;i++)
            l[i].read(f);
    }
    void evaluate(matrix <T> &in)
    {
        l[0].in(in);
        for(int i=1;i<no_layer;i++)
            l[i].evaluate();
    }
    void back_propagate(matrix <T> &t)
    {
        l[no_layer-1].calc_dE_by_dy_output(t);
        l[no_layer-1].calc_dE_by_dw();
        l[no_layer-1].calc_dE_by_db();
        l[no_layer-1].calc_error(t);
        for(int i=no_layer-2;i>0;i--)
        {
            l[i].calc_dE_by_dy();
            l[i].calc_dE_by_dw();
            l[i].calc_dE_by_db();
        }
    }
    void set_change_zero()
    {
        for(int i=0;i<no_layer;i++)
            l[i].set_change_zero();
    }
    void set_error_zero()
    {
        l[no_layer-1].set_error_zero();
    }
    void change(T heta, int batch_size)
    {
        for(int i=1;i<no_layer;i++)
            l[i].change(heta,batch_size);
    }
    void train(vector <neural_data<T>> &batch,T heta,int batch_size)
    {
        set_change_zero();
        set_error_zero();
        for(int i=0;i<batch_size;i++)
        {
            evaluate(batch[i].in);
            back_propagate(batch[i].t);
        }
        change(heta,batch_size);
        //l[no_layer-1].get_final_error(batch_size);
        //error_list.push_back(l[no_layer-1].error);
    }
    void train(neural_dataset<T> &ds,neural_dataset<T> &ds_dev,T heta,int batch_size,vector <T> &error_list)
    {
        vector <neural_data<T>> batch(batch_size,neural_data<T>(0,ds.ino,ds.tno));
        cout << "\ncount_net" << ds.count_net/batch_size;
        for(int i=0;i<ds.count_net/batch_size;i++)
        {
            for(int j=0;j<batch_size;j++)
                batch[j]=ds.data[i*batch_size+j];
            train(batch,heta,batch_size);
            T dev_error=get_error(ds_dev,100);
            cout << "\nno_of_batch=" << i << "dev_set error=" << dev_error;
            error_list.push_back(dev_error);
            //cout << "\nno of _batch=" << i << " error=" << l[no_layer-1].error;
            //l[no_layer-1].y.print();
            //batch[batch_size-1].t.print();
        }
        //return;
        if(ds.count_net%batch_size>0)
        {
            cout << "\nextra batch of size: " << ds.count_net%batch_size;
            int i=ds.count_net/batch_size;
            for(int j=0;j<ds.count_net%batch_size;j++)
                batch[j]=ds.data[i*batch_size+j];
            T dev_error=get_error(ds_dev,100);
            cout << "\ndev_set error=" << dev_error;
            train(batch,heta,ds.count_net%batch_size);
            error_list.push_back(dev_error);
        }
        set_change_zero();
        set_error_zero();
    }
    T get_error(vector <neural_data<T>> &batch,int batch_size)
    {
        set_change_zero();
        set_error_zero();
        for(int i=0;i<batch_size;i++)
        {
            evaluate(batch[i].in);
            l[no_layer-1].calc_error(batch[i].t);
        }
        l[no_layer-1].get_final_error(batch_size);
        return l[no_layer-1].error;
    }
    T get_error(neural_dataset<T> &ds,int batch_size)
    {
        T error_final=0;
        vector <neural_data<T>> batch(batch_size,neural_data<T>(0,ds.ino,ds.tno));
        cout << "\ncount_net" << ds.count_net/batch_size;
        for(int i=0;i<ds.count_net/batch_size;i++)
        {
            for(int j=0;j<batch_size;j++)
                batch[j]=ds.data[i*batch_size+j];
            error_final+=get_error(batch,batch_size);
            //cout << "\nno of _batch=" << i << " error_final=" << error_final/(i+1);
            //l[no_layer-1].y.print();
            //batch[batch_size-1].t.print();
        }
        int i=ds.count_net/batch_size;
        if(i>0)
        {
            for(int j=0;j<ds.count_net%batch_size;j++)
                batch[j]=ds.data[i*batch_size+j];
            error_final+=get_error(batch,batch_size);
            //cout << "\nno of _batch=" << i << " error_final=" << error_final/(ds.count_net/batch_size+1);
            error_final/=ds.count_net/batch_size+(ds.count_net%batch_size)/batch_size;
        }
        return error_final;
    }
};
#endif
