/*
 * =====================================================================================
 *
 *       Filename:  utils.h
 *
 *    Description:  General functions 
 *
 *        Version:  1.0
 *        Created:  05/01/2012 11:58:32 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  David Nilosek (), drn2369@cis.rit.edu
 *        Company:  Rochester Institute of Technology
 *
 * =====================================================================================
 */
#ifndef UTILS_H
#define UTILS_H

#include	<string>
#include	<sstream>

#include	<cv.h>
using namespace std;

template <typename T> T StringToNumber(string &Str){
	stringstream ss(Str);
	T result;
	return ss >> result ? result : 0;
};

template <typename T> string NumberToString(T Num){
	ostringstream ss;
	ss << Num;
	return ss.str();
}

void rot2quat(cv::Mat, double*);
void quat2rot(double[4],cv::Mat&);

#endif
