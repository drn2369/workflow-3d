/*
 * =====================================================================================
 *
 *       Filename:  Reader.cpp
 *
 *    Description:  Reader class
 *
 *        Version:  1.0
 *        Created:  05/01/2012 10:59:22 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  David Nilosek (), drn2369@cis.rit.edu
 *        Company:  Rochester Institute of Technology
 *
 * =====================================================================================
 */


#include	<fstream>
#include	<string>


#include	<cv.h>

#include	"IO.h"
#include	"utils.h"

// Base reader constructor, sets file path
IOInterface::IOInterface(const char* fileP){
	//Set the file path
	setFilePath(fileP);
}

// Inital match file reader constructor
IMatch_Reader::IMatch_Reader(const char* fileP,IntVecPair& fPair,
                             vector<int>& numM, vector<IntVecPair>& mList):IOInterface(fileP){
	readFile(fPair,numM,mList);
}

POS_Reader::POS_Reader(const char* fileP):IOInterface(fileP){
	readFile();
}

TXT_Reader::TXT_Reader(const char* fileP):IOInterface(fileP){
	readFile();
}
TXT_Writer::TXT_Writer(const char* fileP):IOInterface(fileP){
}

// Key file reader constructor
Key_Reader::Key_Reader(const char* fileP,vector<cv::Point2f>& featurePoints):IOInterface(fileP){
	readFile(featurePoints);
}
Key_Reader::Key_Reader(const char* fileP,vector<cv::Point2f>& featurePoints, double px, double py):IOInterface(fileP){
	readFile(featurePoints,px,py);
}
PLYPMVS_Reader::PLYPMVS_Reader(const char* fileP):IOInterface(fileP){
	readFile();
}

// PLY file writer constructor
PLY_Writer::PLY_Writer(const char* fileP):IOInterface(fileP){
};

// Inital match file reader
void IMatch_Reader::readFile(IntVecPair& fPair, vector<int>& numM, vector<IntVecPair>& mList){
	
	const char* filePath = this->getFilePath();
	ifstream inputFile;
	inputFile.open(filePath);

	while(!inputFile.eof() ){
	// file starts with frame pair and number of matches
	  string f1;
	  string f2;
	  string numP;

	  inputFile >> f1 >> f2 >> numP;

	  int frame1 = StringToNumber<int>(f1);
	  int frame2 = StringToNumber<int>(f2);
	  pair<int,int> frPair = make_pair(frame1,frame2);
	 
	  int numPoints = StringToNumber<int>(numP);

	  if(numPoints==0){
	    continue;
	  }
	  IntVecPair matches;

	  for(int i=0;i<numPoints;i++){
            int key1,key2;
	    inputFile>>key1>>key2;
	    pair<int,int> mPair = make_pair(key1,key2);
	    matches.push_back(mPair);
	  }
	  //Update Data members
	  fPair.push_back(frPair);
	  numM.push_back(numPoints);
	  mList.push_back(matches);
	}

	inputFile.close();
}

void Key_Reader::readFile(vector<cv::Point2f>& featureP){

	const char* filePath = this->getFilePath();
	ifstream inputFile;
	inputFile.open(filePath);

		int numKeys;
		int lengthDisct;

		inputFile >> numKeys >> lengthDisct;
		
		for(int i=0;i<numKeys;i++){
			float x,y,o,s;
			int null;
			// Key file is y, x apparently. 
			inputFile >> y >> x >> o >> s;
			for(int j=0;j<lengthDisct;j++){
				inputFile >> null;
			}
			//Make 2d feature point
			//FIX need to impliment using POS input
			cv::Point2f feat(x,y);
			//Add to vector
			featureP.push_back(feat);
		}

	inputFile.close();

}

void Key_Reader::readFile(vector<cv::Point2f>& featureP,double px,double py){

	const char* filePath = this->getFilePath();
	ifstream inputFile;
	inputFile.open(filePath);

		int numKeys;
		int lengthDisct;

		inputFile >> numKeys >> lengthDisct;
		
		for(int i=0;i<numKeys;i++){
			float x,y,o,s;
			int null;
			// Key file is y, x apparently. 
			inputFile >> y >> x >> o >> s;
			for(int j=0;j<lengthDisct;j++){
				inputFile >> null;
			}
			//Make 2d feature point
			cv::Point2f feat((x-px),(y-py));
//			cv::Point2f feat((x),(y));
			//Add to vector
			featureP.push_back(feat);
		}

	inputFile.close();
}

void POS_Reader::readFile(){

	const char* filePath = this->getFilePath();
	ifstream inputFile;
	inputFile.open(filePath);

	    double x,y,z;

	    //INPUT
	    inputFile >> x >> y >> z;

	    inputFile.close();
	    
	    cv::Mat P = (cv::Mat_<double>(3,1) <<
	    		   x,
	    		   y,
	    		   z);
	 
	    cam = P;
}

void TXT_Reader::readFile(){

	const char* filePath = this->getFilePath();
	ifstream inputFile;
	inputFile.open(filePath);

	    string c;
	    double p00,p01,p02,p03;
	    double p10,p11,p12,p13;
	    double p20,p21,p22,p23;

//INPUT
	    inputFile >> c;
	    inputFile >> p00 >> p01 >> p02 >> p03;
	    inputFile >> p10 >> p11 >> p12 >> p13;
	    inputFile >> p20 >> p21 >> p22 >> p23;

	    inputFile.close();
	    
	    cv::Mat P = (cv::Mat_<double>(3,4) <<
	    		   p00, p01, p02, p03,
	    		   p10, p11, p12, p13,
	    		   p20, p21, p22, p23 );
	 
	    cam = P;
}

void TXT_Writer::writeCam( cv::Mat *cam){

	const char* filePath = this->getFilePath();
	ofstream outputFile;
	outputFile.open(filePath);

	outputFile << *cam;

	outputFile.close();
}

void PLYPMVS_Reader::readFile(){

	const char* filePath = this->getFilePath();
	ifstream inputFile;
	inputFile.open(filePath);

	string dump;
	int numP;
 	
	//Header
	for(int i= 0; i<6; i++){
		inputFile >> dump;
	}
	inputFile >> numP;
	//Rest of header
	for(int i=0;i<28;i++){
		inputFile >> dump;
	}
	//Points
	for(int i=0;i<numP;i++){
		double x,y,z;
		double nx,ny,nz;
		int r,g,b;

		vector<int> color;
		//Read Point data
		inputFile >> x >> y >> z;
		inputFile >> nx >> ny >> nz;
		inputFile >> r >> g >> b;
		
		//Form point data
		cv::Mat point = (cv::Mat_<double>(3,1) << x,y,z);
		cv::Mat norm = (cv::Mat_<double>(3,1) << nx,ny,nz);		

		color.push_back(r);
		color.push_back(g);
		color.push_back(b);

		//Store points data
		pointCloud.push_back(point);
		pointCloudN.push_back(norm);
		pointCloudC.push_back(color);
	}
	inputFile.close();
}

void PLY_Writer::writePLY(vector<cv::Point3d> wp){

	const char* filePath = this->getFilePath();
	ofstream outputFile;
	outputFile.open(filePath);

	outputFile << "ply\n";
	outputFile << "format ascii 1.0\n";
	outputFile << "element vertex " << wp.size() << "\n";
	outputFile << "property float x\n";
	outputFile << "property float y\n";
	outputFile << "property float z\n";
	outputFile << "end_header\n";

	//Print all points
	for(unsigned int i = 0; i<wp.size();i++){
	 outputFile << wp[i].x << " " << wp[i].y << " " << wp[i].z << "\n";
	}
	outputFile << "0 0 0 0\n";

	outputFile.close();
}

void PLY_Writer::writePLY(vector<cv::Mat>* pointC,vector<cv::Mat >* pointCN,vector<vector<int> >* pointCC ){

	const char* filePath = this->getFilePath();
	ofstream head(filePath);

	string header = "ply\nformat binary_little_endian 1.0\nelement vertex " + NumberToString<unsigned int>(pointC->size()) + "\n"
			+"property float x\nproperty float y\nproperty float z\nproperty float nx\nproperty float ny\nproperty float nz\n"
			+"property uchar diffuse_red\nproperty uchar diffuse_green\nproperty uchar diffuse_blue\nend_header\n";

	head << header;
	head.close();

	ofstream lines(filePath, ios::app | ios::binary);
	
	int nPts = static_cast<int>(pointC->size());
	//Write out each point	
	for(int i = 0; i<nPts; i++){

		float x = static_cast<float>((*pointC)[i].at<double>(0,0));
		float y = static_cast<float>((*pointC)[i].at<double>(1,0));
		float z = static_cast<float>((*pointC)[i].at<double>(2,0));
		float nx = static_cast<float>((*pointCN)[i].at<double>(0,0));
		float ny = static_cast<float>((*pointCN)[i].at<double>(1,0));
		float nz = static_cast<float>((*pointCN)[i].at<double>(2,0));
		unsigned char r = static_cast<unsigned char>((*pointCC)[i][0]);
		unsigned char g = static_cast<unsigned char>((*pointCC)[i][1]);
		unsigned char b = static_cast<unsigned char>((*pointCC)[i][2]);
		
		lines.write( reinterpret_cast<char*>(&x), sizeof(x));
		lines.write( reinterpret_cast<char*>(&y), sizeof(y));
		lines.write( reinterpret_cast<char*>(&z), sizeof(z));
		lines.write( reinterpret_cast<char*>(&nx), sizeof(nx));
		lines.write( reinterpret_cast<char*>(&ny), sizeof(ny));
		lines.write( reinterpret_cast<char*>(&nz), sizeof(nz));
		lines.write( reinterpret_cast<char*>(&r), sizeof(r));
		lines.write( reinterpret_cast<char*>(&g), sizeof(g));
		lines.write( reinterpret_cast<char*>(&b), sizeof(b));
	}
	lines.close();
}
