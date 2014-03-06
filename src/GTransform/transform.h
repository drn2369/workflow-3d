/*
 * =====================================================================================
 *
 *       Filename:  transform.h
 *
 *    Description:  Class definitions for the rigid body transform
 *
 *        Version:  1.0
 *        Created:  02/04/2013 03:26:53 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  David Nilosek (), drn2369@cis.rit.edu
 *        Company:  Rochester Institute of Technology
 *
 * =====================================================================================
 */
#ifndef		TRANSFORM_H
#define		TRANSFORM_H

#include	<vector>
#include	<utility>

#include	<cv.h>

using namespace std;

/*
 * =====================================================================================
 *        Class:  Transform
 *  Description:  class for computing the rigid body transform Y = sAX+b
 * =====================================================================================
 */

class Transform{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		Transform(vector<pair<cv::Mat,cv::Mat> >* camP);                /* constructor */

		/* ====================  ACCESSORS     ======================================= */
		cv::Mat getR(){return R;}
		cv::Mat getT(){return t;}
		double getS(){return s;}
		cv::Mat getSim();
		/* ====================  MUTATORS      ======================================= */
		void setCamP(vector<pair<cv::Mat,cv::Mat> >* cP){camP = cP;}
		void setR(cv::Mat R_){R=R_;}
		void setT(cv::Mat t_){t=t_;}
		void setS(double s_){s=s_;}
		/* ====================  OPERATORS     ======================================= */
		void calculateTransform();
		void calculateTransform2();
		void transformPoints(vector<cv::Mat>* points);
		void transformNorms(vector<cv::Mat>* norms);
		void transformCam(cv::Mat* cam);
		/* ====================  DATA MEMBERS  ======================================= */
	protected:

	private:
		vector< pair<cv::Mat, cv::Mat> >* camP;
		cv::Mat R;
		cv::Mat t;
		double s;


}; /* -----  end of class Transform  ----- */
#endif
