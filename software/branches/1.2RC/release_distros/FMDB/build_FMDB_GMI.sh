wget http://www.scorec.rpi.edu/FMDB/source/buildFMDBSerial.env
wget http://www.scorec.rpi.edu/FMDB/source/buildFMDBSerial.sh
chmod 755 buildFMDBSerial.sh
source buildFMDBSerial.env 
export SCOREC_SVN=0 
./buildFMDBSerial.sh

