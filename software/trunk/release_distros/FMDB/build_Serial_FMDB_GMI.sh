wget http://www.scorec.rpi.edu/FMDB/source/buildFMDBSerial.env
wget http://www.scorec.rpi.edu/FMDB/source/buildFMDBSerial.sh
wget http://www.scorec.rpi.edu/FMDB/source/auxillaryBuildScripts.tar
tar xvf auxillaryBuildScripts.tar
chmod 755 buildFMDBSerial.sh
source buildFMDBSerial.env 
export CREATE_TARBALLS=0
export SCOREC_SVN=0 
export ENABLE_IMESH=0
export HTTP_DOWNLOAD=http://www.scorec.rpi.edu/FMDB/source
./buildFMDBSerial.sh

