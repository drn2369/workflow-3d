User's Guide
============

This work flow takes a large amount of computational power to run, so the
run time will vary depending heavily on the specs of the system it is run on.
The following graphic shows the process for this work flow. Each step can be
run individually, however, it needs the input from the previous step in order
to run properly. 

Test data
---------
Starting from the base directory the following commands should run each
dataset:
 
 - For the non-geo-accurate reconstruction:
---------------------------
$ cd workspace/demo
$ sh RunProcess.sh -a
---------------------------

 - For the geo-accurate reconstruction:
------------------------------
$ cd workspace/demo
$ sh RunProcess.sh -ag
------------------------------

 - The output of the geo-accurate reconstruction upon successful completion is:
-----------------------------
$ sh RunProcess.sh [-options]
[- Prepping Data -]
[- Running siftGPU -]
[- Running Bundler -]
[- Preparing PMVS -]
[- Running CMVS -]
[- Running PMVS -]
[- Running GTransform -]
[- Clean Up -]
[- Done -]
----------------------------
 
 - The three-dimensional point clouds will be in the results directory upon
   completion. The *.ply files in the root of the reults directory are the
   non-geoaccurate point clouds. If GTransform was run, the geo-accurate
   models will reside in trans/models/.

 - The transformed camera projection matricies are also written out by
   GTransform, they will reside in trans/txt/. 

 - The georegistration transform will reside in trans/transform.txt
   
 - The ply file format can be viewed in many different software packages, the one we recommend is
   link:http://meshlab.sourceforge.net/[Meshlab].

 - There are a number of algorithms that can be used for the meshing process,
   although, none are presented here. Meshlab provides a number of surface
   reconstruction algorithms that work quite well, such as poisson
   reconstruction.

RunProcess.sh
-------------
This script is used to run the work flow and contains a number of options for
controlling how everything is run.

Usage
~~~~~
sh RunProcess.sh [-options]

Options
~~~~~~~
*-a* ::
    Run the whole work flow (equivalent to -sbp)
*-s* ::
    Run siftGPU
*-b* ::
    Run Bundler
*-p* ::
    Run CMVS/PMVS
*-g* ::
    Run GTransform
*-k* ::
    Prevent the script from running cleanup. This will keep all the output of
    siftGPU, Bundler, CMVS/PMVS, and GTransform.
*-d = 0* ::
    Set the GPU device number for siftGPU.
*-x = 2000* ::
    Set the maximum dimension for an image for siftGPU, this will cause
    siftGPU to down sample images larger than this dimension.
*-y = 8000* ::
    Set the maximum number of features for siftGPU. This is useful for running
    siftGPU at full resolution but limiting the memory usage.
*-f = 6111.11* ::
    Set the focal length of the camera used to take the images, currently
    assuming all images taken with the same camera at the same focal length.
    The focal length must be in pixel units (i.e. focal length in mm / pixel
    size in mm).
*-i = 0.0001* ::
    Set the focal length constraint weight for the bundler adjustment in
    Bundler.
*-c = 30* ::
    Set the max cluster size for CMVS.
*-l = 1* ::
    Set the level for PMVS.
*-e = 2* ::
    Set the cell size for PMVS.
*-t = 0.7* ::
    Set the threshold for PMVS.
*-w = 7* ::
    Set the window size for PMVS.
*-m = 3* ::
    Set the min image number for PMVS.
*-u = 2*  ::
    Set the number of CPUs to use for CMVS and PMVS.

Example Usage
~~~~~~~~~~~~~

Running the whole work flow on a set of images with a focal length of 5000
pixels, using 6 CPUs, and with a PMVS threshold of 0.9:

--------------------------------------
$ sh RunProcess -a -f 5000 -u 6 -t 0.9
--------------------------------------

Alternatively, you can edit the default values in RunProcess.sh to change
parameters.

Using your own data
-------------------
For running the work flow in non-geo-accurate mode, your data can be run in
the same fashion as the example for the non-geo-accurate test data. Remember
to set the appropriate focal lengths by using -f focal length or altering the
default value in RunProcess.sh .

For running the work flow in geo-accurate mode, each image must be accompanied
by a POS file named imageFileName.pos. Each POS file must contain the X,Y, and
Z positions of the camera frame. These coordinates should be in the desired
Euclidean output coordinate system (i.e. UTM, NEU, etc).
