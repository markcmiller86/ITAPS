wget http://www.scorec.rpi.edu/FMDB/source/buildFMDBParallel.env
wget http://www.scorec.rpi.edu/FMDB/source/buildFMDBParallel.sh
wget http://www.scorec.rpi.edu/FMDB/source/auxillaryBuildScripts.tar
tar xvf auxillaryBuildScripts.tar
source buildFMDBParallel.env 
chmod 755 buildFMDBParallel.sh
./buildFMDBParallel.sh

