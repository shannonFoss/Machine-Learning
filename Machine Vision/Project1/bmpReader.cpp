#include <iostream>
#include <fstream> //file io
#include <stdlib.h>
#include <cstdio>
/*
[0][0] = lower left row = 0 col = 0
[1][0] = row = 1 col = 0 height
[0][2] = row = 0 col = 2 width
*/
using namespace std;
const int width=800, height= 600;
const int size= 128;
const int display=32;
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
int main(){
	char h[54];//header
	int grow=0;

	ifstream fin;
	fin.open("flowers.bmp", ios::binary);
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
	ofstream fout("flowers_grey.bmp", ios::binary);
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

	//histogram
	ofstream foutE("flowers_equalhisto.bmp", ios::binary);
	ofstream foutH("flowers_histograms.bmp", ios::binary);
	for(i=0;i<54;i++)
		foutE<<h[i];
	//393270 =60036
	h[2]=(char)54;h[3]=(char)0;h[4]=(char)6;h[5]=(char)0;
	h[18]=(char)0;h[19]=(char)1;h[20]=(char)0;h[21]=(char)0;
	h[22]=(char)0;h[23]=(char)2;h[24]=(char)0;h[25]=(char)0;
	for(i=0;i<54;i++)
		foutH<<h[i];

	float histo[256];
	float equal_histo[256];
	float histogram[256];
	for (i=0; i<256; i++){
		equal_histo[i]= 0.0; 
		histo[i] = 0.0;
	}
	for (int j=0;j<height;j++){
		for (i=0; i<width; i++){
			histo[(int)rgrey[i][j]] += 1.0;
		}
	}
		// scale histogram to [0, 255] for display purpose
		float gmin = 256;
		float gmax = -1;
		for (i=0; i<256; i++){
			if (histo[i]< gmin) gmin = histo[i];
			if (histo[i]> gmax) gmax = histo[i];
		}
		float gdelta = gmax - gmin;

		for (i=0; i <256; i++){
			// scale for display purposes
			histogram[i] = unsigned char ((histo[i] - gmin)*(255/gdelta));
			// compute the relative frequency histogram h’(n)
			histo[i] = histo[i]/(width*height);
		}
		
		// compute mapping f(n)
		// equal[] is the mapping function f(n)
		float hsum=0.0;
		float fequal[256];
		unsigned char equal[256];
		float fmax=0.0;
		for (i=0; i<256; i++){
			hsum = hsum + histo[i];
			fequal[i] = hsum*(256-1) + 0.5;
			if (fequal[i] > fmax) fmax = fequal[i];
			equal[i] = unsigned char (fequal[i]);

		}
		unsigned char equalized[height][width];
	// xd and yd are dimensions of original image
	// equalized[][] is the equalized image
	// Use of this temporary array is for illustration only, can use pointer, e.g., *indexr
	// equal_histo[] is the histogram array of the equalized image
		for (j=0;j<height;j++){
			for (i=0; i<width; i++){//rgbfix
				equalized[j][i] = equal[rgrey[j][i]];
				equal_histo[equalized[j][i]] +=1;
			}
		}
		// scale the equal_histo[] for display purpose
		gmin = 256;
		gmax = -1;
		for (i=0; i<256; i++){
			if (equal_histo[i]< gmin) gmin = equal_histo[i];
			if (equal_histo[i]> gmax) gmax = equal_histo[i];

		}
		// equal_histogram[] is the scaled histogram array
		gdelta = gmax - gmin;
		unsigned char equal_histogram[256];
		for (i=0; i <256; i++)
		        equal_histogram[i] = unsigned char ((equal_histo[i] - gmin)*(255)/gdelta);
		
		// combining the scaled histogram arrays to display.
		// histogram_image[][] is the image of the two histograms
		// use of this temporary array is for illustration only, can use pointer, e.g., *indexr
		unsigned char histogram_image[512][256];
		for (j = 0; j <512; j++){
		   for (i=0; i< 256; i++){
		       if (j < 256 ){
			// histogram of original image is displayed on top
					if (histogram[i] < (255-j)) histogram_image[j][i] = 255;
					else histogram_image[j][i] = 0;
		      }
		      else {
			// histogram of equalized image is displayed at bottom
					if (equal_histogram[i] < (511-j)) histogram_image[j][i] = 255;
					else 
						histogram_image[j][i] =0;
		      }
		   }
		}	
		for(int i=511;i>=0;i--){
			for(int j=0;j<256;j++){
                foutH<<histogram_image[i][j]<<histogram_image[i][j]<<histogram_image[i][j];
			}
		}
		for(int i=0;i<height;i++){
			for(int j=0;j<width;j++){
                foutE<<equalized[i][j]<<equalized[i][j]<<equalized[i][j];
			}
		}

		foutH.close();
		foutE.close();

	ofstream fout2("flowers_spatial.bmp", ios::binary);
	fillHeader(h);

	for(i=0;i<54;i++)
		fout2<<h[i];
	pluckSq(160,225);//(height,width)
	if(size!=display){
		grow=(int)display/size;
		grower(grow);
	}

	if(size!=display){
		for(int i=0;i<display;i++){
			for(int j=0;j<display;j++){
				fout2<<rspatial[i][j]<<gspatial[i][j]<<bspatial[i][j];
			}	
		}
	}else{
		for(int i=0;i<display;i++){
			for(int j=0;j<display;j++){
				fout2<<rsquare[i][j]<<gsquare[i][j]<<bsquare[i][j];
			}	
		}
	}

	fout2.close();

	return 0;
}