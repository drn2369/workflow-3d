// siftGPU.cpp 
//
// This program will take in a list of images formatted for Bundler
// inputm run siftGPU, and write out the keyfiles for each image.
// Then it will match each keyfile to each other and put that out in
// Bundler format
//
// Args: image list, match output file name, max dimension, max features, GPU num
// David Nilosek (drn2369 at rit.edu) 3/1/2012

#include <stdlib.h>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <iostream>
#include <SiftGPU.h>
#include <SiftMatchCU.h>

using namespace std;

int main(int argc, char* argv[]){

 //Vectors for filenames, nums,descriptors and keys
 vector<string> fileVec;
 vector< int > numVec;
 vector< vector< float> > descVec;
 vector< vector< SiftGPU::SiftKeypoint> > keyVec;

 //Create Sift and Sift Matcher object
 SiftGPU *sift = new SiftGPU;

 //Setup sift objects
 char * args[] = {"-fo","-1","-v","1","-tc",argv[4],"-maxd",argv[3],"-cuda",argv[5]}; 
 
 int argz = sizeof(args)/sizeof(char*);
 sift->ParseParam(argz,args);

 //Read filenames
 ifstream filenames(argv[1],ifstream::in);

 //New format
 while( !filenames.eof() ){
 	string path;

	filenames >> path;
	fileVec.push_back(path);
}
 filenames.close(); 

 //Run Sift on images and save
 for(unsigned int i =0; i<fileVec.size()-1; i++){
 	cout << fileVec.size();
	//Run Sift and write out
	string str = fileVec[i];
	string rep = "key";

	str.replace( str.find("jpg"), 3, rep);

	sift->RunSIFT(fileVec[i].c_str());
	sift->SaveSIFT(str.c_str());

	//Store nums,keys,and descriptors
	vector<float> descript(1);
	vector<SiftGPU::SiftKeypoint> key(1);
	int num;

	num = sift->GetFeatureNum();
	// Allocate memory
	key.resize(num); descript.resize(128*num);
	sift->GetFeatureVector(&key[0],&descript[0]);
	//Store
	numVec.push_back(num);
	descVec.push_back(descript);
	keyVec.push_back(key);
}
 //Cleanup
 delete sift;

 //Match Sift features and output
 ofstream matches(argv[2],ofstream::out);

 for(unsigned int i =0; i<fileVec.size()-1; i++){
	cout << "Matching image " << i << endl;
	// Timer
	clock_t start;
	double duration;
	start = std::clock();

	for(unsigned int j = 0; j<i; j++){

        SiftMatchGPU matcher(16000);
        matcher.SetLanguage(SiftMatchGPU::SIFTMATCH_CUDA);
        matcher.VerifyContextGL();

	//Set up matcher
	matcher.SetDescriptors(0,numVec[j], &(descVec[j][0]) );
	matcher.SetDescriptors(1,numVec[i], &(descVec[i][0]) );

	//match and get result
        int (*match_buf)[2] = new int[numVec[i]][2];
	float dstMax = 0.7;
	float ratioMax = 0.5;
	int num_match = matcher.GetSiftMatch(numVec[i],match_buf,dstMax,ratioMax,1);

	//Write out
	matches << j << " " << i << endl << num_match << endl;
	for(int k = 0; k < num_match; k++){
	matches << match_buf[k][0] << " " << match_buf[k][1] << endl;
	}

	//Cleanup
	delete match_buf;
  }
	//Timer
	duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
	cout <<"Took: " << duration << " seconds" << endl;
 }
 matches.close();

return 0;
}
