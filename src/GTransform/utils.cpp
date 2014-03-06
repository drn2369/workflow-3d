/*
 * =====================================================================================
 *
 *       Filename:  utils.cpp
 *
 *    Description:  General Functions
 *
 *        Version:  1.0
 *        Created:  05/01/2012 12:00:37 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  David Nilosek (), drn2369@cis.rit.edu
 *        Company:  Rochester Institute of Technology
 *
 * =====================================================================================
 */

#include	<sstream>
#include	<cmath>

#include	"utils.h"

using namespace std;

/**BASED ON SBA UTILS**/
void rot2quat(cv::Mat Rot, double *quat){
	double tmp[4];
	double mag = 0;
	int maxP;

	//Find max quat
	tmp[0] = 1.0 + Rot.at<double>(0,0) + Rot.at<double>(1,1) + Rot.at<double>(2,2);
	tmp[1] = 1.0 + Rot.at<double>(0,0) - Rot.at<double>(1,1) - Rot.at<double>(2,2);
	tmp[2] = 1.0 - Rot.at<double>(0,0) + Rot.at<double>(1,1) - Rot.at<double>(2,2);
	tmp[3] = 1.0 - Rot.at<double>(0,0) - Rot.at<double>(1,1) + Rot.at<double>(2,2);

	for(int i=0; i<4;i++){
	     if(tmp[i] > mag){
   		mag = tmp[i];
	     	maxP=i;
		}
	}	

	if(maxP==0){
	     quat[0] = sqrt(tmp[0])*0.5;
	     quat[1] = (Rot.at<double>(2,1) - Rot.at<double>(1,2))/(4.0*quat[0]); 
	     quat[2] = (Rot.at<double>(0,2) - Rot.at<double>(2,0))/(4.0*quat[0]); 
	     quat[3] = (Rot.at<double>(1,0) - Rot.at<double>(0,1))/(4.0*quat[0]); 
	}

	if(maxP==1){
	     quat[1] = sqrt(tmp[1])*0.5;
	     quat[0] = (Rot.at<double>(2,1) - Rot.at<double>(1,2))/(4.0*quat[1]); 
	     quat[2] = (Rot.at<double>(1,0) + Rot.at<double>(0,1))/(4.0*quat[1]); 
	     quat[3] = (Rot.at<double>(0,2) + Rot.at<double>(2,0))/(4.0*quat[1]); 
	}

	if(maxP==2){
	     quat[2] = sqrt(tmp[2])*0.5;
	     quat[0] = (Rot.at<double>(0,2) - Rot.at<double>(2,0))/(4.0*quat[2]); 
	     quat[1] = (Rot.at<double>(1,0) + Rot.at<double>(0,1))/(4.0*quat[2]); 
	     quat[3] = (Rot.at<double>(2,1) + Rot.at<double>(1,2))/(4.0*quat[2]); 
	}
	
	if(maxP==3){
	     quat[3] = sqrt(tmp[3])*0.5;
	     quat[0] = (Rot.at<double>(1,0) - Rot.at<double>(0,1))/(4.0*quat[3]); 
	     quat[1] = (Rot.at<double>(0,2) + Rot.at<double>(2,0))/(4.0*quat[3]); 
	     quat[2] = (Rot.at<double>(2,1) + Rot.at<double>(1,2))/(4.0*quat[3]); 
	}

	/*Enforce unit length*/
	mag = quat[0]*quat[0] + quat[1]*quat[1] + quat[2]*quat[2] +quat[3]*quat[3];
	
	if(mag != 1.0){
	  mag = 1.0 / sqrt(mag);
	  quat[0]*=mag;
	  quat[1]*=mag;
	  quat[2]*=mag;
	  quat[3]*=mag;
	}
}


void quat2rot(double quat[4], cv::Mat &Rot){

	/*Enforce unit length*/
	double mag = quat[0]*quat[0] + quat[1]*quat[1] + quat[2]*quat[2] +quat[3]*quat[3];
	
	if(mag != 1.0){
	  mag = 1.0 / sqrt(mag);
	  quat[0]*=mag;
	  quat[1]*=mag;
	  quat[2]*=mag;
	  quat[3]*=mag;
	}
	Rot.at<double>(0,0) = quat[0]*quat[0]+quat[1]*quat[1]-quat[2]*quat[2]-quat[3]*quat[3]; 
	Rot.at<double>(0,1) = 2*(quat[1]*quat[2]-quat[0]*quat[3]); 
	Rot.at<double>(0,2) = 2*(quat[1]*quat[3]+quat[0]*quat[2]); 

	Rot.at<double>(1,0) = 2*(quat[1]*quat[2]+quat[0]*quat[3]); 
	Rot.at<double>(1,1) = quat[0]*quat[0]+quat[2]*quat[2]-quat[1]*quat[1]-quat[3]*quat[3]; 
	Rot.at<double>(1,2) = 2*(quat[2]*quat[3]-quat[0]*quat[1]); 
	
        Rot.at<double>(2,0) = 2*(quat[1]*quat[3]-quat[0]*quat[2]); 
	Rot.at<double>(2,1) = 2*(quat[2]*quat[3]+quat[0]*quat[1]); 
	Rot.at<double>(2,2) = quat[0]*quat[0]+quat[3]*quat[3]-quat[1]*quat[1]-quat[2]*quat[2];
 
}
