#!/bin/bash

# Download Source

#Bundler
echo "[Downloading Bundler source]"
wget http://phototour.cs.washington.edu/bundler/distr/bundler-v0.4-source.tar.gz
#git clone https://github.com/snavely/bundler_sfm.git

#PMVS
echo "[Downloading PMVS source]"
wget http://www.di.ens.fr/pmvs/pmvs-2-fix0.tar.gz

#CMVS
echo "[Downloading CMVS source]"
wget http://www.di.ens.fr/cmvs/cmvs-fix2.tar.gz

#Set up source tree
echo "[Setting up source tree]"

	#Unpack
	tar -xzf bundler-v0.4-source.tar.gz     
	tar -xzf pmvs-2-fix0.tar.gz
	tar -xzf cmvs-fix2.tar.gz

	#Combine CMVS/PMVS
	cp -r cmvs/program/ pmvs-2/

	#Clean up
	rm *.tar.gz
        rm -rf cmvs/

#Patch Bundler
echo "[Patching]"

	#Fix Orginal Bundler Makefile and BundlerApp.h
	cp patches/Makefile.bundlerOrig bundler-v0.4-source/src/Makefile
	cp patches/BundlerApp.h bundler-v0.4-source/src/
	#Patch PMVS
	patch -p0 -si patches/pmvs.patch
