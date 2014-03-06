/*
 * =====================================================================================
 *
 *       Filename:  positions.h
 *
 *    Description:  Class defintions for position extraction
 *
 *        Version:  1.0
 *        Created:  02/04/2013 01:02:44 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  David Nilosek (), drn2369@cis.rit.edu
 *        Company:  Rochester Institute of Technology
 *
 * =====================================================================================
 */
#ifndef		POSITIONS_H
#define		POSITIONS_H

#include	<vector>
#include	<string>
#include	<utility>

#include	<cv.h>

using namespace std;

/*
 * =====================================================================================
 *        Class:  Positions
 *  Description:  Extract camera positions from PMVS txt files and POS files
 * =====================================================================================
 */
class Positions{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		Positions ( const char* basePath, vector<string> camList);      /* constructor */
		/* ====================  ACCESSORS     ======================================= */
		string getBasePath(){return basePath;}
		vector< pair<cv::Mat, cv::Mat> >* getCamPositions(){return &camPositions;}
		/* ====================  MUTATORS      ======================================= */
		void setBasePath(const char* baseP){basePath = baseP;}
		void setCamList(vector<string> camL){camList = camL;}
		/* ====================  OPERATORS     ======================================= */
		void readPositions();
		cv::Mat getOrigin(cv::Mat P);
		/* ====================  DATA MEMBERS  ======================================= */
	protected:

	private:
		string basePath;
		vector< pair<cv::Mat, cv::Mat> > camPositions;
		vector<string> camList;
	
}; /* -----  end of class Positions  ----- */
#endif
