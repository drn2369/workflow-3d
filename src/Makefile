#Top level makefile for Bundler-CMVS/PMVS workflow

default:
	mkdir -p ../bin
# Make siftGPU
	cd sift; $(MAKE)
# Make Bundler
	cd bundler-v0.4-source; $(MAKE)
# Make CMVS/PMVS
	cd pmvs-2/program/main; $(MAKE)
# Make GTrans
	cd GTransform; $(MAKE)
clean:
#Clean siftGPU
	cd sift; $(MAKE) clean
#Clean Bundler
	cd bundler-v0.4-source; $(MAKE) clean
#Clean CMVS/PMVS
	cd pmvs-2/program/main; $(MAKE) clean
#Clean GTrans
	cd GTransform; $(MAKE) clean
