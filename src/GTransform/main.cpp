/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  Main routine for GTranslate
 *
 *        Version:  1.0
 *        Created:  02/04/2013 12:24:54 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  David Nilosek (), drn2369@cis.rit.edu
 *        Company:  Rochester Institute of Technology
 *
 * =====================================================================================
 */
#include	<vector>
#include	<string>
#include	<iostream>
#include	<fstream>

#include	"positions.h"
#include	"transform.h"
#include	"IO.h"

using namespace std;

//First argument is base path for PMVS folder, second is path to PMVS file list
//third is the path to the model list
int main ( int argc, char *argv[] ) {

	//Define
	vector<string> pmvsList; 
	vector<string> modelList;
	vector<string> camList;

	//Read in pmvs list
	ifstream file(argv[2]);
	//Define
	string tempP;
	while(file >> tempP){
		pmvsList.push_back(tempP);
	}
	file.close();

	//Read in model list
	ifstream fileM(argv[3]);
	//Define
	string tempM;
	while(fileM >> tempM){
		modelList.push_back(tempM);
	}
	fileM.close();

	//Read all the positiosn
	Positions readPos(argv[1], pmvsList);

	//Calculate Transform
	Transform trans(readPos.getCamPositions());

	//Transform and write out each point cloud
	for(unsigned int i=0; i<modelList.size(); i++){
		//Read PLY file
		string filePath = argv[1];
		filePath = filePath+"models/"+modelList[i]+".ply";
		PLYPMVS_Reader ply(filePath.c_str());

		//Transform PLY file
		trans.transformPoints(ply.getPointCloud());
		trans.transformNorms(ply.getPointCloudN());

		//Write out PLY file
		string writePath = argv[1];
		writePath = writePath+"models/"+modelList[i]+"-trans.ply";
		PLY_Writer plyO(writePath.c_str());
		plyO.writePLY(ply.getPointCloud(),ply.getPointCloudN(),ply.getPointCloudC());

	}

	//Transform cameras
	for(unsigned int i=0; i < pmvsList.size(); i++){
		string filePath = argv[1];
		filePath = filePath + "txt/"+pmvsList[i]+".txt";
		TXT_Reader cam(filePath.c_str());	

		//Transform Camera
		trans.transformCam(cam.getCam());

		//Write out Camera
		string writePath = argv[1];
		writePath = writePath + "txt/" +pmvsList[i]+"-trans.txt";
		TXT_Writer camO(writePath.c_str());
		camO.writeCam(cam.getCam());
	}
		//Save Transform
		cv::Mat R = trans.getR();
		cv::Mat T = trans.getT();
		double s = trans.getS();

		string svPath = argv[1];
		svPath = svPath+"transform.txt";
		ofstream svTrans(svPath.c_str());

		svTrans << s << endl;
		svTrans << R << endl;
		svTrans << T << endl;
	        svTrans.close();

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
