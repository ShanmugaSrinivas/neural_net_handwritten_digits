#include "dataset.hpp"
#include <sstream>
#include <stdlib.h>
#include <time.h>

void make_datafile(string folder,string initial)
{
    stringstream ss;
    for(int i=0;i<10;i++)
    {
        neural_dataset <float> ds(28*28,10);
        ss.clear();
        ss.str("");
        ss << folder << i;
        ds.read_directory_of_same_type(ss.str(),i);
        ss.clear();
        ss.str("");
        ss << initial << i;
        ds.write(ss.str());
        cout << endl;
    }
}
void merge_randomly(string initial)
{
    stringstream ss;
    vector <neural_dataset <float>> ds(10,neural_dataset<float>(28*28,10));
    for(int i=0;i<10;i++)
    {
        ss.clear();
        ss.str("");
        ss << initial << i;
        ds[i].read(ss.str());
        cout << "\nread i=" << i << "\tcount=" << ds[i].count_net;
    }
    neural_dataset <float> ds_all(28*28,10);
    srand(time(NULL));
    while(1)
    {
        int r=rand()%10;
        if(ds_all.count[r]<ds[r].count_net)
        {
            ds_all.add_data(r,ds_all.count_net);
            ds_all.data[ds_all.data.size()-1]=ds[r].data[ds_all.count[r]-1];
            ds_all.data[ds_all.data.size()-1].id=ds_all.count_net-1;
        }
        else
        {
            //cout << "\nelse " << ds_all.count_net;
            bool found=false;
            for(int i=0;i<10;i++)
            {
                //cout << "\nall=" << ds_all.count[i] << " " << ds[i].count_net;
                if(ds_all.count[i]<ds[i].count_net)
                    found=true;
            }
            if(found==false)
                break;
        }
    }
    int temp=0;
    for(int i=0;i<10;i++)
        temp+=ds[i].count_net;
    cout << endl << temp << endl << ds_all.count_net << endl;
    ds_all.write(initial);
}
void randomize_dataset(string dsname)
{
    srand(time(NULL));
    neural_dataset <float> ds_all(dsname);
    for(int j=0;j<100;j++)
    {
        for(int i=0;i<ds_all.data.size();i++)
        {
            int last=ds_all.data.size()/4;
            int r1=rand()%ds_all.data.size();
            int r2=rand()%last;
            r2=ds_all.data.size()-last+r2;
            neural_data<float> temp=ds_all.data[r1];
            ds_all.data[r1]=ds_all.data[r2];
            ds_all.data[r2]=temp;
        }
        cout << "\n" << j;
    }
    ds_all.write(dsname+"_rand");
}
void debug_s(string initial)
{
    neural_dataset <float> ds(initial);
    //ds.read("test");
    for(int i=0;i<ds.count_net;i++)
        if(ds.data[i].in.m!=28*28)
            cout << endl << ds.data[i].in.m;
}
void xor_dataset()
{
    neural_dataset<float> ds(2,2);
    for(int i=0;i<10000;i++)
    {
        int r=rand()%2;
        ds.add_data(r,i);
        if(r==0)
        {
            int r1=rand()%2;
            if(r1==0)
            {
                ds.data[ds.data.size()-1].in.e(0,0)=0;
                ds.data[ds.data.size()-1].in.e(1,0)=0;
            }
            else
            {
                ds.data[ds.data.size()-1].in.e(0,0)=1;
                ds.data[ds.data.size()-1].in.e(1,0)=1;
            }
            ds.data[ds.data.size()-1].t.e(0,0)=1;
            ds.data[ds.data.size()-1].t.e(1,0)=0;
        }
        else
        {
            int r1=rand()%2;
            if(r1==0)
            {
                ds.data[ds.data.size()-1].in.e(0,0)=0;
                ds.data[ds.data.size()-1].in.e(1,0)=1;
            }
            else
            {
                ds.data[ds.data.size()-1].in.e(0,0)=1;
                ds.data[ds.data.size()-1].in.e(1,0)=0;
            }
            ds.data[ds.data.size()-1].t.e(0,0)=0;
            ds.data[ds.data.size()-1].t.e(1,0)=1;
        }
    }
    ds.write("xor");
}
void merge_train_and_test(string ftrain,string ftest)
{
    neural_dataset <float> ds_train(ftrain);
    neural_dataset <float> ds_test(ftest);
    for(int i=0;i<ds_test.count_net;i++)
    {
        ds_train.data.push_back(ds_test.data[i]);
        ds_train.data[ds_train.data.size()-1].id=ds_train.data.size()-1;
    }
    for(int i=0;i<ds_train.count.size();i++)
        ds_train.count[i]+=ds_test.count[i];
    ds_train.count_net+=ds_test.count_net;
    ds_train.write(ftrain+ftest);
}
void split_train_test_dev(string fmain,string ftrain,string ftest,string fdev,float prop_train,float prop_test,float prop_dev)
{
    float sum=prop_train+prop_test+prop_dev;
    prop_train/=sum;
    prop_test/=sum;
    prop_dev/=sum;

    neural_dataset <float> ds_main(fmain);
    neural_dataset <float> ds_train(28*28,10);
    neural_dataset <float> ds_test(28*28,10);
    neural_dataset <float> ds_dev(28*28,10);
    int total=ds_main.count_net;
    int train=0,
        test=prop_train*total,
        dev=test+prop_test*total;
    for(int i=0;i<test;i++)
    {
        ds_train.data.push_back(ds_main.data[i]);
        ds_train.data[ds_train.data.size()-1].id=ds_train.data.size()-1;
        for(int j=0;i<ds_train.count.size();i++)
            ds_train.count[j]+=ds_main.data[i].t.e(j,0);
        ds_train.count_net++;
    }
    for(int i=test;i<dev;i++)
    {
        ds_test.data.push_back(ds_main.data[i]);
        ds_test.data[ds_test.data.size()-1].id=ds_test.data.size()-1;
        for(int j=0;i<ds_test.count.size();i++)
            ds_test.count[j]+=ds_main.data[i].t.e(j,0);
        ds_test.count_net++;
    }
    for(int i=dev;i<total;i++)
    {
        ds_dev.data.push_back(ds_main.data[i]);
        ds_dev.data[ds_dev.data.size()-1].id=ds_dev.data.size()-1;
        for(int j=0;i<ds_dev.count.size();i++)
            ds_dev.count[j]+=ds_main.data[i].t.e(j,0);
        ds_dev.count_net++;
    }
    ds_train.write(ftrain);
    ds_test.write(ftest);
    ds_dev.write(fdev);
}
int main()
{
    split_train_test_dev("traintest_rand_rand","0train","0test","0dev",85,10,5);
    //randomize_dataset("traintest_rand");
    //merge_train_and_test("train","test");
    //xor_dataset();
    /*make_datafile("/home/shan/s/Projects/mnist/mnist_png/testing/","test");
    cout << "\rdone make_datafile" << endl;
    merge_randomly("test");
    debug_s("test");
    make_datafile("/home/shan/s/Projects/mnist/mnist_png/training/","train");
    cout << "\rdone make_datafile" << endl;
    merge_randomly("train");
    debug_s("train");*/
    /*make_datafile("/home/shan/s/Projects/mnist/mnist_png/training/","train");
    cout << endl;
    neural_dataset <float> ds(28*28,10);
    ds.read("test0");
    for(int i=0;i<784;i++)
        cout << ds.data[100].in.e(i,0) << endl;*/
    return 0;
}
