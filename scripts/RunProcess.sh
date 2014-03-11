#!/bin/bash

# Script to run siftGPU-Bundler-CMVS/PMVS workflow
#
#Version 1.1.0
#
# Usage:
#	Copy RunProcess.sh to a working directory. The directory must contain
#	a file named list.txt, which should list the full path to each image per line.
#	In order to run GTransform, each image must have an associated .pos file in the
#	same directory as the image. The .pos file must contain an X,Y and Z coordinate
#	in the desired euclidean coordinate system (i.e ECEF or UTM/Alt). 
#
#	The workflow must be run in the following order: Sift, Bundler, CMVS/PMVS, 
#	GTransform. Each successive step can be rerun multiple times as long as the
#	previous step has been run at least once.
#	
# Results:
#	This workflow outputs a three-dimensional point cloud of the scene derived
#	from the input imagery. This point cloud is represented in a .ply file which
#	can be found in the results directory. If GTransform is run, an additional
#	.ply file will be created in the results directory where the x,y,z coordinates of
#	each point are in the coordinate system defined in the .pos files which were 
#	provided along with the imagery.
#
# Options:
#    -a  Run the whole workflow without GTransform (equivalent to -sbp)
#    -s  Run Sift
#    -b  Run Bundler
#    -p  Run CMVS/PMVS
#    -g  Run GTransform
#    -d  Set GPU device number (default 0)
#    -x  Set max dimension for siftGPU (default 2000)
#    -y  Set the max number of features for siftGPU (default 8000)
#    -f  Set Focal Length of images in pixels (default 6111.11) 
#    -i  Set Focal Length constraint weight (default 0.0001)
#    -c  Set Max Cluster Size for CMVS (default 30)
#    -l  Set Level for PMVS (default 1)
#    -e  Set Cell Size for PMVS (default 2)
#    -t  Set Threshold for PMVS (default 0.7)
#    -w  Set window size for PMVS (default 7)
#    -m  Set Min image number for PMVS (default 3)
#    -u  Set Number of CPUS used for CMVS/PMVS (default 12)
#    -k  Prevent anything from being deleted
#
# Example Usage:
#  To run whole workflow with GTransofrm on a set of images with a focal length of 500 pixels,
#  on 6 CPUs, with a Threshold of 0.9:
#
#  sh RunProcess -ag -f 500 -u 6 -t 0.9
#
#
# Copywrite (c) 2014 David Nilosek (drn2369 at rit.edu) 3/5/2014

#-----------PARAMETERS--------------

#-----------SIFTGPU-----------------
_DEVICE_NUM=0
_MAX_D=2000
_MAX_F=8000
#-----------IMAGE-------------------
_FOCAL=6111.11

#-----------BUNDLER-----------------
_CONSTRAIN_F_WEIGHT=0.0001

#-----------CMVS--------------------
_MAX_CLUSTER_SIZE=30;

#-----------PMVS--------------------
_LEVEL=1
_CSIZE=2
_THRESH=0.7
_WSIZE=7
_MIN_IMG=3
_CPU_NUM=12

#-----------RUN FLAGS---------------
RUN_SIFT=false
CLEAN_SIFT=false
RUN_BUNDLE=false
RUN_PMVS=false
RUN_GTRANS=false

RAN_SIFT=false
RAN_BUNDLE=false
RAN_PMVS=false

DEL_ON=true
#BINARY LOCATIONS

#full path needed
BASEPATH=/opt/workflow3d

BINPATH=$BASEPATH/bin
SCRIPTPATH=$BASEPATH/scripts

SIFT=$BINPATH/siftGPU
MATCHKEYS=$BINPATH/KeyMatchFull
BUNDLER=$BINPATH/bundler
BUNDLE2PMVS=$BINPATH/Bundle2PMVS
CMVS=$BINPATH/cmvs
GENOP=$BINPATH/genOption
GTRANS=$BINPATH/GTransform

PATH=$PATH:$BINPATH
#-----------PARSE OPTIONS---------------------
while getopts "asbpgkd:f:i:c:l:e:t:w:m:u:x:y:" flag; do
  case "$flag" in
	a)RUN_PREP=true
	  RUN_SIFT=true
	  RUN_BUNDLE=true
	  RUN_PMVS=true;;
	s)RUN_SIFT=true;;
	b)RUN_BUNDLE=true;;
	p)RUN_PMVS=true;;
	g)RUN_GTRANS=true;;
	k)DEL_ON=false;;
	d)_DEVICE_NUM=$OPTARG;;
	x)_MAX_D=$OPTARG;;
	f)_MAX_F=$OPTARG;;
	f)_FOCAL=$OPTARG;;
	i)_CONSTRAIN_F_WEIGHT=$OPTARG;;
	c)_MAX_CLUSTER_SIZE=$OPTARG;;
	l)_LEVEL=$OPTARG;;
	e)_CSIZE=$OPTARG;;
	t)_THRESH=$OPTARG;;
	w)_WSIZE=$OPTARG;;
	m)_MIN_IMG=$OPTARG;;
	u)_CPU_NUM=$OPTARG;;
  esac
done
#------------CODE-----------------------------
#---------------------------------------------

#Check if version matches
VERSION=`grep -m 1 '#Version' $BASH_SOURCE | cut -d' ' -f 2`
SRCVERSION=`grep -m 1 '#Version' $SCRIPTPATH/RunProcess.sh | cut -d' ' -f 2`

#Only for cyclone..
#if [ "$VERSION" != "$SRCVERSION" ]
#   then
#	echo "Please get newest script from $SCRIPTPATH"
#	exit 1
#fi

#Make sure there are command line arugments
if [ $# -eq 0 ]
  then
	echo "No Command Options Found"
	exit 1
fi

#Check for nessecary files
if [ ! -f list.txt ]
then
	echo "list.txt missing"
	exit 1
fi

	#Make logs,results,and backup directory
	mkdir -p ./logs
	mkdir -p ./results
	mkdir -p ./results/backup
# Run Sift
if  $RUN_SIFT ; then
	#Check if user wants to run again
	if [ -f results/backup/keys.tar.gz ]
	 then
	   echo "SIFT has already been run for this image set. A second run is only necessary if the image list has changed"
	   read -p "Continue Running SIFT? (y/n)"
	   if [ $REPLY = "y" ]; then
	     echo "[- Running siftGPU -]"
	     $SIFT list.txt matches.init.txt $_MAX_D $_MAX_F $_DEVICE_NUM > ./logs/sift.log
	     CLEAN_SIFT=true
	     RAN_SIFT=true
	   fi
          else
	 #Run SIFT
	 echo "[- Running siftGPU -]"
	 $SIFT list.txt matches.init.txt $_MAX_D $_MAX_F $_DEVICE_NUM > ./logs/sift.log
	 CLEAN_SIFT=true
	 RAN_SIFT=true
	fi
fi

if  $RUN_BUNDLE ; then

 #Check if this is a second run
 if [[ ! -f results/backup/keys.tar.gz ]] && [[ "$RAN_SIFT" != "true" ]]; then
	echo "SIFT must be run prior to Bundler"
	exit 1
 fi

 #Unpack if it is
 if [ -f results/backup/keys.tar.gz ]; then
	tar -xzf results/backup/keys.tar.gz -C /
	mv results/backup/matches.init.txt ./
 fi

# Generate the options file for running bundler 
mkdir -p ./bundle
rm -f ./options.txt

	echo "--match_table matches.init.txt" >>./options.txt
	echo "--output bundle.out" >> ./options.txt
	echo "--output_all bundle_" >> ./options.txt
	echo "--output_dir bundle" >> ./options.txt
	echo "--variable_focal_length" >> ./options.txt
	echo "--use_focal_estimate" >> ./options.txt
	echo "--constrain_focal" >> ./options.txt
	echo "--constrain_focal_weight ${_CONSTRAIN_F_WEIGHT}" >> ./options.txt
	echo "--estimate_distortion" >> ./options.txt
	echo "--run_bundle" >> ./options.txt

	#Clean up
	rm -f list.bundler.txt
	cp list.txt list.bundler.txt
	sed -i "s/$/ 0 $_FOCAL/" list.bundler.txt

	# Run Bundler!
	echo "[- Running Bundler -]"
	rm -f constraints.txt
	rm -f pairwise_scores.txt
	$BUNDLER ./list.bundler.txt --options_file ./options.txt > ./logs/bundler.log

	RAN_BUNDLE=true
fi

if $RUN_PMVS ; then
  #Check if this is a second run
  if [[ ! -f results/backup/bundle.tar.gz ]] && [[ "$RAN_BUNDLE" != "true"  ]]; then
	echo "Bundler must be run prior to PMVS"
	exit 1
  fi

  #Unpack if it is
  if [ -f results/backup/bundle.tar.gz ]; then
      tar -xzf results/backup/bundle.tar.gz
  fi
  	#Prep PMVS
	echo "[- Preparing PMVS -]"
	$BUNDLE2PMVS ./list.txt ./bundle/bundle.out > ./logs/bundle2pmvs.log

        NEWBIN=$(printf "%s\n" "$BINPATH" | sed 's/[\&/]/\\&/g')
        sed -i "s/BUNDLER_BIN_PATH=/BUNDLER_BIN_PATH=${NEWBIN}/g" './pmvs/prep_pmvs.sh'
	
	sh ./pmvs/prep_pmvs.sh &> ./logs/prep_pmvs.log
	mv bundle.rd.out ./pmvs/
	cd pmvs/
	echo "[- Running CMVS -]"
	# max image, num cpus
	$CMVS ./ $_MAX_CLUSTER_SIZE $_CPU_NUM &> ../logs/cmvs.log
	# level, csize, thresh, wsize, minImg, cpu num
	$GENOP ./ $_LEVEL $_CSIZE $_THRESH $_WSIZE $_MIN_IMG $_CPU_NUM &> ../logs/genop.log

	echo "[- Running PMVS -]"
	#clean pmvs script
	sed -i "s/pmvs\//.\//g" './pmvs.sh'

	sh pmvs.sh &> ../logs/pmvs.log
	cd ../

	RAN_PMVS=true
fi

if $RUN_GTRANS; then
	#check to see if this is a second run
	if [[ ! -f results/backup/pmvs.tar.gz ]] && [[ "$RAN_PMVS" != "true" ]];
	  then
	   echo "PMVS must be run before Geotrans"
	   exit 1
	fi

	#Unpack if it is
	if [ -f results/backup/pmvs.tar.gz ]; then
	  tar -xzf results/backup/pmvs.tar.gz
	  tar -xzf results/backup/plys.tar.gz
	fi

	#Prep gtrans
	echo "[- Running GTransform -]"
	cd pmvs/
	mkdir -p pos
	rm -f prep_gtrans.sh

	dirN=`dirname $(grep -m 1 jpg ../list.txt)`
        newDir=$(printf "%s\n" "$dirN" | sed 's/[\&/]/\\&/g')

	#Create script to copy POS files
	grep "jpg" prep_pmvs.sh | sed -e 's/.jpg/.pos/g' | sed -e 's/.rd//g' | sed -e 's/visualize/pos/g' | sed -e 's/mv//' | sed -e 's/pmvs\///g' | cat >> prep_gtrans.sh

	sed -i 's/^[ \t]*//' prep_gtrans.sh
	sed  -i "s/^/cp $newDir\//" prep_gtrans.sh
	sh prep_gtrans.sh
	
	#Create inputs for GTrans
	ls models/*ply | sed -e 's/models\///g' | sed -e 's/.ply//g' > listModels.txt
	ls txt/*txt | sed -e 's/txt\///g' | sed -e 's/.txt//g' > listPMVS.txt
	$GTRANS ./ listPMVS.txt listModels.txt &> ../logs/gtrans.log
	
	#Move output to seperate folder so a second run will not get confused
	mkdir -p trans
	mkdir -p trans/models/
	mkdir -p trans/txt/
	
	mv transform.txt trans/
	mv models/*-trans.ply trans/models/
	mv txt/*-trans.txt trans/txt/

	cd ../
fi

#Clean up if needed
if $DEL_ON; then
echo "[- Clean Up -]"

	dirN=`dirname $(grep -m 1 jpg list.txt)`
	
	if $CLEAN_SIFT; then
	    (tar -czf results/backup/keys.tar.gz ${dirN}/*key 3>&1 1>&2 2>&3 | egrep -v "tar: Removing leading|tar: Error exit delayed from previous errors") 3>&1 1>&2 2>&3
	    mv matches.init.txt results/backup/
	    rm -f ${dirN}/*key 
	fi

	if $RUN_BUNDLE; then
	 tar -czf results/backup/bundle.tar.gz bundle/
	 rm -rf bundle/
	 rm -f matches.init.txt ${dirN}*key
	 rm -f constraints.txt list.bundler.txt matches.prune.txt matches.ransac.txt 
	 rm -f matches.corresp.txt nmatches.corresp.txt nmatches.prune.txt nmatches.ransac.txt options.txt pairwise_scores.txt
	fi

	if $RUN_PMVS; then
	 rm -rf bundle/
	 mv -f pmvs/transform.txt resultsdd
	 tar -czf results/backup/plys.tar.gz pmvs/models/*ply pmvs/*ply
	 tar -czf results/backup/patches.tar.gz pmvs/models/*patch 
	 tar -czf results/backup/psets.tar.gz pmvs/models/*pset
	 tar -czf results/backup/pmvs.tar.gz pmvs/prep_pmvs.sh pmvs/txt/ pmvs/option*
	 mv -f pmvs/models/*ply results/
	 rm -rf list.rd.txt pmvs/
	fi

	if $RUN_GTRANS; then
	 if [ -d pmvs/ ]; then
	  mv -f pmvs/trans/ results/
	  rm -rf pmvs/
	 fi
	fi
fi
echo "[- Done -]"
