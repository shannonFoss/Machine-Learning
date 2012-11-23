#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

int const countries=195;
int const features=2;
int const noClusters = 10;
float const threshold =.5;
string names[countries]={ "Afghanistan", "Albania", "Algeria", "Andorra", "Angola", "AntiguaBarbuda", "Argentina", "Armenia", "Australia", "Austria", "Azerbaljan",//11
	"Bahamas", "Bahrain", "Bangladesh", "Barbados", "Belarus", "Belgium", "Belize", "Benin", "Bhutan", "Bolivia", "BosniaHerzegovina", "Botswana", "Brazil", "Brunei", "Bulgaria", "BurkinaFaso", "Burundi",//17
	"Cambodia", "Cameroon", "Canada", "CapeVerde", "CentralAfricanRepublic", "Chad", "Chile", "China", "Columbia", "Comoros", "CongoDem", "CongoRep", "CostaRica", "CoteDIvoire", "Croatia", "Cuba", "Cyprus", "CzechRepublic",//18
	"Denmark", "Djibouti", "Dominica", "DominicanRepublic",//4
	"Ecuador", "Egypt", "ElSalvador", "EquatorialGuinea", "Eritrea", "Estonia", "Ethiopia",//7
	"Figi", "Finland", "France",//3
	"Gabon", "Gambia", "Georgia", "Germany", "Ghana", "Greece", "Grenada", "Guatemala", "Guinea", "GuineaBissau", "Guyana", //11
	"Hati", "Honduras", "Hungary",//3
	"Iceland", "India", "Indonesia", "Iran", "Iraq", "Ireland", "Israel", "Italy",//8
	"Jamaica", "Japan", "Jordan", //3
	"Kazakhstan", "Kenya", "Kiribati", "NorthKorea", "SouthKorea", "Kosovo", "Kuwait", "Kyrgyzstan",//8
	"Laos", "Latvia", "Lebanon", "Lesotho", "Liberia", "Libya", "Liechtenstein", "Lithuania", "Luxembourg", //9
	"Macedonia", "Madagascar", "Malawi", "Malaysia", "Maldives", "Mali", "Malta", "MarshallIslands", "Mauritania", "Mauritius", "Mexico", "Micronesia", "Moldova", "Monaco", "Mongolia", "Montenegro", "Morocco", "Mozambique", "Myanmar",//19
	"Namibia", "Nauru", "Nepal", "Netherlands", "NewZealand", "Nicaragua", "Niger", "Nigeria", "Norway",//9
	"Oman",//1
	"Pakistan", "Palau", "Panama", "PapuaNewGuinea", "Paraguay", "Peru", "Phillippines", "Poland", "Portugal", //9
	"Qatar", //1
	"Romania", "Russia", "Rwanda",//3
	"StKittsNevis", "StLucia", "StVincent", "Samoa", "SanMarino", "SaoTomePrincipe", "SaudiArabia", "Senegal", "Serbia", "Seychellies", "SierraLeone", "Singapore", "Slovakia", "Slovenia", "SolomonIslands", "Somalia", "SouthAfrica", "Spain", "SriLanka", "Sudan", "Suriname", "Swaziland", "Sweden", "Switzerland", "Syria",//25
	"Taiwan", "Tajikistan", "Tanzania", "Thailand", "TimorLeste", "Togo", "Tonga", "TrinidadTobago", "Tunisia", "Turkey", "Turkmenistan", "Tuvalu", //12
	"Uganda", "Ukraine", "UAE", "UK", "US", "Uruguay", "Uzbekistan", //7
	"Vanuatu", "VaticanCity", "Venezuela", "Vietnam", //4
	"Yemen",//1
	"Zambia", "Zimbabwe"//2
};
void findMax(float data[countries][features], float maxs[features]);
//finds maximum country value for each feature and stores it in maxs
void findMin(float data[countries][features], float mins[features]);
//finds minimum country value for each feature and stores it in mins
void findEuclid(float normal[countries][features], float eucl[countries][countries]);
//finds euclidian distances between each country
void bsas(float normal[countries][features]);
//http://www.cs.uu.nl/docs/vakken/pr/07-08/saarloos1.pdf
void gas(float normal[countries][features]);
//http://fconyx.ncifcrf.gov/lukeb/agclust.html
void kmeansClear(float normal[countries][features]);
void kmeans(float normal[countries][features]);
//http://fourier.eng.hmc.edu/e161/lectures/classification/node11.html
class Cluster{
public:
	Cluster();//constructor
	float min;			//keeps the min dist of the cluster
	int minimum;		//keeps the min cluster id
	int cntrys[countries];	//array of countries in the cluster
	float vect[features];   //mean vector
	int size;				//number of countries in the cluster
};
Cluster::Cluster(){
	minimum = -1;
	min =0.0;
	size=0;
	for(int i=0;i<features;i++){
		vect[i]=0.0;
	}
}

//int main (int argc, char * const argv[]) {
int main(){
	// array[rows][cols]
	float data[countries][features];
	float normal[countries][features];
	//float euclid[countries][countries];
	float maxs[features];
	float mins[features];
	
	ifstream ctryfile("data2.txt");//input file
	//inserting data from file
	if(ctryfile.is_open()){
		for(int i=0;i<countries;i++){
			for(int j=0;j<features;j++){
				ctryfile>> data[i][j];
			}
		}
		ctryfile.close();
		
		//find maxs and mins for normalizing
		findMax(data, maxs);
		findMin(data, mins);
		for(int i=0;i<countries;i++){
			for(int j=0;j<features;j++){
				normal[i][j] = ((data[i][j]-mins[j])/(maxs[j]-mins[j]));
				//cout<<normal[i][j]<<"\t";
			}
			//cout<<endl;
		}
		
		//fill euclidian array
		//findEuclid(normal, euclid);
		
		//clustering
		//bsas(normal);
		//gas(normal);
		//kmeansClear(normal);
		//kmeans(normal);
		
		//for(int i=0;i<countries;i++){
		//	for(int j=0;j<features;j++){
		//		cout<< normal[i][j]<<"\t";
		//	}
		//	cout<<endl;
		//}
		
	}
	else
		cout<< "unable to open file"<<endl;
	
    return 0;
}

void findMax(float data[countries][features], float maxs[features]){
	
	float tempMax=0;
	for(int i=0;i<features;i++){
		for(int j=0;j<countries;j++){
			if(tempMax < data[j][i]){
				tempMax = data[j][i];
			}
		}
		maxs[i]=tempMax;
		tempMax=0;
	}
}
void findMin(float data[countries][features], float mins[features]){
	float tempMin=200000000000000.0;//largest value was 14 trillion	
	for(int i=0;i<features;i++){
		for(int j=0;j<countries;j++){
			if(tempMin > data[j][i]){
				tempMin = data[j][i];
			}
		}
		mins[i]=tempMin;
		tempMin=200000000000000.0;
	}
	
}
void findEuclid(float normal[countries][features], float eucl[countries][countries]){
	float sum=0.0;
	float diff=0.0;
	for(int i=0; i<countries;i++){
		for(int k=0;k<countries;k++){
			if(i==k)
				eucl[i][k]=0.0;
			else{
				for(int j=0;j<features;j++){
					diff= normal[i][j]-normal[k][j];
					sum= sum+(diff*diff);
				}
				eucl[i][k] = sqrt(sum);
				sum=0.0;
			}
		}
	}
}
void bsas(float normal[countries][features]){
	Cluster clusters[countries];
	int numClust=0;
	float sum=0.0;
	float diff=0.0;
	float minimum=100.0;
	int minHolder=-1;
	
	for(int i=0;i<countries;i++){
		if(i==0){//seeding the first cluster
			clusters[numClust].cntrys[i]=i;
			clusters[numClust].size++;
			for(int j=0;j<features;j++){
				clusters[numClust].vect[j]=normal[i][j];
			}
			numClust++;
		}
		else{
			for(int x=0;x<numClust;x++){
				for(int j=0;j<features;j++){
					//dist betw cluster and point
					diff= clusters[x].vect[j] - normal[i][j];
					sum = sum +(diff* diff);
				}
				clusters[x].min=sqrt(sum);
				sum = 0.0;
			}
			for(int x=0;x<numClust;x++){
				if(clusters[x].min < minimum && clusters[x].min < threshold){
					minimum = clusters[x].min;
					minHolder = x;//cluster to be assigned to
				}
				minimum =100.0;
			}
			if(minHolder < 0){//make new cluster
				clusters[numClust].cntrys[clusters[numClust].size] = i;
				clusters[numClust].size++;
				for(int j=0;j<features;j++){
					clusters[numClust].vect[j] = normal[i][j];
				}
				numClust++;
			}else{//put in x cluster
				clusters[minHolder].cntrys[clusters[minHolder].size] = i;
				clusters[minHolder].size++;
				for(int j=0;j<features;j++){
					clusters[numClust].vect[j] += normal[i][j];
					clusters[numClust].vect[j] /= 2;
				}
			}
			minHolder=-1;
		}
	}
	//display bsas
	for(int i=0;i<numClust;i++){
		cout<< "Cluster #"<<i<<":"<<endl;
		for(int j=0;j<clusters[i].size;j++){
			cout<< names[clusters[i].cntrys[j]]<<"\t";
			for(int k=0;k<features;k++){
				cout<<normal[clusters[i].cntrys[j]][k]<<"\t";
			}
			cout<<endl;
		}
		cout<<endl;
	}
}

void gas(float normal[countries][features]){
	Cluster clusters[countries];
	int numClust=0;
	float locMin=100.0, globMin=100.0;
	int x=0, y=0;
	float sum=0.0, diff=0.0;
	bool cont =true;
	
	for(int i=0;i<countries;i++){//initializing clusters
		clusters[i].cntrys[clusters[i].size] = i;
		clusters[i].size++;
		for(int j=0;j<features;j++){
			clusters[i].vect[j]= normal[i][j];
		}
		numClust++;
	}
	while(numClust>0 && cont==true){
		for(int i=0;i<countries;i++){//finding minimum distance
			for(int j=0;j<countries;j++){
				if(i!=j && clusters[i].size>0 && clusters[j].size>0){//make sure not finding distance with itself
					for(int k=0;k<features;k++){
						diff= normal[i][k] - normal[j][k];
						sum = sum + (diff*diff);
					}
					if(sqrt(sum)<locMin){
						locMin=sqrt(sum);
						y=j;
					}
					sum = 0.0;
				}
			}
			if(locMin<globMin){
				globMin	=locMin;
				x=i;
			}			
			clusters[i].minimum=y;
			clusters[i].min = locMin;
			locMin = 100.0;
		}
		globMin = 100.0;
		if(clusters[x].min < threshold){
			for(int i=0;i<clusters[clusters[x].minimum].size;i++){//add clusters countries to the new cluster
				clusters[x].cntrys[clusters[x].size]=clusters[clusters[x].minimum].cntrys[i];
				clusters[x].size++;
				for(int j=0;j<features;j++){
					clusters[x].vect[j] += clusters[clusters[x].minimum].vect[j];
					clusters[x].vect[j] /= 2;
				}
			}
			clusters[clusters[x].minimum].size=0;
			numClust--;
		}else{
			cont=false;
		}
	}//end while
	//display gas
	for(int i=0;i<countries;i++){
		if(clusters[i].size>0){
			cout<< "Cluster #"<<i<<":"<<endl;
			for(int j=0;j<clusters[i].size;j++){
				cout<< names[clusters[i].cntrys[j]]<<"\t";
				for(int k=0;k<features;k++){
					cout<<normal[clusters[i].cntrys[j]][k]<<"\t";
				}
				cout<<endl;
			}
			cout<<endl;
		}
	}
}

void kmeansClear(float normal[countries][features]){
	Cluster clusters[noClusters];
	float means[noClusters][features];
	float oldMeans[noClusters][features];
	bool cont =true;
	int randNum[noClusters];
	float sum=0.0, diff=0.0, test=0.0;	
	float locMin=100.0;
	int count=0, y=0;
	srand((unsigned)time(0));
	
	for(int i=0;i<noClusters;i++){//pick random points as seeds
		randNum[i]= rand()%countries;//0-194
	}
	for(int i=0;i<noClusters;i++){
		for(int j=0;j<features;j++){
			means[i][j]= normal[randNum[i]][j];
		}
	}
	
	while(cont==true && count<1000){
		for(int i=0;i<noClusters;i++){
			for(int j=0;j<features;j++){
				oldMeans[i][j]=means[i][j];
			}
		}
		for(int i=0;i<countries;i++){//finding minimum distance btwn points and means
			for(int j=0;j<noClusters;j++){
				for(int k=0;k<features;k++){
					diff= means[j][k] - normal[i][k];
					sum = sum + (diff*diff);
				}
				if(sqrt(sum)<locMin){
					locMin=sqrt(sum);
					y=j;
				}
				sum = 0.0;
			}
			locMin=100.0;
			clusters[y].cntrys[clusters[y].size]= i;//assign country to cluster
			clusters[y].size++;
			for(int k=0;k<features;k++){
				clusters[y].vect[k]+=normal[i][k];
				clusters[y].vect[k]/=2;
			}
		}
		for(int i=0;i<noClusters;i++){//make new mean vector from clusters
			for(int j=0;j<features;j++){
				means[i][j] = clusters[i].vect[j];
				diff = oldMeans[i][j]-means[i][j];
				sum = sum+(diff*diff);
			}
			if(sqrt(sum)>test){
				test=sqrt(sum);
			}
			sum=0.0;
		}
		if(test>threshold && count <999){
			for(int i=0;i<noClusters;i++){
				clusters[i].size=0;
				for(int j=0;j<features;j++){
					clusters[i].vect[j]=0.0;
				}
			}
		}else{
			cont=false;
		}
		test=0.0;
		count++;
	}//end while
	//display kmeans
	for(int i=0;i<noClusters;i++){
		cout<< "Cluster #"<<i<<":"<<endl;
		for(int j=0;j<clusters[i].size;j++){
			cout<< names[clusters[i].cntrys[j]]<<"\t";
			for(int k=0;k<features;k++){
				cout<<normal[clusters[i].cntrys[j]][k]<<"\t";
			}
			cout<<endl;
		}
		cout<<endl;
	}
}

void kmeans(float normal[countries][features]){
	Cluster clusters[noClusters];
	float means[noClusters][features];
	float oldMeans[noClusters][features];
	bool cont =true;
	int randNum[noClusters];
	int c[countries];
	float sum=0.0, diff=0.0, test=0.0;	
	float locMin=100.0;
	int count=0, y=0, tempRand;
	
	srand((unsigned)time(0));
	
	for(int i=0;i<noClusters;i++){//pick random points as seeds
		randNum[i]= rand()%countries;//0-194
	}
	for(int i=0;i<noClusters;i++){
		for(int j=0;j<features;j++){
			means[i][j]= normal[randNum[i]][j];
		}
	}
	
	while(cont==true && count<1000){
		for(int i=0;i<noClusters;i++){
			for(int j=0;j<features;j++){
				oldMeans[i][j]=means[i][j];
			}
		}
		for(int i=0;i<countries;i++){//finding minimum distance btwn points and means
			for(int j=0;j<noClusters;j++){
				for(int k=0;k<features;k++){
					diff= means[j][k] - normal[i][k];
					sum = sum + (diff*diff);
				}
				if(sqrt(sum)<locMin){
					locMin=sqrt(sum);
					y=j;
				}
				sum = 0.0;
			}
			locMin=100.0;
			c[i]=y;
			clusters[y].cntrys[clusters[y].size]= i;//assign country to cluster
			clusters[y].size++;
			for(int k=0;k<features;k++){
				clusters[y].vect[k]+=normal[i][k];
				clusters[y].vect[k]/=2;
			}
		}
		for(int i=0;i<noClusters;i++){
			if(clusters[i].size==0){//if cluster is empty, assign it a new country
				tempRand= rand()%countries;
				while(clusters[c[tempRand]].size<2)
					tempRand= rand()%countries;
				clusters[i].cntrys[clusters[i].size]=tempRand;
				clusters[i].size++;
				for(int k=0;k<features;k++){
					clusters[i].vect[k]+=normal[i][k];
				}
				for(int j=0;j<clusters[c[tempRand]].size;j++){
					if(clusters[c[tempRand]].cntrys[j]==tempRand)
					   y=j;
				}
				for(int j=y;j<clusters[c[tempRand]].size-1;j++){//remove from old loc
					clusters[c[tempRand]].cntrys[j]=clusters[c[tempRand]].cntrys[j+1];
				}
				clusters[c[tempRand]].size--;
				for(int k=0;k<features;k++){
					clusters[c[tempRand]].vect[k]*=2;
					clusters[c[tempRand]].vect[k]-=normal[tempRand][k];
				}
			}
		}
		for(int i=0;i<noClusters;i++){//make new mean vector from clusters
			for(int j=0;j<features;j++){
				means[i][j] = clusters[i].vect[j];
				diff = oldMeans[i][j]-means[i][j];
				sum = sum+(diff*diff);
			}
			if(sqrt(sum)>test){
				test=sqrt(sum);
			}
			sum=0.0;
		}
		if(test>threshold && count <999){
			for(int i=0;i<noClusters;i++){
				clusters[i].size=0;
				for(int j=0;j<features;j++){
					clusters[i].vect[j]=0.0;
				}
			}
		}else{
			cont=false;
		}
		test=0.0;
		count++;
	}//end while
	//display kmeans
	for(int i=0;i<noClusters;i++){
		cout<< "Cluster #"<<i<<":"<<endl;
		for(int j=0;j<clusters[i].size;j++){
			cout<< names[clusters[i].cntrys[j]]<<"\t";
			for(int k=0;k<features;k++){
				cout<<normal[clusters[i].cntrys[j]][k]<<"\t";
			}
			cout<<endl;
		}
		cout<<endl;
	}
}