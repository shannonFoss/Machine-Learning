#include <iostream>
#include <fstream> //file io
#include <stdlib.h>
#include <math.h>
#include <cstdio>
/*
[0][0] = lower left row = 0 col = 0
[1][0] = row = 1 col = 0 height
[0][2] = row = 0 col = 2 width
*/
#define PI 3.14159265
using namespace std;
const int width=600, height= 900;
const int size= 512;
const int display=512;
unsigned char r[height][width];
unsigned char g[height][width];
unsigned char b[height][width];

unsigned char rgrey[height][width];
unsigned char ggrey[height][width];
unsigned char bgrey[height][width];

unsigned char rsquare[size][size];
unsigned char gsquare[size][size];
unsigned char bsquare[size][size];

unsigned char rspatial[display][display];
unsigned char gspatial[display][display];
unsigned char bspatial[display][display];

unsigned char edgeArray[height][width];
unsigned char edgeArray2[height][width];
unsigned char zeroArray[height][width];
int tempArray[height][width];
int tempArray1[height][width];
int tempArray2[height][width];
double gaussArray[height][width];


void makeGrey(int x,int y){

	int greyVal,tempr, tempg, tempb;
	tempr=(int)r[x][y];
	tempg=(int)g[x][y];
	tempb=(int)b[x][y];
	greyVal =(tempr+tempg+tempb)/3;
	rgrey[x][y]=(unsigned char)greyVal;
	ggrey[x][y]=(unsigned char)greyVal;
	bgrey[x][y]=(unsigned char)greyVal;
}

void make64Grey(int x,int y){

	int greyVal, tempr, tempg, tempb, rem;
	tempr=(int)r[x][y];
	tempg=(int)g[x][y];
	tempb=(int)b[x][y];
	greyVal = (tempr+tempg+tempb)/3;
	rem = greyVal %4;
	greyVal= greyVal-rem;
	rgrey[x][y]=(unsigned char)greyVal;
	ggrey[x][y]=(unsigned char)greyVal;
	bgrey[x][y]=(unsigned char)greyVal;

}
void make32Grey(int x,int y){

	int greyVal, tempr, tempg, tempb, rem;
	tempr=(int)r[x][y];
	tempg=(int)g[x][y];
	tempb=(int)b[x][y];
	greyVal = (tempr+tempg+tempb)/3;
	rem = greyVal %8;
	greyVal=greyVal-rem;
	rgrey[x][y]=(unsigned char)greyVal;
	ggrey[x][y]=(unsigned char)greyVal;
	bgrey[x][y]=(unsigned char)greyVal;

}
void make16Grey(int x,int y){

	int greyVal, tempr, tempg, tempb, rem;
	tempr=(int)r[x][y];
	tempg=(int)g[x][y];
	tempb=(int)b[x][y];
	greyVal = (tempr+tempg+tempb)/3;
	rem = greyVal %16;
	greyVal=greyVal-rem;
	rgrey[x][y]=(unsigned char)greyVal;
	ggrey[x][y]=(unsigned char)greyVal;
	bgrey[x][y]=(unsigned char)greyVal;

}void make8Grey(int x,int y){

	int greyVal, tempr, tempg, tempb;
	tempr=(int)r[x][y];
	tempg=(int)g[x][y];
	tempb=(int)b[x][y];
	greyVal = (tempr+tempg+tempb)/3;
	if(greyVal==0)
		greyVal=0;
	else if(greyVal<=32)
		greyVal=32;
	else if(greyVal<=64)
		greyVal=64;
	else if(greyVal<=96)
		greyVal=96;
	else if (greyVal<=128)
		greyVal=128;
	else if(greyVal<=160)
		greyVal=160;
	else if (greyVal<=192)
		greyVal=192;
	else if(greyVal<=224)
		greyVal=224;
	else
		greyVal=255;
	rgrey[x][y]=(unsigned char)greyVal;
	ggrey[x][y]=(unsigned char)greyVal;
	bgrey[x][y]=(unsigned char)greyVal;
}
void make4Grey(int x,int y){

	int greyVal, tempr, tempg, tempb;
	tempr=(int)r[x][y];
	tempg=(int)g[x][y];
	tempb=(int)b[x][y];
	greyVal = (tempr+tempg+tempb)/3;
	if(greyVal==0)
		greyVal=0;
	else if(greyVal<=64)
		greyVal=64;
	else if (greyVal<=128)
		greyVal=128;
	else if (greyVal<=192)
		greyVal=192;
	else
		greyVal=255;
	rgrey[x][y]=(unsigned char)greyVal;
	ggrey[x][y]=(unsigned char)greyVal;
	bgrey[x][y]=(unsigned char)greyVal;
}
void make2Grey(int x,int y){

	int greyVal, tempr, tempg, tempb;
	tempr=(int)r[x][y];
	tempg=(int)g[x][y];
	tempb=(int)b[x][y];
	greyVal = (tempr+tempg+tempb)/3;
	if(greyVal<128)
		greyVal=0;
	else
		greyVal=255;
	rgrey[x][y]=(unsigned char)greyVal;
	ggrey[x][y]=(unsigned char)greyVal;
	bgrey[x][y]=(unsigned char)greyVal;
}
void pluckSq(int x, int y){
	int mid = size/2;
	int a=0,c=0;
	for(int i=x-mid;i<(x-mid)+size;i++){
		for(int j=y-mid;j<(y-mid)+size;j++){
			rsquare[a][c] = rgrey[i][j];
			gsquare[a][c] = ggrey[i][j];
			bsquare[a][c] = bgrey[i][j];
			c++;
		}
		c=0;
		a++;
	}
}
void grower(int factor){
	if(factor>1){
		for(int i=0,x=0;i<display;i+=factor,x++){
			for(int j=0,y=0;j<display;j+=factor,y++){
				for(int k=0;k<factor;k++){
					rspatial[i][j+k] =rsquare[x][y];
					gspatial[i][j+k] =gsquare[x][y];
					bspatial[i][j+k] =bsquare[x][y];
				}
			}
			for(int m=1;m<factor;m++){
				for(int n=0;n<display;n++){
					rspatial[i+m][n] = rspatial[i][n];
					gspatial[i+m][n] = gspatial[i][n];
					bspatial[i+m][n] = bspatial[i][n];
				}
			}
		}
	}
	else if(factor<1){
		factor=(int)size/display;
		for(int i=0,x=0;i<size;i+=factor,x++){
			for(int j=0,y=0;j<size;j+=factor,y++){
				rspatial[x][y] = rsquare[i][j];
				gspatial[x][y] = gsquare[i][j];
				bspatial[x][y] = bsquare[i][j];
			}
		}
	}
}
void fillHeader(char h[]){
	h[2]=(char)0;h[3]=(char)0;h[4]=(char)0;h[5]=(char)0;
	h[18] =(char)0;h[19] =(char)0;h[20] =(char)0;h[21] =(char)0;
	h[22] =(char)0;h[23] =(char)0;h[24] =(char)0;h[25] =(char)0;
	switch(display){
		case 16://filesize = 822
			h[2]=(char)54;h[3]=(char)3;
			h[18] =(char)16;h[22] =(char)16;
			break;
		case 32://filesize = 3126 =c36		
			h[2]=(char)54;h[3]=(char)12;
			h[18] =(char)32;h[22] =(char)32;
			break;
		case 64://filesize = 12342 =3036		
			h[2]=(char)54;h[3]=(char)48;
			h[18] =(char)64;h[22] =(char)64;
			break;
		case 128://filesize = 49206 =C036		
			h[2]=(char)54;h[3]=(char)192;
			h[18] =(char)128;h[22] =(char)128;
			break;
		case 256://filesize = 196662 =30036	
			h[2]=(char)54;h[4]=(char)3;
			h[19] =(char)1;h[23] =(char)1;
			break;
		case 512://filesize = 196662 =c0036	
			h[2]=(char)54;h[4]=(char)12;
			h[19] =(char)2;h[23] =(char)2;
			break;
	}
}
int getWidth(char header[]){
	int w=0;
	for(int i=3;i>-1;i--){
		w=w<<8;w=w|header[18+i];
	}
	return w;
}
int getHeight(char header[]){
	int h=0;
	for(int i=3;i>-1;i--){
		h=h<<8;h=h|header[22+i];
	}
	return h;
}
void robertsCross(){
	int robx[2][2] = {{1,0},{0,-1}};
	int roby[2][2] = {{0,1},{-1,0}};
	int total =0;
	int subx=0;
	int suby=0;
	int b;
	for(int a=0;a<height-1;a++){
		for(b=0;b<width-1;b++){
			for(int i=0;i<2;i++){
				for(int j=0;j<2;j++){
					subx = subx + (int)rgrey[a+i][b+j]* robx[i][j];
					suby = suby + (int)rgrey[a+i][b+j]* roby[i][j];
				}
			}
			if(subx<0)
				subx = subx*-1;
			if(suby<0)
				suby = suby*-1;
			total = subx+suby;
			if(total > 255)
				total =255;
			if(total >15)//remove these for non thresholding
				total =0;
			else
				total=255;
			edgeArray[a][b] = (char)total;
			subx=0,suby=0;
		}
		edgeArray[a][b] = (char)255;
	}
	for(int j=0;j<width;j++)
		edgeArray[a][j] = (char)255;
}
void sobels(){
	int sobx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
	int soby[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};
	int total = 0;
	int subx=0;
	int suby=0;
	int a,b;
	for(a=0;a<height;a++){
		for(b=0;b<width;b++){
			for(int i=0;i<3;i++){
				for(int j=0;j<3;j++){
					if(a==0&&b==0){
						if(i==0||j==0){}
						else{
							subx= subx + (int)rgrey[a-1+i][b-1+j]*sobx[i][j];
							suby= suby + (int)rgrey[a-1+i][b-1+j]*soby[i][j];
						}
					}
					else if(a==0){
						if(i==0){}
						else{
							subx = subx + (int)rgrey[a-1+i][b-1+j]*sobx[i][j];
							suby = suby + (int)rgrey[a-1+i][b-1+j]*soby[i][j];
						}
					}
					else if(b==0){
						if(j==0){}
						else{
							subx = subx + (int)rgrey[a-1+i][b-1+j]*sobx[i][j];
							suby = suby + (int)rgrey[a-1+i][b-1+j]*soby[i][j];
						}
					}
					else if(a==height-1 && b==width-1){
						if(i==2||j==2){}
						else{
							subx = subx + (int)rgrey[a-1+i][b-1+j]*sobx[i][j];
							suby = suby + (int)rgrey[a-1+i][b-1+j]*soby[i][j];
						}
					}
					else if(a==height-1){
						if(i==2){}
						else{
							subx = subx + (int)rgrey[a-1+i][b-1+j]*sobx[i][j];
							suby = suby + (int)rgrey[a-1+i][b-1+j]*soby[i][j];
						}
					}
					else if(b==width-1){
						if(j==2){}
						else{
							subx = subx + (int)rgrey[a-1+i][b-1+j]*sobx[i][j];
							suby = suby + (int)rgrey[a-1+i][b-1+j]*soby[i][j];
						}
					}
					else{						
						subx = subx + (int)rgrey[a-1+i][b-1+j]*sobx[i][j];
						suby = suby + (int)rgrey[a-1+i][b-1+j]*soby[i][j];
					}
				}
			}
			if(subx<0)
				subx = subx*-1;
			if(suby<0)
				suby = suby*-1;
			total = subx+suby;
			if(total > 255)
				total =255;				
			if(total <25)//remove these for non thresholding
				total =0;
			else
				total=255;		
			edgeArray[a][b] = (char)total;
			subx=0,suby=0;
		}
	}						
}

void gaussian(int gsize, double sigma){
	//int sevsum=4096;//64^2
	//double tosum=1099511627776;	//1048576^2
	double sevbysev[7][7];	
	double tobyto[21][21];	
	int total = 0;
	double subx=0.0;
	int fsize = gsize/2;
	double power;
//	int array7[7] = {1,6,15,20,15,6,1};
//	float temp[height][width];
//
//	for(int a=0;a<height;a++){
//		for(int b=0;b<width;b++){
//			if( b<=fsize ||b>=width-fsize){
//				edgeArray[a][b] = (unsigned char)0;
//			}else{
//				subx =0.0;
//				for(int i=0;i<gsize;i++){
//					subx = subx + rgrey[a][b-fsize+i]*array7[i];
//				}
//				temp[a][b] = subx/sevsum;
//			}
//		}
//	}
//	for(int a=0;a<height;a++){
//		for(int b=0;b<width;b++){
//			if(a<=fsize||a>=height-fsize){
//				edgeArray[a][b] = (unsigned char)0;
//			}else{
//				suby =0.0;
//				for(int i=0;i<gsize;i++){
//					suby = suby + temp[a-fsize+i][b]*array7[i];
//				}
//				suby = suby/sevsum;
//				total = suby;
//				if(total > 255)
//					total =255;		
//				edgeArray[a][b] = (char)total;
//			}
//		}
//	}
//}
	double counter=0;

	if(gsize ==7){//create the gaussian matrix
		//double array7[7];// = {1,6,15,20,15,6,1};
		for(int i=0, k=-1*fsize; i< gsize;i++,k++){
			for(int j=0, m=-1*fsize;j<gsize;j++, m++){
				//array7[i] = array7[i]/64;
				power= -1*(((m*m)+(k*k))/(2*sigma*sigma));
				sevbysev[i][j] = (1/(2*PI*sigma*sigma))*exp(power);
				counter += sevbysev[i][j];
			}
		}
		for(int i=0; i< gsize;i++){
			for(int j=0;j<gsize;j++){
				sevbysev[i][j] = sevbysev[i][j] /counter;///sevsum;
			}
		}
		cout<< "counter =" <<counter;
	}else if(gsize ==21){
		//double array21[21];// = {1,20,190,1140,4845,15504,38760,77520,125970,167960,184756,167960,125970,77520,38760,15504,4845,1140,190,20,1};
		for(int i=0,k=-1*fsize; i< gsize;i++,k++){
			for(int j=0, m=-1*fsize;j<gsize;j++,m++){
				//array21[i] = array21[i]/1048576;
				power= -1*(((m*m)+(k*k))/(2*sigma*sigma));
				tobyto[i][j] = (1/(2*PI*sigma*sigma))*exp(power);
				counter += tobyto[i][j];
			}
		}
		for(int i=0; i< gsize;i++){
			for(int j=0;j<gsize;j++){
				tobyto[i][j] = tobyto[i][j]/counter;///tosum;
			}
		}
	}
	
	int differencex=0;
	int differencey=0;
	for(int a=0;a<height;a++){
		for(int b=0;b<width;b++){
			differencey = a-fsize;
			for(int i=0;i<gsize;i++){
				if(differencey>=0){//see if difference is negative: if it is then skip the sum
					differencex = b-fsize;
					for(int j=0;j<gsize;j++){
						if(differencex>=0){//see if difference is negative
							if(gsize == 7){//do the sum
								subx = subx + (int)rgrey[a-fsize+i][b-fsize+j]* sevbysev[i][j];
							}
							else if(gsize == 21){
								subx = subx + (int)rgrey[a-fsize+i][b-fsize+j]* tobyto[i][j];
							}
						}
						differencex++;
					}
				}
				differencey++;
			}
			//if(subx<0)
			//	subx = 0;//subx = subx*-1;
			gaussArray[a][b] = subx;
			total = (int)(subx);
			if(total > 255)
				total =255;		
			edgeArray[a][b] = (char)total;
			subx=0.0;
		}
	}
}
void laplacian(){
	int lap[3][3] = {{1,1,1},{1,-8,1},{1,1,1}};
	int total = 0;
	double subx=0;
	int a,b;
	double min=256, max=-256;
	for(a=0;a<height;a++){
		for(b=0;b<width;b++){
			for(int i=0;i<3;i++){
				for(int j=0;j<3;j++){
					if(a==0&&b==0){
						if(i==0||j==0){}
						else{
							subx= subx + gaussArray[a-1+i][b-1+j]*lap[i][j];
						}
					}
					else if(a==0){
						if(i==0){}
						else{
							subx = subx + gaussArray[a-1+i][b-1+j]*lap[i][j];
						}
					}
					else if(b==0){
						if(j==0){}
						else{
							subx = subx + gaussArray[a-1+i][b-1+j]*lap[i][j];
						}
					}
					else if(a==height-1 && b==width-1){
						if(i==2||j==2){}
						else{
							subx = subx + gaussArray[a-1+i][b-1+j]*lap[i][j];
						}
					}
					else if(a==height-1){
						if(i==2){}
						else{
							subx = subx + gaussArray[a-1+i][b-1+j]*lap[i][j];
						}
					}
					else if(b==width-1){
						if(j==2){}
						else{
							subx = subx + gaussArray[a-1+i][b-1+j]*lap[i][j];
						}
					}
					else{						
						subx = subx + gaussArray[a-1+i][b-1+j]*lap[i][j];
					}
				}
			}
			if(subx>max){
				max = subx;
			}
			if(subx<min){
				min = subx;
			}
			//if(subx<0)
			//	subx =0; //subx*-1;
			//total = subx;
			//if(total > 255)
			//	total =255;		
			tempArray[a][b] = (int)subx;
			subx=0;
		}
	}	
	for(int i=0;i<height;i++){//normalizing range of values
		for(int j=0;j<width;j++){
			edgeArray2[i][j] = (char)((tempArray[i][j] - min)*(255/(max-min)));
		}
	}
}
void lapOfGauss(int gsize, double sigma){
	double sevbysev[7][7];	
	double tobyto[21][21];	
	double numerator;
	double denomerator;
	double fraction;
	double base;
	int total = 0;
	double subx=0.0;
	int fsize = gsize/2;
	double lap;
	double count =0;
	double min=256, max=-256;
	if(gsize ==7){//create the gaussian matrix
		for(int i=0, k=-1*fsize; i< gsize;i++,k++){//height->y
			for(int j=0, m=-1*fsize;j<gsize;j++,m++){//width->x
				numerator = (m*m)+(k*k);
				denomerator = 2*sigma*sigma;
				fraction = -1*(numerator/denomerator);
				lap = 1/(PI*sigma*sigma*sigma*sigma);
				sevbysev[i][j]= (-1*(lap))*(1+fraction)*exp(fraction);
				//base = ((numerator-denomerator)/(pow(sigma,4.0)));
				//sevbysev[i][j] = pow(base, fraction);
				count= count + sevbysev[i][j];

			}
		}
		cout<<count;
		for(int i=0;i<gsize;i++){
			for(int j=0;j<gsize;j++){
				sevbysev[i][j] = sevbysev[i][j] / count;
			}
		}

	}else if(gsize ==21){
		for(int i=0,k=-1*fsize; i< gsize;i++,k++){
			for(int j=0, m=-1*fsize; j<gsize;j++,m++){
				numerator = (m*m)+(k*k);
				denomerator = 2*sigma*sigma;
				fraction = -1*(numerator/denomerator);
				lap = 1/(PI*sigma*sigma*sigma*sigma);
				//array21[i] = array21[i]/1048576;
				//power= -1*((j*j)/(2*sigma*sigma));
	//			array21[i] = 1/((sqrt(2*PI)*sigma)*(pow(E, power)));
				tobyto[i][j] = (-1*(lap))*(1+fraction)*exp(fraction);///tosum;
				count = count +tobyto[i][j];
			}
		}
		cout<<count;	
		for(int i=0;i<gsize;i++){
			for(int j=0;j<gsize;j++){
				tobyto[i][j] = tobyto[i][j] / count;
			}
		}
	}
	
	int differencex=0;
	int differencey=0;
	for(int a=0;a<height;a++){
		for(int b=0;b<width;b++){
			differencey = a-fsize;
			for(int i=0;i<gsize;i++){
				if(differencey>=0){//see if difference is negative: if it is then skip the sum
					differencex = b-fsize;
					for(int j=0;j<gsize;j++){
						if(differencex>=0){//see if difference is negative
							if(gsize == 7){//do the sum
								subx = subx + (int)rgrey[a-fsize+i][b-fsize+j]* sevbysev[i][j];
							}
							else if(gsize == 21){
								subx = subx + (int)rgrey[a-fsize+i][b-fsize+j]* tobyto[i][j];
							}
						}
						differencex++;
					}
				}
				differencey++;
			}
			//if(subx<0)
			//	subx = subx*-1;
			if(subx>max){
				max = subx;
			}
			if(subx<min){
				min = subx;
			}
			tempArray[a][b] = (int)subx;
			//total = (int)(subx);
			//if(total > 255)
			//	total =255;		
			//edgeArray2[a][b] = (char)total;
			subx=0.0;
		}
	}
	for(int i=0;i<height;i++){//normalizing range of values
		for(int j=0;j<width;j++){
			edgeArray2[i][j] = (char)((tempArray[i][j] - min)*(255/(max-min)));
		}
	}
}
void zeroCross(){
	int temp;
	for(int a=0;a<height;a++){
		for(int b=0;b<width;b++){
			int min = 256,max=-256;
			for(int i =0;i<3;i++){
				for(int j=0;j<3;j++){
					temp = tempArray[a+i][b+j];
					if(temp<min)
						min=temp;
					if(temp>max)
						max=temp;
				}
			}
			if(min < -1 && max > 1)
				zeroArray[a][b] = (char) 255;
			else 
				zeroArray[a][b] = (char) 0;
		}
	}
}

void thinning(){
	int array1[3][3] = {{0,0,0},{-1,1,-1},{1,1,1}};
	int array2[3][3] = {{0,-1,1},{0,1,1},{0,-1,1}};
	int array3[3][3] = {{1,1,1},{-1,1,-1},{0,0,0}};
	int array4[3][3] = {{1,-1,0},{1,1,0},{1,-1,0}};
	int array5[3][3] = {{-1,0,0},{1,1,0},{-1,1,-1}};
	int array6[3][3] = {{0,0,-1},{0,1,1},{-1,1,-1}};
	int array7[3][3] = {{-1,1,-1},{0,1,1},{0,0,-1}};
	int array8[3][3] = {{-1,1,-1},{1,1,0},{-1,0,0}};
	int count=1;		
	int quit=0;
	for(int a=0;a<height;a++){
		for(int b=0;b<width;b++){
			if(a==0||b==0||a==height-1||b==width-1){
				tempArray1[a][b] =0;
				tempArray2[a][b]=0;
			}else if((int)zeroArray[a][b]==0){
				tempArray1[a][b] = 0;
				tempArray2[a][b] = 0;
			}else{
				tempArray1[a][b] = 1;
				tempArray2[a][b] = 1;
			}
		}
	}	
	while (count!=0 && quit <=7){
		count =0;
		for(int a=1;a<height-1;a++){
			for(int b=1;b<width-1;b++){
				if(array1[0][0]==tempArray1[a-1][b-1]&&array1[0][1]==tempArray1[a-1][b]&&array1[0][2]==tempArray1[a-1][b+1]&&array1[1][1]==tempArray1[a][b]&&array1[2][0]==tempArray1[a+1][b-1]&&array1[2][1]==tempArray1[a+1][b]&&array1[2][2]==tempArray1[a+1][b+1]){
					tempArray2[a][b] = 0;
					count++;
				}
			}
		}
		for(int a=1;a<height-1;a++){
			for(int b=1;b<width-1;b++){
				tempArray1[a][b] = tempArray2[a][b];
			}
		}
		for(int a=1;a<height-1;a++){
			for(int b=1;b<width-1;b++){
				if(array2[0][0]==tempArray1[a-1][b-1]&&array2[0][2]==tempArray1[a-1][b+1]&&array2[1][0]==tempArray1[a][b-1]&&array2[1][1]==tempArray1[a][b]&&array2[1][2]==tempArray1[a][b+1]&&array2[2][0]==tempArray1[a+1][b-1]&&array2[2][2]==tempArray1[a+1][b+1]){
					tempArray2[a][b] = 0;
					count++;
				}
			}
		}		
		for(int a=1;a<height-1;a++){
			for(int b=1;b<width-1;b++){
				tempArray1[a][b] = tempArray2[a][b];
			}
		}
		for(int a=1;a<height-1;a++){
			for(int b=1;b<width-1;b++){
				if(array3[0][0]==tempArray1[a-1][b-1]&&array3[0][1]==tempArray1[a-1][b]&&array3[0][2]==tempArray1[a-1][b+1]&&array3[1][1]==tempArray1[a][b]&&array3[2][0]==tempArray1[a+1][b-1]&&array3[2][1]==tempArray1[a+1][b]&&array3[2][2]==tempArray1[a+1][b+1]){
					tempArray2[a][b] = 0;
					count++;
				}
			}
		}		
		for(int a=1;a<height-1;a++){
			for(int b=1;b<width-1;b++){
				tempArray1[a][b] = tempArray2[a][b];
			}
		}
		for(int a=1;a<height-1;a++){
			for(int b=1;b<width-1;b++){
				if(array4[0][0]==tempArray1[a-1][b-1]&&array4[0][2]==tempArray1[a-1][b+1]&&array4[1][0]==tempArray1[a][b-1]&&array4[1][1]==tempArray1[a][b]&&array4[1][2]==tempArray1[a][b+1]&&array4[2][0]==tempArray1[a+1][b-1]&&array4[2][2]==tempArray1[a+1][b+1]){
					tempArray2[a][b] = 0;
					count++;
				}
			}
		}		
		for(int a=1;a<height-1;a++){
			for(int b=1;b<width-1;b++){
				tempArray1[a][b] = tempArray2[a][b];
			}
		}
		for(int a=1;a<height-1;a++){
			for(int b=1;b<width-1;b++){
				if(array5[0][1]==tempArray1[a-1][b]&&array5[0][2]==tempArray1[a-1][b+1]&&array5[1][0]==tempArray1[a][b-1]&&array5[1][1]==tempArray1[a][b]&&array5[1][2]==tempArray1[a][b+1]&&array5[2][1]==tempArray1[a+1][b]){
					tempArray2[a][b] = 0;
					count++;
				}
			}
		}		
		for(int a=1;a<height-1;a++){
			for(int b=1;b<width-1;b++){
				tempArray1[a][b] = tempArray2[a][b];
			}
		}
		for(int a=1;a<height-1;a++){
			for(int b=1;b<width-1;b++){
				if(array6[0][0]==tempArray1[a-1][b-1]&&array6[0][1]==tempArray1[a-1][b]&&array6[1][0]==tempArray1[a][b-1]&&array6[1][1]==tempArray1[a][b]&&array6[1][2]==tempArray1[a][b+1]&&array6[2][1]==tempArray1[a+1][b]){
					tempArray2[a][b] = 0;
					count++;
				}
			}
		}		
		for(int a=1;a<height-1;a++){
			for(int b=1;b<width-1;b++){
				tempArray1[a][b] = tempArray2[a][b];
			}
		}
		for(int a=1;a<height-1;a++){
			for(int b=1;b<width-1;b++){
				if(array7[0][1]==tempArray1[a-1][b]&&array7[1][0]==tempArray1[a][b-1]&&array7[1][1]==tempArray1[a][b]&&array7[1][2]==tempArray1[a][b+1]&&array7[2][0]==tempArray1[a+1][b-1]&&array7[2][1]==tempArray1[a+1][b]){
					tempArray2[a][b] = 0;
					count++;
				}
			}
		}		
		for(int a=1;a<height-1;a++){
			for(int b=1;b<width-1;b++){
				tempArray1[a][b] = tempArray2[a][b];
			}
		}
		for(int a=1;a<height-1;a++){
			for(int b=1;b<width-1;b++){
				if(array8[0][1]==tempArray1[a-1][b]&&array8[1][0]==tempArray1[a][b-1]&&array8[1][1]==tempArray1[a][b]&&array8[1][2]==tempArray1[a][b+1]&&array8[2][1]==tempArray1[a+1][b]&&array8[2][2]==tempArray1[a+1][b+1]){
					tempArray2[a][b] = 0;
					count++;
				}
			}
		}		
		for(int a=1;a<height-1;a++){
			for(int b=1;b<width-1;b++){
				tempArray1[a][b] = tempArray2[a][b];
			}
		}
		cout<<"morph count= "<<count<<endl;
		quit++;
	}
	for(int a=0;a<height;a++){
		for(int b=0;b<width;b++){
			if(tempArray1[a][b] ==1){
				zeroArray[a][b] = (char)255;
			}else{
				zeroArray[a][b] = (char)0;
			}
		}
	}
}
void morph(){

	int array1[3][3] = {{1,1,1},{1,1,1},{1,1,1}};
	for(int a=0;a<height;a++){
		for(int b=0;b<width;b++){
			if(a==0||b==0||a==height-1||b==width-1){
				tempArray1[a][b] =0;
				tempArray2[a][b]=0;
			}else if((int)rsquare[a][b]==0){
				tempArray1[a][b] = 0;
				tempArray2[a][b] = 0;
			}else{
				tempArray1[a][b] = 1;
				tempArray2[a][b] = 1;
			}
		}
	}	
	for(int a=1;a<height-1;a++){
		for(int b=1;b<width-1;b++){
			if(tempArray1[a][b] ==0){
				if(array1[0][0]==tempArray1[a-1][b-1]||array1[0][1]==tempArray1[a-1][b]||array1[0][2]==tempArray1[a-1][b+1]||array1[1][0]==tempArray1[a][b-1]||array1[1][1]==tempArray1[a][b]||array1[1][2]==tempArray1[a][b+1]||array1[2][0]==tempArray1[a+1][b-1]||array1[2][1]==tempArray1[a+1][b]||array1[2][2]==tempArray1[a+1][b+1]){
					tempArray2[a][b] =1;
				}
			}
		}
	}	
	for(int a=0;a<height;a++){
		for(int b=0;b<width;b++){
			if(tempArray1[a][b]==0&&tempArray2[a][b]==1){
				zeroArray[a][b]=(char)255;
			}else{
				zeroArray[a][b]=(char)0;
			}
		}
	}	
}
void extraCredit(){
	int array1[14][14]={{0,0,0,0,0,1,1,1,1,0,0,0,0,0},//center=7,7
					{0,0,0,1,1,1,1,1,1,1,1,0,0,0},
					{0,0,1,1,1,1,1,1,1,1,1,1,0,0},
					{0,1,1,1,1,1,1,1,1,1,1,1,1,0},
					{0,1,1,1,1,1,1,1,1,1,1,1,1,0},
					{1,1,1,1,1,1,1,1,1,1,1,1,1,1},
					{1,1,1,1,1,1,1,1,1,1,1,1,1,1},
					{1,1,1,1,1,1,1,1,1,1,1,1,1,1},
					{1,1,1,1,1,1,1,1,1,1,1,1,1,1},
					{0,1,1,1,1,1,1,1,1,1,1,1,1,0},
					{0,1,1,1,1,1,1,1,1,1,1,1,1,0},
					{0,0,1,1,1,1,1,1,1,1,1,1,0,0},
					{0,0,0,1,1,1,1,1,1,1,1,0,0,0},
					{0,0,0,0,0,1,1,1,1,0,0,0,0,0}};
	int array2[7][7]={{1,1,1,1,1,1,1},
					  {1,1,1,1,1,1,1},
					  {1,1,1,1,1,1,1},
					  {1,1,1,1,1,1,1},
					  {1,1,1,1,1,1,1},
					  {1,1,1,1,1,1,1},
					  {1,1,1,1,1,1,1}};
	for(int a=0;a<height;a++){
		for(int b=0;b<width;b++){
			if((int)rsquare[a][b]==0){
				tempArray1[a][b] = 0;
				tempArray2[a][b] = 0;
			}else{
				tempArray1[a][b] = 1;
				tempArray2[a][b] = 1;
			}
		}
	}
	bool test;
	for(int a=7;a<height-7;a++){//remove circles
		for(int b=7;b<width-7;b++){
			test = true;
			for(int i= 0, k=-6;i<14;i++,k++){
				for(int j=0, m=-6;j<14;j++,m++){
					if(array1[i][j]==1){
						if(tempArray1[a+k][b+m]==0){
							test =false;
						}
					}
				}
			}
			if(test==true){
				for(int i= 0, k=-6;i<14;i++,k++){
					for(int j=0, m=-6;j<14;j++,m++){
						if(array1[i][j]==1){
							tempArray2[a+k][b+m]=0;
						}
					}
				}
			}
		}
	}		
	for(int a=0;a<height;a++){
		for(int b=0;b<width;b++){
			tempArray1[a][b] = tempArray2[a][b];
		}
	}
	for(int a=3;a<height-3;a++){//dialation
		for(int b=3;b<width-3;b++){
			test = false;
			if(tempArray1[a][b]==1){
				test =true;
			}
			if(test==true){
				for(int i= 0, k=-1;i<3;i++,k++){
					for(int j=0, m=-1;j<3;j++,m++){
						tempArray2[a+k][b+m]= 1;
					}
				}
			}
		}
	}		
	for(int a=0;a<height;a++){
		for(int b=0;b<width;b++){
			tempArray1[a][b] = tempArray2[a][b];
		}
	}
	for(int a=3;a<height-3;a++){//erosion
		for(int b=3;b<width-3;b++){
			test = false;
			for(int i= 0, k=-1;i<3;i++,k++){
				for(int j=0, m=-1;j<3;j++,m++){
					if(tempArray1[a+k][b+m]==0){
						test =true;
					}
				}
			}
			if(test==true){
				for(int i= 0, k=-1;i<3;i++,k++){
					for(int j=0, m=-1;j<3;j++,m++){
						tempArray2[a+k][b+m]= 0;
					}
				}
			}
		}
	}
	for(int a=0;a<height;a++){
		for(int b=0;b<width;b++){
			if(tempArray2[a][b]==1){
				zeroArray[a][b]=(char)255;
			}else{
				zeroArray[a][b]=(char)0;
			}
		}
	}	
}
int main(){
	char h[54];//header
	int grow=0;

	ifstream fin;
	fin.open("pattern1.bmp", ios::binary);
	for(int k=0; k<54;k++){//read in header
		h[k]=fin.get();
	}

	for(int i=0;i<height;i++){//read in data
		for(int j=0;j<width;j++){
			r[i][j]=fin.get();
			g[i][j]=fin.get();
			b[i][j]=fin.get();
		}
	}
	for(int i=0; i<54;i++)
		cout<<(int)h[i]<<" ";
	cout<<endl;

	fin.close();
	ofstream fout("pattern1_grey.bmp", ios::binary);
	for(i=0;i<54;i++)
		fout<<h[i];

////normal color output
//	for(int x=0;x<height;x++){	
//		for(int y=0;y<width;y++){
//			fout<<r[x][y]<<g[x][y]<<b[x][y];
//		}
//	}

//grey output
	for(int x=0;x<height;x++){	
		for(int y=0;y<width;y++){
			makeGrey(x,y);
			fout<<rgrey[x][y]<<ggrey[x][y]<<bgrey[x][y];
		}
	}
	fout.close();

	////histogram
	//ofstream foutE("flowers_equalhisto.bmp", ios::binary);
	//ofstream foutH("flowers_histograms.bmp", ios::binary);
	//for(i=0;i<54;i++)
	//	foutE<<h[i];
	////393270 =60036
	//h[2]=(char)54;h[3]=(char)0;h[4]=(char)6;h[5]=(char)0;
	//h[18]=(char)0;h[19]=(char)1;h[20]=(char)0;h[21]=(char)0;
	//h[22]=(char)0;h[23]=(char)2;h[24]=(char)0;h[25]=(char)0;
	//for(i=0;i<54;i++)
	//	foutH<<h[i];

	//float histo[256];
	//float equal_histo[256];
	//float histogram[256];
	//for (i=0; i<256; i++){
	//	equal_histo[i]= 0.0; 
	//	histo[i] = 0.0;
	//}
	//for (int j=0;j<height;j++){
	//	for (i=0; i<width; i++){
	//		histo[(int)rgrey[i][j]] += 1.0;
	//	}
	//}
	//	// scale histogram to [0, 255] for display purpose
	//	float gmin = 256;
	//	float gmax = -1;
	//	for (i=0; i<256; i++){
	//		if (histo[i]< gmin) gmin = histo[i];
	//		if (histo[i]> gmax) gmax = histo[i];
	//	}
	//	float gdelta = gmax - gmin;

	//	for (i=0; i <256; i++){
	//		// scale for display purposes
	//		histogram[i] = unsigned char ((histo[i] - gmin)*(255/gdelta));
	//		// compute the relative frequency histogram h’(n)
	//		histo[i] = histo[i]/(width*height);
	//	}
	//	
	//	// compute mapping f(n)
	//	// equal[] is the mapping function f(n)
	//	float hsum=0.0;
	//	float fequal[256];
	//	unsigned char equal[256];
	//	float fmax=0.0;
	//	for (i=0; i<256; i++){
	//		hsum = hsum + histo[i];
	//		fequal[i] = hsum*(256-1) + 0.5;
	//		if (fequal[i] > fmax) fmax = fequal[i];
	//		equal[i] = unsigned char (fequal[i]);

	//	}
	//	unsigned char equalized[height][width];
	//// xd and yd are dimensions of original image
	//// equalized[][] is the equalized image
	//// Use of this temporary array is for illustration only, can use pointer, e.g., *indexr
	//// equal_histo[] is the histogram array of the equalized image
	//	for (j=0;j<height;j++){
	//		for (i=0; i<width; i++){//rgbfix
	//			equalized[j][i] = equal[rgrey[j][i]];
	//			equal_histo[equalized[j][i]] +=1;
	//		}
	//	}
	//	// scale the equal_histo[] for display purpose
	//	gmin = 256;
	//	gmax = -1;
	//	for (i=0; i<256; i++){
	//		if (equal_histo[i]< gmin) gmin = equal_histo[i];
	//		if (equal_histo[i]> gmax) gmax = equal_histo[i];

	//	}
	//	// equal_histogram[] is the scaled histogram array
	//	gdelta = gmax - gmin;
	//	unsigned char equal_histogram[256];
	//	for (i=0; i <256; i++)
	//	        equal_histogram[i] = unsigned char ((equal_histo[i] - gmin)*(255)/gdelta);
	//	
	//	// combining the scaled histogram arrays to display.
	//	// histogram_image[][] is the image of the two histograms
	//	// use of this temporary array is for illustration only, can use pointer, e.g., *indexr
	//	unsigned char histogram_image[512][256];
	//	for (j = 0; j <512; j++){
	//	   for (i=0; i< 256; i++){
	//	       if (j < 256 ){
	//		// histogram of original image is displayed on top
	//				if (histogram[i] < (255-j)) histogram_image[j][i] = 255;
	//				else histogram_image[j][i] = 0;
	//	      }
	//	      else {
	//		// histogram of equalized image is displayed at bottom
	//				if (equal_histogram[i] < (511-j)) histogram_image[j][i] = 255;
	//				else 
	//					histogram_image[j][i] =0;
	//	      }
	//	   }
	//	}	
	//	for(int i=511;i>=0;i--){
	//		for(int j=0;j<256;j++){
 //               foutH<<histogram_image[i][j]<<histogram_image[i][j]<<histogram_image[i][j];
	//		}
	//	}
	//	for(int i=0;i<height;i++){
	//		for(int j=0;j<width;j++){
 //               foutE<<equalized[i][j]<<equalized[i][j]<<equalized[i][j];
	//		}
	//	}

	//	foutH.close();
	//	foutE.close();
//******************************
//*    spatial
//*******************************
	//ofstream fout2("arteries_spatial512.bmp", ios::binary);
	//fillHeader(h);

	//for(i=0;i<54;i++)
	//	fout2<<h[i];
	//pluckSq(460,310);//(height,width)
	//if(size!=display){
	//	grow=(int)display/size;
	//	grower(grow);
	//}

	//if(size!=display){
	//	for(int i=0;i<display;i++){
	//		for(int j=0;j<display;j++){
	//			fout2<<rspatial[i][j]<<gspatial[i][j]<<bspatial[i][j];
	//		}	
	//	}
	//}else{
	//	for(int i=0;i<display;i++){
	//		for(int j=0;j<display;j++){
	//			fout2<<rsquare[i][j]<<gsquare[i][j]<<bsquare[i][j];
	//		}	
	//	}
	//}

	//fout2.close();

	//***********************************
	//*          edge detector 
	//***********************************

	ofstream foutEdge("pattern1_SobEdge.bmp", ios::binary);
	for(int i=0;i<54;i++)
		foutEdge<<h[i];
	//robertsCross();
	sobels();
	//gaussian(21, 4);
	//laplacian();
	//lapOfGauss(7,1);
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
            foutEdge<<edgeArray[i][j]<<edgeArray[i][j]<<edgeArray[i][j];
		}
	}
	foutEdge.close();
//**************************************
//*      zero crossings
//**************************************

	//ofstream foutZero("Ziyi_Zhang_zero.bmp", ios::binary);
	//for(int i=0;i<54;i++)
	//	foutZero<<h[i];
	//zeroCross();
	//for(int i=0;i<height;i++){
	//	for(int j=0;j<width;j++){
 //           foutZero<<zeroArray[i][j]<<zeroArray[i][j]<<zeroArray[i][j];
	//	}
	//}
	//foutZero.close();

//*****************************
//*          morphology
//*****************************
	//ofstream foutMorph("pattern1_morph.bmp", ios::binary);
	//for(int i=0;i<54;i++)
	//	foutMorph<<h[i];
	////morph();
	////extraCredit();
	//thinning();
	//for(int i=0;i<height;i++){
	//	for(int j=0;j<width;j++){
 //           foutMorph<<zeroArray[i][j]<<zeroArray[i][j]<<zeroArray[i][j];
	//	}
	//}
	//foutMorph.close();

	return 0;
}