/*
 * =====================================================================================
 *
 *       Filename:  transform.cpp
 *
 *    Description:  Class implimentation for rigid body transform
 *
 *        Version:  1.0
 *        Created:  02/04/2013 03:34:23 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  David Nilosek (), drn2369@cis.rit.edu
 *        Company:  Rochester Institute of Technology
 *
 * =====================================================================================
 */

#include	"transform.h"

Transform::Transform(vector<pair<cv::Mat,cv::Mat> >* camP){
	setCamP(camP);
	//calculateTransform();
	calculateTransform2();
}

//Cacluate rigid body transform using 
//Helmert tranform
void Transform::calculateTransform(){

    //Step 1: Calculate mean
	//Define	
	vector<pair<cv::Mat,cv::Mat> >::iterator it;
	cv::Mat srcMeanT  = (cv::Mat_<double>(3,1) << 0,0,0);
	cv::Mat dstMeanT  = (cv::Mat_<double>(3,1) << 0,0,0);

	for( it = camP->begin(); it !=camP->end(); ++it){
		//Add each vector togeather
		srcMeanT += it->first;
		dstMeanT += it->second;
	}

	//Extract nPts
	int nPts = camP->size();

	//Define manual divide for some reason...
	cv::Mat srcMean = (cv::Mat_<double>(3,1) << srcMeanT.at<double>(0,0)/nPts,
						    srcMeanT.at<double>(1,0)/nPts,
						    srcMeanT.at<double>(2,0)/nPts);
	cv::Mat dstMean = (cv::Mat_<double>(3,1) << dstMeanT.at<double>(0,0)/nPts,
						    dstMeanT.at<double>(1,0)/nPts,
						    dstMeanT.at<double>(2,0)/nPts);

   //Step 2: Create zero mean nPoint set
   	//Define
	vector<pair<cv::Mat,cv::Mat> > zeroMean;

	//Reset iterator
	it = camP->begin();

	//Fill
	for(int i =0; i<nPts; i++){

		cv::Mat srcZM = (cv::Mat_<double>(3,1) <<
			it->first.at<double>(0,0) - srcMean.at<double>(0,0),
			it->first.at<double>(1,0) - srcMean.at<double>(1,0), 
			it->first.at<double>(2,0) - srcMean.at<double>(2,0)); 

		cv::Mat dstZM = (cv::Mat_<double>(3,1) <<
			it->second.at<double>(0,0) - dstMean.at<double>(0,0),
			it->second.at<double>(1,0) - dstMean.at<double>(1,0), 
			it->second.at<double>(2,0) - dstMean.at<double>(2,0)); 

		zeroMean.push_back(make_pair(srcZM,dstZM));

		//Increment iterator
		++it;
	}
   
  //Step 3: Calculate correlation matrix
	//Define
	cv::Mat CT = (cv::Mat_<double>(3,3) << 0,0,0,
					       0,0,0,
					       0,0,0);

	//Sum over i dst*src^t
	for(unsigned int i=0; i < zeroMean.size(); i++){
		cv::Mat tempC;
		tempC = zeroMean[i].second*zeroMean[i].first.t();
		CT += tempC;
	}
	//Define, again manual divide
	cv::Mat C = (cv::Mat_<double>(3,3) << 
		CT.at<double>(0,0)/nPts, CT.at<double>(0,1)/nPts, CT.at<double>(0,2)/nPts, 
		CT.at<double>(1,0)/nPts, CT.at<double>(1,1)/nPts, CT.at<double>(1,2)/nPts,
		CT.at<double>(2,0)/nPts, CT.at<double>(2,1)/nPts, CT.at<double>(2,2)/nPts) ;


  //Step 4: Perform SVD
  	//Define
	cv::SVD svd(C);

  //Step 5: Calculate sigma to take care of -1 determinant
	cv::Mat UVt = svd.u*svd.vt;
	double sigma = cv::determinant(UVt);

  //Step 6: Calculate Roatation Matrix
	cv::Mat d = (cv::Mat_<double>(3,3) << 1, 0, 0,
	                                      0, 1, 0,
					      0, 0, sigma);
	cv::Mat A = svd.u*d*svd.vt;

  //Step 7: Calculate Scale
  	double g = 0;
	double alpha;

	for(int i = 0; i < nPts; i++){
		cv::Mat Ax = A*zeroMean[i].first;
		cv::Mat tempG = Ax.t()*zeroMean[i].second;
		g += tempG.at<double>(0,0);
	}

	if( g > 0){
		double nX = 0;
		for(int i = 0; i < nPts; i++){
			cv::Mat tNx = zeroMean[i].first.t()*zeroMean[i].first;
			nX += tNx.at<double>(0,0);
		}
		alpha = g/nX;
	}else{
		alpha = 0;
	}

  //Step 8: Calculate Translation
  cv::Mat B = dstMean - alpha*A*srcMean;

  //Store transform
  this->setR(A);
  this->setT(B);
  this->setS(alpha);
}	

//Slightly different way of calculating transform
void Transform::calculateTransform2(){

    //Step 1: Calculate mean
	//Define	
	vector<pair<cv::Mat,cv::Mat> >::iterator it;
	cv::Mat srcMeanT  = (cv::Mat_<double>(3,1) << 0,0,0);
	cv::Mat dstMeanT  = (cv::Mat_<double>(3,1) << 0,0,0);

	for( it = camP->begin(); it !=camP->end(); ++it){
		//Add each vector togeather
		srcMeanT += it->first;
		dstMeanT += it->second;
	}

	//Extract nPts
	int nPts = camP->size();

	//Define manual divide for some reason...
	cv::Mat srcMean = (cv::Mat_<double>(3,1) << srcMeanT.at<double>(0,0)/nPts,
						    srcMeanT.at<double>(1,0)/nPts,
						    srcMeanT.at<double>(2,0)/nPts);
	cv::Mat dstMean = (cv::Mat_<double>(3,1) << dstMeanT.at<double>(0,0)/nPts,
						    dstMeanT.at<double>(1,0)/nPts,
						    dstMeanT.at<double>(2,0)/nPts);

   //Step 2: Create zero mean nPoint set
   	//Define
	vector<pair<cv::Mat,cv::Mat> > zeroMean;

	//Reset iterator
	it = camP->begin();

	//Fill
	for(int i =0; i<nPts; i++){

		cv::Mat srcZM = (cv::Mat_<double>(3,1) <<
			it->first.at<double>(0,0) - srcMean.at<double>(0,0),
			it->first.at<double>(1,0) - srcMean.at<double>(1,0), 
			it->first.at<double>(2,0) - srcMean.at<double>(2,0)); 

		cv::Mat dstZM = (cv::Mat_<double>(3,1) <<
			it->second.at<double>(0,0) - dstMean.at<double>(0,0),
			it->second.at<double>(1,0) - dstMean.at<double>(1,0), 
			it->second.at<double>(2,0) - dstMean.at<double>(2,0)); 

		zeroMean.push_back(make_pair(srcZM,dstZM));

		//Increment iterator
		++it;
	}
  
  //Step 3: Calculate Correlation Matrix
	//Define
	cv::Mat H = (cv::Mat_<double>(3,3) << 0,0,0,
					       0,0,0,
					       0,0,0);

	//Sum over i src*dst^t
	for(unsigned int i=0; i < zeroMean.size(); i++){
		cv::Mat tempC;
		tempC = zeroMean[i].first*zeroMean[i].second.t();
		H += tempC;
	}

  //Step 4: SVD H
	cv::SVD svd(H);


  //Step 5: Calculate R
  	cv::Mat A = svd.vt.t()*svd.u.t();

  //Step 6: Adjust R for reflection case
  	if( cv::determinant(A) < 0){
		//Multiply 3rd column by -1
		A.at<double>(0,2) = A.at<double>(0,2) * -1;
		A.at<double>(1,2) = A.at<double>(1,2) * -1;
		A.at<double>(2,2) = A.at<double>(2,2) * -1;
	}

  //Step 7: Calculate scale from vector magnitude
	double srcM = 0.0;
	double dstM = 0.0;
	
	//Sum up magnitiudes
	for(unsigned int i=0; i < zeroMean.size(); i++){
		cv::Mat srcMt;
		cv::Mat dstMt;

		srcMt = zeroMean[i].first.t()*zeroMean[i].first;
		dstMt = zeroMean[i].second.t()*zeroMean[i].second;

		srcM += srcMt.at<double>(0,0);
		dstM += dstMt.at<double>(0,0);
	}

	double s2 = dstM/srcM;
	double alpha = sqrt(s2);

  //Step 8: Calculate Translation
  cv::Mat B = dstMean - alpha*A*srcMean;

  //Store transform
  this->setR(A);
  this->setT(B);
  this->setS(alpha);

}

void Transform::transformPoints(vector<cv::Mat>* points){

	//Transform each point
	vector<cv::Mat >::iterator it;

	for(it=points->begin(); it != points->end(); ++it){
		//Apply transform and replace value
		cv::Mat tPoint = s*R*(*it) + t;
		(*it) = tPoint;
	}

}

void Transform::transformNorms(vector<cv::Mat>* norms){

	//Transform each normal
	vector<cv::Mat>::iterator it;

	for(it=norms->begin(); it != norms->end(); ++it){
		//Rotate each normal
		cv::Mat tNorm = R*(*it);
		(*it) = tNorm;
	}

}

void Transform::transformCam(cv::Mat* cam){

	//Get Sim
	cv::Mat Sim = getSim();

	//Might have to normalize?
	cv::Mat Punnorm = *cam*Sim.inv();
	cv::Mat Pn;
	normalize(Punnorm,Pn,1,0,cv::NORM_INF);
	*cam = Pn;

/*
	//TESTING
	cout << *cam << endl;

	//Defitions
	cv::Mat k,Rot,Th;

	cv::decomposeProjectionMatrix(*cam, k, Rot, Th);


	cv::Mat Tih = (cv::Mat_<double>(3,1) << Th.at<double>(0,0)/Th.at<double>(3,0), 
						Th.at<double>(1,0)/Th.at<double>(3,0),
						Th.at<double>(2,0)/Th.at<double>(3,0));

	cout << k << endl << Rot << endl << Th << endl << Tih << endl;
	
	//Transform center
	cv::Mat tPoint = s*R*Tih + t;

	//Rotate Rotation matrix
	cv::Mat newR = Rot*R;

	//Recompose Projection Matrix
	cv::Mat RT = -newR*tPoint;
	cv::Mat RRT;
	cv::hconcat(newR,RT,RRT);
	*cam = k*RRT;
*/
}

//Returns 3x3 similarity matrix

cv::Mat Transform::getSim(){

	cv::Mat sR = s*R;
	cv::Mat sRT;

	cv::hconcat(sR,t,sRT);

	cv::Mat base = (cv::Mat_<double>(1,4) << 0, 0,0,1);

	cv::Mat Sim;

	cv::vconcat(sRT,base,Sim);

	return Sim;
}
