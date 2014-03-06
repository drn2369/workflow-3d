/*
 * =====================================================================================
 *
 *       Filename:  Reader.h
 *
 *    Description:  Classes for input
 *
 *        Version:  1.0
 *        Created:  05/01/2012 10:52:19 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  David Nilosek (), drn2369@cis.rit.edu
 *        Company:  Rochester Institute of Technology
 *
 * =====================================================================================
 */
#ifndef READER_H
#define READER_H

#include	<vector>
#include	<utility>

#include	<cv.h>

#include	"utils.h"

using namespace std;

typedef vector<pair<int,int> > IntVecPair;

/*
 * =====================================================================================
 *        Class:  Reader
 *  Description:  Abstract reader class
 * =====================================================================================
 */
class IOInterface{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		IOInterface (const char* fileP);                             /* constructor */
		/* ====================  ACCESSORS     ======================================= */
		const char* getFilePath(){return filePath;}		
		/* ====================  MUTATORS      ======================================= */
		void setFilePath(const char* fileP){filePath = fileP;}
		/* ====================  OPERATORS     ======================================= */
		
		/* ====================  DATA MEMBERS  ======================================= */
	protected:

	private:
		const char* filePath;
}; /* -----  end of class Reader  ----- */


/*
 * =====================================================================================
 *        Class:  IMatch_Reader
 *  Description:  Reads the inital match file
 * =====================================================================================
 */
class IMatch_Reader : public IOInterface{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		IMatch_Reader(const char* fileP,IntVecPair& fPair,
			      vector<int>& numM, vector<IntVecPair>& mList);    /* constructor */
		/* ====================  ACCESSORS     ======================================= */

		/* ====================  MUTATORS      ======================================= */
		
		/* ====================  OPERATORS     ======================================= */
		void readFile(IntVecPair& fPair, vector<int>& numM, vector<IntVecPair>& mList);
		/* ====================  DATA MEMBERS  ======================================= */
	protected:

	private:

}; /* -----  end of class IMatch_Reader  ----- */


/*
 * =====================================================================================
 *        Class:  Key_Reader
 *  Description:  Reads the key files
 * =====================================================================================
 */
class Key_Reader : public IOInterface{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		Key_Reader (const char* fileP, vector<cv::Point2f>& featureP);   /* constructor */
		Key_Reader (const char* fileP, vector<cv::Point2f>& featureP, double px, double py);
		/* ====================  ACCESSORS     ======================================= */

		/* ====================  MUTATORS      ======================================= */

		/* ====================  OPERATORS     ======================================= */
		void readFile(vector<cv::Point2f>& featureP);
		void readFile(vector<cv::Point2f>& featureP, double xp, double yp);
		/* ====================  DATA MEMBERS  ======================================= */
	protected:

	private:

}; /* -----  end ofZZ class Key_Reader  ----- */


/*
 * =====================================================================================
 *        Class:  POS_Reader
 *  Description:  Read POS files
 * =====================================================================================
 */
class POS_Reader : public IOInterface{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		POS_Reader (const char* fileP);                             /* constructor */
		/* ====================  ACCESSORS     ======================================= */
		cv::Mat getCam(){return cam;}
		/* ====================  MUTATORS      ======================================= */

		/* ====================  OPERATORS     ======================================= */
		void readFile();
		/* ====================  DATA MEMBERS  ======================================= */
	protected:

	private:
	    cv::Mat cam;
}; /* -----  end of class POS_Reader  ----- */


/*
 * =====================================================================================
 *        Class:  TXT_Reader
 *  Description:  Read PMVS txt camera file
 * =====================================================================================
 */
class TXT_Reader : public IOInterface {
	public:
		/* ====================  LIFECYCLE     ======================================= */
		TXT_Reader (const char* fileP);                             /* constructor */

		/* ====================  ACCESSORS     ======================================= */
		cv::Mat* getCam(){return &cam;}
		/* ====================  MUTATORS      ======================================= */

		/* ====================  OPERATORS     ======================================= */
		void readFile();
		/* ====================  DATA MEMBERS  ======================================= */
	protected:

	private:
		cv::Mat cam;
}; /* -----  end of class TXT_Reader  ----- */

/*
 * =====================================================================================
 *        Class:  TXT_Writer
 *  Description:  Read PMVS txt camera file
 * =====================================================================================
 */
class TXT_Writer : public IOInterface {
	public:
		/* ====================  LIFECYCLE     ======================================= */
		TXT_Writer (const char* fileP);                             /* constructor */

		/* ====================  ACCESSORS     ======================================= */
		/* ====================  MUTATORS      ======================================= */

		/* ====================  OPERATORS     ======================================= */
		void writeCam(cv::Mat *cam);
		/* ====================  DATA MEMBERS  ======================================= */
	protected:

	private:
}; /* -----  end of class TXT_Reader  ----- */




/*
 * =====================================================================================
 *        Class:  PLYPMVS_Reader
 *  Description: Read PLYs writte by PMVS  
 * =====================================================================================
 */
class PLYPMVS_Reader : public IOInterface{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		PLYPMVS_Reader (const char* fileP);                             /* constructor */

		/* ====================  ACCESSORS     ======================================= */
		vector<cv::Mat>* getPointCloud(){return &pointCloud;}
		vector<cv::Mat >* getPointCloudN(){return &pointCloudN;}
		vector<vector<int> >* getPointCloudC(){return &pointCloudC;}
		/* ====================  MUTATORS      ======================================= */

		/* ====================  OPERATORS     ======================================= */
		void readFile();
		/* ====================  DATA MEMBERS  ======================================= */
	protected:

	private:
		vector<cv::Mat> pointCloud;
		vector<cv::Mat> pointCloudN;
		vector<vector<int> > pointCloudC;
}; /* -----  end of class PLY_Reader  ----- */

/*
 * =====================================================================================
 *        Class:  PLY_Writer
 *  Description:  Write PLY files
 * =====================================================================================
 */
class PLY_Writer : public IOInterface{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		PLY_Writer (const char* fileP);                             /* constructor */
		/* ====================  ACCESSORS     ======================================= */

		/* ====================  MUTATORS      ======================================= */

		/* ====================  OPERATORS     ======================================= */
		void writePLY(vector<cv::Point3d>);
		void writePLY(vector<cv::Mat>*,vector<cv::Mat >*,vector<vector<int> >* );
		/* ====================  DATA MEMBERS  ======================================= */
	protected:

	private:

}; /* -----  end of class PLY_Writer  ----- */

/*
 * =====================================================================================
 *        Class:  PMVS_Writer
 *  Description:  Writes files for PMVS
 * =====================================================================================i
 */
class PMVS_Writer : public IOInterface{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		PMVS_Writer (const char* fileP);                             /* constructor */
		/* ====================  ACCESSORS     ======================================= */

		/* ====================  MUTATORS      ======================================= */

		/* ====================  OPERATORS     ======================================= */
		void writePMVS();
		/* ====================  DATA MEMBERS  ======================================= */
	protected:

	private:

}; /* -----  end of class PMVS_Writer  ----- */

#endif
