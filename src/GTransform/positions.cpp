/*
 * =====================================================================================
 *
 *       Filename:  positions.cpp
 *
 *    Description:  Class implimentation for position extraction
 *
 *        Version:  1.0
 *        Created:  02/04/2013 01:09:04 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  David Nilosek (), drn2369@cis.rit.edu
 *        Company:  Rochester Institute of Technology
 *
 * =====================================================================================
 */
#include	<iostream>

#include	"positions.h"
#include	"IO.h"

Positions::Positions(const char* basePath, vector<string> camList){
	setBasePath(basePath);
	setCamList(camList);
	
	readPositions();
}


void Positions::readPositions(){

	//Read all cams in
	for(unsigned int i =0; i<camList.size();i++){
		//TXT file
		string txtFile = basePath + "txt/" + camList[i] + ".txt";
		//POS file
		string posFile = basePath + "pos/" +  camList[i] + ".pos";

		//Read in TXT
		TXT_Reader txtRead(txtFile.c_str());
		//Read in POS
		POS_Reader posRead(posFile.c_str());

		//Get origin from Camera matrix
		cv::Mat camP = getOrigin(*txtRead.getCam());

		//cout << camP << endl;
		//cout << posRead.getCam() << endl;

		//Store positions
		camPositions.push_back(make_pair(camP, posRead.getCam()));
		
	}

	//TEST WRITE OUT STUFF
	for(int i = 0; i < camPositions.size(); i++){
		cout << camPositions[i].first << endl;
	}
	for(int i = 0; i < camPositions.size(); i++){
		cout << camPositions[i].second << endl;
	}


}

cv::Mat Positions::getOrigin(cv::Mat P){

	//Define
	double x,y,z;

	//Get origin of projection matrix 
	//from the right null space
	
	cv::SVD svd(P, cv::SVD::FULL_UV);
	//Right null vector last col of V
	//Convert from homo to inhomo
	
	x = svd.vt.at<double>(3,0)/svd.vt.at<double>(3,3);
	y = svd.vt.at<double>(3,1)/svd.vt.at<double>(3,3);
	z = svd.vt.at<double>(3,2)/svd.vt.at<double>(3,3);

	cv::Mat pos = (cv::Mat_<double>(3,1) << x,y,z);

	return pos;
}
