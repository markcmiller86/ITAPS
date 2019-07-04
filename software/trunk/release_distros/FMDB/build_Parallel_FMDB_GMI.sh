wget http://www.scorec.rpi.edu/FMDB/source/buildFMDBParallel.env
wget http://www.scorec.rpi.edu/FMDB/source/buildFMDBParallel.sh
wget http://www.scorec.rpi.edu/FMDB/source/auxillaryBuildScripts.tar
tar xvf auxillaryBuildScripts.tar
chmod 755 buildFMDBParallel.sh
source buildFMDBParallel.env 
export CREATE_TARBALLS=0
export SCOREC_SVN=0 
export ENABLE_IMESHP=1
export HTTP_DOWNLOAD=http://www.scorec.rpi.edu/FMDB/source
./buildFMDBParallel.sh

