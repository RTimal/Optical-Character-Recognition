// Otsu.cpp : Defines the entry point for the console application.
//Rajiev Timal Project 2 Computer Vision . Due 12/23/2011

#include "stdafx.h"
#include <iostream>
#include "CImg.h"//image processing library Cimg
#include <limits>
#include <map>
#include <algorithm>
#include <string>
#include <fstream>

using namespace std;
using namespace cimg_library;//namespace for image processing library 

//specify samplefilename here
const char* samples = "images/sample digits.bmp";
const char* samples2= "images/sample digits 4-9.bmp";
//Below are the names of the images to process
const char* image1 = "images/IMG_1.bmp";
const char* image2 = "images/IMG_2.bmp";
const char* image3 = "images/IMG_3.bmp";
const char* image4 = "images/IMG_4.bmp";
const char* image5 = "images/IMG_5.bmp";
const char* image6 = "images/IMG_6.bmp";
const char* image7 = "images/IMG_7.bmp";
const char* image8 = "images/IMG_8.bmp";
const char* image9 = "images/IMG_9.bmp";

int imagecounter=0;

//this is a struct to hold the boundingbox of a character, including characteristics of the bounding box and the properties
//of the number stored within the bounding box.
struct Boundingbox{
	int maxx; //max x coordinate
	int minx;//min x coordinate
	int maxy; //max y
	int miny; //min y
	int area;//area of box
	int areacharacter;//area of character within box
	float averager;//average r, used in rbar, cbar,  the centroid - normalized for height/width of box
	float averagec;//average c - normalized for height/width of box
	//featurevector[0] is the row moment of the character
	//featurevector[1] is the column moment of the character
	//featurevector[2] is the mixed moment of the character
	float featurevector[4];
	string c;
};

//map with bounding boxes and feature vectors
//each map corresponds to each image, each member of the map is a boundingbox containing character information
map<int, Boundingbox> samplesboxes;
map<int, Boundingbox> samples2boxes;
map<int, Boundingbox> image1boxes;
map<int, Boundingbox> image2boxes;
map<int, Boundingbox> image3boxes;
map<int, Boundingbox> image4boxes;
map<int, Boundingbox> image5boxes;
map<int, Boundingbox> image6boxes;
map<int, Boundingbox> image7boxes;
map<int, Boundingbox> image8boxes;
map<int, Boundingbox> image9boxes;

//Holds max and min values for the current character
int bounds[4];

//converts to black and white, accepts a manual threshhold t
void binarize(CImg<unsigned int>  *image,int t);

//gets bounding box coordinates of characters
int getBoundingBoxes(CImg<unsigned int>  *image,map<int, Boundingbox>& boundingboxes);

//fills in each character, returning the min,max x and y values for valid characters
void floodfill(int j, int i,int bounds[],int **picturemap);

//fills in the feature vector array with the features that i use
void extractFeatures(CImg<unsigned int>  *image, int numboxes,map<int, Boundingbox>& boundingboxes);

//remove areas that are too small to be a character from the actual image, leaving only the connected character in the bounding box, this is done when the bounding box is calculated
//therefore, the coordinates that the boundingbox returns contains the connected character and nothing else
void removeartifacts(CImg<unsigned int> *image,Boundingbox box);

//get centroid info, normalized with width and height of bounding box, used to calculate moments
void getcentroidinfo(CImg<unsigned int>  *image, int numboxes,map<int, Boundingbox>& boundingboxes);

//minimum distance classifier, outputs to classes.txt
void classifyandoutput(CImg<unsigned int>  *image, int numboxes,map<int, Boundingbox>& boundingboxes,map<int, Boundingbox>& sampleboxes,map<int, Boundingbox>& sampleboxes2);

//main function
//*-----------------------------------------------------------------------------------------------------------------
void main()
{	//get boundingboxes and features for the samples, later I will compare the samples to the other ones.
	CImg<unsigned int>  *image = new CImg<unsigned int>(samples);//initialize point to sample image
		cout << "\n";
	cout << "Processing sample digits.bmp -binarizing and getting bounding boxes";
	cout << "\n";
	binarize(image,50);//binarize sample image
	int samplenumboxes = getBoundingBoxes(image,samplesboxes);
	//set characters for the sample images, this is used later when other images are compared to the sample, the minimum distance character takes on the value in sampleboxes[n].c
	samplesboxes[0].c="0";
	samplesboxes[1].c="3";
	samplesboxes[2].c="4";
	samplesboxes[3].c="1";
	samplesboxes[4].c="2";
	//get centroid, store in bounding box
	getcentroidinfo(image,samplenumboxes,samplesboxes);
	//extract features, store in bounding box
	extractFeatures(image,samplenumboxes,samplesboxes);
					//cout << "rowmoment: " << samplesboxes[3].featurevector[1];
					//cout<< "\n";
	image->save("images/samplebinary.bmp");//save binary image
	//image->display();
	///Get bounding boxes for images, startign with samples
	//*****************************************
	/*image->load(samples2);
	cout << "\n";
	cout << "Processing sample digits 4-9.bmp - binarizing and getting bounding boxes";
	cout << "\n";
	binarize(image,50);//binarize sample image
	int sample2numboxes = getBoundingBoxes(image,samples2boxes);
	//set characters for the sample images, this is used later when other images are compared to the sample, the minimum distance character takes on the value in sampleboxes[n].c
	samples2boxes[0].c="5";
	samples2boxes[1].c="8";
	samples2boxes[2].c="9";
	samples2boxes[3].c="6";
	samples2boxes[4].c="7";
	//get centroid, store in bounding box
	getcentroidinfo(image,sample2numboxes,samples2boxes);
	//extract features, store in bounding box
	extractFeatures(image,sample2numboxes,samples2boxes);
					//cout << "rowmoment: " << samplesboxes[3].featurevector[1];
					//cout<< "\n";
	image->save("images/sample2binary.bmp");//save binary image
	//image->display();
	///Get bounding boxes for images, startign with samples
	*/
	//*****************************************
		cout << "\n";
	cout << "Processing img_1.bmp - Binarizing, Getting Bounding Boxes, Extracting Features";
	cout << "\n";
	image->load(image1);
	binarize(image,50);
	int image1numboxes=getBoundingBoxes(image,image1boxes);
	getcentroidinfo(image,image1numboxes,image1boxes);
	extractFeatures(image,image1numboxes,image1boxes);
	classifyandoutput(image,image1numboxes,image1boxes,samplesboxes,samples2boxes);
	//cout << "rowmoment: " << image1boxes[3].featurevector[1];
	//cout<< "\n";
	//cout << "mixedmoment: " << image1boxes[0].featurevector[1];
	cout<< "\n";
	//cout << "columnmoment: " << image1boxes[0].featurevector[2];
	image->save("images/img_1binary.bmp");
	//image->display();
//*****************************************
		cout << "\n";
	cout << "Processing img_2.bmp - Binarizing, Getting Bounding Boxes, Extracting Features";
	cout << "\n";
	image->load(image2);
	binarize(image,70);
	int image2numboxes=getBoundingBoxes(image,image2boxes);
	getcentroidinfo(image,image2numboxes,image2boxes);
	extractFeatures(image,image2numboxes,image2boxes);
	classifyandoutput(image,image2numboxes,image2boxes,samplesboxes,samples2boxes);
		//cout << "rowmoment: " << image2boxes[0].featurevector[1];
	//cout<< "\n";
	//cout << "mixedmoment: " << image2boxes[4].featurevector[1];
	cout<< "\n";
	//cout << "columnmoment: " << image2boxes[4].featurevector[2];
	image->save("images/img_2binary.bmp");
	//image->display();
	//*****************************************	
		cout << "\n";
	cout << "Processing img_3.bmp - Binarizing, Getting Bounding Boxes, Extracting Features";
	cout << "\n";
	image->load(image3);
	binarize(image,70);
	int image3numboxes=getBoundingBoxes(image,image3boxes);
	getcentroidinfo(image,image3numboxes,image3boxes);
	extractFeatures(image,image3numboxes,image3boxes);
	classifyandoutput(image,image3numboxes,image3boxes,samplesboxes,samples2boxes);
	image->save("images/img_3binary.bmp");
	//image->display();
	//*****************************************
		cout << "\n";
	cout << "Processing img_4.bmp - Binarizing, Getting Bounding Boxes, Extracting Features";
	cout << "\n";
	image->load(image4);
	binarize(image,70);
	int image4numboxes=getBoundingBoxes(image,image4boxes);
	getcentroidinfo(image,image4numboxes,image4boxes);
	extractFeatures(image,image4numboxes,image4boxes);
	classifyandoutput(image,image4numboxes,image4boxes,samplesboxes,samples2boxes);
	image->save("images/img_4binary.bmp");
	//image->display();
	//*****************************************
		cout << "\n";
	cout << "Processing img_5.bmp - Binarizing, Getting Bounding Boxes, Extracting Features";
	cout << "\n";
	image->load(image5);
	binarize(image,50);
	int image5numboxes=getBoundingBoxes(image,image5boxes);
	getcentroidinfo(image,image5numboxes,image5boxes);
	extractFeatures(image,image5numboxes,image5boxes);
		classifyandoutput(image,image5numboxes,image5boxes,samplesboxes,samples2boxes);
	image->save("images/img_5binary.bmp");
	//image->display();
	//****************************************
		cout << "\n";
	cout << "Processing img_6.bmp - Binarizing, Getting Bounding Boxes, Extracting Features";
	cout << "\n";
	image->load(image6);
	binarize(image,50);
	int image6numboxes=getBoundingBoxes(image,image6boxes);
	getcentroidinfo(image,image6numboxes,image6boxes);
	extractFeatures(image,image6numboxes,image6boxes);
	classifyandoutput(image,image6numboxes,image6boxes,samplesboxes,samples2boxes);
	image->save("images/img_6binary.bmp");
	//image->display();
	//Te following commented out is for the extra credit part which I may hand in if I get to work properly.
	/*
	//*****************************************
	cout << "Processing img_7.bmp -Binarizing, Getting Bounding Boxes, Extracting Features";
	cout << "\n";
	image->load(image7);
	binarize(image,50);
	int image7numboxes=getBoundingBoxes(image,image7boxes);
	getcentroidinfo(image,image7numboxes,image7boxes);
	extractFeatures(image,image7numboxes,image7boxes);
	image->save("images/img_7binary.bmp");
	image->display();
	******************************************/
	/*cout << "Processing img_8.bmp - Binarizing, Getting Bounding Boxes, Extracting Features";
	cout << "\n";
	image->load(image8);
	binarize(image,50);
	int image8numboxes=getBoundingBoxes(image,image8boxes);
	getcentroidinfo(image,image8numboxes,image8boxes);
	extractFeatures(image,image8numboxes,image8boxes);
	classifyandoutput(image,image8numboxes,image8boxes,samplesboxes,samples2boxes);
	image->save("images/img_8binary.bmp");
	image->display();
	//*****************************************
	cout << "Processing img_9.bmp - Binarizing, Getting Bounding Boxes, Extracting Features";
	cout << "\n";
	image->load(image9);
	binarize(image,50);
	int image9numboxes=getBoundingBoxes(image,image9boxes);
	//extractFeatures(image,image9numboxes,image9boxes);
	image->save("images/img_9binary.bmp");
	image->display();
		//******************************************/
	cout<< "Results saved in images/classes.txt, enter any number to terminate";
	int a;
	cin >>a;
}

//*-------------------------------------------------------------------------------------------------------
int getBoundingBoxes(CImg<unsigned int>  *image,map<int, Boundingbox>& boundingboxes){
	int numdigits = 0;
	int pixelvalue;
	int num=0;
	int bounds[4]={0,image->width(),0,image->height()};//maxx0,minx=512,maxy=0=,miny=512
	//allocate memory for connected component 2d array
	int **picturemap = (int **)malloc(image->width() * sizeof(int *));
	for(int i = 0; i < image->width(); i++)
		picturemap[i] = (int *)malloc(image->height() * sizeof(int));
	//create new array based on image, to be used with flood fill
	for (int j =0;j<image->width();j++){
		for(int i =0;i<image->height();i++){
			pixelvalue=image->atXY(j,i,0);
			if (pixelvalue==0){
				picturemap[j][i]=1;
			}
			else{
				picturemap[j][i]=0;
			}
		}
	}

//cant go to the edge of the image bcause cant check pixels over the edge, will get an error
for (int j =1;j<image->width()-1;j++){
	for(int i =1;i<image->height()-1;i++){
		if (picturemap[j][i]==1){
			//num++;
			//get bounds, then store them in the bounding box
			floodfill(j,i,bounds,picturemap);//bounds for the surrounding pixels will be compared 
			//cout << num <<": " << bounds[0]<< ","<< bounds[1] << "," << bounds[2]<< " "<<bounds[3];
			//cout << "\n";
			Boundingbox box;//create a bounding box object
			box.maxx=bounds[0];//set the parameters of this object
			box.minx=bounds[1];
			box.maxy=bounds[2];
			box.miny=bounds[3];
			box.area=(bounds[0]-bounds[1])*(bounds[2]-bounds[3]);
			//if the box is small, it means that theres not a chracter, so only store boxes with big areas. 
			if(box.area>=40){
				boundingboxes[num]=box;
		//	cout << num <<": " << box.maxx << ","<< box.minx << "," << box.maxy << " "<< box.miny << " area:"<< boundingboxes[num].area;
			//cout << "\n";
			num++;}else{
				//if the box is small, remove the pixels, they are not from the character, this will only leave the connected character in the box
			removeartifacts(image,box);	//remove extra pixels from this box.
			}//store these bounds in a data structure
			//reset bounds for next iteration
			bounds[0]=0;
			bounds[1]=image->width();
			bounds[2]=0;
			bounds[3]=image->height();
		}
	}
}

	return num;
}

void extractFeatures(CImg<unsigned int>  *image, int numboxes,map<int, Boundingbox>& boundingboxes){
	double rowmomentsum=0.0;
	double mixedmomentsum=0.0;
	double columnmomentsum=0.0;
	double rowmoment=0.0;
	double mixedmoment=0.0;
	double columnmoment=0.0;
	//for every box,extract the row moment, column moment, and mixed moment of its character and store it in the boundingbox struct
	//this is done using the formulas in the textbook. Also, the Euler number is calculated.
	for(int i = 0;i<=numboxes-1;i++){
		//second order row moment
		for(int j = boundingboxes[i].minx;j<=boundingboxes[i].maxx;j++){
				for(int k = boundingboxes[i].miny;k<=boundingboxes[i].maxy;k++){
					if(image->atXY(j,k,0)==0){
						double n1= (k-boundingboxes[i].miny);
						double n2= (boundingboxes[i].maxy-boundingboxes[i].miny);
						double r = n1/n2; //divided to normalize
						double averager=boundingboxes[i].averager;
						rowmomentsum+=(r-averager)*(r-averager);
					}
					}
				}

		rowmoment=rowmomentsum/boundingboxes[i].areacharacter;
		//store in featurevector for current bounding box
		boundingboxes[i].featurevector[0]=rowmoment;

		//second order column moment
			for(int j = boundingboxes[i].minx;j<=boundingboxes[i].maxx;j++){
				for(int k = boundingboxes[i].miny;k<=boundingboxes[i].maxy;k++){
							if(image->atXY(j,k,0)==0){
						double n1= (j-boundingboxes[i].minx);
						double n2= (boundingboxes[i].maxx-boundingboxes[i].minx);
						double c = n1/n2; //divided to normalize
						double averagec=boundingboxes[i].averagec;
						columnmomentsum+=(c-averagec)*(c-averagec);
					}
					}
				}

		columnmoment=columnmomentsum/boundingboxes[i].areacharacter;
		//store in featurevector for current bounding box
		boundingboxes[i].featurevector[1]=columnmoment;


		//second order mixed moment
			for(int j = boundingboxes[i].minx;j<=boundingboxes[i].maxx;j++){
				for(int k = boundingboxes[i].miny;k<=boundingboxes[i].maxy;k++){
							if(image->atXY(j,k,0)==0){
						double n1= (j-boundingboxes[i].minx);
						double n2= (boundingboxes[i].maxx-boundingboxes[i].minx);
						double c = n1/n2; //divided to normalize
						double n3= (k-boundingboxes[i].miny);
						double n4= (boundingboxes[i].maxy-boundingboxes[i].miny);
						double r = n3/n4; //divided to normalize
						double averagec=boundingboxes[i].averagec;
						double averager=boundingboxes[i].averager;
						mixedmomentsum+=(c-averagec)*(r-averager);
					}
				}
			}
			//find euler number. This is done by using a scanline that goes across the midline of the character. In numbers 0 and 4, the scanline will detect
			//a hole, so the euler number for 0 and 4 becomes 1-1 = 0, for the other numbers it will be 1. This is featurevector[3]
			int pixelcolor=0;
			int strokecount=0;
			int instroke=0;
				for(int j = boundingboxes[i].minx;j<=boundingboxes[i].maxx;j++){
					pixelcolor=image->atXY(j,boundingboxes[i].miny+(boundingboxes[i].maxy-boundingboxes[i].miny)/2,0);
					//pixelcolor=image->atXY(j,boundingboxes[i].miny+(boundingboxes[i].maxy-boundingboxes[i].miny)/2,1)=3;
					//pixelcolor=image->atXY(j,boundingboxes[i].miny+(boundingboxes[i].maxy-boundingboxes[i].miny)/2,2)=4;
					//image->atXY(j,boundingboxes[i].maxy/2,0)=7;
					if(pixelcolor==0&&instroke==0){instroke=1;}
					if(pixelcolor==255&&instroke==1){instroke=0;strokecount+=1;}
					if(pixelcolor==0&&j==boundingboxes[i].maxx&&instroke==1){instroke=0;strokecount+=1;}
			}

				if(strokecount==2){
					//euler number
						boundingboxes[i].featurevector[3]=0; //if the line drawn across intersects two strokes, there is a hole, so the euler number is 1-1=0
				}else

				{
					boundingboxes[i].featurevector[3]=1;//if it only intersects 1 stroke, the euler number is 1-0 = 0. There are no holes

				}

		mixedmoment=mixedmomentsum/boundingboxes[i].areacharacter;
		//store in featurevector for current bounding box
		boundingboxes[i].featurevector[2]=mixedmoment;
		//reset sums
		rowmomentsum=0.0;
		mixedmomentsum=0.0;
		columnmomentsum=0.0;
		rowmoment=0.0;
		mixedmoment=0.0;
		columnmoment=0.0;
	}
	return;
}

//this fills an entire character that it detects, setting traversed pixels to to  a number other than 1 so it wont go back to them, fills in an array  bounds [] with the max/min axis alignned bounding box
//int bounds[4]={0,image->width(),0,image->height()};//maxx0,minx=512,maxy=0=,miny=512 .. used above
void floodfill(int j, int i,int bounds[],int **picturemap){
if (picturemap[j][i]!=1){return;}
picturemap[j][i]=2;
if(j>bounds[0]){bounds[0]=j;}
if(j<bounds[1]){bounds[1]=j;}
if(i>bounds[2]){bounds[2]=i;}
if(i<bounds[3]){bounds[3]=i;};
floodfill(j,i-1,bounds,picturemap);//top
floodfill(j,i+1,bounds,picturemap);//bottom
floodfill(j-1,i,bounds,picturemap);//left
floodfill(j+1,i,bounds,picturemap);//right
floodfill(j+1,i-1,bounds,picturemap);//bottomright
floodfill(j-1,i-1,bounds,picturemap);//bottomleft
floodfill(j-1,i+1,bounds,picturemap);//topleft
floodfill(j+1,i+1,bounds,picturemap);//topright
return;
}

//binarize the image based on a manually entered threshold value, 50 or 70 worked well for all the images, Otsu's method did not provide good segmentation with these images
void binarize(CImg<unsigned int> *image,int t){
	int pixelvalue=0;
	for (int i =0;i<image->height();i++){
		for (int j =0;j<image->width();j++){
			pixelvalue=image->atXY(j,i,0);
			if (pixelvalue>t){//if pixelvalue is< than the threshhold, set it to 200
				pixelvalue=image->atXY(j,i,0)=255;
				pixelvalue=image->atXY(j,i,1)=255;
				pixelvalue=image->atXY(j,i,2)=255;}else
				{
				pixelvalue=image->atXY(j,i,0)=0;
				pixelvalue=image->atXY(j,i,1)=0;
				pixelvalue=image->atXY(j,i,2)=0;
				}
		}
	}
}

//remove pixels that are not numbers, based on size of the bounding box of these pixels
void removeartifacts(CImg<unsigned int> *image,Boundingbox box){
	for(int j = box.minx;j<=box.maxx;j++){
				for(int k = box.miny;k<=box.maxy;k++){
					image->atXY(j,k,0)=255;
					image->atXY(j,k,1)=255;
					image->atXY(j,k,2)=255;
				}
			}
}

//get the centroid of the CHARACTER , normalized for height and width of the bounding box, store in that characters bounding box
void getcentroidinfo(CImg<unsigned int>  *image, int numboxes,map<int, Boundingbox>& boundingboxes){
	float rsum =0;
	float csum=0;
	int area=0;
	for(int i = 0;i<=numboxes-1;i++){
			for(int j = boundingboxes[i].minx;j<=boundingboxes[i].maxx;j++){
				for(int k = boundingboxes[i].miny;k<=boundingboxes[i].maxy;k++){
					if(image->atXY(j,k,0)==0){
					area+=1;
					csum+=(j-boundingboxes[i].minx);
					rsum+=(k-boundingboxes[i].miny);
					}
				}
			}
			boundingboxes[i].areacharacter=area;
			boundingboxes[i].averager=(rsum/area)/(boundingboxes[i].maxy-boundingboxes[i].miny);
			boundingboxes[i].averagec=(csum/area)/(boundingboxes[i].maxx-boundingboxes[i].minx);
	}
}
//classify using a minimum distance classifier, output to textfile classes.txt
void classifyandoutput(CImg<unsigned int>  *image, int numboxes,map<int, Boundingbox>& boundingboxes,map<int, Boundingbox>& sampleboxes,map<int, Boundingbox>& sampleboxes2){
	double mindistance=200;
	//for all characters, compare each character to the samples and take the value of the sample character with the minimum distance from this character
	for(int j =0;j<=numboxes-1;j++){
		for (int i=0;i<=4;i++){
			double distance = sqrt(
				  (boundingboxes[j].featurevector[0]-sampleboxes[i].featurevector[0])*(boundingboxes[j].featurevector[0]-sampleboxes[i].featurevector[0]) 
		     	+ (boundingboxes[j].featurevector[1]-sampleboxes[i].featurevector[1])*(boundingboxes[j].featurevector[1]-sampleboxes[i].featurevector[1])
				+ (boundingboxes[j].featurevector[2]-sampleboxes[i].featurevector[2])*(boundingboxes[j].featurevector[2]-sampleboxes[i].featurevector[2])
				+ (boundingboxes[j].featurevector[3]-sampleboxes[i].featurevector[3])*(boundingboxes[j].featurevector[3]-sampleboxes[i].featurevector[3])
				);
			if(distance<mindistance){mindistance = distance;boundingboxes[j].c=sampleboxes[i].c;}
			//below will be used for the extra credit
			// distance = sqrt(
			//	  (boundingboxes[j].featurevector[0]-sampleboxes2[i].featurevector[0])*(boundingboxes[j].featurevector[0]-sampleboxes2[i].featurevector[0]) 
		    // 	+ (boundingboxes[j].featurevector[1]-sampleboxes2[i].featurevector[1])*(boundingboxes[j].featurevector[1]-sampleboxes2[i].featurevector[1])
			//	+ (boundingboxes[j].featurevector[2]-sampleboxes2[i].featurevector[2])*(boundingboxes[j].featurevector[2]-sampleboxes2[i].featurevector[2])
			//	+ (boundingboxes[j].featurevector[3]-sampleboxes2[i].featurevector[3])*(boundingboxes[j].featurevector[3]-sampleboxes2[i].featurevector[3])
			//	);
			//if(distance<mindistance){mindistance = distance;boundingboxes[j].c=samples2boxes[i].c;}

		}
		cout <<"\n";
		cout << boundingboxes[j].c;cout << "-"; cout << "distance from character: " << mindistance;
				cout <<"\n";
		mindistance=200;
	}

	//output the boundingbox locations and contents to the file images/classes.txt
		ofstream myfile;
  myfile.open ("images/classes.txt",ios::app);//open file
  if (imagecounter==0){
	  myfile << "\n";
	  myfile << "<--------------sample digits.bmp--------------------->";////output the r,c coordinates of the sample image to the file
	  myfile << "\n";
	  for (int i=0;i<=4;i++){
				myfile << "Number:" <<  samplesboxes[i].c;
		myfile<<"\n";
		myfile << "upper left r,c " << samplesboxes[i].miny<<","<<samplesboxes[i].minx;
		myfile<<"\n";
		myfile << "bottom right r,c " << samplesboxes[i].maxx <<","<<samplesboxes[i].maxy;
		myfile<<"\n";
		myfile<<"\n";
		}
  }
  imagecounter++;
	   myfile << "\n";
	  myfile<< "<---------------IMAGE_"<<imagecounter<<".bmp------------>";//output the r,c coordinates of the other images to the file
	   myfile << "\n";
	     	for(int j =0;j<=numboxes-1;j++){
		myfile << "Number:" <<  boundingboxes[j].c;
		myfile<<"\n";
		myfile << "upper left r,c " << boundingboxes[j].miny<<","<<boundingboxes[j].minx;
		myfile<<"\n";
		myfile << "bottom right r,c " << boundingboxes[j].maxx <<","<<boundingboxes[j].maxy;
		myfile<<"\n";
		myfile<<"\n";
	}
  myfile.close();
  return;
}