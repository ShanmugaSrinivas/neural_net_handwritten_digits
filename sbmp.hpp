#ifndef SBMP_HPP
#define SBMP_HPP
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <assert.h>
#include <vector>
#include <fstream>
using namespace std;

struct pixel_bgr{
	unsigned char b,g,r;
};

struct pixel_rgb{
	unsigned char r,g,b;
};

struct mask{
	char mat[3][3];
};

class bitmap
{
public:
	//file
	unsigned char bfType_a,bfType_b;
	unsigned int bfSize,reserved,bfOffBits;
	//info
	unsigned int biSize,biWidth,biHeight;
	unsigned char biPlanes_a,biPlanes_b,biBitCount_a,biBitCount_b;
	unsigned int biCompression,biSizeImage,biXPelsPerMeter,biYPelsPerMeter,biClrUsed,biClrImportant;
	vector <pixel_bgr> pixel;
	
	void display();
	bitmap()
	{
		init();
	}
	~bitmap()
	{
	}
	bitmap(const bitmap &b)
	{
		make(b.biWidth,b.biHeight,b.pixel);
	}
	void resize_data(size_t size)
	{
		pixel_bgr p={0,0,0};
		pixel=vector <pixel_bgr>(size,p);
		pixel.shrink_to_fit();
	}
	void init()
	{
		//file
		bfType_a=66;bfType_b=77;
		reserved=0;bfOffBits=54;
		//info
		biSize=40;
		biPlanes_a=1;biPlanes_b=0;biBitCount_b=0;
		biCompression=0;biSizeImage=0;biXPelsPerMeter=0;biYPelsPerMeter=0;biClrUsed=0;biClrImportant=0;

		//need to set
		//bfSize=bfSize;
			bfSize=54+(biWidth*3+4-(biWidth*3)%4)*biHeight;
		//biBitCount_a=biBitCount_a;//maybe b one too if necessary (size too big)
			biBitCount_a=24;
	}
	void blank(unsigned int biWidth_arg,unsigned int biHeight_arg)
	{
		init();
		biWidth=biWidth_arg;
		biHeight=biHeight_arg;
		resize_data(biWidth*biHeight);
	}
	void blank(unsigned int biWidth_arg,unsigned int biHeight_arg,pixel_rgb p)
	{
		init();
		biWidth=biWidth_arg;
		biHeight=biHeight_arg;
		resize_data(biWidth*biHeight);
		for(int i=0;i<biWidth*biHeight;i++)
		{
			pixel[i].r=p.r;
			pixel[i].g=p.g;
			pixel[i].b=p.b;
		}
	}
	void blank(unsigned int biWidth_arg,unsigned int biHeight_arg,pixel_bgr p)
	{
		init();
		biWidth=biWidth_arg;
		biHeight=biHeight_arg;
		resize_data(biWidth*biHeight);
		for(int i=0;i<biWidth*biHeight;i++)
		{
			pixel[i]=p;
		}
	}
	void make(/*unsigned int bfSize,*/unsigned int biWidth_arg,unsigned int biHeight_arg,/*unsigned int biBitCount_a,*/pixel_bgr *pixel_arg)
	{
		init();
		biWidth=biWidth_arg;
		biHeight=biHeight_arg;
		resize_data(biWidth*biHeight);
		for(int i=0;i<biWidth*biHeight;i++)
			pixel[i]=pixel_arg[i];
	}
	void make(/*unsigned int bfSize,*/unsigned int biWidth_arg,unsigned int biHeight_arg,/*unsigned int biBitCount_a,*/vector <pixel_bgr> pixel_arg)
	{
		init();
		biWidth=biWidth_arg;
		biHeight=biHeight_arg;
		resize_data(biWidth*biHeight);
		pixel=pixel_arg;
	}
	void make(/*unsigned int bfSize,*/unsigned int biWidth_arg,unsigned int biHeight_arg,/*unsigned int biBitCount_a,*/pixel_rgb *pixel_arg)
	{
		init();
		biWidth=biWidth_arg;
		biHeight=biHeight_arg;
		resize_data(biWidth*biHeight);
		for(int i=0;i<biWidth*biHeight;i++)
		{
			pixel_rgb ptemp=pixel_arg[i];
			pixel[i].r=ptemp.r;
			pixel[i].g=ptemp.g;
			pixel[i].b=ptemp.b;
		}
	}
	void make(/*unsigned int bfSize,*/unsigned int biWidth_arg,unsigned int biHeight_arg,/*unsigned int biBitCount_a,*/vector <pixel_rgb> pixel_arg)
	{
		init();
		biWidth=biWidth_arg;
		biHeight=biHeight_arg;
		resize_data(biWidth*biHeight);
		for(int i=0;i<biWidth*biHeight;i++)
		{
			pixel_rgb ptemp=pixel_arg[i];
			pixel[i].r=ptemp.r;
			pixel[i].g=ptemp.g;
			pixel[i].b=ptemp.b;
		}
	}
	void write(FILE *fp)
	{
		fwrite(&bfType_a,1, 1,fp);
		fwrite(&bfType_b,1, 1,fp);
		fwrite(&bfSize,1, 4,fp);
		fwrite(&reserved,1, 4,fp);
		fwrite(&bfOffBits,1, 4,fp);
		fwrite(&biSize,1, 4,fp);
		fwrite(&biWidth,1, 4,fp);
		fwrite(&biHeight,1, 4,fp);
		fwrite(&biPlanes_a,1, 1,fp);
		fwrite(&biPlanes_b,1, 1,fp);
		fwrite(&biBitCount_a,1, 1,fp);
		fwrite(&biBitCount_b,1, 1,fp);
		fwrite(&biCompression,1, 4,fp);
		fwrite(&biSizeImage,1, 4,fp);
		fwrite(&biXPelsPerMeter,1, 4,fp);
		fwrite(&biYPelsPerMeter,1, 4,fp);
		fwrite(&biClrUsed,1, 4,fp);
		fwrite(&biClrImportant,4, 1,fp);

		unsigned char blank[]={0,0,0};
		int i;
		for(i=biHeight-1;i>=0;i--)
		{
			fwrite((char *)pixel.data()+i*biWidth*3,1,biWidth*3,fp);
			if((biWidth*3)%4>0)
				fwrite(blank,1,4-(biWidth*3)%4,fp);
		}
	}
	void write(string file)
	{
		FILE *fp;
		fp=fopen(file.c_str(),"wb");
		assert(fp!=NULL);
		write(fp);
		fclose(fp);
	}
	void read(FILE *fp)
	{
		fread(&bfType_a,1, 1,fp);
		fread(&bfType_b,1, 1,fp);
		fread(&bfSize,1, 4,fp);
		fread(&reserved,1, 4,fp);
		fread(&bfOffBits,1, 4,fp);
		fread(&biSize,1, 4,fp);
		fread(&biWidth,1, 4,fp);
		fread(&biHeight,1, 4,fp);
		fread(&biPlanes_a,1, 1,fp);
		fread(&biPlanes_b,1, 1,fp);
		fread(&biBitCount_a,1, 1,fp);
		fread(&biBitCount_b,1, 1,fp);
		fread(&biCompression,1, 4,fp);
		fread(&biSizeImage,1, 4,fp);
		fread(&biXPelsPerMeter,1, 4,fp);
		fread(&biYPelsPerMeter,1, 4,fp);
		fread(&biClrUsed,1, 4,fp);
		fread(&biClrImportant,4, 1,fp);

		resize_data(biWidth*biHeight);

		unsigned char blank[]={0,0,0};
		int i;
		for(i=biHeight-1;i>=0;i--)
		{
			fread((char *)pixel.data()+i*biWidth*3,1,biWidth*3,fp);
			if((biWidth*3)%4>0)
				fread(blank,1,4-(biWidth*3)%4,fp);
		}
	}
	void read(string file)
	{
		FILE *fp;
		fp=fopen(file.c_str(),"rb");
		assert(fp!=NULL);
		read(fp);
		fclose(fp);
	}
	void black_and_white()
	{
		unsigned char temp[3];
		unsigned int itemp=0;
		int i,j;
		for(i=0;i<biHeight;i++)
		{
			for(j=0;j<biWidth;j++)
			{
				temp[0]=pixel[i*biWidth+j].r;
				temp[1]=pixel[i*biWidth+j].g;
				temp[2]=pixel[i*biWidth+j].b;
				itemp=(temp[0]+temp[1]+temp[2])/3;
				pixel[i*biWidth+j].r=itemp;
				pixel[i*biWidth+j].g=itemp;
				pixel[i*biWidth+j].b=itemp;
			}
		}
	}
	bitmap masked(mask m)
	{
		bitmap bi;
		bi.blank(biWidth,biHeight);
		
		unsigned int r,g,b;
		
		for(int i=0;i<biHeight;i++)
		{
			for(int j=0;j<biWidth;j++)
			{
				int count=0;
				r=g=b=0;
				for(int k=0;k<3;k++)
				{
					if((i+(k-1))>=0)
					for(int l=0;l<3;l++)
					{
						if((j+(l-1))>=0)
						{
							r+=pixel[(i+(k-1))*biWidth+(j+(l-1))].r*m.mat[k][l];
							g+=pixel[(i+(k-1))*biWidth+(j+(l-1))].g*m.mat[k][l];
							b+=pixel[(i+(k-1))*biWidth+(j+(l-1))].b*m.mat[k][l];
							count++;
						}
					}
				}
				bi.pixel[i*biWidth+j].r=r/count;
				bi.pixel[i*biWidth+j].g=g/count;
				bi.pixel[i*biWidth+j].b=b/count;
			}
		}
		return bi;
	}
	void mask(mask m)
	{
		bitmap b=masked(m);
		pixel=b.pixel;
	}
};
class sbmp
{
public:
	unsigned int length,width;
	vector <pixel_rgb> pixel;
	void display();
	sbmp()
	{
		length=width=0;
	}
	~sbmp()
	{
	}
	sbmp(const sbmp &copy)
	{
		length=copy.length;
		width=copy.width;
		pixel=copy.pixel;
	}
	inline void resize_data(size_t size)
	{
		pixel_rgb p={0,0,0};
		pixel=vector <pixel_rgb>(size,p);
		pixel.shrink_to_fit();
	}
	void write(fstream &f)
	{
		f.write((char *)&length,sizeof(length));
		f.write((char *)&width,sizeof(width));
		for(int i=0;i<pixel.size();i++)
			f.write((char *)&pixel[i],sizeof(pixel[i]));
	}
	void read(fstream &f)
	{
		f.read((char *)&length,sizeof(length));
		f.read((char *)&width,sizeof(width));
		pixel=vector <pixel_rgb>(length*width,{0,0,0});
		for(int i=0;i<pixel.size();i++)
			f.read((char *)&pixel[i],sizeof(pixel[i]));
	}
	void blank(unsigned int length_arg,unsigned int width_arg)
	{
		length=length_arg;
		width=width_arg;
		resize_data(length*width);
	}
	void load(bitmap &b)
	{
		length=b.biWidth;
		width=b.biHeight;
		resize_data(length*width);
		pixel_rgb temp_rgb;
		pixel_bgr temp_bgr;
		for(int i=0;i<b.biHeight;i++)
		{
			for(int j=0;j<b.biWidth;j++)
			{
				temp_bgr=b.pixel[i*b.biWidth+j];
				temp_rgb.r=temp_bgr.r;
				temp_rgb.g=temp_bgr.g;
				temp_rgb.b=temp_bgr.b;
				pixel[i*b.biWidth+j]=temp_rgb;
			}
		}
	}
	bitmap to_bmp()
	{
		bitmap b;
		b.blank(length,width);
		pixel_rgb temp_rgb;
		pixel_bgr temp_bgr;
		for(int i=0;i<b.biHeight;i++)
		{
			for(int j=0;j<b.biWidth;j++)
			{
				temp_rgb=pixel[i*b.biWidth+j];
				temp_bgr.r=temp_rgb.r;
				temp_bgr.g=temp_rgb.g;
				temp_bgr.b=temp_rgb.b;
				b.pixel[i*b.biWidth+j]=temp_bgr;
			}
		}
		return b;
	}
};
class sbmpf
{
public:
	unsigned int length,width;
	vector <float> pixel;
	bool normalized;
	void display();
	sbmpf()
	{
		length=width=0;
		normalized=false;
	}
	~sbmpf()
	{
	}
	inline void resize_data(size_t size)
	{
		pixel=vector <float>(size,0.0);
		pixel.shrink_to_fit();
	}
	void blank(unsigned int length_arg,unsigned int width_arg)
	{
		length=length_arg;
		width=width_arg;
		resize_data(length*width);
	}
	void normalize()
	{
		if(normalized==false)
		{
			for(int i=0;i<width;i++)
			{
				for(int j=0;j<length;j++)
				{
					pixel[i*length+j]/=255.0;
				}
			}
			normalized=true;
		}
	}
	void unnormalize()
	{
		if(normalized==true)
		{
			for(int i=0;i<width;i++)
			{
				for(int j=0;j<length;j++)
				{
					pixel[i*length+j]*=255.0;
				}
			}
			normalized=false;
		}
	}
	void load(sbmp &sb)
	{
		length=sb.length;
		width=sb.width;
		normalized=false;
		for(int i=0;i<width;i++)
		{
			for(int j=0;j<length;j++)
			{
				pixel_rgb temp_rgb=sb.pixel[i*length+j];
				pixel[i*length+j]=(temp_rgb.r+temp_rgb.g+temp_rgb.b)/3.0;
			}
		}
	}
	void load(bitmap &b)
	{
		length=b.biWidth;
		width=b.biHeight;
		normalized=false;
		resize_data(length*width);
		pixel_bgr temp_bgr;
		for(int i=0;i<b.biHeight;i++)
		{
			for(int j=0;j<b.biWidth;j++)
			{
				temp_bgr=b.pixel[i*b.biWidth+j];
				pixel[i*b.biWidth+j]=(temp_bgr.r+temp_bgr.g+temp_bgr.b)/3.0;
			}
		}
	}
	bitmap to_bmp()
	{
		bitmap b;
		b.blank(length,width);
		for(int i=0;i<b.biHeight;i++)
		{
			for(int j=0;j<b.biWidth;j++)
			{
				if(normalized==false)
				{
					pixel_bgr temp_bgr;
					temp_bgr.r=temp_bgr.g=temp_bgr.b=pixel[i*b.biWidth+j];
					b.pixel[i*b.biWidth+j]=temp_bgr;
				}
				else
				{
					pixel_bgr temp_bgr;
					temp_bgr.r=temp_bgr.g=temp_bgr.b=pixel[i*b.biWidth+j]*255.0;
					b.pixel[i*b.biWidth+j]=temp_bgr;
				}
			}
		}
		return b;
	}
};
#endif