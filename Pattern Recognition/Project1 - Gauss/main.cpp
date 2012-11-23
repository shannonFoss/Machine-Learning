#include <iostream>
#include <iomanip>
#include <time.h>
#include <math.h>
#include <stdlib.h>

using namespace std;
const int PI = 3.14159;
class Point{
private:
	double x, y;
	
public:
	Point();
	double getx();
	double gety();
	void setx(double);
	void sety(double);
//	Point &operator=(Point &);
};

Point::Point(){
	x=0.0;
	y=0.0;
}
//Point &Point::operator=(Point &rhs){
//	x = rhs.x;
//	y = rhs.y;
//	return *this;
//}
double Point::getx(){
	return x;
}
double Point::gety(){
	return y;
}
void Point::setx(double input){
	x = input;
}
void Point::sety(double input){
	y = input;
}

float randomNumber();//creates random float numbers between 0 and 1
double gaussian(double, double);
Point boxMuller(Point, Point);//creates random points
void divyUp(Point [], Point [], Point [], Point [], int [], int []);//creates training and testing sets
void bayesian(Point, Point, Point [], int [], int [], Point, Point);
void minDist(Point, Point, Point [], int []);
void nearestNeighbor(int, Point [], Point [], int [], int []);
void hybrid(Point [], Point [], int [], int [], float, float);//hybrid classifier
void perceptron(Point [], Point [], int [], int []);
const int MAXARRAY = 20;

int main (int argc, char * const argv[]) {
	srand((unsigned)time(NULL));
	
	Point sigma1, sigma2;
	sigma1.setx(1.0); sigma1.sety(1.0);
	sigma2.setx(1.0); sigma2.sety(1.0);
	Point mean1, mean2;
	mean1.setx(0.0); mean1.sety(0.0);
	mean2.setx(1.0); mean2.sety(1.0);
	float avgSigx, avgSigy;
	avgSigx=(sigma1.getx()+sigma2.getx())/2;
	avgSigy=(sigma1.gety()+sigma2.gety())/2;
	float testBayes=0.0;
	int countBayes=0;
	Point p1[MAXARRAY], p2[MAXARRAY];
	Point training[MAXARRAY], testing[MAXARRAY];
	int tr[MAXARRAY], te[MAXARRAY], testSet[MAXARRAY];//classification array

	for(int i=0;i<MAXARRAY;i++){
		p1[i] = boxMuller(sigma1, mean1);
		p2[i] = boxMuller(sigma2, mean2);
		//cout<< p1[i].getx()<<","<<p1[i].gety()<<endl;
	}	

	divyUp(p1, p2, training, testing, tr, te);
	hybrid(training, testing, tr, testSet, avgSigx, avgSigy);
	//bayesian(mean1, mean2, testing, tr, testSet, sigma1, sigma2);
	//minDist(mean1, mean2, testing, testSet);
	//nearestNeighbor(7, training, testing, tr, testSet);
	//perceptron(training,testing, tr, testSet);
	for(int i=0;i<MAXARRAY;i++){
		if(te[i] == testSet[i]){
			countBayes++;
		}
	}
	testBayes = ((float)countBayes/MAXARRAY)*100.0;
	cout<< "Bayes ="<<testBayes<<"%"<<endl;
    return 0;
}

float randomNumber(){//generates random numbers
	//cout<<"Random number [0-99]: "<<rand()%100<<endl;
	//cout<<"Random number [1-10]: "<<(rand()%10)+1<<endl;
	//cout<<"Random number [10-20]: "<<rand()%(20-10+1)+10<<endl;//(upper-lower+1)+lower
	//cout<<"Random number [1-10]: "<<1+int((10-1+1)*rand()/(RAND_MAX +1.0))<<endl;//lower+int(((upper-lower)+1)*rand()/(RAND_MAX + 1.0)); 
	//cout<<"Rand [0-1]: "<<(float)rand()/RAND_MAX<<endl;
	float rand1;
	rand1=(float)rand()/RAND_MAX;
	return rand1;
}

double gaussian(double sig, double mn){
	double tmp, x1;
	float rand1;
	rand1 = randomNumber();
	tmp = (rand1 - mn)/sig;
	x1 = exp(-tmp * tmp/2);
	return x1;
}

Point boxMuller(Point sig, Point mn){
	float rand1, rand2;
	double radius, x,y;	
	Point p1;
	do{
		rand1 = randomNumber();
		rand2 = randomNumber();
		x = 2.0 * rand1 - 1.0;
		y = 2.0 * rand2 - 1.0;
		radius = x * x + y * y;
	}while(radius >= 1.0);
	radius = sqrt(log(radius)*(-2.0/radius));
	x *= radius;
	y *= radius;
	x = x *sig.getx() +mn.getx();
	y = y *sig.gety() +mn.gety();	
	p1.setx(x);
	p1.sety(y);
	return p1;
}

void divyUp(Point p1[MAXARRAY], Point p2[MAXARRAY], Point train[MAXARRAY], Point test[MAXARRAY], int tr1[MAXARRAY], int te1[MAXARRAY]){
	int rand1, rand2;
	for(int i=0;i<MAXARRAY;i++){//make better
		rand1 = rand()%2;
		if(rand1){//if 1 then put in training, else put in test
			rand2 = rand()%2;
			if(rand2){//if 1 then pull from p2
				train[i] = p2[i];
				tr1[i] = 2;				
				test[i] = p1[i];
				te1[i] = 1;
			}else{
				train[i] = p1[i];
				tr1[i] = 1;
				test[i] = p2[i];
				te1[i] = 2;
			}
		}else{
			rand2 - rand()%2;
			if(rand2){//if 1 then pull from p2
				test[i] = p2[i];
				te1[i] = 2;
				train[i] = p1[i];
				tr1[i] = 1;
			}else{
				test[i] = p1[i];
				te1[i] = 1;
				train[i] = p2[i];
				tr1[i] = 2;
			}
		}
	}
}

void bayesian(Point m1, Point m2, Point test[MAXARRAY], int tr1[MAXARRAY], int ts1[MAXARRAY], Point s1, Point s2 ){
	float res1, res2;
	float mul1, mul2;
	float inv1[2], inv2[2], temp1[2], temp2[2],temp3[2], temp4[2], temp5, temp6;
	int cCount1=0, cCount2=0;
	for(int i=0;i<MAXARRAY;i++){//counting all class points
		if(tr1[i]==1)
			cCount1++;
		else
			cCount2++;
	}
	mul1 = 1/(s1.getx()*s1.gety());
	mul2 = 1/(s2.getx()*s2.gety());
	inv1[0] = mul1*s1.gety(); inv1[1]= mul1*s1.getx();
	inv2[0] = mul2*s2.gety(); inv2[1]= mul2*s2.getx();
	
	for(int i=0;i<MAXARRAY;i++){
		temp1[0] = (test[i].getx()-m1.getx());
		temp1[1] = (test[i].gety()-m1.gety());
		temp2[0] = (test[i].getx()-m2.getx());
		temp2[1] = (test[i].gety()-m2.gety());
		temp3[0] = (temp1[0]*inv1[0]);
		temp3[1] = (temp1[1]*inv1[1]);
		temp4[0] = (temp2[0]*inv2[0]);
		temp4[1] = (temp2[1]*inv2[1]);
		temp5 = (temp3[0]*temp1[0] + temp3[1]*temp1[1]);
		temp6 = (temp4[0]*temp2[0] + temp4[1]*temp2[1]);
		res1 = -0.5*temp5 + (log(cCount1/cCount1+cCount2)) + (-1*log(2*PI)-(0.5*log(s1.getx()*s1.getx()*s1.gety()*s1.gety())));
		res2 = -0.5*temp6 + (log(cCount2/cCount1+cCount2)) + (-1*log(2*PI)-(0.5*log(s2.getx()*s2.getx()*s2.gety()*s2.gety())));
		if(res1>res2){
			ts1[i]=1;
		}else{
			ts1[i]=2;
		}
	}
}
void minDist(Point m1, Point m2, Point test[MAXARRAY], int ts1[MAXARRAY]){
	float temp1[2], temp2[2], temp3, temp4, res1, res2;
	
	for(int i=0;i<MAXARRAY;i++){
		temp1[0] = test[i].getx() - m1.getx();
		temp1[1] = test[i].gety() - m1.gety();
		temp2[0] = test[i].getx() - m2.getx();
		temp2[1] = test[i].gety() - m2.gety();
		temp3 = temp1[0]*temp1[0] + temp1[1]*temp1[1];
		temp4 = temp2[0]*temp2[0] + temp2[1]*temp2[1];
		res1 = sqrt(temp3);
		res2 = sqrt(temp4);
		if(res1<res2){
			ts1[i] = 1;
		}else{
			ts1[i] = 2;
		}
	}
	
}
void nearestNeighbor(int k, Point train[MAXARRAY], Point test[MAXARRAY], int tr1[MAXARRAY], int ts1[MAXARRAY]){
	float lowest[7] = {999,999,999,999,999,999,999};
	int track[7] = {0,0,0,0,0,0,0};
	float dist=0.0;
	int count1, count2;
	for(int i=0;i<MAXARRAY;i++){
		count1=0; count2=0;
		for(int j=0;j<MAXARRAY;j++){
			dist = sqrt(((test[i].getx()-train[j].getx())*(test[i].getx()-train[j].getx()))+((test[i].gety()-train[j].gety())*(test[i].gety()-train[j].gety())));
			int m=0;
			while(m<k && lowest[m]<dist){
				m++;
			}
			int n=k-1;
			while(n>m){
				lowest[n]=lowest[n-1];
				track[n] = track[n-1];
				n--;
			}
			lowest[m] = dist;
			track[m] = j;
		}
		for(int x=0;x<k;x++){
			if(tr1[track[x]]==1){
				count1++;
			}else{
				count2++;
			}
		}
		if(count1>count2){
			ts1[i] =1;
		}else{
			ts1[i] =2;
		}
		for(int y=0;y<7;y++){//reset
			lowest[y]=999;
			track[y]=0;
		}
	}
}


void hybrid(Point train[MAXARRAY], Point test[MAXARRAY], int tr1[MAXARRAY], int ts1[MAXARRAY], float avgSx, float avgSy){
	int cCount1=0, cCount2=0;//class count
	int tCount1=0, tCount2=0;//training count
	float prob1=0.0, prob2=0.0;
	for(int i=0;i<MAXARRAY;i++){//counting all class points
		if(tr1[i]==1)
			cCount1++;
		else
			cCount2++;
	}
	for(int i=0;i<MAXARRAY; i++){//finding points nearby
		tCount1=0;
		tCount2=0;
		prob1=0.0;
		prob2=0.0;
		for(int j=0;j<MAXARRAY;j++){//points in trainer
			if((train[j].getx() <= test[i].getx()+avgSx && train[j].getx() >= test[i].getx() -avgSx) && (train[j].gety() <= test[i].gety()+avgSy && train[j].gety() >= test[i].gety()-avgSy)){
				if(tr1[j]==1)//created a bounding box around test point
					tCount1++;//counts class members in box
				else
					tCount2++;				
			}
		}
		prob1 = ((float)cCount1/cCount1+cCount2) * ((float)tCount1/cCount1);
		prob2 = ((float)cCount2/cCount1+cCount2) * ((float)tCount2/cCount2);
		//cout<< prob1 <<"   "<<prob2<<endl;
		if(prob1>=prob2)//assigns a class to test points based on probability
			ts1[i] = 1;
		else 
			ts1[i] = 2;
	}
}

void perceptron(Point train[MAXARRAY], Point test[MAXARRAY], int tr1[MAXARRAY], int ts1[MAXARRAY]){
	float weight[3]={0,0,0};
	float temp[3]={0,0,0};
	float res1;
	int cl;
	float x[3];
	float rho=1.0;
	int counter=1;
	bool flag=true;
	cout<<"training..."<<endl;
	while(flag && counter <10000){
		flag=false;
		for(int i=0;i<MAXARRAY;i++){
			cl=0;
			x[0]= train[i].getx();
			x[1]= train[i].gety();
			x[2]= 1;
			
			temp[0]=weight[0]*x[0];
			temp[1]=weight[1]*x[1];
			temp[2]=weight[2]*x[2];
			res1=temp[0]+temp[1]+temp[2];
			
			if(res1>0){
				cl=1;
			}else if(res1<0){
				cl=2;
			}
			if(cl!=tr1[i]){
				flag=true;
				if(res1<=0 && tr1[i]==1){
					counter++;
					x[0] = x[0]*rho;
					x[1] = x[1]*rho;
					x[2] = x[2]*rho;
					
					weight[0] = weight[0]+x[0];
					weight[1] = weight[1]+x[1];
					weight[2] = weight[2]+x[2];
				}else if (res1>=0 && tr1[i]==2){
					counter++;
					x[0] = x[0]*-1*rho;
					x[1] = x[1]*-1*rho;
					x[2] = x[2]*-1*rho;
					
					weight[0] = weight[0]+x[0];
					weight[1] = weight[1]+x[1];
					weight[2] = weight[2]+x[2];
				}else
					cout<<"this exists?"<<endl;
			}
		}
	}//end w
	cout<<"testing..."<<endl;
	for(int j=0;j<MAXARRAY;j++){
		cl=0;
		x[0]=test[j].getx();
		x[1]=test[j].gety();
		x[2]=1;
		
		temp[0]= weight[0]*x[0];
		temp[1]= weight[1]*x[1];
		temp[2]= weight[2]*x[2];
		res1= temp[0]+temp[1]+temp[2];
		
		if(res1>0){
			cl=1;
		}else if(res1<0){
			cl=2;
		}
		ts1[j]=cl;
	}
}










