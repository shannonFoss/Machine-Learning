#include <iostream>
#include <fstream> //file io
#include <stdlib.h>
#include <math.h>
#include <cstdio>

using namespace std;
#define PI 3.14159265
const int width=640, height= 480;

unsigned char r[height][width];
unsigned char g[height][width];
unsigned char b[height][width];
unsigned char rgrey[height][width];
unsigned char edgeArray[height][width];
unsigned char edgeArray1[height][width]={0};
double gaussArray[height][width];
unsigned char borderArray[height][width];
int houghArray[180][180]={0};//length (4), angle(1)
unsigned char cornerArray[height][width]={0};

void makeGrey(int x,int y){
	int greyVal,tempr, tempg, tempb;
	tempr=(int)r[x][y];
	tempg=(int)g[x][y];
	tempb=(int)b[x][y];
	greyVal =(tempr+tempg+tempb)/3;
	rgrey[x][y]=(unsigned char)greyVal;
}
void sobels(){
	int sobx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
	int soby[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};
	int total = 0;
	double subx=0;
	double suby=0;
	int a,b;
	for(a=0;a<height;a++){
		for(b=0;b<width;b++){
			for(int i=0;i<3;i++){
				for(int j=0;j<3;j++){
					if(a==0&&b==0){
						if(i==0||j==0){}
						else{
							subx= subx + gaussArray[a-1+i][b-1+j]*sobx[i][j];
							suby= suby + gaussArray[a-1+i][b-1+j]*soby[i][j];
						}
					}
					else if(a==0){
						if(i==0){}
						else{
							subx = subx + gaussArray[a-1+i][b-1+j]*sobx[i][j];
							suby = suby + gaussArray[a-1+i][b-1+j]*soby[i][j];
						}
					}
					else if(b==0){
						if(j==0){}
						else{
							subx = subx + gaussArray[a-1+i][b-1+j]*sobx[i][j];
							suby = suby + gaussArray[a-1+i][b-1+j]*soby[i][j];
						}
					}
					else if(a==height-1 && b==width-1){
						if(i==2||j==2){}
						else{
							subx = subx + gaussArray[a-1+i][b-1+j]*sobx[i][j];
							suby = suby + gaussArray[a-1+i][b-1+j]*soby[i][j];
						}
					}
					else if(a==height-1){
						if(i==2){}
						else{
							subx = subx + gaussArray[a-1+i][b-1+j]*sobx[i][j];
							suby = suby + gaussArray[a-1+i][b-1+j]*soby[i][j];
						}
					}
					else if(b==width-1){
						if(j==2){}
						else{
							subx = subx + gaussArray[a-1+i][b-1+j]*sobx[i][j];
							suby = suby + gaussArray[a-1+i][b-1+j]*soby[i][j];
						}
					}
					else{						
						subx = subx + gaussArray[a-1+i][b-1+j]*sobx[i][j];
						suby = suby + gaussArray[a-1+i][b-1+j]*soby[i][j];
					}
				}
			}
			if(subx<0)
				subx = subx*-1;
			if(suby<0)
				suby = suby*-1;
			total = (int)(subx+suby);
			if(total > 255)
				total =255;				
			if(total <79)//remove these for non thresholding
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


int main(){
	char h[54];//header
	
	ifstream fin;
	fin.open("C:\\Documents and Settings\\Shannon Foss\\My Documents\\Visual Studio Projects\\Hough\\inputPics\\Book2.bmp", ios::binary);
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
	fin.close();

	ofstream fout("C:\\Documents and Settings\\Shannon Foss\\My Documents\\Visual Studio Projects\\Hough\\outputPics\\Book2_grey.bmp", ios::binary);
	for(i=0;i<54;i++)
		fout<<h[i];
	//grey output
	for(int x=0;x<height;x++){	
		for(int y=0;y<width;y++){
			makeGrey(x,y);
			fout<<rgrey[x][y]<<rgrey[x][y]<<rgrey[x][y];
		}
	}
	fout.close();
	//***********************************
	//*          edge detector 
	//***********************************

	ofstream foutEdge("C:\\Documents and Settings\\Shannon Foss\\My Documents\\Visual Studio Projects\\Hough\\outputPics\\Book2_SobEdge.bmp", ios::binary);
	for(int i=0;i<54;i++)
		foutEdge<<h[i];
	gaussian(7, 1.7);
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			rgrey[i][j]= edgeArray[i][j];
		}
	}
	sobels();
	for(int i=0;i<height;i++){//dialation
		for(int j=0;j<width;j++){
			if(edgeArray[i][j]==(unsigned char)255 && edgeArray[i-1][j]==(unsigned char)0 && edgeArray[i][j-1]==(unsigned char)0 && edgeArray[i+1][j]==(unsigned char)0 && edgeArray[i][j+1]==(unsigned char)0){
				edgeArray[i][j]=(unsigned char)0;
				edgeArray1[i][j]=(unsigned char)0;
			}
			if(edgeArray[i][j]==(unsigned char)255){
				edgeArray1[i-1][j-1]=(unsigned char)255;
				edgeArray1[i-1][j]=(unsigned char)255;				
				edgeArray1[i-1][j+1]=(unsigned char)255;
				edgeArray1[i][j-1]=(unsigned char)255;				
				edgeArray1[i][j+1]=(unsigned char)255;
				edgeArray1[i+1][j-1]=(unsigned char)255;				
				edgeArray1[i+1][j]=(unsigned char)255;
				edgeArray1[i+1][j+1]=(unsigned char)255;
			}
		}
	}

	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
            foutEdge<<edgeArray1[i][j]<<edgeArray1[i][j]<<edgeArray1[i][j];
		}
	}
	foutEdge.close();
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
            borderArray[i][j]=0;
		}
	}
	bool first;
	int last;
	for(int i=0;i<height;i++){
		first = false;
		last =0;
		for(int j=0;j<width;j++){
			if(i>5&&j>5&&i<height-5 &&j<width-5){
				if(edgeArray1[i][j]==(unsigned char)255 && first==false){
					first =true;
					last=j;
					borderArray[i][j]=(unsigned char)255;
				}else{
					if(edgeArray1[i][j]==(unsigned char)255){
						last =j;
					}
				}
			}
		}			
		if(last!=0)
			borderArray[i][last]=(unsigned char)255;
	}
	bool top;
	int bottom;
	for(int i=0;i<width;i++){
		top = false;
		bottom =0;
		for(int j=0;j<height;j++){
			if(i>5&&j>5&&i<width-5 &&j<height-5){
				if(edgeArray1[j][i]==(unsigned char)255 && top==false){
					top =true;
					bottom=j;
					borderArray[j][i]=(unsigned char)255;
				}else{
					if(edgeArray1[j][i]==(unsigned char)255){
						bottom =j;
					}
				}
			}
		}			
		if(bottom!=0)
			borderArray[bottom][i]=(unsigned char)255;
	}
	ofstream foutBorder("C:\\Documents and Settings\\Shannon Foss\\My Documents\\Visual Studio Projects\\Hough\\outputPics\\Book2_Border.bmp", ios::binary);
	for(int i=0;i<54;i++)
		foutBorder<<h[i];
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			foutBorder<<borderArray[i][j]<<borderArray[i][j]<<borderArray[i][j];
		}
	}
	foutBorder.close();
//************************************
//              Hough
//************************************
	//ofstream foutHough("C:\\Documents and Settings\\Shannon Foss\\My Documents\\Visual Studio Projects\\Hough\\outputPics\\Book1_Hough.bmp", ios::binary);
	////width*height*3+54 = 97254 = 17BE6
	//h[2]=(char)230;h[3]=(char)123;h[4]=(char)1;h[5]=(char)0;
	//h[18]=(char)180;h[19]=(char)0;h[20]=(char)0;h[21]=(char)0;
	//h[22]=(char)180;h[23]=(char)0;h[24]=(char)0;h[25]=(char)0;
	//for(int i=0;i<54;i++)
	//	foutHough<<h[i];
	//double r;
	//int ri;
	//for(int y=0;y<height;y++){
	//	for(int x=0;x<width;x++){
	//		if(borderArray[x][y] == (unsigned char)255){
	//			for(int d=0;d<180;d++){
	//				//r=xcos0+ysin0
	//				//rad = deg *PI/180
	//				r=x*(cos((d*PI)/180))+y*(sin((d*PI)/180));
	//				ri = (int)floor(r/9);
	//				ri=ri+90;
	//				if(ri>=0 &&ri<180){
	//					if(houghArray[ri][d]<255){
	//						houghArray[ri][d]++;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
	//for(int i=0;i<180;i++){
	//	for(int j=0;j<180;j++){
	//		foutHough<<(unsigned char)houghArray[i][j]<<(unsigned char)houghArray[i][j]<<(unsigned char)houghArray[i][j];
	//	}
	//}

	//foutHough.close();
//***********************************
//               corners
//***********************************
	ofstream foutCorner("C:\\Documents and Settings\\Shannon Foss\\My Documents\\Visual Studio Projects\\Hough\\outputPics\\Book2_Corners.bmp", ios::binary);
	for(int i=0;i<54;i++)
		foutCorner<<h[i];

	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			if((borderArray[i][j+7]==(unsigned char)255&&borderArray[i-7][j-7]==(unsigned char)255)||(borderArray[i][j+7]==(unsigned char)255&&borderArray[i-7][j-6]==(unsigned char)255)||(borderArray[i][j+7]==(unsigned char)255&&borderArray[i-7][j-5]==(unsigned char)255)||(borderArray[i][j+7]==(unsigned char)255&&borderArray[i-7][j-4]==(unsigned char)255)||(borderArray[i][j+7]==(unsigned char)255&&borderArray[i-7][j-3]==(unsigned char)255)||(borderArray[i][j+7]==(unsigned char)255&&borderArray[i-7][j-2]==(unsigned char)255)||(borderArray[i][j+7]==(unsigned char)255&&borderArray[i-7][j-1]==(unsigned char)255)||(borderArray[i][j+7]==(unsigned char)255&&borderArray[i-7][j]==(unsigned char)255)){
				cornerArray[i][j]=(unsigned char)255;
				cout<<"found";
			}
			if((borderArray[i][j-7]==(unsigned char)255&&borderArray[i-7][j+7]==(unsigned char)255)||(borderArray[i][j-7]==(unsigned char)255&&borderArray[i-7][j+6]==(unsigned char)255)||(borderArray[i][j-7]==(unsigned char)255&&borderArray[i-7][j+5]==(unsigned char)255)||(borderArray[i][j-7]==(unsigned char)255&&borderArray[i-7][j+4]==(unsigned char)255)||(borderArray[i][j-7]==(unsigned char)255&&borderArray[i-7][j+3]==(unsigned char)255)||(borderArray[i][j-7]==(unsigned char)255&&borderArray[i-7][j+2]==(unsigned char)255)||(borderArray[i][j-7]==(unsigned char)255&&borderArray[i-7][j+1]==(unsigned char)255)||(borderArray[i][j-7]==(unsigned char)255&&borderArray[i-7][j]==(unsigned char)255)){
				cornerArray[i][j]=(unsigned char)255;
				cout<<"found1";
			}
		}
	}
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			foutCorner<<cornerArray[i][j]<<cornerArray[i][j]<<cornerArray[i][j];
		}
	}

	foutCorner.close();



	return 0;
}