#include <vector>
#include <iostream>
#include <fstream>
#include <assert.h>
#include "matrix.hpp"
#include "sbmp.hpp"
#include "../02file_system_explorer/sfs_linux_nu.hpp"
//#include "dataset.hpp"

using namespace std;

template <class T>
class neural_data
{
public:
    int id;
    matrix <T> in;
    matrix <T> t;
    neural_data(int ida,int inno,int tno)
    {
        id=ida;
        in.resize(inno,1);
        t.resize(tno,1);
    }
    void read(fstream &f)
    {
        f.read((char *)&id,sizeof(id));
        in.read(f);
        t.read(f);
    }
    void read(string fname)
    {
        fstream f(fname,ios::binary | ios::in);
        read(f);
    }
    void write(fstream &f)
    {
        f.write((char *)&id,sizeof(id));
        in.write(f);
        t.write(f);
    }
    void write(string fname)
    {
        fstream f(fname,ios::binary | ios::out);
        write(f);
    }
};

template <class T>
class neural_dataset
{
public:
    vector <neural_data<T>> data;
    vector <int> count;
    int count_net;
    int ino,tno;
    neural_dataset(int inoa,int tnoa)
    {
        ino=inoa;
        tno=tnoa;
        count.resize(tno,0);
        count_net=0;
    }
    neural_dataset(string fname)
    {
        read(fname);
    }
    neural_dataset(fstream &f)
    {
        read(f);
    }
    void write(fstream &f)
    {
        size_t size=data.size();
        f.write((char *)&size,sizeof(size));
        for(int i=0;i<size;i++)
            data[i].write(f);
        size=count.size();
        f.write((char *)&size,sizeof(size));
        f.write((char *)count.data(),sizeof(count[0])*size);
        f.write((char *)&count_net,sizeof(count_net));
        f.write((char *)&ino,sizeof(ino));
        f.write((char *)&tno,sizeof(tno));
    }
    void write(string fname)
    {
        fstream f(fname,ios::out | ios::binary);
        write(f);
    }
    void read(fstream &f)
    {
        size_t size=0;
        f.read((char *)&size,sizeof(size));
        data.resize(size,neural_data<T>(0,0,0));
        for(int i=0;i<size;i++)
            data[i].read(f);
        f.read((char *)&size,sizeof(size));
        count.resize(size);
        f.read((char *)count.data(),sizeof(count[0])*size);
        f.read((char *)&count_net,sizeof(count_net));
        f.read((char *)&ino,sizeof(ino));
        f.read((char *)&tno,sizeof(tno));
    }
    void read(string fname)
    {
        fstream f(fname,ios::in | ios::binary);
        read(f);
    }
    void add_data(unsigned int type,int id)
    {
        data.push_back(neural_data<T>(id,ino,tno));
        count_net++;
        count[type]++;
    }
	void read_bmp_file(string filename,unsigned int no,unsigned int id,unsigned int type)
	{
		bitmap b;
		b.read(filename);
		assert(ino==b.biWidth*b.biHeight);
		data[no].id=id;
		for(int i=0;i<ino;i++)
		{
			data[no].in.e(i,0)=(b.pixel[i].r+b.pixel[i].g+b.pixel[i].b)/3.0/255.0;
		}
		for(int i=0;i<tno;i++)
		{
			data[no].t.e(i,0)=0.0;
		}
		assert(type<tno);
		data[no].t.e(type,0)=1.0;
	}
	void read_directory_of_same_type(string dir,unsigned int type)
	{
		assert(type<tno);
		open_folder of;
		assert(of.open(dir)==0);
		do
		{
			if(of.f.name.length()>=4 && of.f.name.substr(of.f.name.length()-4,4)==".bmp")
			{
				add_data(type,data.size());
				read_bmp_file(of.path+'/'+of.f.name,count_net-1,count_net-1,type);
                assert(data[data.size()-1].in.m==28*28);
                assert(data[data.size()-1].in.n==1);
				//exit(-1);
			}
			cout << "\r" << count_net;
		}while(of.load_next()==0);
	}
};
