#!/bin/bash

# *****************************************************************************
#
# Script: iMeshP_test
#
# Purpose:
#    This script is used to run iMeshP UnitTests for ITAPS implementations
#
# Initial Implementation:
#    April 2011
#
# Script layout:
#   Section 1:  Initializations & Usage Information
#               usage: information on command line options
#
#   Section 2:  File & Error Handling Routines
#               uncompress_untar:    uncompress and untar source code tar-balls
#               checkHandleError:    enter error information into error log file
#               apply_error_filters: remove erroneous messages
#
#   Section 3:  MOAB Routines
#               get_source_MOAB:     get source code from defined location    
#               get_builddir_MOAB:   find directory in which source was built
#               build_check_MOAB:    configure, build and verify build 
#
#   Section 4:  FMDB Routines
#               get_source_FMDB:     get source code from defined location
#               get_builddir_FMDB:   find directory in which source was built
#               build_check_FMDB:    configure, build and verify build 
#
#   Section 5:  IPComman Routines
#               get_source_IPCom:     get source code from defined location
#               get_builddir_IPCom:   find directory in which source was built
#               build_check_IPCom:    configure, build and verify build 
#
#  Section 6:   Zoltan & ParMETIS Routines
#               get_source_ZOLTAN:     get source code from defined location
#               get_builddir_ZOLTAN:   find directory in which source was built
#               build_check_ZOLTAN:    configure, build and verify build 
#
#  Section 7:   UnitTestP Routines
#               get_source_unitTestP:     get source code from defined location
#               get_builddir_unitTestP:   find directory in which source was built
#               build_run_unitTestP:    configure, build and verify build 
#
#   Summary File Routines:
#               create_exec_summary: create PASS/FAIL summary in html
#               create_html_index: create list of *.results files in html
#               create_summary: create test results summary file
#               mail_summary:   email test results summary file
#
#   'Main' Program:
#               Process command line arguments
#               Building MOAB
#               Building FMDB
#               Building IPComman
#               Building ParMETIS & Zoltan
#               Building unitTestP
#               Run unitTestP for each implementation
#               create summary results file
#
#
# Error Return Codes
#     0 - NO errors 
#    10 - file compression error
#    20 - file checkout error
#    30 - configuration error
#    35 - make error
#    40 - make install error
#    50 - cannot find file error
#    55 - cannot find directory error
#
#
# Modifications:
#
#   Ellen M. Hill, Tue Apr 12 16:17:22 PDT 2011
#   Initial implementation started; adapted from run_iMesh_test.sh
# *****************************************************************************

# *****************************************************************************
#     Section 1 - Initializations
# -----------------------------------------------------------------------------
#  This section initializes:
#     file names, paths and version numbers for each implementation
#     implementation names
#     error log file
#     total error counter
#     mesh definition file name
#     help/usage routine
# *****************************************************************************

# *****************************************************************************
#     Set file names and version numbers
# *****************************************************************************
export ITAPS_BRANCH="branches/1.4RC"

export MOAB_VERSION="4.5.0RC1"
export MOAB_SOURCE="moab-${MOAB_VERSION}.tar.gz"
export MOAB_LINK=""
export MOAB_PATH="http://ftp.mcs.anl.gov/pub/fathom"
export MOAB_MESHFILE="MOAB_iMeshP_test_file.h5m"

export FMDB_VERSION=""
export FMDB_SOURCE="build_Parallel_FMDB_GMI.sh"
export FMDB_LINK=""
export FMDB_PATH="http://www.scorec.rpi.edu/FMDB/source"
export FMDB_MESHBASE="24quad_p"                           ##  this is the base name for the files
export FMDB_MESHFILE="24quad_p0.sms"

export IPCOMM_VERSION="1.0"
export IPCOMM_SOURCE="ipcomman-${IPCOMM_VERSION}.tar.gz"
export IPCOMM_LINK=""
export IPCOMM_PATH="https://www.scorec.rpi.edu/FMDB/source"

export ZOLTAN_VERSION="3.601"
export ZOLTAN_SOURCE="zoltan_distrib_v${ZOLTAN_VERSION}.tar.gz"
export ZOLTAN_LINK=""
export ZOLTAN_PATH="http://www.cs.sandia.gov/~kddevin/Zoltan_Distributions"

export IZOLTANP_VERSION="0.8"
export IZOLTANP_SOURCE="izoltanparallel-${IZOLTANP_VERSION}.tar.gz"
export IZOLTANP_LINK=""
export IZOLTANP_PATH="https://redmine.scorec.rpi.edu/anonsvn/itaps/software/${ITAPS_BRANCH}/release_distros"

export ITAPS_MESQ_VERSION="2.99"
export ITAPS_MESQ_SOURCE="mesquite-${ITAPS_MESQ_VERSION}.tar.gz"
export ITAPS_MESQ_LINK=""
export ITAPS_MESQ_PATH="http://software.sandia.gov/~bktidwe"


export IMESHP_PATH="https://redmine.scorec.rpi.edu/anonsvn/itaps/software/${ITAPS_BRANCH}/interfaces/iMeshP/test/unit"

export IMESH_DATA="https://redmine.scorec.rpi.edu/anonsvn/itaps/software/${ITAPS_BRANCH}/interfaces/iMesh/data"

export IMESHP_DATA="https://redmine.scorec.rpi.edu/anonsvn/itaps/software/${ITAPS_BRANCH}/interfaces/iMeshP/data"

export IMESH_HDR_PATH="https://redmine.scorec.rpi.edu/anonsvn/itaps/software/${ITAPS_BRANCH}/interfaces/iMesh/spec"

export IBASE_HDR_PATH="https://redmine.scorec.rpi.edu/anonsvn/itaps/software/${ITAPS_BRANCH}/interfaces/iBase/spec"

export IMESHP_HDR_PATH="https://redmine.scorec.rpi.edu/anonsvn/itaps/software/${ITAPS_BRANCH}/interfaces/iMeshP/spec"

export SUPPRESS_FILE="https://redmine.scorec.rpi.edu/anonsvn/itaps/software/${ITAPS_BRANCH}/tools/ITAPS.supp"

# *****************************************************************************
#     Set directory names for test execution and results
# *****************************************************************************
export TEST_RESULTS_DIR="/usr/local/itaps/testing/test_results"

if [ ! -d ${TEST_RESULTS_DIR} ]
then
   mkdir ${TEST_RESULTS_DIR}
fi


# *****************************************************************************
#     Set date & time of tests
# *****************************************************************************
datetim=`date '+%c'`
rundate=`date '+%Y%m%d'`


# *****************************************************************************
#     Set implementaton names
# *****************************************************************************
export IMPL_NAMES="MOAB FMDB"


# *****************************************************************************
#     set number of implementations and number of options
# *****************************************************************************
num_impl=2
num_opts=18

max_ind=$((num_impl*num_opts))

# *****************************************************************************
#     Initialize output file array to 'DNR' and set starting indicies
#     for each implementation
# *****************************************************************************
declare -a filarr
declare -a pfarr

indm=0            #MOAB starting index
indf=$num_opts    #FMDB starting index

for (( i=0; i<${max_ind}; i++ )) 
do 
   filarr[$i]="ZZZ"
done


# *****************************************************************************
#     Set name of iMesh definition file
# *****************************************************************************
DEFS_FILE="iMesh-Defs.inc"
DEFSP_FILE="iMeshP-Defs.inc"


# *****************************************************************************
#     help/how-to-use routine
# *****************************************************************************
usage()
{
   printf "\n"
   printf "Usage: %s [options]\n" $0
   printf "\n"
   printf "%-15s %s\n" "-h or --help" "Prints this help package and terminates"
   printf "\n"
   printf "The following flags effect global behavior:\n"
   printf "   %-20s %s [Default: %s]\n" "--no-clean-build" \
                       "Do not remove build directories"  "$CLEAN_BUILD"
   printf "   %-20s %s [Default: %s]\n" "--no-email" \
                       "Do not email test results." "$EMAIL_SUMMARY"
   printf "\n"
   printf "The following flags DISABLE the downloads for each code:\n"
   printf "   %-11s %s [Default: %s]\n" "--no-moab" "Do NOT download MOAB" \
                                     "$GET_MOAB"
   printf "   %-11s %s [Default: %s]\n" "--no-fmdb" "Do NOT download FMDB" \
                                     "$GET_FMDB"
   printf "   %-11s %s [Default: %s]\n" "--no-ipcomm" "Do NOT download IPComman" \
                                     "$GET_IPCOMM"
   printf "   %-11s %s [Default: %s]\n" "--no-zoltan" "Do NOT download Zoltan" \
                                     "$GET_ZOLTAN"
   printf "   %-11s %s [Default: %s]\n" "--no-imeshp" \
                                   "Do NOT download iMesh_unitTestP" "$GET_IMESHP"
   printf "\n"
   printf "The following flags are used to change the default version number "
   printf "for each code:\n"
   printf "   %-16s %s [Default: %s]\n" "--moab-version" \
                                     "Download MOAB version <number>" \
                                     "$MOAB_VERSION"
   printf "   %-16s %s [Default: %s]\n" "--fmdb-version" \
                                     "Download FMDB version <number>" \
                                     "$FMDB_VERSION"
   printf "   %-16s %s [Default: %s]\n" "--ipcomm-version" \
                                     "Download IPComman version <number>" \
                                     "$IPCOMM_VERSION"
   printf "   %-16s %s [Default: %s]\n" "--zoltan-version" \
                                     "Download Zoltan version <number>" \
                                     "$ZOLTAN_VERSION"
   printf "\n"
   printf "The following flags are used to change the default directory path "
   printf "for each code:\n"
   printf "   %-13s %s [Default: %s]\n" "--moab-path" "/path/to/MOAB" \
                                     "$MOAB_PATH"
   printf "   %-13s %s [Default: %s]\n" "--fmdb-path" "/path/to/FMDB" \
                                     "$FMDB_PATH"
   printf "   %-13s %s [Default: %s]\n" "--ipcomm-path" "/path/to/IPComman" \
                                     "$ITAPS_REFI_PATH"
   printf "   %-13s %s [Default: %s]\n" "--mesq-path" "/path/to/Zoltan&ParMETIS" \
                                     "$ZOLTAN_PATH"
   printf "   %-13s %s [Default: %s]\n" "--imeshp-path" "/path/to/IMESHP_unitTest" \
                                     "$IMESHP_PATH"
   printf "\n"
   printf "The following flags are used to change the default input mesh file\n"
   printf "   %-13s %s [Default: %s]\n" "--moab-file" "Use specified input mesh file" \
                                     "$MOAB_MESHFILE"
   printf "   %-13s %s [Default: %s]\n" "--fmdb-file" "Use specified input mesh file" \
                                     "$FMDB_MESHFILE"
   printf "\n"
}



# *****************************************************************************
#     Section 2 - File & Error Handling Routines
# -----------------------------------------------------------------------------
# *****************************************************************************
# Function: uncompress_untar
#
# Purpose: Uncompress and untar file
#
# Programmer: Ellen Hill (modified from build_visit script)
# Date: Mon Nov 2 2009
#
# *****************************************************************************
function uncompress_untar
{
   if [[ "$1" = "" ]]
   then
      return 0            # source code is not in tar file; was likely acquired with SVN co
   fi

   if [[ $(echo $1 | egrep "\.gz$" ) != "" ]]
   then
      COMPRESSTYPE="gzip"
   elif [[ $(echo $1 | egrep "\.bz2$" ) != "" ]]
   then
      COMPRESSTYPE="bzip"
   elif [[ $(echo $1 | egrep "\.tgz$" ) != "" ]]
   then
      COMPRESSTYPE="tarzip"
   elif [[ $(echo $1 | egrep "\.tar$" ) != "" ]]
   then
      COMPRESSTYPE="tarball"
   elif [[ $(echo $1 | egrep "\.sh$" ) != "" ]]
   then
      COMPRESSTYPE="script"
   else
      echo "Unsupported compression type, error return"
      return 10
   fi

   case $COMPRESSTYPE in
      gzip)
           gunzip -f $1
           tar xf ${1%.gz}
           ;;
     tarball)
           tar xf $1
           ;;
      tarzip)
           gunzip -f $1
           tar xf "${1%.tgz}.tar"
           ;;
      bzip)
           bunzip2 $1
           tar xf ${1%.bz2}
           ;;
   esac
   return 0
}

# *****************************************************************************
# Function: checkHandleError
#
# Purpose: Logs information for errors
#
# Programmer: Ellen Hill (modified from Mark Miller script)
# Date: Mon Dec 7 2009
#
# *****************************************************************************
function checkHandleError
{

#     initialize argument variables
   local errorStatus="999"
   local shortSubject=""
   local errorDetails=""

#     get argument values
   local errorStatus=$1
   local shortSubject=$2
   local errorDetails=$3
   local timeofError=`date '+%I:%M:%S %p'`

##     return if error status is zero; no error occurred
   if [[ "$errorStatus" == "0" ]]
   then
      return 0
   fi

##    increment total error counter
   errTot=`expr $errTot + 1`

##     put info into error log file
   echo "*********************************************************" >> $ERRORLOG
   echo "Message #${errTot} [$timeofError]: $shortSubject" >> $ERRORLOG

   case $errorStatus in
      10) echo "unsupported file compression type error" >> $ERRORLOG
          ;;
      20) echo "file checkout error" >> $ERRORLOG
          ;;
      30) echo "CONFIGURE error" >> $ERRORLOG
          ;;
      35) echo "MAKE error" >> $ERRORLOG
          ;;
      40) echo "MAKE INSTALL error" >> $ERRORLOG
          ;;
      50) echo "cannot find file error" >> $ERRORLOG
          ;;
      55) echo "cannot find directory error" >> $ERRORLOG
          ;;
   esac

   if [[ "$errorDetails" != "" ]]
   then
      echo "Details follow . . . " >> $ERRORLOG
      if [[ -e $errorDetails ]]
      then
         cat $errorDetails >> $ERRORLOG
      else
         echo $errorDetails >> $ERRORLOG
      fi
   else
      echo "Error flag = " $errorStatus >> $ERRORLOG
   fi

##     add blank lines in preparation for next error
   echo "" >> $ERRORLOG
   echo "" >> $ERRORLOG

   return 0
}


# *****************************************************************************
# Function: apply_error_filters
#
# Purpose: Set error messages to be removed
#
# Programmer: Ellen Hill 
# Date: Mon March 15 2010
#
# *****************************************************************************
function apply_error_filters ()
{
   ErrorFile=$1
   if [ -s "${ErrorFile}" ]
   then
      for ii in \
         'all-recursive' \
         'ar:\ creating' \
         'make' \
         'Making' \
         'HAVE_CONFIG_H' \
         '.deps' \
         '.libs' \
         'libtool'
         do
            if (egrep "${ii}" $ErrorFile >/dev/null)
            then
               mv $ErrorFile $ErrorFile.tmp
               egrep -v "${ii}" $ErrorFile.tmp > $ErrorFile
               echo "-- applied filter:${ii}" >> $ErrorFile.orig
               cat $ErrorFile.tmp >> $ErrorFile.orig
            fi
         done
   fi
}


# *****************************************************************************
# Function: find_MPI
#
# Purpose: find directory & name of MPI
#
# Programmer: Ellen Hill 
# Date: Mon June 20 2011
#
# *****************************************************************************
function find_MPI
{

#
#     find MPI wrapper code; i.e. mpiCC
    echo ""
    echo "Find MPI . . ."

    MPI_DIR=""
    MPI_NAME=""
    MPINAMES="mpiCC mpicxx mpic++"
    MPIDIRS="/usr/bin /usr/local/bin"

    for mdir in ${MPIDIRS}
    do
       for mname in ${MPINAMES}
       do
          if [ -x ${mdir}/${mname} ]
          then
             export MPI_NAME=${mname}
             export MPI_DIR=${mdir}
             break
          fi
       done
    done

    if [ "${MPI_NAME}" = "" ] || [ "${MPI_DIR}" = "" ]
    then
       echo "Can NOT find MPI; Giving up"
       return 50
    else
       echo "MPI Directory: " ${MPI_DIR}
       echo "MPI Code: " ${MPI_NAME}
    fi

#
#     Get MPI build command then extract compiler name
    tmpmpi="mpifile"
    ${MPI_NAME} -show > $tmpmpi
    while read InpL
    do
        case $InpL in
                 "") 
                     ;;
           "Build"*) 
                     ;;
                  *) 
                     export MPI_BUILD=`echo $InpL | cut -d" " -f2-`
                     ;;
       esac
    done < $tmpmpi
    rm -rf $tmpmpi
}


# *****************************************************************************
#     Section 3 - MOAB Routines
# -----------------------------------------------------------------------------
# *****************************************************************************
# Function: get_source_MOAB
#
# Purpose:  download MOAB source code from distribution URL
#
#           The checkout.log file has the status of the file checkout.  When
#           complete the next-to-last line contains the time, file size and
#           name as well as the word 'saved'.  Completion of the file checkout
#           is determined when the word 'saved' is found.  On some systems it is
#           the 6th symbol and on others it is the 7th symbol (when the date is 
#           also included). 
#
#           NOTE:  If the file cannot be download within 2 minutes an error
#                  message is written and immediate return from this routine
#
# Programmer: Ellen Hill (modified from build_visit script)
# Date: Tue Apr 12 2011
#
# *****************************************************************************
function get_source_MOAB
{
    echo "Getting ${MOAB_SOURCE} from ${MOAB_PATH}. . . "
    wget ${MOAB_PATH}/${MOAB_SOURCE} \
         1> MOAB.checkout 2> MOAB.checkout.log

    sleep 30
    timelimit=0
    retval=0
    if [[ -z MOAB.checkout.log ]]
    then
       retval=20
    else
       endlog1=`tail --lines=2 MOAB.checkout.log | cut -d" " -f6`
       endlog2=`tail --lines=2 MOAB.checkout.log | cut -d" " -f7`
       while [[ "$endlog1" != "saved"  &&  "$endlog2" != "saved" ]]
       do
          sleep 60
          endlog1=`tail --lines=2 MOAB.checkout.log | cut -d" " -f6`
          endlog2=`tail --lines=2 MOAB.checkout.log | cut -d" " -f7`
          timelimit=`expr $timelimit + 1`
          if [[ "$timelimit" == "2" ]]
          then
             echo "Problem checking out MOAB source code"
             retval=20
             break
          fi
       done
    fi

#     remove MOAB.checkout if it is an empty file
    find . -name 'MOAB.checkout' -size 0 -exec rm -f {} \;

#
#     set error flag for summary file
    if [[ "$retval" != "0" ]]
    then
       numerr=`grep -i -e "error" MOAB.checkout.log | wc -l`
       filarr[$indm]="${numerr}:MOAB.checkout.log"
       indm=`expr $indm + 1`
       filarr[$indm]="DNR"
       indm=`expr $indm + 1`
       filarr[$indm]="DNR"
       indm=`expr $indm + 1`
       filarr[$indm]="DNR"
       indm=`expr $indm + 1`
       filarr[$indm]="DNR"
       indm=`expr $indm + 1`
       return 20
    else
       filarr[$indm]=""
       if [[ ! -z "$(cat MOAB.checkout.log)" ]]
       then
          numwarn=`grep -i -e "warn" MOAB.checkout.log|wc -l`
          if [[ "$numwarn" != "0" ]]
          then
             filarr[$indm]="WARN:MOAB.checkout.log"
          fi
       fi
       indm=`expr $indm + 1`
    fi
    
    return 0
}


# *****************************************************************************
# Function: get_builddir_MOAB
#
# Purpose: Find the MOAB directory name into which the source code 
#          was untarred
#
# Programmer: Ellen Hill 
# Date: Tue Apr 12 2011
#
# *****************************************************************************
function get_builddir_MOAB
{
    MOAB_BUILD_DIR=""
    files=`ls`
    for moab_dir in ${files} 
    do
       if [[ -d ${moab_dir} ]]
       then
          case ${moab_dir} in
             *MO* | mo*)
                 export MOAB_BUILD_DIR=`pwd`/${moab_dir}
                 break
                 ;;
               *)
                 ;;
          esac
       fi
    done
    if [[ -z $MOAB_BUILD_DIR ]]
    then
       echo "Cannot find MOAB source code directory; error return"
       return 55
    else
       echo " MOAB build directory: ${MOAB_BUILD_DIR}"
       return 0
    fi
}


# *************************************************************************** #
# Function: build_check_MOAB
#
# Purpose: Builds ITAPS MOAB implementation
#          Configure and make MOAB with outputs going into log and err
#             files, respectively
#          directories containing the iMesh-Defs.h are found and LD_LIBRARY_PATH
#             updated to include them
#          Run 'make check' for internal test cases
#
# Programmer: Ellen Hill (modified from build_visit Function 8.15)
# Date: Tue Apr 12 2011
# *************************************************************************** #
function build_check_MOAB
{
    MOAB_DEFS_DIR=""

    echo "Configuring MOAB. . ."
    MOAB_INSTALL_DIR="$MOAB_BUILD_DIR/iMeshDir"
    cd $MOAB_BUILD_DIR || echo "Cannot cd to $MOAB_BUILD_DIR build dir."
    ./configure --enable-imesh --with-mpi --with-zlib \
                --with-hdf5=/usr/local/itaps/hdf5-1.8.6-parallel \
                --prefix=$MOAB_INSTALL_DIR \
                1> MOAB.config.log 2> MOAB.config.err
    retval=$?
    cat config.log >> MOAB.config.log

#     save MOAB config error return value & file name (if non-zero length)
#     if MOAB config failed, set MOAB make to N/A and MOAB make-install to DNR
    if [[ "$retval" != "0" ]] 
    then
       echo "MOAB configure failed, Giving up"
       numerr=`grep -i -e "error" MOAB.config.err | wc -l`
       filarr[$indm]="${numerr}:MOAB.config.err"
       indm=`expr $indm + 1`
       filarr[$indm]="DNR"
       indm=`expr $indm + 1`
       filarr[$indm]="DNR"
       indm=`expr $indm + 1`
       filarr[$indm]="DNR"
       indm=`expr $indm + 1`
       return 30
    else
       filarr[$indm]=""
       if [[ ! -z "$(cat MOAB.config.err)" ]]
       then
          numwarn=`grep -i -e "warn" MOAB.config.err|wc -l`
          if [[ "$numwarn" != "0" ]]
          then
             filarr[$indm]="WARN:MOAB.config.err"
          fi
       fi
       indm=`expr $indm + 1`
    fi

    echo "Building MOAB. . . (~10 minutes)"
    make 1> MOAB.make.log 2> MOAB.make.err
    retval=$?

#     save MOAB make error return value & file name (if non-zero length)
    if [[ "$retval" != "0" ]] 
    then
       echo "MOAB make failed, Giving up"
       numerr=`grep -i -e "error" MOAB.make.err | wc -l`
       filarr[$indm]="${numerr}:MOAB.make.err"
       indm=`expr $indm + 1`
       filarr[$indm]="DNR"
       indm=`expr $indm + 1`
       filarr[$indm]="DNR"
       indm=`expr $indm + 1`
       return 40
    else
       filarr[$indm]=""
       if [[ ! -z "$(cat MOAB.make.err)" ]]
       then
          numwarn=`grep -i -e "warn" MOAB.make.err|wc -l`
          if [[ "$numwarn" != "0" ]]
          then
             filarr[$indm]="WARN:MOAB.make.err"
          fi
       fi
       indm=`expr $indm + 1`
    fi

    echo "Installing MOAB. . . "
    make install 1> MOAB.mkinstall.log 2> MOAB.mkinstall.err
    retval=$?

#     save MOAB make-install error return value & file name (if non-zero length)
    if [[ "$retval" != "0" ]] 
    then
       echo "MOAB make-install failed, Giving up"
       numerr=`grep -i -e "error" MOAB.mkinstall.err | wc -l`
       filarr[$indm]="${numerr}:MOAB.mkinstall.err"
       indm=`expr $indm + 1`
       filarr[$indm]="DNR"
       indm=`expr $indm + 1`
       return 40
    else
       filarr[$indm]=""
       if [[ ! -z "$(cat MOAB.mkinstall.err)" ]]
       then
          numwarn=`grep -i -e "warn" MOAB.mkinstall.err|wc -l`
          if [[ "$numwarn" != "0" ]]
          then
             filarr[$indm]="WARN:MOAB.mkinstall.err"
          fi
       fi
       indm=`expr $indm + 1`
    fi

    echo "Done building MOAB"

#     find directory containing iMeshP-Defs.inc file
    defs_dirs=`find ${MOAB_INSTALL_DIR} -name ${DEFSP_FILE}`
    if [[ ${defs_dirs} == "" ]] 
    then    #### try to find iMesh-Defs.inc
       defs_dirs=`find ${MOAB_INSTALL_DIR} -name ${DEFS_FILE}`
       if [[ ${defs_dirs} == "" ]] 
       then 
          echo "Cannot find ${DEFSP_FILE} or ${DEFS_FILE} for MOAB, error return"
          return 50
       else
          MOAB_DEFS_FILE=${DEFS_FILE}
       fi
    else
       MOAB_DEFS_FILE=${DEFSP_FILE}
    fi
    MOAB_DEFS_DIR=`dirname ${defs_dirs}`

    echo " MOAB ${DEFS_FILE} is stored in directory:"
    echo "    ${MOAB_DEFS_DIR}"
    export LD_LIBRARY_PATH="$MOAB_DEFS_DIR":"$LD_LIBRARY_PATH"

#     Run internal tests
    make check > moab_results
    
#     count number of tests passed and failed
    passed=0
    failed=0

    while read InputLine
    do
       case $InputLine in
          *PASS*) passed=`expr $passed + 1`
                  ;;
          *FAIL*) failed=`expr $failed + 1`
                  ;;
       esac
    done < moab_results

    if [[ $failed > 0 ]]
    then
       filarr[$indm]="${failed}:moab_results"
       indm=`expr $indm + 1`
    else
       filarr[$indm]=""
       indm=`expr $indm + 1`
    fi

    num_tsts=`expr $passed + $failed`
    if [[ $passed > 0 ]]
    then
       echo " MOAB Check Test Results: $passed/$num_tsts PASSED"
       return 0
    fi

    return $failed
}


# *************************************************************************** #
# Function: run_MOAB_unitTest
#
# Purpose: Builds and runs MOAB unitTests
#
# Programmer: Ellen Hill
# Date: Tue May 17 2011 13:22;43 PDT
#
# *************************************************************************** #
function run_MOAB_unitTest
{
    cd $IMESHP_BUILD_DIR || echo "Cannot cd to $IMESHP_BUILD_DIR build dir."

#     save input arguments
    IMPL_NAME=$1
    IMPL_BUILD_DIR=$2

#     initialize pass/fail values
    make_err=999
    run_err=999
    valg_err=999

#
# if build directory is NULL, return error
    if [[ "${IMPL_BUILD_DIR}" = "" ]]
    then
       return 55
    fi

#
# inititalize return value
    retval=0

#
# remove any existing files
    if [[ -x MOAB_iMeshP_unitTest ]]
    then
       rm -rf *.o MOAB_iMeshP_unitTest
    fi

#
#     Create make file for building iMeshP_unitTest
    create_iMeshP_Make ${IMPL_NAME}_unitTest ${IMPL_BUILD_DIR}

    echo " "
    echo "Building MOAB_iMeshP_unitTest . . ."
    make -f Makefile.${IMPL_NAME}_unitTest 1> imeshpMOAB_${IMPL_NAME}.make.log \
                                           2> imeshpMOAB_${IMPL_NAME}.make.err

    if [ -x MOAB_iMeshP_unitTest ] 
    then
       make_err=0
       echo "Running MOAB_iMeshP_unitTest for ${IMPL_NAME} . . ."
       if [ "${IMPL_NAME}" = "FMDB" ]
       then
          run_cmd="./MOAB_iMeshP_unitTest -i ${FMDB_MESHBASE}"
       else
          run_cmd="./MOAB_iMeshP_unitTest"
       fi
       mpirun -np 6 ${run_cmd} \
            1> imeshpMOAB_${IMPL_NAME}.out 2> imeshpMOAB_${IMPL_NAME}.msg
       run_err=$?

       echo "Running MOAB_iMeshP_unitTest with valgrind for ${IMPL_NAME} . . ."
       mpirun -np 6 valgrind --suppressions=../ITAPS.supp --show-reachable=yes \
            --leak-check=full ${run_cmd} \
               1> imeshpMOAB_${IMPL_NAME}.valg.out 2> imeshpMOAB_${IMPL_NAME}.valg.msg
       valg_err=$?
    else
       make_err=1
    fi

    echo "Done running MOAB_iMeshP_unitTest for ${IMPL_NAME}"
    echo " "
    
    return 0
}

# *****************************************************************************
#     Section 5 - FMDB Routines
# -----------------------------------------------------------------------------
# *****************************************************************************
# Function: get_source_FMDB
#
# Purpose:  download FMDB source code from distribution URL
#
#           The checkout.log file has the status of the file checkout.  When
#           complete the next-to-last line contains the time, file size and
#           name as well as the word 'saved'.  Completion of the file checkout
#           is determined when the word 'saved' is found.  On some systems it is
#           the 6th symbol and on others it is the 7th symbol (when the date is 
#           also included). 
#
#           NOTE:  If the file cannot be download within 2 minutes an error
#                  message is written and immediate return from this routine
#
# Programmer: Ellen Hill 
# Date: Tue Apr 12 2011
#
# *****************************************************************************
function get_source_FMDB
{
    echo "Getting ${FMDB_SOURCE} from ${FMDB_PATH}. . . "
    wget ${FMDB_PATH}/${FMDB_SOURCE} \
         1> FMDB.checkout 2> FMDB.checkout.log

    sleep 30
    timelimit=0
    retval=0
    if [[ -z FMDB.checkout.log ]]
    then
       retval=20
    else
       endlog1=`tail --lines=2 FMDB.checkout.log | cut -d" " -f6`
       endlog2=`tail --lines=2 FMDB.checkout.log | cut -d" " -f7`
       while [[ "$endlog1" != "saved" &&  "$endlog2" != "saved"  ]]
       do
          sleep 60
          endlog1=`tail --lines=2 FMDB.checkout.log | cut -d" " -f6`
          endlog2=`tail --lines=2 FMDB.checkout.log | cut -d" " -f7`
          timelimit=`expr $timelimit + 1`
          if [[ "$timelimit" == "2" ]]
          then
             echo "Problem checking out FMDB source code"
             retval=20
             break
          fi
       done
    fi

#     remove FMDB.checkout if it is an empty file
    find . -name 'FMDB.checkout' -size 0 -exec rm -f {} \;

#
#     set error flag for summary file
    if [[ "$retval" != "0" ]]
    then
       numerr=`grep -i -e "error" FMDB.checkout.log | wc -l`
       filarr[$indf]="${numerr}:FMDB.checkout.log"
       indf=`expr $indf + 1`
       filarr[$indf]="DNR"
       indf=`expr $indf + 1`
       filarr[$indf]="DNR"
       indf=`expr $indf + 1`
       filarr[$indf]="DNR"
       indf=`expr $indf + 1`
       filarr[$indf]="DNR"
       indf=`expr $indf + 1`
       return 20
    else
       filarr[$indf]=""
       if [[ ! -z "$(cat FMDB.checkout.log)" ]]
       then
          numwarn=`grep -i -e "warn" FMDB.checkout.log|wc -l`
          if [[ "$numwarn" != "0" ]]
          then
             filarr[$indf]="WARN:FMDB.checkout.log"
          fi
       fi
       indf=`expr $indf + 1`
    fi

#     change permissions to make script executable
    chmod +x ${FMDB_SOURCE}

    return 0
}


# *************************************************************************** #
# Function: build_check_FMDB
#
# Purpose: Builds ITAPS FMDB implementation
#
# Programmer: Ellen Hill (modified from build_visit Function 8.15)
# Date: Tue Apr 12 2011
# *************************************************************************** #
function build_check_FMDB
{

    echo "Building FMDB. . ."
#
#     set environment to build FMDB with iMeshP
    export ENABLE_ITAPS=1

#
#     make script executable
    chmod +x ${FMDB_SOURCE}

    ./${FMDB_SOURCE} 1> FMDB.config.log 2> FMDB.config.err
    retval=$?

#     save FMDB error return value & file name (if non-zero length)
#     the build script controls config & make for FMDB so all messages 
#     are in the *.config.* files; so the same file is used for config/make
#     and make-install; no option for make-test
    if [[ "$retval" != "0" ]] 
    then
       echo "FMDB configure failed, Giving up"
       numerr=`grep -i -e "error" FMDB.config.err | wc -l`
       filarr[$indf]="${numerr}:FMDB.config.err"
       indf=`expr $indf + 1`
       filarr[$indf]="DNR"
       indf=`expr $indf + 1`
       filarr[$indf]="DNR"
       indf=`expr $indf + 1`
       filarr[$indf]="N/A"
       indf=`expr $indf + 1`
    else
       filarr[$indf]=""
       if [[ ! -z "$(cat FMDB.config.err)" ]]
       then
          numwarn=`grep -i -e "warn" FMDB.config.err|wc -l`
          if [[ "$numwarn" != "0" ]]
          then
             filarr[$indf]="WARN:FMDB.config.err"
          fi
       fi
       indf=`expr $indf + 1`
       filarr[$indf]=""
       indf=`expr $indf + 1`
       filarr[$indf]=""
       indf=`expr $indf + 1`
       filarr[$indf]=""
       indf=`expr $indf + 1`
    fi

    echo "Done building FMDB"

#     find name of directory in which FMDB was built
#     create tar file of SCOREC_Software directory
    FMDB_BUILD_DIR=""
    files=`ls`
    for fmdb_dir in ${files} 
    do
       if [[ -d ${fmdb_dir} ]]
       then
          case ${fmdb_dir} in
             SCOREC*)
                 tar -cf FMDB.tar SCOREC_Software/
                 bldfiles=`ls SCOREC*`
                 for bldir in ${bldfiles} 
                 do
                    case ${bldir} in
                       FMDB-*)
                              export FMDB_BUILD_DIR=`pwd`/${fmdb_dir}/${bldir}
                              break
                              ;;
                    esac
                 done
                 ;;
             FMDB*)
                 export FMDB_BUILD_DIR=`pwd`/${fmdb_dir}
                 break
                 ;;
               *)
                 ;;
          esac
       fi
    done

    cat ${FMDB_BUILD_DIR}/config.log >> FMDB.config.log

#     print error message and exit if build directory not found

#     copy *.log and *.err to the defined build dir so they are accounted for
#     in the summaty files
    if [[ -z $FMDB_BUILD_DIR ]]
    then
       echo "Cannot find FMDB source code directory; error return"
       return 55
    else
       echo " FMDB build directory: ${FMDB_BUILD_DIR}"
       cp -f FMDB.c* ${FMDB_BUILD_DIR}
    fi

#     find installation directory
    files=`ls`
    for fmdb_dir in ${files} 
    do
       if [[ -d ${fmdb_dir} ]]
       then
          case ${fmdb_dir} in
             FMDB*|fmdb*)
                 if [[ -d ${fmdb_dir}/lib ]]
                 then
                    export FMDB_INSTALL_DIR=`pwd`/${fmdb_dir}
                    break
                 fi
                 ;;
               *)
                 ;;
          esac
       fi
    done

#     find directory containing iMeshP-Defs.inc file
    defs_dirs=`find ${FMDB_BUILD_DIR} -name ${DEFSP_FILE}`
    if [[ ${defs_dirs} == "" ]] 
    then    #### try to find iMesh-Defs.inc
       defs_dirs=`find ${FMDB_BUILD_DIR} -name ${DEFS_FILE}`
       if [[ ${defs_dirs} == "" ]] 
       then 
          echo "Cannot find ${DEFSP_FILE} or ${DEFS_FILE} for FMDB, error return"
          return 50
       else
          FMDB_DEFS_FILE=${DEFS_FILE}
       fi
    else
       FMDB_DEFS_FILE=${DEFSP_FILE}
    fi
    FMDB_DEFS_DIR=`dirname ${defs_dirs}`

    echo " FMDB ${DEFSP_FILE} is stored in directory:"
    echo "    ${FMDB_DEFS_DIR}"

    export LD_LIBRARY_PATH="${FMDB_INSTALL_DIR}/lib":"${LD_LIBRARY_PATH}"
    return 0
}


# *****************************************************************************
#     Section 5 - IPComman Routines
# -----------------------------------------------------------------------------
# *****************************************************************************
# Function: get_source_IPCOMM
#
# Purpose:  download IPComman source code from distribution URL
#
#           The checkout.log file has the status of the file checkout.  When
#           complete the next-to-last line contains the time, file size and
#           name as well as the word 'saved'.  Completion of the file checkout
#           is determined when the word 'saved' is found.  On some systems it is
#           the 6th symbol and on others it is the 7th symbol (when the date is 
#           also included). 
#
#           When an SVN checkout is done from the trunk, the word 'revision' is
#           used to determine a successful checkout.
#
#           NOTE:  If the file cannot be download within 2 minutes an error
#                  message is written and immediate return from this routine
#
# Programmer: Ellen Hill
# Date: Tue Apr 12 2011
#
# *****************************************************************************
function get_source_IPCOMM
{
#     IPCOMM_LINK is a symbolic link to the source code tar file
#     it will contain the name of the actual tar file that then must be
#     retrieved.
    if [[ "${IPCOMM_LINK}" != "" ]]
    then
       wget ${IPCOMM_PATH}/${IPCOMM_LINK}
       IPCOMM_SOURCE=`cat ${IPCOMM_LINK} | sed -e 's/link //'`
    fi

#     initialize return value
    retval=0

    echo "Getting IPComman from ${IPCOMM_PATH}. . . "
    if [[ "${IPCOMM_SOURCE}" = "" ]]
    then
       if [[ ! -d IPComman ]]
       then
          mkdir IPComman
       fi
       svn co ${IPCOMM_PATH} ./IPComman 1>IPCOMM.checkout 2> IPCOMM.checkout.log
       ipcomm_file="IPCOMM.checkout"
       sleep 30
       if [[ -z ${ipcomm_file} ]]
       then
          retval=20
       else
          endlog=`tail --lines=1 ${ipcomm_file} | cut -d" " -f3`
          while [[ "$endlog" != "revision" ]]
          do
             sleep 60
             endlog=`tail --lines=1 ${ipcomm_file} | cut -d" " -f3`
             timelimit=`expr $timelimit + 1`
             if [[ "$timelimit" == "2" ]]
             then
                echo "Problem with SVN checkout of IPComman source code"
                retval=20
                break
             fi
          done
       fi
    else
       wget ${IPCOMM_PATH}/${IPCOMM_SOURCE} 1> IPCOMM.checkout 2> IPCOMM.checkout.log
       ipcomm_file="IPCOMM.checkout.log"
       sleep 30
       if [[ -z ${ipcomm_file} ]]
       then
          retval=20
       else
          endlog1=`tail --lines=2 ${ipcomm_file} | cut -d" " -f6`
          endlog2=`tail --lines=2 ${ipcomm_file} | cut -d" " -f7`
          while [[ "$endlog1" != "saved"  &&  "$endlog2" != "saved" ]]
          do
             sleep 60
             endlog1=`tail --lines=2 ${ipcomm_file} | cut -d" " -f6`
             endlog2=`tail --lines=2 ${ipcomm_file} | cut -d" " -f7`
             timelimit=`expr $timelimit + 1`
             if [[ "$timelimit" == "2" ]]
             then
                echo "Problem checking out IPComman source code"
                retval=20
                break
             fi
          done
       fi
    fi

#     remove IPCOMM.checkout if it is an empty file
    find . -name 'IPCOMM.checkout' -size 0 -exec rm -f {} \;
    find . -name 'IPCOMM.checkout.log' -size 0 -exec rm -f {} \;

    if [ "${retval}" != "0" ]
    then
       return ${retval}
    else
       return 0
    fi
}


# *****************************************************************************
# Function: get_builddir_IPCOMM
#
# Purpose: Find the IPCOMM directory name into which the source code 
#          was untarred
#
# Programmer: Ellen Hill 
# Date: Tue Apr 12 2011
#
# *****************************************************************************
function get_builddir_IPCOMM
{
    IPCOMM_BUILD_DIR=""
    files=`ls`
    for ipcomm_dir in ${files} 
    do
       if [[ -d ${ipcomm_dir} ]]
       then
          case ${ipcomm_dir} in
             *IPC* | ipc*)
                 export IPCOMM_BUILD_DIR=`pwd`/${ipcomm_dir}
                 break
                 ;;
               *)
                 ;;
          esac
       fi
    done
    if [[ -z $IPCOMM_BUILD_DIR ]]
    then
       echo "Cannot find IPComman source code directory; error return"
       return 55
    else
       echo " IPComman build directory: ${IPCOMM_BUILD_DIR}"
       return 0
    fi
}


# *************************************************************************** #
# Function: build_check_IPCOMM
#
# Purpose: Builds IPComman
#
# Programmer: Ellen Hill 
# Date: Tue Apr 12 2011
# *************************************************************************** #
function build_check_IPCOMM
{
    IPCOMM_DEFS_DIR=""

    echo "Configuring IPComman. . ."
    IPCOMM_INSTALL_DIR=$IPCOMM_BUILD_DIR/iMeshDir
    cd $IPCOMM_BUILD_DIR || echo "Cannot cd to $IPCOMM_BUILD_DIR build dir."

    ./configure --with-mpi --prefix=$IPCOMM_INSTALL_DIR \
                1> IPCOMM.config.log 2> IPCOMM.config.err
    retval=$?
    cat config.log >> IPCOMM.config.log

    if [[ "$retval" != "0" ]] 
    then
       echo "IPComman configure failed, Giving up"
       return 30
    fi

    echo "Building IPComman. . . "
    make 1> IPCOMM.make.log 2> IPCOMM.make.err
    retval=$?

#     save IPCOMM make error return value & file name (if non-zero length)
#     if IPCOMM make failed, set IPCOMM make-install to N/A
    if [[ "$retval" != "0" ]] 
    then
       echo "IPComman make failed, Giving up"
       return 35
    fi

#     filter out messages for creating lib files
    if [[ -s "IPCOMM.make.err" ]]
    then
       apply_error_filters "IPCOMM.make.err"
    fi

    echo "Installing IPComman. . . "
    make install 1> IPCOMM.mkinstall.log 2> IPCOMM.mkinstall.err
    retval=$?

#     save IPCOMM make-install error return value & file name (if non-zero length)
    if [[ "$retval" != "0" ]] 
    then
       echo "IPComman make failed, Giving up"
       return 40
    fi

    echo "Done building IPComman"

    return 0
}

# *****************************************************************************
#     Section 6 - Zoltan & ParMETIS Routines
# -----------------------------------------------------------------------------
# *****************************************************************************
# Function: get_source_ZOLTAN
#
# Purpose:  download Zoltan source code from distribution URL
#
#           The checkout.log file has the status of the file checkout.  When
#           complete the next-to-last line contains the time, file size and
#           name as well as the word 'saved'.  Completion of the file checkout
#           is determined when the word 'saved' is found.  On some systems it is
#           the 6th symbol and on others it is the 7th symbol (when the date is 
#           also included). 
#
#           When an SVN checkout is done from the trunk, the word 'revision' is
#           used to determine a successful checkout.
#
#           NOTE:  If the file cannot be download within 2 minutes an error
#                  message is written and immediate return from this routine
#
# Programmer: Ellen Hill
# Date: Tue Mar 29 2011
#
# *****************************************************************************
function get_source_ZOLTAN
{
#     ZOLTAN_LINK is a symbolic link to the source code tar file
#     it will contain the name of the actual tar file that then must be
#     retrieved.

#
#     initialize return value
    retval=0

    if [[ "${ZOLTAN_LINK}" != "" ]]
    then
       wget ${ZOLTAN_PATH}/${ZOLTAN_LINK}
       ZOLTAN_SOURCE=`cat ${ZOLTAN_LINK} | sed -e 's/link //'`
    fi

    echo "Getting Zoltan from ${ZOLTAN_PATH}. . . "
    if [[ "${ZOLTAN_SOURCE}" = "" ]]
    then
       if [[ ! -d Zoltan ]]
       then
          mkdir Zoltan
       fi
       svn co ${ZOLTAN_PATH} ./Zoltan 1>ZOLTAN.checkout 2> ZOLTAN.checkout.log
       sleep 30
       if [[ -z ZOLTAN.checkout ]]
       then
          retval=20
       else
          endlog=`tail --lines=1 ZOLTAN.checkout | cut -d" " -f3`
          while [[ "$endlog" != "revision" ]]
          do
             sleep 60
             endlog=`tail --lines=1 ZOLTAN.checkout | cut -d" " -f3`
             timelimit=`expr $timelimit + 1`
             if [[ "$timelimit" == "2" ]]
             then
                echo "Problem with SVN checkout of ZOLTAN source code"
                retval=20
                break
             fi
          done
       fi
    else
       wget ${ZOLTAN_PATH}/${ZOLTAN_SOURCE} 1> ZOLTAN.checkout 2> ZOLTAN.checkout.log
       sleep 30
       if [[ -z ZOLTAN.checkout.log ]]
       then
          retval=20
       else
          endlog1=`tail --lines=2 ZOLTAN.checkout.log | cut -d" " -f6`
          endlog2=`tail --lines=2 ZOLTAN.checkout.log | cut -d" " -f7`
          while [[ "$endlog1" != "saved"  &&  "$endlog2" != "saved" ]]
          do
             sleep 60
             endlog1=`tail --lines=2 ZOLTAN.checkout.log | cut -d" " -f6`
             endlog2=`tail --lines=2 ZOLTAN.checkout.log | cut -d" " -f7`
             timelimit=`expr $timelimit + 1`
             if [[ "$timelimit" == "2" ]]
             then
                echo "Problem checking out Zoltan source code"
                retval=20
                break
             fi
          done
       fi
    fi

#     remove checkout files if an empty file
    find . -name 'ZOLTAN.checkout' -size 0 -exec rm -f {} \;
    find . -name 'ZOLTAN.checkout.log' -size 0 -exec rm -f {} \;

    if [[ "$retval" != "0" ]]
    then
       return 20
    fi

    return 0
}


# *****************************************************************************
# Function: get_builddir_ZOLTAN
#
# Purpose: Find the ZOLTAN directory name into which the source code 
#          was untarred
#
# Programmer: Ellen Hill 
# Date: Tue Mar 29 2011
#
# *****************************************************************************
function get_builddir_ZOLTAN
{
    ZOLTAN_SRC_DIR=""
    files=`ls`
    for zolt_dir in ${files} 
    do
       if [[ -d ${zolt_dir} ]]
       then
          case ${zolt_dir} in
             Zol* | zol*)
                 export ZOLTAN_SRC_DIR=`pwd`/${zolt_dir}
                 break
                 ;;
               *)
                 ;;
          esac
       fi
    done

    if [[ -z ${ZOLTAN_SRC_DIR} ]]
    then
       echo "Cannot find ZOLTAN source code directory; error return"
       return 55
    else
       export ZOLTAN_BUILD_DIR="${ZOLTAN_SRC_DIR}/myBuild"
       mkdir ${ZOLTAN_BUILD_DIR}
       echo " ZOLTAN build directory: ${ZOLTAN_BUILD_DIR}"
       return 0
    fi
}


# *************************************************************************** #
# Function: build_check_ZOLTAN
#
# Purpose: Builds ZOLTAN
#          Creates separate directory (myBuild) in which to build Zoltan
#          Configure & make Zoltan with outputs in log & err files
#
# Programmer: Ellen Hill 
# Date: Tue Mar 29 2011
#
# *************************************************************************** #
function build_check_ZOLTAN
{

#
#     initialize return value
    retval=0

    echo "Configuring Zoltan . . ."
    ZOLTAN_INSTALL_DIR=$ZOLTAN_BUILD_DIR/iMeshPDir
    cd $ZOLTAN_BUILD_DIR || echo "Cannot cd to $ZOLTAN_BUILD_DIR build dir."

    ../configure --enable-mpi --disable-examples --prefix=$ZOLTAN_INSTALL_DIR \
                 --with-gnumake --with-parmetis \
                 --with-parmetis-incdir=${ZOLT_PARM_BUILD_DIR} \
                 --with-parmetis-libdir=${ZOLT_PARM_BUILD_DIR} \
                 1> ZOLTAN.config.log 2> ZOLTAN.config.err
    retval=$?
    cat config.log >> ZOLTAN.config.log

#     save ZOLTAN.config error return value & file name (if non-zero length)
#     if ZOLTAN.config failed, set ZOLTAN.make to N/A and ZOLTAN.make-install to N/A
    if [[ "$retval" != "0" ]] 
    then
       echo "ZOLTAN.configure failed, Giving up"
       return 30
    fi

    echo "Building Zoltan. . . "
    make everything 1> ZOLTAN.make.log 2> ZOLTAN.make.err

    if [[ "$?" != "0" ]] 
    then
       echo "ZOLTAN make failed, Giving up"
       return 35
    fi

    echo "Installing Zoltan. . . "
    make install 1> ZOLTAN.mkinstall.log 2> ZOLTAN.mkinstall.err

    if [[ "$?" != "0" ]] 
    then
       echo "ZOLTAN make failed, Giving up"
       return 40
    fi

    export LD_LIBRARY_PATH="$ZOLTAN_INSTALL_DIR":"$LD_LIBRARY_PATH"
  
    echo "Done building Zoltan"
    return 0
}


# *****************************************************************************
# Function: get_builddir_PARM
#
# Purpose: Find the directory name into which the ParMETIS source code 
#          was untarred
#
# Programmer: Ellen Hill 
# Date: Tue Mar 29 2011
#
# *****************************************************************************
function get_builddir_ZOLT_PARM
{
    ZOLT_PARM_BUILD_DIR=""
    files=`ls`
    for parm_dir in ${files} 
    do
       if [[ -d ${parm_dir} ]]
       then
          case ${parm_dir} in
             Par* | par*)
                 export ZOLT_PARM_BUILD_DIR=`pwd`/${parm_dir}
                 break
                 ;;
               *)
                 ;;
          esac
       fi
    done

    if [[ -z $ZOLT_PARM_BUILD_DIR ]]
    then
       echo "Cannot find PARMETIS source code directory for Zoltan; error return"
       return 55
    else
       echo " PARMETIS build directory for Zoltan: ${ZOLT_PARM_BUILD_DIR}"
       return 0
    fi
}


# *************************************************************************** #
# Function: build_ZOLT_PARM
#
# Purpose: Builds PARMETIS for Zoltan with outputs in log & err files
#
# Programmer: Ellen Hill 
# Date: Tue Mar 29 2011
#
# *************************************************************************** #
function build_ZOLT_PARM
{

#     initialize return value
    retval=0
 
    cd $ZOLT_PARM_BUILD_DIR || echo "Cannot cd to $ZOLT_PARM_BUILD_DIR build dir."

    echo "Building ParMETIS. . . "
    make 1> ZOLT_PARM.make.log 2> ZOLT_PARM.make.err
    retval=$?

#     if lib*metis exists reset retval to indicate that the libs were
#     properly built; even though a follow-on test failed
    if [ ! -z "$(cat libparmetis.a)" ] && [ ! -z "$(cat libmetis.a)" ]
    then
       retval=0
    fi

    if [ "$retval" != "0" ] 
    then
       echo "ParMETIS make failed, Giving up"
       echo " "
       return 35
    fi

    export LD_LIBRARY_PATH="$ZOLT_PARM_BUILD_DIR":"$LD_LIBRARY_PATH"
  
    echo "Done building PARMETIS for Zoltan"
    echo " "

    return 0
}

# *****************************************************************************
#     Section 7 - UnitTestP Routines
# -----------------------------------------------------------------------------
# *****************************************************************************
# Function: get_source_unitTestP
#
# Purpose:  get iMeshP unitTest source code
#
#           Gets needed header files; iMesh.h, iBase.h and iMeshP.h
#
#           The checkout.log file has the status of the file checkout.  When
#           complete the next-to-last line contains the time, file size and
#           name as well as the word 'saved'.  Completion of the file checkout
#           is determined when the word 'saved' is found.  On some systems it is
#           the 6th symbol and on others it is the 7th symbol (when the date is 
#           also included). 
#
#           NOTE:  If the file cannot be download within 2 minutes an error
#                  message is written and immediate return from this routine
#
# Programmer: Ellen Hill
# Date: Tue Mar 29 2011
#
# *****************************************************************************
function get_source_unitTestP
{

#     initialize return value
    retval=0

    echo "Getting iMeshP_unitTest sources from ${IMESHP_PATH}. . . "
    svn co ${IMESHP_PATH} imeshp_unitTest_dir \
         1> IMESHP.checkout 2> IMESHP.checkout.log
    sleep 30
    if [[ -z IMESHP.checkout ]]
    then
       retval=20
    else
       endlog=`tail --lines=1 IMESHP.checkout | cut -d" " -f3`
       while [[ "$endlog" != "revision" ]]
       do
          sleep 60
          endlog=`tail --lines=1 IMESHP.checkout | cut -d" " -f3`
          timelimit=`expr $timelimit + 1`
          if [[ "$timelimit" == "2" ]]
          then
             echo "Problem with SVN checkout of iMeshP source code"
             retval=20
             break
          fi
       done
    fi

#   remove checkout if it is an empty file
    find . -name 'IMESHP.checkout' -size 0 -exec rm -f {} \;
    find . -name 'IMESHP.checkout.log' -size 0 -exec rm -f {} \;
#
#     get needed header files and move them to the test sub-directory
    wget ${IMESH_HDR_PATH}/iMesh.h  1> iMesh.hdr.checkout 2> iMesh.hdr.checkout.log
    wget ${IBASE_HDR_PATH}/iBase.h  1> iBase.hdr.checkout 2> iBase.hdr.checkout.log
    wget ${IMESHP_HDR_PATH}/iMeshP.h  1> iMeshP.hdr.checkout 2> iMeshP.hdr.checkout.log
    mv *.h imeshp_unitTest_dir/

#   remove checkout if it is an empty file
    find . -name 'iMesh.hdr.checkout' -size 0 -exec rm -f {} \;
    find . -name 'iMesh.checkout.log' -size 0 -exec rm -f {} \;
    find . -name 'iBase.hdr.checkout' -size 0 -exec rm -f {} \;
    find . -name 'iBase.checkout.log' -size 0 -exec rm -f {} \;
    find . -name 'iMeshP.hdr.checkout' -size 0 -exec rm -f {} \;
    find . -name 'iMeshP.checkout.log' -size 0 -exec rm -f {} \;
#
#     make tar files of iMesh source code 
    tar -cf iMeshP_unitTest.tar imeshp_unitTest_dir/

    return $retval
}


# *****************************************************************************
# Function: get_builddir_unitTestP
#
# Purpose: Find the IMESHP directory name into which the source code was untarred
#
# Programmer: Ellen Hill 
# Date: Tue Mar 29 2011
#
# *****************************************************************************
function get_builddir_unitTestP
{
    export IMESHP_BUILD_DIR="$(pwd)/imeshp_unitTest_dir"
}


# *****************************************************************************
# Function: create_iMeshP_Make
#
# Purpose: Create Makefile.iMeshP for implementation being used
#
# Programmer: Ellen Hill 
# Date: Tue Mar 29 2011
#
# *****************************************************************************
function create_iMeshP_Make
{

#     save arguments
    impl_nam=$1
    impl_dir=$2
    
#     set defs file based on implementation name
    if [ "${impl_name}" = "MOAB" ]
    then
       impl_defs=${DEFS_FILE}
    else
       impl_defs=${DEFSP_FILE}
    fi

#     set location of MPI header and libs
    MPI_INC="-I/usr/include/mpi"
    MPI_LIB="-L/usr/lib -lmpi -lmpi_cxx"

    MFILE="Makefile."${impl_nam}

    echo " "
    echo "Creating makefile: " $MFILE

#     write out needed commands
    echo "## Makefile for building iMeshP_unitTest with" ${impl_nam} > $MFILE
    echo " " >> $MFILE
    echo "include ${impl_dir}/${impl_defs}" >> $MFILE
    echo " " >> $MFILE
    echo "MPI_INC="${MPI_INC} >> $MFILE
    echo " " >> $MFILE
    echo "MPI_LIB="${MPI_LIB} >> $MFILE
    echo " " >> $MFILE
    echo "ALL_INCLUDES=$\(IMESH_INCLUDES\)" >> $MFILE
    echo " " >> $MFILE
    echo "LIBS=$\(IMESH_LIBS\)" >> $MFILE
    echo " " >> $MFILE
    echo "DEPS=$\(IMESH_FILES\)" >> $MFILE
    echo " " >> $MFILE

    case ${impl_nam} in
       *_unitTest) echo "all: MOAB_iMeshP_unitTest" >> $MFILE
                   echo " " >> $MFILE
                   echo "MOAB_iMeshP_unitTest: $\(DEPS\)" >> $MFILE
                   echo $MPI_NAME "-fPIC -o MOAB_iMeshP_unitTest MOAB_iMeshP_unitTest.cc $\(ALL_INCLUDES\) $\(MPI_INC\) $\(LIBS\) $\(MPI_LIB\)" >> $MFILE
                   echo " " >> $MFILE
                   ;;
                *) echo "all: iMeshP_unitTest" >> $MFILE
                   echo " " >> $MFILE
                   echo "iMeshP_unitTest: $\(DEPS\)" >> $MFILE
                   echo $MPI_NAME "-fPIC -o iMeshP_unitTest iMeshP_unitTest.cc $\(ALL_INCLUDES\) $\(MPI_INC\) $\(LIBS\) $\(MPI_LIB\)" >> $MFILE
                   echo " " >> $MFILE
                   ;;
    esac

    echo "depend:" >> $MFILE
    echo "makedepend $\(ALL_INCLUDES\) -Y *.cc 2> /dev/null" >> $MFILE
    echo " " >> $MFILE
    echo "clean:" >> $MFILE
    echo "rm -rf *.o" >> $MFILE
    echo " " >> $MFILE
    echo "distclean: clean" >> $MFILE
    echo "rm -rf *iMeshP_unitTest" >> $MFILE

#
#     create temporary file; add tab character as needed for make; replace original
#     Makefile.iMeshP with temporary file
    tmpf="tmpfil"
    while read InpL
    do
       case $InpL in
               *\*) echo $InpL | sed -e 's/\//g' >> $tmpf
                    ;;
          "maked"*) echo $InpL | sed -e 's/maked/	maked/g' >> $tmpf
                    ;;
             "rm"*) echo $InpL | sed -e 's/rm/	rm/g' >> $tmpf
                    ;;
            "mpi"*) echo $InpL | sed -e 's/mpi/	mpi/g' >> $tmpf
                    ;;
                 *) echo $InpL >> $tmpf
                    ;;
       esac
    done < $MFILE

    rm -rf $MFILE
    mv $tmpf $MFILE

    return 0
}


# *************************************************************************** #
# Function: build_run_unitTestP
#
# Purpose: Builds and runs iMeshP unitTests
#
# Programmer: Ellen Hill
# Date: Tue Mar 29 2011
#
# *************************************************************************** #
function build_run_unitTestP
{
    cd $IMESHP_BUILD_DIR || echo "Cannot cd to $IMESHP_BUILD_DIR build dir."

#
# save input arguments
    IMPL_NAME=$1
    IMPL_BUILD_DIR=$2
    IMPL_MESH_FILE=$3

#
# inititalize pass/fail values for iMeshP_unitTest
    make_err=999
    nock_err=999
    valg_err=999
    run_err=999

#
# if build directory is NULL, return error
    if [[ "${IMPL_BUILD_DIR}" = "" ]]
    then
       return 55
    fi

#
# inititalize return value
    retval=0

#
# remove any existing files
    if [[ -x iMeshP_unitTest ]]
    then
       rm -rf *.o iMeshP_unitTest
    fi

#
#     Create make file for building iMeshP_unitTest
    create_iMeshP_Make ${IMPL_NAME} ${IMPL_BUILD_DIR}

    echo " "
    echo "Building iMeshP_unitTest for ${IMPL_NAME} . . ."
    make -f Makefile.${IMPL_NAME} 1> imeshp_${IMPL_NAME}.make.log \
                                  2> imeshp_${IMPL_NAME}.make.err
    if [ $? != 0 ]
    then
       make_err=1
       if [ -n "$(grep speccheck imeshp_${IMPL_NAME}.make.err | grep 'Error 4')" ]
       then
          echo "Spec-check failed for ${IMPL_NAME}; trying with spec-check disabled . . ."
          make -f Makefile.${IMPL_NAME} ITAPS_NOCKSPEC=1 \
                                        1> imeshp_${IMPL_NAME}.make.nospec.log \
                                        2> imeshp_${IMPL_NAME}.make.nospec.err
          if [ $? != 0 ]
          then
             echo "iMeshP_unitTest make failed for ${IMPL_NAME}; even with spec-check disabled . . ."
             nock_err=1
             return 35
          else
             nock_err=0
          fi
       else
          echo "iMeshP_unitTest make failed for ${IMPL_NAME}, Giving up"
          return 35
       fi
    else
       make_err=0
    fi

    if [ -x iMeshP_unitTest ] 
    then
       make_err=0
       if [ "${IMPL_NAME}" = "MOAB" ]
       then 
          inpfil="../${MOAB_MESHFILE}"
       else
          inpfil="../${FMDB_MESHBASE}.sms"
       fi
       echo "Running iMeshP_unitTest for ${IMPL_NAME} . . . "
       mpirun -np 6 ./iMeshP_unitTest ${inpfil} \
            1> imeshp_${IMPL_NAME}.out 2> imeshp_${IMPL_NAME}.msg
       run_err=$?
       for (( i=0; i<6; i++ )) 
       do 
          mv -f Output_0$i.txt Output_${IMPL_NAME}0$i.txt
       done

       echo "Running iMeshP_unitTest with valgrind for ${IMPL_NAME} . . . "
       mpirun -np 6 valgrind --suppressions=../ITAPS.supp --show-reachable=yes --leak-check=full \
               ./iMeshP_unitTest ${inpfil} \
               1> imeshp_${IMPL_NAME}.valg.out 2> imeshp_${IMPL_NAME}.valg.msg
       valg_err=$?
       for (( i=0; i<6; i++ )) 
       do 
          mv -f Output_0$i.txt Output_${IMPL_NAME}0$i.valg.txt
       done
    fi

    echo "Done running iMeshP_unitTest for ${IMPL_NAME}"
    echo " "

    return 0
}

# *****************************************************************************
#     Section 8 - izoltan-parallel Routines
# -----------------------------------------------------------------------------
# *****************************************************************************
# Function: get_source_izoltanp
#
# Purpose:  get izoltan-parallel source code
#
#           The checkout.log file has the status of the file checkout.  When
#           complete the next-to-last line contains the time, file size and
#           name as well as the word 'saved'.  Completion of the file checkout
#           is determined when the word 'saved' is found.  On some systems it is
#           the 6th symbol and on others it is the 7th symbol (when the date is 
#           also included). 
#
#           NOTE:  If the file cannot be download within 2 minutes an error
#                  message is written and immediate return from this routine
#
# Programmer: Ellen Hill
# Date: Thu May 26 2011
#
# *****************************************************************************
function get_source_izoltanp
{
#
#     IZOLTANP_LINK is a symbolic link to the source code tar file
#     it will contain teh name of the actual tar file that then must be
#     retrieved.
    if [ "${IZOLTANP_LINK}" != "" ]
    then
       wget ${IZOLTANP_PATH}/${IZOLTANP_LINK}
       IZOLTANP_SOURCE=`cat ${IZOLTANP_LINK} | sed -e 's/link//g'`
    fi

#     initialize
    timelimit=0
    retval=0

    echo "Getting izoltanparallel from ${IZOLTANP_PATH}. . . "
    if [[ "${IZOLTANP_SOURCE}" = "" ]]
    then
       if [[ ! -d IZOLTANP ]]
       then
          mkdir IZOLTANP
       fi
       svn co ${IZOLTANP_PATH} ./IZOLTANP 1>IZOLTANP.checkout 2> IZOLTANP.checkout.log
       sleep 30
       if [[ -z IZOLTANP.checkout ]]
       then
          retval=20
       else
          endlog=`tail --lines=1 IZOLTANP.checkout | cut -d" " -f3`
          while [[ "$endlog" != "revision" ]]
          do
             sleep 60
             endlog=`tail --lines=1 IZOLTANP.checkout | cut -d" " -f3`
             timelimit=`expr $timelimit + 1`
             if [[ "$timelimit" == "2" ]]
             then
                echo "Problem with SVN checkout of IZOLTANP source code"
                retval=20
                break
             fi
          done
       fi
    else
       wget ${IZOLTANP_PATH}/${IZOLTANP_SOURCE} 1> IZOLTANP.checkout 2> IZOLTANP.checkout.log
       sleep 30
       if [[ -z IZOLTANP.checkout.log ]]
       then
          retval=20
       else
          endlog1=`tail --lines=2 IZOLTANP.checkout.log | cut -d" " -f6`
          endlog2=`tail --lines=2 IZOLTANP.checkout.log | cut -d" " -f7`
          while [[ "$endlog1" != "saved"  &&  "$endlog2" != "saved" ]]
          do
             sleep 60
             endlog1=`tail --lines=2 IZOLTANP.checkout.log | cut -d" " -f6`
             endlog2=`tail --lines=2 IZOLTANP.checkout.log | cut -d" " -f7`
             timelimit=`expr $timelimit + 1`
             if [[ "$timelimit" == "2" ]]
             then
                echo "Problem checking out iZoltan-parallel source code"
                retval=20
                break
             fi
          done
       fi
    fi

#     remove checkout files if an empty file
    find . -name 'IZOLTANP.checkout' -size 0 -exec rm -f {} \;
    find . -name 'IZOLTANP.checkout.log' -size 0 -exec rm -f {} \;

    return 0
}


# *****************************************************************************
# Function: get_builddir_izoltanp
#
# Purpose: Find the iZoltan-parallel directory name into which the source code 
#          was untarred
#
# Programmer: Ellen Hill 
# Date: Thu May 26 2011
#
# *****************************************************************************
function get_builddir_izoltanp
{
    IZOLTANP_SRC_DIR=""
    files=`ls`
    for izoltanp_dir in ${files} 
    do
       if [[ -d ${izoltanp_dir} ]]
       then
          case ${izoltanp_dir} in
             IZo* | izo*)
                 export IZOLTANP_BUILD_DIR=`pwd`/${izoltanp_dir}
                 break
                 ;;
               *)
                 ;;
          esac
       fi
    done

    if [ -z ${IZOLTANP_BUILD_DIR} ]
    then
       echo "Cannot find izoltan-parallel source code directory; error return"
       return 55
    else
       echo " IZOLTANP build directory: ${IZOLTANP_BUILD_DIR}"
       return 0
    fi
}


# *************************************************************************** #
# Function: build_check_izoltanp
#
# Purpose: Builds izoltan-parallel
#          Creates separate directory (myBuild) in which to build iZoltan-parallel
#          Configure & make iZoltan-parallel with outputs in log & err files
#
# Programmer: Ellen Hill 
# Date: Thu May 26 2011
#
# *************************************************************************** #
function build_check_izoltanp
{

#     save input arguments
    IMPL_NAME=$1
    IMPL_DIR=$2
    IMPL_DEFS_FILE=$3
#
#     initialize return value
    retval=0

#     initialize pass/fail values
    config_err=999
    make_err=999
    run_err=999
    
    echo "Configuring iZoltan-parallel for ${IMPL_NAME} . . ."
    IZOLTANP_INSTALL_DIR=$IZOLTANP_BUILD_DIR/iMeshPDir
    cd $IZOLTANP_BUILD_DIR || echo "Cannot cd to $IZOLTANP_BUILD_DIR build dir."

#     remove any files from prvious builds
    rm -rf *.o iMeshPZoltanTest

    ./configure IMESH_DEFS=${IMPL_DIR}/${IMPL_DEFS_FILE} \
                --with-parmetis=${ZOLT_PARM_BUILD_DIR} \
                --with-zoltan-include=${ZOLTAN_INSTALL_DIR}/include \
                --with-zoltan-lib=${ZOLTAN_INSTALL_DIR}/lib \
                --with-mpi-include=${ZOLTAN_INSTALL_DIR}/include \
                --with-mpi-lib=${ZOLTAN_INSTALL_DIR}/lib \
                --prefix=$IZOLTANP_INSTALL_DIR \
                 1> IZOLTANP_${IMPL_NAME}.config.log 2> IZOLTANP_${IMPL_NAME}.config.err

    if [ $? != 0 ]
    then
       config_err=1
       return 30
    fi
    config_err=0
    cat config.log >> IZOLTANP_${IMPL_NAME}.config.log

    echo "Building iZoltan-parallel for ${IMPL_NAME} . . . "
    make 1> IZOLTANP_${IMPL_NAME}.make.log 2> IZOLTANP_${IMPL_NAME}.make.err
    if [ $? != 0 ]
    then
       make_err=1
       return 35
    fi
    make_err=0

    echo "Running iZoltan-parallel for ${IMPL_NAME} . . . "
    if [ "${IMPL_NAME}" = "MOAB" ]
    then 
       inpfil="../${MOAB_MESHFILE}"
    else
       inpfil="../${FMDB_MESHBASE}.sms"
    fi

    mpirun -np 6 ./iMeshPZoltanTest 6 ${inpfil} REGION new_mesh_${IMPL_NAME} > IZOLTANP_${IMPL_NAME}.results
    if [ $? != 0 ]
    then
       run_err=1
       return 40
    fi
    run_err=0
  
    return 0
}



# *****************************************************************************
#     Section 8 - MESQUITE Routines
# -----------------------------------------------------------------------------
# *************************************************************************** #
# Function: get_source_MESQ
#
# Purpose: gets source for Mesquite
#
# Programmer: Ellen Hill
# Date: Fri Apr 09 2010
#
# *************************************************************************** #
function get_source_MESQ
{
    echo "Getting ${ITAPS_MESQ_SOURCE} from ${ITAPS_MESQ_PATH}. . . "
    wget ${ITAPS_MESQ_PATH}/${ITAPS_MESQ_SOURCE} \
         1> MESQ.checkout 2> MESQ.checkout.log

    sleep 30
    timelimit=0
    retval=0
    if [[ -z MESQ.checkout.log ]]
    then
       retval=20
    else
       endlog1=`tail --lines=2 MESQ.checkout.log | cut -d" " -f6`
       endlog2=`tail --lines=2 MESQ.checkout.log | cut -d" " -f7`
       while [[ "$endlog1" != "saved" &&  "$endlog2" != "saved"  ]]
       do
          sleep 60
          endlog1=`tail --lines=2 MESQ.checkout.log | cut -d" " -f6`
          endlog2=`tail --lines=2 MESQ.checkout.log | cut -d" " -f7`
          timelimit=`expr $timelimit + 1`
          if [[ "$timelimit" == "2" ]]
          then
             echo "Problem checking out Mesquite source code"
             retval=20
             break
          fi
       done
    fi

#     remove MESQ.checkout if it is an empty file
    find . -name 'MESQ.checkout' -size 0 -exec rm -f {} \;

    return $retval
}


# *****************************************************************************
# Function: get_builddir_MESQ
#
# Purpose: Find the Mesquite directory name into which the source code 
#          was untarred
#
# Programmer: Ellen Hill 
# Date: Mon Apr 12 2010
#
# *****************************************************************************
function get_builddir_MESQ
{
    MESQ_BUILD_DIR=""
    files=`ls`
    for mesq_dir in ${files} 
    do
       if [[ -d ${mesq_dir} ]]
       then
          case ${mesq_dir} in
             mesq*) export MESQ_BUILD_DIR=`pwd`/${mesq_dir}
                    break
                    ;;
                 *)
                    ;;
          esac
       fi
    done
    if [[ -z $MESQ_BUILD_DIR ]]
    then
       echo "Cannot find Mesquite source code directory; error return"
       return 55
    else
       echo " Mesquite build directory: ${MESQ_BUILD_DIR}"
       return 0
    fi
}


# *************************************************************************** #
# Function: build_check_MESQ
#
# Purpose: Builds Mesquite with each implementation
#          Configure and make MESQ with outputs going into log and err
#             files, respectively
#          Run 'make check' to verify successful build
#
# Programmer: Ellen Hill
# Date: Fri Apr 9 2010
#
# *************************************************************************** #
function build_check_MESQ
{
#
#     get arguments
    IMPL_NAME=$1
    IMPL_DIR=$2
    IMPL_DEFS_FILE=$3
#
#     initialize error return values
    config_err=999
    make_err=999
    check_err=999
#
#     if build directory is NULL, return error
    if [[ "${IMPL_DIR}" = "" ]]
    then
       return 55
    fi
#
#     if a Makefile exists, remove everything from prior runs
    if [ -e Makefile ]
    then
       make distclean
    fi

    echo "Configuring Mesquite with $IMPL_NAME . . ."
    cd $MESQ_BUILD_DIR || echo "Cannot cd to $MESQ_BUILD_DIR build dir."
    ./configure --with-imesh=$IMPL_DIR/$IMPL_DEFS_FILE \
                --with-imeshp=$IMPL_DIR/$IMPL_DEFS_FILE  --with-mpi \
                  1> MESQ_$IMPL_NAME.config.log  2> MESQ_$IMPL_NAME.config.err
    if [[ $? != 0 ]] 
    then
       echo "Mesquite configure failed with $IMPL_DIR, Giving up"
       config_err=1
       return 30
    else
       config_err=0
    fi

    echo "Building Mesquite with $IMPL_NAME. . . "
    make 1> MESQ_$IMPL_NAME.make.log 2> MESQ_$IMPL_NAME.make.err 
    if [[ $? != 0 ]] 
    then
       echo "Mesquite make failed with $IMPL_NAME, Giving up"
       make_err=1
       return 35
    else
       make_err=0
    fi
    echo "Done building Mesquite with $IMPL_NAME"

#     run make check to build test codes then just run imesh tests
    make check > MESQ_$IMPL_NAME.results
#
#     count number of tests failed
    failed=0

    while read InputLine
    do
       case $InputLine in
          *FAIL*) failed=`expr $failed + 1`
                  ;;
       esac
    done < MESQ_$IMPL_NAME.results

#
#     set check error flag
    if [[ $failed > 0 ]]
    then
       check_err=1
    else
       check_err=0
    fi

    return $failed
}


# *****************************************************************************
#     Summary File Routines
# -----------------------------------------------------------------------------
# *************************************************************************** #
# Function: create_exec_summary
#
# Purpose: creates an executive summary with pass/fail results only
#
# Programmer: Ellen Hill
# Date: Tue Feb 2 2011
#
# *************************************************************************** #
function create_exec_summary
{

   echo "Creating unitTestP executive summary file"

   EXEC_SUMM="UnitTestP_exec.${rundate}.html"

   printf "<html>\n" > $EXEC_SUMM
   printf "<head>\n" >> $EXEC_SUMM
   printf "<style type=\"text/css\">\n" >> $EXEC_SUMM
   printf "\n" >> $EXEC_SUMM

   printf "#test_results\n" >> $EXEC_SUMM
   printf "{\n" >> $EXEC_SUMM
   printf "font-family:\"Courier New\", Courier, monospace;\n" >> $EXEC_SUMM
   printf "border:1px solid black;\n" >> $EXEC_SUMM
   printf "}\n" >> $EXEC_SUMM
   printf "\n" >> $EXEC_SUMM

   printf "#test_results th;\n" >> $EXEC_SUMM
   printf "{\n" >> $EXEC_SUMM
   printf "font-size:1.1em;\n" >> $EXEC_SUMM
   printf "text-align:center;\n" >> $EXEC_SUMM
   printf "padding-top:3px;\n" >> $EXEC_SUMM
   printf "padding-bottom:1px;\n" >> $EXEC_SUMM
   printf "}\n" >> $EXEC_SUMM
   printf "\n" >> $EXEC_SUMM

   printf "#test_results td;\n" >> $EXEC_SUMM
   printf "{\n" >> $EXEC_SUMM
   printf "font-size:1em;\n" >> $EXEC_SUMM
   printf "text-align:center;\n" >> $EXEC_SUMM
   printf "border:1px solid black;\n" >> $EXEC_SUMM
   printf "}\n" >> $EXEC_SUMM
   printf "\n" >> $EXEC_SUMM

   printf ".toprow\n" >> $EXEC_SUMM
   printf "{\n" >> $EXEC_SUMM
   printf "text-align:center;\n" >> $EXEC_SUMM
   printf "}\n" >> $EXEC_SUMM
   printf "\n" >> $EXEC_SUMM

   printf ".opts\n" >> $EXEC_SUMM
   printf "{\n" >> $EXEC_SUMM
   printf "text-align:left;\n" >> $EXEC_SUMM
   printf "}\n" >> $EXEC_SUMM
   printf "\n" >> $EXEC_SUMM

   printf ".dats\n" >> $EXEC_SUMM
   printf "{\n" >> $EXEC_SUMM
   printf "text-align:center;\n" >> $EXEC_SUMM
   printf "}\n" >> $EXEC_SUMM
   printf "\n" >> $EXEC_SUMM

   printf "</style>\n" >> $EXEC_SUMM
   printf "</head>\n" >> $EXEC_SUMM
   printf "\n" >> $EXEC_SUMM

   printf "<body>\n" >> $EXEC_SUMM

   printf "<table border=\"1\"" >> $EXEC_SUMM
   printf " cellpadding=\"10\">\n" >> $EXEC_SUMM
   printf "<table id=\"test_results\">\n" >> $EXEC_SUMM
   printf "<caption>iMeshP ${ITAPS_BRANCH} Test Results Summary <br /> ${datetim}</caption>\n" >> $EXEC_SUMM
   printf "\n" >> $EXEC_SUMM

#
#      set pass/fail array
   for ((kk=0; kk<${max_ind}; kk++ ))
   do
      case ${filarr[$kk]} in
            "ZZZ") pfarr[$kk]="    "
                   ;;
               "") pfarr[$kk]="PASS"
                   ;;
         "WARN:"*) pfarr[$kk]="WARN"
                   tmp=`echo ${filarr[$kk]} | cut -d":" -f2-`
                   filarr[$kk]=$tmp
                   ;;
            "DNR") pfarr[$kk]="DNR"
                   ;;
            "N/A") pfarr[$kk]="N/A"
                   ;;
                *) nerr=`echo ${filarr[$kk]} | cut -d":" -f1`
                   if [ "${nerr}" = "0" ]
                   then
                      pfarr[$kk]="FAIL"
                   else
                      pfarr[$kk]="FAIL:${nerr}"
                   fi
                   tmp=`echo ${filarr[$kk]} | cut -d":" -f2-`
                   filarr[$kk]=$tmp
                   ;;
      esac
   done

#
#     print table column headings and test results
   printf "<tr class=\"toprow\">\n" >> $EXEC_SUMM
   printf "   <th>test</th>\n" >> $EXEC_SUMM
   printf "   <th>MOAB</th>\n" >> $EXEC_SUMM
   printf "   <th>FMDB</th>\n" >> $EXEC_SUMM
   printf "</tr>\n" >> $EXEC_SUMM
   printf "\n" >> $EXEC_SUMM

   for (( indx=0; indx<$num_opts; indx++ ))
   do
      indx1=`expr $indx + $num_opts`

      case $indx in
         0) optnam="download"          #start of implementation tests
            ;;
         1) optnam="configure"
            ;;
         2) optnam="make"
            ;;
         3) optnam="make-install"
            ;;
         4) optnam="make-check"
            ;;
         5) optnam="make-spec-ck"      #start of iMeshP tests
            ;;
         6) optnam="make-loose-spec-ck"
            ;;
         7) optnam="run"
            ;;
         8) optnam="valgrind"
            ;;
         9) optnam="make"              #start of MOAB_iMeshP tests
            ;;
        10) optnam="run"
            ;;
        11) optnam="valgrind"
            ;;
        12) optnam="configure"         #start of izoltan-parallel
            ;;
        13) optnam="make"
            ;;
        14) optnam="run"
            ;;
        15) optnam="configure"         #start of Mesquite parallel
            ;;
        16) optnam="make"
            ;;
        17) optnam="run"
            ;;
      esac
#
#     print header for each section of tests
      if [[ $indx -eq 5 ]]
      then
         echo "<th colspan=\"3\">- - - - - iMeshP_unitTest Tests - - - - -</th>" >> $EXEC_SUMM
      elif [[ $indx -eq 9 ]]
      then
         echo "<th colspan=\"3\">- - - - - MOAB_iMeshP_unitTest Tests - - - - -</th>" >> $EXEC_SUMM
      elif [[ $indx -eq 12 ]]
      then
         echo "<th colspan=\"3\">- - - - - izoltan-parallel Tests - - - - -</th>" >> $EXEC_SUMM
      elif [[ $indx -eq 15 ]]
      then
         echo "<th colspan=\"3\">- - - - - Mesquite Parallel Tests - - - - -</th>" >> $EXEC_SUMM
      fi

      printf "\n" >> $EXEC_SUMM
      printf "<tr>\n" >> $EXEC_SUMM
      printf "   <td class=\"opts\">%s</td>\n" ${optnam} >> $EXEC_SUMM
#
#     print clickable links for those tests that failed

      if [[ "${pfarr[$indx]}" = "PASS" || "${pfarr[$indx]}" = "N/A" || "${pfarr[$indx]}" = "DNR" ]]
      then
         printf "   <td class=\"dats\">%s</td>\n" ${pfarr[$indx]} >> $EXEC_SUMM
      else
         filnam="<a href=https://itaps.mcs.anl.gov/${save_dir_href}/${filarr[$indx]}>${pfarr[$indx]}</a>"
         printf "   <td class=\"dats\">%s %s</td>\n" ${filnam} >> $EXEC_SUMM
      fi

      if [[ "${pfarr[$indx1]}" = "PASS" || "${pfarr[$indx1]}" = "N/A" || "${pfarr[$indx1]}" = "DNR" ]]
      then
         printf "   <td class=\"dats\">%s</td>\n" ${pfarr[$indx1]} >> $EXEC_SUMM
      else
         filnam="<a href=https://itaps.mcs.anl.gov/${save_dir_href}/${filarr[$indx1]}>${pfarr[$indx1]}</a>"
         printf "   <td class=\"dats\">%s %s</td>\n" ${filnam} >> $EXEC_SUMM
      fi

      printf "</tr>\n" >> $EXEC_SUMM
   done

   printf "\n" >> $EXEC_SUMM
   printf "</table>\n" >> $EXEC_SUMM

   printf "%s<br>" "------------------------------------------------------------------------" >> $EXEC_SUMM
   errinfo="<a href=https://itaps.mcs.anl.gov/${save_dir_href}/${SUMFILE}>${SUMFILE}</a>"
   printf "For detailed test results see: ${errinfo}<br>" >> $EXEC_SUMM
   
   printf "\n" >> $EXEC_SUMM
   printf "</body>\n" >> $EXEC_SUMM
   printf "</html>\n" >> $EXEC_SUMM

   echo "Done creating unitTest executive summary file"
}


# *************************************************************************** #
# Function: create_summary
#
# Purpose: creates summary of test results
#
# Programmer: Ellen Hill
# Date: Thu Nov 12 2009
#
# *************************************************************************** #
function create_summary
{
   echo "Creating unitTestP summary file"
   BUILD_DIRS="${MOAB_BUILD_DIR} ${FMDB_BUILD_DIR}"
   BUILD_DIRS="${BUILD_DIRS} ${IPCOMM_BUILD_DIR} ${ZOLT_PARM_BUILD_DIR} ${ZOLTAN_BUILD_DIR} ${MESQ_BUILD_DIR}"

   tmpfile="tmp.file"
   SUMFILE="UnitTestP_summary.${rundate}.html"
   
   CurDir=`pwd`

   CLOGCNT=0
   CERRCNT=0
   DWRNCNT=0
   DERRCNT=0

   printf "<html>\n" > $SUMFILE
   printf "<head></head>\n" >> $SUMFILE
   printf "<PRE>\n" >> $SUMFILE
   printf "<body>\n" >> $SUMFILE
   printf "iMeshP ${ITAPS_BRANCH} Error Summary for ${datetim}<br>\n" >> $SUMFILE
   printf "%s<br>" "-----------------------------------------------" >> $SUMFILE
   printf "Operating system: `uname -a`<br>\n" >> $SUMFILE
   printf "Execution Directory:      %s<br>\n" $CurDir >> $SUMFILE
   printf "Results Directory:        %s<br>\n" $save_dir >> $SUMFILE
   printf "%s<br>\n" "-----------------------------------------------" >> $SUMFILE
   printf "Compile Summary:<br>" >> $SUMFILE
   printf "                        Status: Warnings: Errors<br>" >> $SUMFILE
#
#    get configure and build related error messages for each implementation
   for idir in ${BUILD_DIRS} 
   do
      impl_name=`basename ${idir}`
      printf "\n"  >> $SUMFILE
      printf "%s\n" $impl_name >> $SUMFILE

      cd ${idir}
      errfils=`ls *.err`
      cd ${CurDir}

      for errf in ${errfils}
      do
         SuccessCount=0
         FailedCount=0
         DWRNCNT=0
         DERRCNT=0
         CTOTCNT=`expr $CTOTCNT + 1`
         fnam=`echo ${errf} | sed -e 's/\.err$//g'`
         if [ "$fnam" = "make" ]     ## do not include any implementation
         then                        ## specific make.err files
            continue
         fi
         if [ -s "${idir}/${errf}" ] 
         then
            errinfo="<a href=https://itaps.mcs.anl.gov/${save_dir_href}/${errf}>${fnam}</a>"
            CERRCNT=`expr $CERRCNT + 1`
            FailedCount=`expr $FailedCount + 1`
            DWRNNUM=0
            DERRNUM=0
            DERRNM1=0
            DWRNNUM=`grep -i -e " warning: " ${idir}/${errf}|wc -l`
            DERRNUM=`grep -i -e " error: " ${idir}/${errf}|wc -l`
            DERRNM1=`grep -i -e " error " ${idir}/${errf}|wc -l`
            DWRNCNT=`expr $DWRNCNT + $DWRNNUM`
            DERRCNT=`expr $DERRCNT + $DERRNUM + $DERRNM1`
         else
            CLOGCNT=`expr $CLOGCNT + 1`
            SuccessCount=`expr $SuccessCount + 1`
            filinfo="<a href=https://itaps.mcs.anl.gov/${save_dir_href}/${fnam}.log>${fnam}</a>"
         fi

         printf "   %-20s" ${fnam} >> $SUMFILE
         if [[ "$FailedCount" != "0" && "$DERRCNT" = "0" ]] ; then
            printf "Warning:    %4d   :   %4d" $DWRNCNT $DERRCNT >> $SUMFILE
            printf "   ${errinfo}<br>" >> $SUMFILE
         elif [[ "$FailedCount" != "0" && "$DERRCNT" != "0" ]] ; then
            printf " Failed:    %4d   :   %4d" $DWRNCNT $DERRCNT >> $SUMFILE
            printf "   ${errinfo}<br>" >> $SUMFILE
         elif [ "$SuccessCount" != "0" ] ; then
            printf " Passed   ${filinfo}<br>" >> $SUMFILE
         else
            printf "N/A<br>" >> $SUMFILE
         fi
      done   #  end of errfils loop
   done   #  end of idir loop

   printf "<br>" >> $SUMFILE
   printf "%2d test with errors<br>" $CERRCNT  >> $SUMFILE
   printf "%2d test succeeded<br>" $CLOGCNT >> $SUMFILE
   printf "%2d test total<br>" $CTOTCNT >> $SUMFILE
   printf "<br>" >> $SUMFILE

   printf "%s<br>" "-----------------------------------------------" >> $SUMFILE

#
#    unitTest Build Summary
   printf "unitTest Compile Summary:<br>" >> $SUMFILE
   printf "                        Status: Warnings: Errors<br>\n" >> $SUMFILE

   cd ${IMESHP_BUILD_DIR}
   errfils=`ls imeshp*.err`
   cd ${CurDir}

   CLOGCNT=0
   CERRCNT=0
   CTOTCNT=0

   for errf in ${errfils}
   do
      SuccessCount=0
      FailedCount=0
      DWRNCNT=0
      DERRCNT=0
      CTOTCNT=`expr $CTOTCNT + 1`
      fnam=`echo ${errf} | sed -e 's/\.err$//g'`
      if [ -s "${IMESHP_BUILD_DIR}/${errf}" ] 
      then
         errinfo="<a href=https://itaps.mcs.anl.gov/${save_dir_href}/${errf}>${fnam}</a>"
         CERRCNT=`expr $CERRCNT + 1`
         FailedCount=`expr $FailedCount + 1`
         DWRNNUM=0
         DERRNUM=0
         DERRNM1=0
         DWRNNUM=`grep -i -e " warning: " ${IMESHP_BUILD_DIR}/${errf}|wc -l`
         DERRNUM=`grep -i -e " error: " ${IMESHP_BUILD_DIR}/${errf}|wc -l`
         DERRNM1=`grep -i -e " error " ${IMESHP_BUILD_DIR}/${errf}|wc -l`
         DWRNCNT=`expr $DWRNCNT + $DWRNNUM`
         DERRCNT=`expr $DERRCNT + $DERRNUM + $DERRNM1`
      else
         CLOGCNT=`expr $CLOGCNT + 1`
         SuccessCount=`expr $SuccessCount + 1`
         filinfo="<a href=https://itaps.mcs.anl.gov/${save_dir_href}/${fnam}.log>${fnam}</a>"
      fi

      printf "   %-20s" ${fnam} >> $SUMFILE
      if [[ "$FailedCount" != "0" && "$DERRCNT" = "0" ]] ; then
         printf "Warning:    %4d   :   %4d" $DWRNCNT $DERRCNT >> $SUMFILE
         printf "   ${errinfo}<br>" >> $SUMFILE
      elif [[ "$FailedCount" != "0" && "$DERRCNT" != "0" ]] ; then
         printf " Failed:    %4d   :   %4d" $DWRNCNT $DERRCNT >> $SUMFILE
         printf "   ${errinfo}<br>" >> $SUMFILE
      elif [ "$SuccessCount" != "0" ] ; then
         printf " Passed   ${filinfo}<br>" >> $SUMFILE
      else
         printf "N/A<br>" >> $SUMFILE
      fi
   done   #  end of errfils loop

   printf "<br>" >> $SUMFILE
   printf "%2d test with errors<br>" $CERRCNT  >> $SUMFILE
   printf "%2d test succeeded<br>" $CLOGCNT >> $SUMFILE
   printf "%2d test total<br>" $CTOTCNT >> $SUMFILE
   printf "<br>" >> $SUMFILE
#
#     unitTest Execution Summary
   printf "%s<br>" "-----------------------------------------------" >> $SUMFILE
   printf "unitTest Execution Summary: <br>" >> $SUMFILE

   for impl in ${IMPL_NAMES}
   do
      printf "<br>" >> $SUMFILE

      cd ${IMESHP_BUILD_DIR}
      outfls=`ls imeshp*${impl}.out`
      cd ${CurDir}

      if [ "${outfls}" = "" ] 
      then
         echo "****$impl Unit Tests:" >> $SUMFILE
         echo "     Not run - Check error files" >> $SUMFILE
      else
         for fil in ${outfls}
         do
            all_pass=0
            tstname=`echo ${fil} | sed -e s/\.out$//g`
            if [[ -r ${IMESHP_BUILD_DIR}/${fil} ]]
            then
               errf="<a href=https://itaps.mcs.anl.gov/${save_dir_href}/${fil}>${fil}</a>"
               echo "     " >> $SUMFILE
               echo "****$tstname Test" >> $SUMFILE
               while read InpL
               do
                  case $InpL in
                     *#Failed*) nf=`echo $InpL | cut -d" " -f6`
                                numfail=`echo $nf | sed -e s/,//g`
                                ;;
                             *)
                                ;;
                  esac
               done < ${IMESHP_BUILD_DIR}/${fil}
               numerr=`grep -i -e "error" ${IMESHP_BUILD_DIR}/${fil} | wc -l`
               numwarn=`grep -i -e "warn" ${IMESHP_BUILD_DIR}/${fil} | wc -l`
               numexit=`grep -i -e "exit" ${IMESHP_BUILD_DIR}/${fil} | wc -l`
               toterr=`expr $numfail + $numerr + $numwarn + $numexit`
               if [ "$toterr" = "0" ]
               then 
                  echo "     All tests PASSED" >> $SUMFILE
               else
                  echo "     Tests with errors: " >> $SUMFILE
                  echo "        failures: $numfail" >> $SUMFILE
                  echo "        warnings: $numwarn" >> $SUMFILE
                  echo "        errors:   $numerr" >> $SUMFILE
                  echo "        exits:    $numexit" >> $SUMFILE
               fi
               if [ ! -z "$(cat ${IMESHP_BUILD_DIR}/${fil})" ]
               then
                  echo "     Results file: ${errf} " >> $SUMFILE
               else
                  echo "     Abnormal termination " >> $SUMFILE
               fi
            else
               echo "****$tstname Tests:" >> $SUMFILE
               echo "     Not run - Check error files" >> $SUMFILE
            fi
         done
      fi
   done
#
#     unitTest with Valgrind Execution Summary
   printf "%s<br>" "-----------------------------------------------" >> $SUMFILE
   printf "unitTest with Valgrind Execution Summary: <br>" >> $SUMFILE

   for impl in ${IMPL_NAMES}
   do
      printf "<br>" >> $SUMFILE

      cd ${IMESHP_BUILD_DIR}
      outfls=`ls imeshp*${impl}.valg.msg`
      cd ${CurDir}

      if [ "${outfls}" = "" ] 
      then
         echo "  **$impl Unit Tests:" >> $SUMFILE
         echo "     Not run - Check error files" >> $SUMFILE
      else
         for fil in ${outfls}
         do
            if [[ -r ${IMESHP_BUILD_DIR}/${fil} ]]
            then
               tstname=`echo ${fil} | sed -e s/\.valg\.msg$//g`
               errf="<a href=https://itaps.mcs.anl.gov/${save_dir_href}/${fil}>${fil}</a>"
               echo "****$tstname Test with Valgrind" >> $SUMFILE
               echo "     Results file: $errf" >> $SUMFILE
               echo "     " >> $SUMFILE
               echo "Last 12 lines of results: " >> $SUMFILE
         
               tail -12 ${IMESHP_BUILD_DIR}/${fil} >> ${SUMFILE}
               echo "     " >> $SUMFILE
            fi
         done
      fi
   done

#
#    iZoltanP Build Summary
   printf "%s<br>" "-----------------------------------------------" >> $SUMFILE
   printf "iZoltanP Compile Summary:<br>" >> $SUMFILE
   printf "                        Status: Warnings: Errors<br>\n" >> $SUMFILE

   cd ${IZOLTANP_BUILD_DIR}
   errfils=`ls IZOLTAN*.err`
   cd ${CurDir}

   CLOGCNT=0
   CERRCNT=0
   CTOTCNT=0

   for errf in ${errfils}
   do
      SuccessCount=0
      FailedCount=0
      DWRNCNT=0
      DERRCNT=0
      CTOTCNT=`expr $CTOTCNT + 1`
      fnam=`echo ${errf} | sed -e 's/\.err$//g'`
      if [ -s "${IZOLTANP_BUILD_DIR}/${errf}" ] 
      then
         errinfo="<a href=https://itaps.mcs.anl.gov/${save_dir_href}/${errf}>${fnam}</a>"
         CERRCNT=`expr $CERRCNT + 1`
         FailedCount=`expr $FailedCount + 1`
         DWRNNUM=0
         DERRNUM=0
         DERRNM1=0
         DWRNNUM=`grep -i -e " warning: " ${IZOLTANP_BUILD_DIR}/${errf}|wc -l`
         DERRNUM=`grep -i -e " error: " ${IZOLTANP_BUILD_DIR}/${errf}|wc -l`
         DERRNM1=`grep -i -e " error " ${IZOLTANP_BUILD_DIR}/${errf}|wc -l`
         DWRNCNT=`expr $DWRNCNT + $DWRNNUM`
         DERRCNT=`expr $DERRCNT + $DERRNUM + $DERRNM1`
      else
         CLOGCNT=`expr $CLOGCNT + 1`
         SuccessCount=`expr $SuccessCount + 1`
         filinfo="<a href=https://itaps.mcs.anl.gov/${save_dir_href}/${fnam}.log>${fnam}</a>"
      fi

      printf "   %-20s" ${fnam} >> $SUMFILE
      if [[ "$FailedCount" != "0" && "$DERRCNT" = "0" ]] ; then
         printf "Warning:    %4d   :   %4d" $DWRNCNT $DERRCNT >> $SUMFILE
         printf "   ${errinfo}<br>" >> $SUMFILE
      elif [[ "$FailedCount" != "0" && "$DERRCNT" != "0" ]] ; then
         printf " Failed:    %4d   :   %4d" $DWRNCNT $DERRCNT >> $SUMFILE
         printf "   ${errinfo}<br>" >> $SUMFILE
      elif [ "$SuccessCount" != "0" ] ; then
         printf " Passed   ${filinfo}<br>" >> $SUMFILE
      else
         printf "N/A<br>" >> $SUMFILE
      fi
   done   #  end of errfils loop

   printf "<br>" >> $SUMFILE
   printf "%2d test with errors<br>" $CERRCNT  >> $SUMFILE
   printf "%2d test succeeded<br>" $CLOGCNT >> $SUMFILE
   printf "%2d test total<br>" $CTOTCNT >> $SUMFILE
   printf "<br>" >> $SUMFILE
#
#     iZoltanP test Summary
   printf "%s<br>" "-----------------------------------------------" >> $SUMFILE
   printf "iZoltanP Execution Summary: <br>" >> $SUMFILE

   for impl in ${IMPL_NAMES}
   do
      printf "<br>" >> $SUMFILE

      cd ${IZOLTANP_BUILD_DIR}
      outfls=`ls IZOLTANP_${impl}.results`
      cd ${CurDir}

      if [ "${outfls}" = "" ] 
      then
         echo "****$impl iZoltanP Tests:" >> $SUMFILE
         echo "     Not run - Check error files" >> $SUMFILE
      else
         for fil in ${outfls}
         do
            tstname=`echo ${fil} | sed -e s/\.results$//g`
            if [[ -r ${IZOLTANP_BUILD_DIR}/${fil} ]]
            then
               errf="<a href=https://itaps.mcs.anl.gov/${save_dir_href}/${fil}>${fil}</a>"
               echo "     " >> $SUMFILE
               echo "****$tstname Test" >> $SUMFILE
               numerr=`grep -i -e "ERROR" ${IZOLTANP_BUILD_DIR}/${fil} | wc -l`
               numexit=`grep -i -e "exit" ${IZOLTANP_BUILD_DIR}/${fil} | wc -l`
               if [ "$numerr" = "0" ] && [ "$numexit" = "0" ]
               then 
                  echo "     All tests PASSED" >> $SUMFILE
               else
                  numerr=`grep -i -e "error" ${IZOLTANP_BUILD_DIR}/${fil} | wc -l`
                  numwarn=`grep -i -e "warn" ${IZOLTANP_BUILD_DIR}/${fil} | wc -l`
                  numexit=`grep -i -e "exit" ${IZOLTANP_BUILD_DIR}/${fil} | wc -l`
                  echo "     Tests with errors: " >> $SUMFILE
                  echo "        warnings: $numwarn" >> $SUMFILE
                  echo "        errors:   $numerr" >> $SUMFILE
                  echo "        exits:    $numexit" >> $SUMFILE
               fi
               if [ ! -z "$(cat ${IZOLTANP_BUILD_DIR}/${fil})" ]
               then
                  echo "     Results file: ${errf} " >> $SUMFILE
               else
                  echo "     Abnormal termination " >> $SUMFILE
               fi
            else
               echo "****$tstname Tests:" >> $SUMFILE
               echo "     Not run - Check error files" >> $SUMFILE
            fi
         done
      fi
   done

#
#    Mesquite Build Summary
   printf "%s<br>" "-----------------------------------------------" >> $SUMFILE
   printf "Mesquite Compile Summary:<br>" >> $SUMFILE
   printf "                        Status: Warnings: Errors<br>\n" >> $SUMFILE

   cd ${MESQ_BUILD_DIR}
   errfils=`ls MESQ*.err`
   cd ${CurDir}

   CLOGCNT=0
   CERRCNT=0
   CTOTCNT=0

   for errf in ${errfils}
   do
      SuccessCount=0
      FailedCount=0
      DWRNCNT=0
      DERRCNT=0
      CTOTCNT=`expr $CTOTCNT + 1`
      fnam=`echo ${errf} | sed -e 's/\.err$//g'`
      if [ -s "${MESQ_BUILD_DIR}/${errf}" ] 
      then
         errinfo="<a href=https://itaps.mcs.anl.gov/${save_dir_href}/${errf}>${fnam}</a>"
         CERRCNT=`expr $CERRCNT + 1`
         FailedCount=`expr $FailedCount + 1`
         DWRNNUM=0
         DERRNUM=0
         DERRNM1=0
         DWRNNUM=`grep -i -e " warning: " ${MESQ_BUILD_DIR}/${errf}|wc -l`
         DERRNUM=`grep -i -e " error: " ${MESQ_BUILD_DIR}/${errf}|wc -l`
         DERRNM1=`grep -i -e " error " ${MESQ_BUILD_DIR}/${errf}|wc -l`
         DWRNCNT=`expr $DWRNCNT + $DWRNNUM`
         DERRCNT=`expr $DERRCNT + $DERRNUM + $DERRNM1`
      else
         CLOGCNT=`expr $CLOGCNT + 1`
         SuccessCount=`expr $SuccessCount + 1`
         filinfo="<a href=https://itaps.mcs.anl.gov/${save_dir_href}/${fnam}.log>${fnam}</a>"
      fi

      printf "   %-20s" ${fnam} >> $SUMFILE
      if [[ "$FailedCount" != "0" && "$DERRCNT" = "0" ]] ; then
         printf "Warning:    %4d   :   %4d" $DWRNCNT $DERRCNT >> $SUMFILE
         printf "   ${errinfo}<br>" >> $SUMFILE
      elif [[ "$FailedCount" != "0" && "$DERRCNT" != "0" ]] ; then
         printf " Failed:    %4d   :   %4d" $DWRNCNT $DERRCNT >> $SUMFILE
         printf "   ${errinfo}<br>" >> $SUMFILE
      elif [ "$SuccessCount" != "0" ] ; then
         printf " Passed   ${filinfo}<br>" >> $SUMFILE
      else
         printf "N/A<br>" >> $SUMFILE
      fi
   done   #  end of errfils loop

   printf "<br>" >> $SUMFILE
   printf "%2d test with errors<br>" $CERRCNT  >> $SUMFILE
   printf "%2d test succeeded<br>" $CLOGCNT >> $SUMFILE
   printf "%2d test total<br>" $CTOTCNT >> $SUMFILE
   printf "<br>" >> $SUMFILE
#
#     Mesquite test Summary
   printf "%s<br>" "-----------------------------------------------" >> $SUMFILE
   printf "Mesquite Execution Summary: <br>" >> $SUMFILE

   for impl in ${IMPL_NAMES}
   do
      printf "<br>" >> $SUMFILE

      cd ${MESQ_BUILD_DIR}
      outfls=`ls MESQ_${impl}.results`
      cd ${CurDir}

      if [ "${outfls}" = "" ] 
      then
         echo "****$impl Mesquite Tests:" >> $SUMFILE
         echo "     Not run - Check error files" >> $SUMFILE
      else
         for fil in ${outfls}
         do
            tstname=`echo ${fil} | sed -e s/\.results$//g`
            if [[ -r ${MESQ_BUILD_DIR}/${fil} ]]
            then
               errf="<a href=https://itaps.mcs.anl.gov/${save_dir_href}/${fil}>${fil}</a>"
               echo "     " >> $SUMFILE
               echo "****$tstname Test" >> $SUMFILE
               numerr=`grep -i -e "ERROR" ${MESQ_BUILD_DIR}/${fil} | wc -l`
               numwarn=`grep -i -e "WARN" ${MESQ_BUILD_DIR}/${fil} | wc -l`
               numexit=`grep -i -e "exit" ${MESQ_BUILD_DIR}/${fil} | wc -l`
               if [ "$numerr" = "0" ] && [ "$numexit" = "0" ]
               then 
                  echo "     All tests PASSED" >> $SUMFILE
               else
                  echo "     Tests with errors: " >> $SUMFILE
                  echo "        warnings: $numwarn" >> $SUMFILE
                  echo "        errors:   $numerr" >> $SUMFILE
                  echo "        exits:    $numexit" >> $SUMFILE
               fi
               if [ ! -z "$(cat ${MESQ_BUILD_DIR}/${fil})" ]
               then
                  echo "     Results file: ${errf} " >> $SUMFILE
               else
                  echo "     Abnormal termination " >> $SUMFILE
               fi
            else
               echo "****$tstname Tests:" >> $SUMFILE
               echo "     Not run - Check error files" >> $SUMFILE
            fi
         done
      fi
   done

   printf "%s<br>" "-----------------------------------------------" >> $SUMFILE
   printf "</body>\n" >> $SUMFILE
   printf "</PRE>\n" >> $SUMFILE
   printf "</html>\n" >> $SUMFILE

   chmod 664 $SUMFILE
   chgrp itaps $SUMFILE
   echo "Done creating iMeshP_unitTest summary file"
}


# *************************************************************************** #
# Function: create_html_index
#
# Purpose: creates an index files of test results directories
#
# Programmer: Ellen Hill
# Date: Fri Jul 2 2010
#
# *************************************************************************** #
function create_html_index
{
   if [ "${ITAPS_BRANCH}" = "trunk" ]
   then
      INDX_FILE="INDEXP.html"
   else
      INDX_FILE="INDEX_RCP.html"
   fi

   printf "<html>\n" > $INDX_FILE
   printf "<head></head>\n" >> $INDX_FILE
   printf "<PRE>\n" >> $INDX_FILE
   printf "<body>\n" >> $INDX_FILE
   printf "  Index of Test Results Files<br>" >> $INDX_FILE
   printf " ${datetim}<br>\n" >> $INDX_FILE
   printf "%s<br>" "-----------------------------------------------" >> $INDX_FILE

   if [ "${ITAPS_BRANCH}" = "trunk" ]
   then
      tst_dirs=`ls -d -r ${TEST_RESULTS_DIR}/resultsp*`
   else
      tst_dirs=`ls -d -r ${TEST_RESULTS_DIR}/resultsp_rc*`
   fi
   for tstdir in ${tst_dirs}
   do
      errfiles=`ls ${tstdir}/*.err`
      failed=0
      for errf in ${errfiles}
      do
         if [ -s ${errf} ]
         then
            failed=`expr ${failed} + 1`
         fi
      done

      results_file=`ls ${tstdir}/UnitTestP_summary*.html`
      result_name=`basename ${results_file}`
      result_path=`basename ${tstdir}`
      smryfile="<a href=${result_path}/${result_name}>${result_name}</a>"
     
      if [ "${failed}" = "0" ]
      then
         printf "${smryfile}  PASS <br>" >> $INDX_FILE
      else
         printf "${smryfile}  FAIL <br>" >> $INDX_FILE
      fi
   done

   printf "%s<br>" "-----------------------------------------------" >> $INDX_FILE
   printf "</body>\n" >> $INDX_FILE
   printf "</PRE>\n" >> $INDX_FILE
   printf "</html>\n" >> $INDX_FILE

   mv -f ${INDX_FILE} ${TEST_RESULTS_DIR}
}


# *************************************************************************** #
# Function: mail_summary
#
# Purpose: emails summary of test results
#          Define mail recipients and send messages
#          Uses 'sendmail' to set 'Content-Type' to 'text/html'
#
# Programmer: Ellen Hill
# Date: Thu Nov 12 2009
#
# *************************************************************************** #
function mail_summary
{


   SUBJECT="iMeshP ${ITAPS_BRANCH} Test Summary ${datetim}"

   (
      echo To: "itaps-dev@lists.mcs.anl.gov"
      echo From: "hill66@llnl.gov"
      echo Subject: ${SUBJECT}
      echo Content-Type: text/html
      echo MIME-Version: 1.0

      cat $EXEC_SUMM

   ) | /usr/sbin/sendmail -t
}


# *****************************************************************************
#     'Main' Program
# -----------------------------------------------------------------------------
# *****************************************************************************
#  Build and test ITAPS implementations and unit tests
# *****************************************************************************

### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Initialize options to download source code
CLEAN_BUILD="clean"
EMAIL_SUMMARY="send"
GET_MOAB="yes"
GET_FMDB="yes"
GET_ZOLTAN="yes"
GET_IPCOMM="yes"
GET_IMESHP="yes"
GET_IZOLTANP="yes"
GET_MESQ="yes"


### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Process command line arguments
next_arg=""

for arg in $@
do
   if test -n "${next_arg}"
   then
      case ${next_arg} in
           moab-vers) MOAB_VERSION="${arg}"
                      ;;
           moab-path) MOAB_PATH="${arg}"
                      ;;
           moab-file) MOAB_MESHFILE="${arg}"
                      ;;
           fmdb-vers) FMDB_VERSION="${arg}"
                      ;;
           fmdb-path) FMDB_PATH="${arg}"
                      ;;
           fmdb-file) FMDB_MESHFILE="${arg}"
                      ;;
         imeshp-path) IMESHP_PATH="${arg}"
                      ;;
           mesq-path) MESQ_PATH="${arg}"
                      ;;
     esac
     next_arg=""
     continue
   fi

   case ${arg} in
           -h | --help) usage
                        exit 99
                        ;;
        --moab-version) next_arg="moab-vers"
                        ;;
        --fmdb-version) next_arg="fmdb-vers"
                        ;;
      --imeshp-version) next_arg="imeshp-vers"
                        ;;
           --moab-path) next_arg="moab-path"
                        ;;
           --fmdb-path) next_arg="fmdb-path"
                        ;;
         --imeshp-path) next_arg="imeshp-path"
                        ;;
           --moab-file) next_arg="moab-file"
                        ;;
           --fmdb-file) next_arg="fmdb-file"
                        ;;
             --no-moab) GET_MOAB="no"
                        ;;
             --no-fmdb) GET_FMDB="no"
                        ;;
           --no-imeshp) GET_IMESHP="no"
                        ;;
             --no-mesq) GET_MESQ="no"
                        ;;
           --no-clean-build) CLEAN_BUILD="no"
                        ;;
           --no-email) EMAIL_SUMMARY="no"
                        ;;
                     *) echo "Unknown option: ${arg}"
                        ;;
   esac
done

### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Set name of error log file; initialize total error counter
ERRORLOG="`pwd`/ErrorLog.UnitTestP"
echo "           Error Log for iMeshP ${ITAPS_BRANCH} UnitTests" > $ERRORLOG
echo "            ${datetim}" >> $ERRORLOG
errTot=0

### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Set current directory
CurrentDirectory=`pwd`

### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Get data files from repo, remove *.checkout file if empty
if [[ ! -r globe_mats.vtk ]]
then
   echo "Getting data file globe_mats.vtk from ${IMESH_DATA}"
   wget ${IMESH_DATA}/globe_mats.vtk 1> GLOBE.checkout 2> GLOBE.checkout.log
   find . -name 'GLOBE.checkout' -size 0 -exec rm -f {} \;
fi

echo "Getting MOAB input file from ${IMESHP_DATA}"
wget ${IMESHP_DATA}/${MOAB_MESHFILE} 1> MOAB.data.checkout 2> MOAB.data.checkout.log
find . -name 'MOAB.data.checkout' -size 0 -exec rm -f {} \;

echo "Getting FMDB input files from ${IMESHP_DATA}"
for (( i=0; i<6; i++ )) 
do 
   wget ${IMESHP_DATA}/${FMDB_MESHBASE}$i.sms 1>> FMDB.data.checkout 2>> FMDB.data.checkout.log
   find . -name 'FMDB.data.checkout' -size 0 -exec rm -f {} \;
done

### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Get Valgrind suppressions from the repo, remove *.checkout file if empty
if [[ ! -r ${SUPPRESS_FILE} ]]
then
   echo "Getting Valgrind suppressions file from ${SUPPRESS_FILE}"
   wget ${SUPPRESS_FILE} 1> Suppress.checkout 2> Suppress.checkout.log
   find . -name 'Suppress.checkout' -size 0 -exec rm -f {} \;
fi

### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Building MOAB
cd ${CurrentDirectory}

err_get_MOAB=0
err_untar_MOAB=0
err_dir_MOAB=0
err_have_MOAB=1

if [[ "${GET_MOAB}" = "yes" ]]
then
   echo " "
   echo "  - - - - - - - - - - - - - - - - - - - - - - - - - - "

   get_source_MOAB
   err_get_MOAB="$?"
   checkHandleError "$err_get_MOAB" "MOAB source code"

   if [[ "${err_get_MOAB}" = "0" ]]
   then
      uncompress_untar ${MOAB_SOURCE}
      err_untar_MOAB="$?"
      checkHandleError "$err_untar_MOAB" "MOAB untar"
   fi

   if [[ "${err_untar_MOAB}" = "0" ]]
   then
      get_builddir_MOAB
      err_dir_MOAB="$?"
      checkHandleError "$err_dir_MOAB" "MOAB build directory"
   fi

   if [[ "${err_dir_MOAB}" = "0" ]]
   then
      build_check_MOAB
      retval=$?
      err_have_MOAB=$retval

      case $retval in
         30) checkHandleError $retval "MOAB config" \
                             "${MOAB_BUILD_DIR}/MOAB.config.err"
             ;;
         35) checkHandleError $retval "MOAB make" \
                             "${MOAB_BUILD_DIR}/MOAB.make.err"
             ;;
         50) checkHandleError $retval "MOAB lib"
             ;;
      esac
   fi
fi

### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Building IPCOMM
cd ${CurrentDirectory}

err_get_IPCOMM=0
err_untar_IPCOMM=0
err_dir_IPCOMM=0
err_have_IPCOMM=1

if [[ "${GET_IPCOMM}" = "yes" ]]
then
   echo " "
   echo "  - - - - - - - - - - - - - - - - - - - - - - - - - - "

   get_source_IPCOMM
   err_get_IPCOMM="$?"
   checkHandleError "$err_get_IPCOMM" "IPCOMM source code"

   if [[ "${err_get_IPCOMM}" = "0" ]]
   then
      uncompress_untar ${IPCOMM_SOURCE}
      err_untar_IPCOMM="$?"
      checkHandleError "$err_untar_IPCOMM" "IPCOMM untar"
   fi

   if [[ "${err_untar_IPCOMM}" = "0" ]]
   then
      get_builddir_IPCOMM
      err_dir_IPCOMM="$?"
      checkHandleError "$err_dir_IPCOMM" "IPCOMM build directory"
   fi

   if [[ "${err_dir_IPCOMM}" = "0" ]]
   then
      build_check_IPCOMM
      retval=$?
      err_have_IPCOMM=$retval

      case $retval in
         30) checkHandleError $retval "IPCOMM config" \
                             "${IPCOMM_BUILD_DIR}/IPCOMM.config.err"
             ;;
         35) checkHandleError $retval "IPCOMM make" \
                             "${IPCOMM_BUILD_DIR}/IPCOMM.make.err"
             ;;
         50) checkHandleError $retval "IPCOMM lib"
             ;;
      esac
   fi
fi


### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Building  Zoltan & ParMETIS
cd ${CurrentDirectory}

err_get_ZOLTAN=0
err_untar_ZOLTAN=0
err_dir_ZOLTAN=0
err_have_ZOLTAN=0

ZOLT_PARM_SOURCE=""
err_get_ZOLT_PARM=0
err_untar_ZOLT_PARM=0
err_dir_ZOLT_PARM=0
err_have_ZOLT_PARM=0

if [[ "${GET_ZOLTAN}" = "yes" ]]
then
   echo " "
   echo "  - - - - - - - - - - - - - - - - - - - - - - - - - - "

   get_source_ZOLTAN
   err_get_ZOLTAN="$?"
   checkHandleError "$err_get_ZOLTAN" "ZOLTAN source code"

   if [[ "${err_get_ZOLTAN}" = "0" ]]
   then
      uncompress_untar ${ZOLTAN_SOURCE}
      err_untar_ZOLTAN="$?"
      checkHandleError "$err_untar_ZOLTAN" "ZOLTAN untar"
   fi

#
#     the Zoltan distribution contains a ParMETIS distribution that needs to be
#     built before Zoltan can be built
   if [[ "${err_untar_ZOLTAN}" = "0" ]]
   then
      tarfls=`ls *.gz`
      for fil in ${tarfls}
      do
         case ${fil} in
            Par* | par*)
                         ZOLT_PARM_SOURCE="${fil}"
                         ;;
                      *)
                         ;;
         esac
      done

      if [[ "X${ZOLT_PARM_SOURCE}" = "X" ]]
      then
         err_get_ZOLT_PARM="20"
         checkHandleError "$err_get_ZOLT_PARM" "PARMETIS Code for Zoltan"
      fi

      if [[ "${err_get_ZOLT_PARM}" = "0" ]]
      then
         uncompress_untar ${ZOLT_PARM_SOURCE}
         err_untar_ZOLT_PARM=$?
         checkHandleError "$err_untar_ZOLT_PARM" "PARMETIS untar for Zoltan"
      fi

      if [[ "${err_untar_ZOLT_PARM}" = "0" ]]
      then
         get_builddir_ZOLT_PARM
         err_dir_ZOLT_PARM=$?
         checkHandleError "$err_dir_ZOLT_PARM" "PARMETIS build directory for Zoltan"
      fi

      if [[ "${err_dir_ZOLT_PARM}" = "0" ]]
      then
         build_ZOLT_PARM
         retval=$?
         err_have_ZOLT_PARM=$retval
         checkHandleError "$retval" "PARMETIS make for Zoltan" "${ZOLT_PARM_BUILD_DIR}/ZOLT_PARM.make.err"
      fi

#
#     if ParMetis built correctly, continue with building Zoltan, otherwise quit
      if [[ "${err_have_ZOLT_PARM}" != "0" ]]
      then
         checkHandleError "50" "ParMetis did not build, Giving up"
      fi

      cd ${CurrentDirectory}
      get_builddir_ZOLTAN
      err_dir_ZOLTAN="$?"
      checkHandleError "$err_dir_ZOLTAN" "Zoltan build directory"
   fi

   if [[ "${err_dir_ZOLTAN}" = "0" ]]
   then
      build_check_ZOLTAN
      retval=$?
      err_have_ZOLTAN=$retval

      case $retval in
         30) checkHandleError $retval "ZOLTAN config" \
                             "${ZOLTAN_BUILD_DIR}/ZOLTAN.config.err"
             ;;
         35) checkHandleError $retval "ZOLTAN make" \
                             "${ZOLTAN_BUILD_DIR}/ZOLTAN.make.err"
             ;;
         40) checkHandleError $retval "ZOLTAN install" \
                             "${ZOLTAN_BUILD_DIR}/ZOLTAN.mkinstall.err"
             ;;
         50) checkHandleError $retval "ZOLTAN lib"
             ;;
      esac
   fi
fi


### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Building FMDB
cd ${CurrentDirectory}

err_get_FMDB=0
err_untar_FMDB=0
err_dir_FMDB=0
err_have_FMDB=1

if [[ "${GET_FMDB}" = "yes" ]]
then
   echo " "
   echo "  - - - - - - - - - - - - - - - - - - - - - - - - - - "

   get_source_FMDB
   err_get_FMDB="$?"
   checkHandleError "$err_get_FMDB" "FMDB source code"

   if [[ "${err_get_FMDB}" = "0" ]]
   then
      uncompress_untar ${FMDB_SOURCE}
      err_untar_FMDB="$?"
      checkHandleError "$err_untar_FMDB" "FMDB untar"
   fi

   if [[ "${err_untar_FMDB}" = "0" ]]
   then
      build_check_FMDB
      retval=$?
      err_have_FMDB=$retval

      case $retval in
         30) checkHandleError $retval "FMDB config" \
                             "${FMDB_BUILD_DIR}/FMDB.config.err"
             ;;
         35) checkHandleError $retval "FMDB make" \
                             "${FMDB_BUILD_DIR}/FMDB.make.err"
             ;;
         40) checkHandleError $retval "FMDB install" \
                             "${FMDB_BUILD_DIR}/FMDB.mkinstall.err"
             ;;
         50) checkHandleError $retval "FMDB lib"
             ;;
      esac
   fi
fi


### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Building UnitTestP
cd ${CurrentDirectory}

err_get_iMeshp=0
err_dir_iMeshp=0

if [[ "${GET_IMESHP}" = "yes" ]]
then
   echo " "
   echo "  - - - - - - - - - - - - - - - - - - - - - - - - - - "

   get_source_unitTestP
   err_get_iMeshP="$?"
   checkHandleError "$err_get_iMeshP" "unitTestP source code"

   if [[ "${err_get_iMeshP}" = "0" ]]
   then
      get_builddir_unitTestP
      err_dir_iMeshP="$?"
      checkHandleError "$err_dir_iMeshP" "unitTestP build directory"
   fi
fi


### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Running UnitTest for each implementation
cd ${CurrentDirectory}

if [[ "${err_dir_iMeshP}" = "0" ]]
then

   find_MPI

   for impl_name in ${IMPL_NAMES}
   do
      cd ${CurrentDirectory}
      case ${impl_name} in
           MOAB) impl_dir=${MOAB_DEFS_DIR}
                 impl_inp=${MOAB_MESHFILE}
                 rm -rf ./imesh_unitTest_dir/iMesh_protos.h
                 rm -rf ./imesh_unitTest_dir/iMeshP_protos.h
                 rm -rf ./imesh_unitTest_dir/MOAB_FCDefs.h
                 cp -f ${impl_inp} ./imeshp_unitTest_dir
                 cp -f ${MOAB_INSTALL_DIR}/include/iMesh_protos.h ./imeshp_unitTest_dir
                 cp -f ${MOAB_INSTALL_DIR}/include/iMeshP_protos.h ./imeshp_unitTest_dir
                 cp -f ${MOAB_INSTALL_DIR}/include/MOAB_FCDefs.h ./imeshp_unitTest_dir
                 indu=$indm
                 ;;
           FMDB) impl_dir=${FMDB_DEFS_DIR}
                 impl_inp=${FMDB_MESHFILE}
                 rm -rf ./imesh_unitTest_dir/iMesh_protos.h
                 rm -rf ./imesh_unitTest_dir/iMeshP_protos.h
                 rm -rf ./imesh_unitTest_dir/*FCDefs.h
                 cp -f ${FMDB_MESHBASE}* ./imeshp_unitTest_dir
                 cp -f ${FMDB_DEFS_DIR}/../imesh/iMesh_protos.h ./imeshp_unitTest_dir
                 cp -f ${FMDB_DEFS_DIR}/iMeshP_protos.h ./imeshp_unitTest_dir
                 cp -f ${FMDB_DEFS_DIR}/../imesh/*_FCDefs.h ./imeshp_unitTest_dir
                 cp -f ${FMDB_DEFS_DIR}/../imeshp/*_FCDefs.h ./imeshp_unitTest_dir
                 indu=$indf
                 ;;
      esac

      build_run_unitTestP ${impl_name} ${impl_dir} ${impl_inp}
      retval=$?

# - - - -   ck: make errors
      if [[ ${make_err} -eq 0 ]]
      then
         filarr[$indu]=""
         if [[ ! -z "$(cat imeshp_${impl_name}.make.err)" ]]
         then
            numwarn=`grep -i -e "warn" imeshp_${impl_name}.make.err|wc -l`
            if [[ "$numwarn" != "0" ]]
            then
               filarr[$indu]="WARN:imeshp_${impl_name}.make.err"
            fi
         fi
      elif [[ ${make_err} -eq 1 ]]
      then
         numerr=`grep -i -e "error" imeshp_${impl_name}.make.err | wc -l`
         filarr[$indu]="${numerr}:imeshp_${impl_name}.make.err"
      elif [[ ${make_err} -eq 999 ]]
      then
         filarr[$indu]="DNR"
      fi
      indu=`expr $indu + 1`

# - - - -   ck: make-no-spec errors 
      if [[ ${nock_err} -eq 0 ]]
      then
         filarr[$indu]=""
         if [[ ! -z "$(cat imeshp_${impl_name}.make.nospec.err)" ]]
         then
            numwarn=`grep -i -e "warn" imeshp_${impl_name}.make.nospec.err|wc -l`
            if [[ "$numwarn" != "0" ]]
            then
               filarr[$indu]="WARN:imeshp_${impl_name}.make.nospec.err"
            fi
         fi
      elif [[ ${nock_err} -eq 1 ]]
      then
         numerr=`grep -i -e "error" imeshp_${impl_name}.make.nospec.err | wc -l`
         filarr[$indu]="${numerr}:imeshp_${impl_name}.make.nospec.err"
      elif [[ ${nock_err} -eq 999 ]]
      then
         filarr[$indu]="N/A"
      fi
      indu=`expr $indu + 1`

# - - - -   ck: run errors
      if [[ ${run_err} -eq 0 ]]
      then
         filarr[$indu]=""
      elif [[ ${run_err} -eq 999 ]]
      then
         filarr[$indu]="DNR"
      elif [[ ${run_err} -ge 1 ]]
      then
         if [[ ! -z "$(cat imeshp_${impl_name}.out)" ]]
         then
            numerr=`grep -i -e "error" imeshp_${impl_name}.out | wc -l`
            numfail=`grep -i -e "failure" imeshp_${impl_name}.out | wc -l`
            toterr=`expr $numerr + $numfail`
            filarr[$indu]="${toterr}:imeshp_${impl_name}.out"
         fi
      fi
      indu=`expr $indu + 1`

# - - - -   ck: valgrind errors
#             when checking for number of errrors, get only the last line of msg
#             file, the msg file contains output from each processor
      if [[ ${valg_err} -eq 0 ]]
      then
         filarr[$indu]=""
      elif [[ ${valg_err} -eq 999 ]]
      then
         filarr[$indu]="DNR"
      elif [[ ${valg_err} -ge 1 ]]
      then
         if [[ -z "$(cat imeshp_${impl_name}.valg.msg)" ]]
         then
            filarr[$indu]="0:imeshp_${impl_name}.valg.out"
         else
            valg_errs=`grep -i -e "ERROR SUMMARY" imeshp_${impl_name}.valg.msg`
            numerr=`echo ${valg_errs} | cut -d" " -f4`
            if [ "${numerr}" = "0" ]
            then
               filarr[$indu]=""
            else
               filarr[$indu]="${numerr}:imeshp_${impl_name}.valg.msg"
            fi
         fi
      fi
      indu=`expr $indu + 1`

# - - - -     reset implementation indicies
      case ${impl_name} in
           MOAB) indm=$indu
                 ;;
           FMDB) indf=$indu
                 ;;
      esac

      case $retval in
         30) checkHandleError $retval "unitTest/${impl_name} config" \
                             "${IMESHP_BUILD_DIR}/imeshp_${impl_name}.config.err"
             ;;
         35) checkHandleError $retval "unitTest/${impl_name} make" \
                             "${IMESHP_BUILD_DIR}/imeshp_${impl_name}.make.err"
             ;;
         55) checkHandleError $retval "unitTest/${impl_name}" \
                             "*** Error with ${impl_name} build; cannot run tests"
             ;;
          *) checkHandleError $retval "Return value from unitTest/${impl_name}"
             ;;
      esac

      echo " "
      echo "  - - - - - - - - - - - - - - - - - - - - - - - - - - "
   done
fi


### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Running MOAB_unitTest for each implementation
cd ${CurrentDirectory}

if [[ "${err_dir_iMeshP}" = "0" ]]
then
   for impl_name in ${IMPL_NAMES}
   do
      cd ${CurrentDirectory}
      case ${impl_name} in
           MOAB) impl_dir=${MOAB_DEFS_DIR}
                 impl_inp=${MOAB_MESHFILE}
                 rm -rf ./imesh_unitTest_dir/iMesh_protos.h
                 rm -rf ./imesh_unitTest_dir/iMeshP_protos.h
                 rm -rf ./imesh_unitTest_dir/MOAB_FCDefs.h
                 cp -f ${impl_inp} ./imeshp_unitTest_dir
                 cp -f ${MOAB_INSTALL_DIR}/include/iMesh_protos.h ./imeshp_unitTest_dir
                 cp -f ${MOAB_INSTALL_DIR}/include/iMeshP_protos.h ./imeshp_unitTest_dir
                 cp -f ${MOAB_INSTALL_DIR}/include/MOAB_FCDefs.h ./imeshp_unitTest_dir
                 indu=$indm
                 ;;
           FMDB) impl_dir=${FMDB_DEFS_DIR}
                 impl_inp=${FMDB_MESHFILE}
                 rm -rf ./imesh_unitTest_dir/iMesh_protos.h
                 rm -rf ./imesh_unitTest_dir/iMeshP_protos.h
                 rm -rf ./imesh_unitTest_dir/IMESH_FCDefs.h
                 cp -f ${FMDB_MESHBASE}* ./imeshp_unitTest_dir
                 cp -f ${FMDB_DEFS_DIR}/../imesh/iMesh_protos.h ./imeshp_unitTest_dir
                 cp -f ${FMDB_DEFS_DIR}/iMeshP_protos.h ./imeshp_unitTest_dir
                 cp -f ${FMDB_DEFS_DIR}/../imesh/IMESH_FCDefs.h ./imeshp_unitTest_dir
                 cp -f ${FMDB_DEFS_DIR}/IMESHP_FCDefs.h ./imeshp_unitTest_dir
                 indu=$indf
                 ;;
      esac

      run_MOAB_unitTest ${impl_name} ${impl_dir}
      retval=$?

# - - - -   ck: make errors
      if [[ ${make_err} -eq 0 ]]
      then
         filarr[$indu]=""
         if [[ ! -z "$(cat imeshpMOAB_${impl_name}.make.err)" ]]
         then
            numwarn=`grep -i -e "warn" imeshpMOAB_${impl_name}.make.err|wc -l`
            if [[ "$numwarn" != "0" ]]
            then
               filarr[$indu]="WARN:imeshpMOAB_${impl_name}.make.err"
            fi
         fi
      elif [[ ${make_err} -eq 1 ]]
      then
         numerr=`grep -i -e "error" imeshpMOAB_${impl_name}.make.err | wc -l`
         filarr[$indu]="${numerr}:imeshpMOAB_${impl_name}.make.err"
      elif [[ ${make_err} -eq 999 ]]
      then
         filarr[$indu]="DNR"
      fi
      indu=`expr $indu + 1`

# - - - -   ck: run errors
      if [[ ${run_err} -eq 0 ]]
      then
         filarr[$indu]=""
      elif [[ ${run_err} -eq 999 ]]
      then
         filarr[$indu]="DNR"
      elif [[ ${run_err} -ge 1 ]]
      then
         if [[ ! -z "$(cat imeshpMOAB_${impl_name}.out)" ]]
         then
            numerr=`grep -i -e "exit" imeshpMOAB_${impl_name}.out | wc -l`
            filarr[$indu]="${numerr}:imeshpMOAB_${impl_name}.out"
         fi
      fi
      indu=`expr $indu + 1`

# - - - -   ck: valgrind errors
#             when cehcking for number of errrors, get only the last line of msg
#             file, the msg file contains output from each processor
      if [[ ${valg_err} -eq 0 ]]
      then
         filarr[$indu]=""
      elif [[ ${valg_err} -eq 999 ]]
      then
         filarr[$indu]="DNR"
      elif [[ ${valg_err} -ge 1 ]]
      then
         if [[ -z "$(cat imeshpMOAB_${impl_name}.valg.msg)" ]]
         then
            filarr[$indu]="0:imeshpMOAB_${impl_name}.valg.out"
         else
            valg_errs=`grep -i -e "ERROR SUMMARY" imeshpMOAB_${impl_name}.valg.msg`
            numerr=`echo ${valg_errs} | cut -d" " -f4`
            if [ "${numerr}" = "0" ]
            then
               filarr[$indu]=""
            else
               filarr[$indu]="${numerr}:imeshpMOAB_${impl_name}.valg.msg"
            fi
         fi
      fi
      indu=`expr $indu + 1`

# - - - -     reset implementation indicies
      case ${impl_name} in
           MOAB) indm=$indu
                 ;;
           FMDB) indf=$indu
                 ;;
      esac

      case $retval in
         30) checkHandleError $retval "MOAB_unitTest/${impl_name} config" \
                             "${IMESHP_BUILD_DIR}/imeshpMOAB_${impl_name}.config.err"
             ;;
         35) checkHandleError $retval "MOAB_unitTest/${impl_name} make" \
                             "${IMESHP_BUILD_DIR}/imeshpMOAB_${impl_name}.make.err"
             ;;
         55) checkHandleError $retval "MOAB_unitTest/${impl_name}" \
                             "*** Error with ${impl_name} build; cannot run tests"
             ;;
          *) checkHandleError $retval "Return value from MOAB_unitTest/${impl_name}"
             ;;
      esac

      echo " "
      echo "  - - - - - - - - - - - - - - - - - - - - - - - - - - "
   done
fi


### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Build izoltan-parallel and run with each implementation
cd ${CurrentDirectory}

err_get_IZOLTANP=0
err_untar_IZOLTANP=0
err_dir_IZOLTANP=0
err_have_IZOLTANP=1


if [ "${GET_IZOLTANP}" = "yes" ]
then
   echo " "
   echo "  - - - - - - - - - - - - - - - - - - - - - - - - - - "

   get_source_izoltanp
   err_get_IZOLTANP="$?"
   checkHandleError "$err_get_IZOLTANP" "izoltanp source code"

   if [[ "${err_get_IZOLTANP}" = "0" ]]
   then
      uncompress_untar ${IZOLTANP_SOURCE}
      err_untar_IZOLTANP="$?"
      checkHandleError "$err_untar_IZOLTANP" "izoltanp untar"
   fi

   if [[ "${err_untar_IZOLTANP}" = "0" ]]
   then
      get_builddir_izoltanp
      err_dir_IZOLTANP="$?"
      checkHandleError "$err_dir_IZOLTANP" "izoltanp build directory"
   fi
fi


### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Running iZoltan-parallel for each implementation
cd ${CurrentDirectory}

if [[ "${err_dir_IZOLTANP}" = "0" ]]
then
   me=`whoami`
   if [ ! -r /tmp/mpd2.console_${me} ]
   then
      mpd &   #### initialize mpi daemon
   fi
   for impl_name in ${IMPL_NAMES}
   do
      case ${impl_name} in
           MOAB) impl_dir=${MOAB_DEFS_DIR}
                 impl_defs=${MOAB_DEFS_FILE}
                 cp -f ${MOAB_MESHFILE} ${IZOLTANP_BUILD_DIR}
                 indl=$indm
                 ;;
           FMDB) impl_dir=${FMDB_DEFS_DIR}
                 impl_defs=${FMDB_DEFS_FILE}
                 rm -rf ${IZOLTANP_BUILD_DIR}/IMESH_FCDefs.h
                 rm -rf ${IZOLTANP_BUILD_DIR}/${MOAB_MESHFILE}
                 cp -f ${FMDB_MESHBASE}* ${IZOLTANP_BUILD_DIR}
                 cp -f ${FMDB_BUILD_DIR}/imesh/IMESH_FCDefs.h ${FMDB_INSTALL_DIR}/include
                 indl=$indf
                 ;;
      esac

      build_check_izoltanp ${impl_name} ${impl_dir} ${impl_defs}
      retval=$?

# - - - -  Set config to N/A as it is not needed
      if [[ ${config_err} -eq 0 ]]
      then
         filarr[$indl]=""
         if [[ ! -z "$(cat IZOLTANP_${impl_name}.config.err)" ]] 
         then
            numwarn=`grep -i -e "warn" IZOLTANP_${impl_name}.config.err|wc -l`
            if [[ "$numwarn" != "0" ]]
            then
               filarr[$indl]="WARN:IZOLTANP_${impl_name}.config.err"
            fi
         fi
      elif [[ ${config_err} -eq 1 ]]
      then
         numerr=`grep -i -e "error" IZOLTANP_${impl_name}.config.err | wc -l`
         filarr[$indl]="${numerr}:IZOLTANP_${impl_name}.config.err"
      elif [[ ${config_err} -eq 999 ]]
      then
         filarr[$indl]="DNR"
      fi
      indl=`expr $indl + 1`

# - - - -   ck: make errors
      if [[ ${make_err} -eq 0 ]]
      then
         filarr[$indl]=""
         if [[ ! -z "$(cat IZOLTANP_${impl_name}.make.err)" ]]
         then
            numwarn=`grep -i -e "warn" IZOLTANP_${impl_name}.make.err|wc -l`
            if [[ "$numwarn" != "0" ]]
            then
               filarr[$indl]="WARN:IZOLTANP_${impl_name}.make.err"
            fi
         fi
      elif [[ ${make_err} -eq 1 ]]
      then
         numerr=`grep -i -e "error" IZOLTANP_${impl_name}.make.err | wc -l`
         filarr[$indl]="${numerr}:IZOLTANP_${impl_name}.make.err"
      elif [[ ${make_err} -eq 999 ]]
      then
         filarr[$indl]="DNR"
      fi
      indl=`expr $indl + 1`

# - - - -   ck: run errors
      if [[ ${run_err} -eq 0 ]]
      then
         filarr[$indl]=""
      elif [[ ${run_err} -eq 1 ]]
      then
         numerr=`grep -i -e "error" IZOLTANP_${impl_name}.results | wc -l`
         filarr[$indl]="${numerr}:IZOLTANP_${impl_name}.results"
      elif [[ ${run_err} -eq 999 ]]
      then
         filarr[$indl]="DNR"
      fi
      indl=`expr $indl + 1`

# - - - -     reset implementation indicies
      case ${impl_name} in
           MOAB) indm=$indl
                 ;;
           FMDB) indf=$indl
                 ;;
      esac

      case $retval in
         30) checkHandleError $retval "izoltanp/${impl_name} config" \
                             "${IZOLTANP_BUILD_DIR}/IZOLTANP_${impl_name}.config.err"
             ;;
         35) checkHandleError $retval "izoltanp/${impl_name} make" \
                             "${IZOLTANP_BUILD_DIR}/IZOLTANP_${impl_name}.make.err"
             ;;
         55) checkHandleError $retval "izoltanp/${impl_name}" \
                             "*** Error with ${impl_name} build; cannot run tests"
             ;;
          *) checkHandleError $retval "Return value from izoltanp/${impl_name}"
             ;;
      esac

      echo " "
      echo "  - - - - - - - - - - - - - - - - - - - - - - - - - - "
   done
fi


### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Building Mesquite with each implementation

echo " "
echo "  - - - - - - - - - - - - - - - - - - - - - - - - - - "

cd ${CurrentDirectory}

err_get_MESQ=0
err_untar_MESQ=0
err_dir_MESQ=0

if [[ "${GET_MESQ}" = "yes" ]]
then
   get_source_MESQ ${ITAPS_MESQ_SOURCE}
   err_get_MESQ="$?"
   checkHandleError "$err_get_MESQ" "Mesquite source code"

   if [[ "${err_get_MESQ}" = "0" ]]
   then
      uncompress_untar ${ITAPS_MESQ_SOURCE}
      err_untar_MESQ="$?"
      checkHandleError "$err_untar_MESQ" "Mesquite untar"
   fi

   if [[ "${err_untar_MESQ}" = "0" ]]
   then
      get_builddir_MESQ
      err_dir_MESQ="$?"
      checkHandleError "$err_dir_MESQ" "Mesquite build directory"
   fi

   if [[ "${err_dir_MESQ}" = "0" ]]
   then
      for impl_name in ${IMPL_NAMES}
      do
         case ${impl_name} in
              MOAB) impl_dir=${MOAB_DEFS_DIR}
                    impl_defs=${MOAB_DEFS_FILE}
                    indq=${indm}
                    ;;
              FMDB) impl_dir=${FMDB_DEFS_DIR}
                    impl_defs=${FMDB_DEFS_FILE}
                    indq=${indf}
                    ;;
         esac

         build_check_MESQ ${impl_name} ${impl_dir} ${impl_defs}
         retval=$?

# - - - -   ck: config errors
         if [[ ${config_err} -eq 0 ]]
         then
            filarr[$indq]=""
            if [[ ! -z "$(cat MESQ_${impl_name}.config.err)" ]]
            then
               numwarn=`grep -i -e "warn" MESQ_${impl_name}.config.err|wc -l`
               if [[ "$numwarn" != "0" ]]
               then
                  filarr[$indq]="WARN:MESQ_${impl_name}.config.err"
               fi
            fi
         elif [[ ${config_err} -eq 1 ]]
         then
            numerr=`grep -i -e "error" MESQ_${impl_name}.config.err|wc -l`
            filarr[$indq]="${numerr}:MESQ_${impl_name}.config.err"
         elif [[ ${config_err} -eq 999 ]]
         then
            filarr[$indq]="DNR"
         fi
         indq=`expr $indq + 1`

# - - - -   ck: make errors
         if [[ ${make_err} -eq 0 ]]
         then
            filarr[$indq]=""
            if [[ ! -z "$(cat MESQ_${impl_name}.make.err)" ]]
            then
               numwarn=`grep -i -e "warn" MESQ_${impl_name}.make.err|wc -l`
               if [[ "$numwarn" != "0" ]]
               then
                  filarr[$indq]="WARN:MESQ_${impl_name}.make.err"
               fi
            fi
         elif [[ ${make_err} -eq 1 ]]
         then
            numerr=`grep -i -e "error" MESQ_${impl_name}.make.err|wc -l`
            filarr[$indq]="${numerr}:MESQ_${impl_name}.make.err"
         elif [[ ${make_err} -eq 999 ]]
         then
            filarr[$indq]="DNR"
         fi
         indq=`expr $indq + 1`

# - - - -   ck: check errors
         if [[ ${check_err} -eq 0 ]]
         then
            filarr[$indq]=""
            if [[ ! -z "$(cat MESQ_${impl_name}.results)" ]]
            then
               numwarn=`grep -i -e "warn" MESQ_${impl_name}.results|wc -l`
               if [[ "$numwarn" != "0" ]]
               then
                  filarr[$indq]="WARN:MESQ_${impl_name}.results"
               fi
            fi
         elif [[ ${check_err} -eq 1 ]]
         then
            numerr=`grep -i -e "failed" MESQ_${impl_name}.results|wc -l`
            filarr[$indq]="${numerr}:MESQ_${impl_name}.results"
         elif [[ ${check_err} -eq 999 ]]
         then
            filarr[$indq]="DNR"
         fi
         indq=`expr $indq + 1`

# - - - -    reset implementation indicies
         case ${impl_name} in
              MOAB) indm=${indq}
                    ;;
              FMDB) indf=${indq}
                    ;;
         esac

         case $retval in
            30) checkHandleError $retval "Mesquite/${impl_name} config" \
                                "${MESQ_BUILD_DIR}/MESQ_${impl_name}.config.err"
                ;;
            35) checkHandleError $retval "Mesquite/${impl_name} make" \
                                "${MESQ_BUILD_DIR}/MESQ_${impl_name}.make.err"
                ;;
            55) checkHandleError $retval \
                           "Mesquite/${impl_name}; problem with build of ${impl_name}"
                ;;
             *) checkHandleError $retval "Return value from Mesquite/${impl_name}"
                ;;
         esac

         echo "  - - - - - - - - - - - - - - - - - - - - - - - - - - "
         echo " "
      done
   fi
fi


### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Define directory in which to save results files
###     Create test summary file and email results
cd ${CurrentDirectory}

if [ "${ITAPS_BRANCH}" = "trunk" ]
then
   save_dir="${TEST_RESULTS_DIR}/resultsp.${rundate}"
   save_dir_href="resultsp.${rundate}"
else
   save_dir="${TEST_RESULTS_DIR}/resultsp_rc.${rundate}"
   save_dir_href="resultsp_rc.${rundate}"
fi 

if [ ! -d ${save_dir} ] 
then
   mkdir ${save_dir}
fi

create_summary
create_exec_summary
if [[ "${EMAIL_SUMMARY}" = "send" ]]
then
    mail_summary
fi

### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Remove LINK files if not NULL
if [[ "${MOAB_LINK}" != "" ]]
then
   rm -rf ${MOAB_LINK}
fi

if [[ "${FMDB_LINK}" != "" ]]
then
   rm -rf ${FMDB_LINK}
fi


### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Store error files in results sub-directory with dates for those
###     cases that were run

if [[ "${GET_MOAB}" = "yes" ]]
then
   mv -f ${MOAB_BUILD_DIR}/MOAB*.err  ${save_dir}
   mv -f ${MOAB_BUILD_DIR}/MOAB*.log  ${save_dir}
   mv -f ${MOAB_BUILD_DIR}/moab_results  ${save_dir}
fi

if [[ "${GET_FMDB}" = "yes" ]]
then
   mv -f ${FMDB_BUILD_DIR}/FMDB*.err  ${save_dir}
   mv -f ${FMDB_BUILD_DIR}/FMDB*.log  ${save_dir}
fi

if [[ "${GET_IMESHP}" = "yes" ]]
then
   mv -f ${IMESHP_BUILD_DIR}/imesh*.err  ${save_dir}
   mv -f ${IMESHP_BUILD_DIR}/imesh*.log  ${save_dir}
   mv -f ${IMESHP_BUILD_DIR}/imesh*.msg  ${save_dir}
   mv -f ${IMESHP_BUILD_DIR}/imesh*.out  ${save_dir}
   mv -f ${IMESHP_BUILD_DIR}/Out*  ${save_dir}
fi

if [[ "${GET_IPCOMM}" = "yes" ]]
then
   mv -f ${IPCOMM_BUILD_DIR}/*.err ${save_dir}
   mv -f ${IPCOMM_BUILD_DIR}/*.log ${save_dir}
fi

if [[ "${GET_ZOLTAN}" = "yes" ]]
then
   mv -f ${ZOLTAN_BUILD_DIR}/*.err ${save_dir}
   mv -f ${ZOLTAN_BUILD_DIR}/*.log ${save_dir}
   mv -f ${ZOLT_PARM_BUILD_DIR}/*.err ${save_dir}
   mv -f ${ZOLT_PARM_BUILD_DIR}/*.log ${save_dir}
fi

if [[ "${GET_IZOLTANP}" = "yes" ]]
then
   mv -f ${IZOLTANP_BUILD_DIR}/*.err ${save_dir}
   mv -f ${IZOLTANP_BUILD_DIR}/*.log ${save_dir}
   mv -f ${IZOLTANP_BUILD_DIR}/*.results ${save_dir}
fi

if [[ "${GET_MESQ}" = "yes" ]]
then
   mv -f ${MESQ_BUILD_DIR}/*.err ${save_dir}
   mv -f ${MESQ_BUILD_DIR}/*.log ${save_dir}
   mv -f ${MESQ_BUILD_DIR}/*.results ${save_dir}
fi

mv -f ${CurrentDirectory}/build_* ${save_dir}
mv -f ${CurrentDirectory}/buildF* ${save_dir}
mv -f ${CurrentDirectory}/*.cron* ${save_dir}
mv -f ${CurrentDirectory}/*.checkout* ${save_dir}
mv -f ${CurrentDirectory}/*.err ${save_dir}
mv -f ${CurrentDirectory}/*.log ${save_dir}
mv -f ${CurrentDirectory}/*.vtk ${save_dir}
mv -f ${CurrentDirectory}/*.html ${save_dir}
mv -f ${CurrentDirectory}/Error* ${save_dir}
mv -f ${CurrentDirectory}/ITAPS* ${save_dir}

### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###      create tar file for SCOREC files
if [[ -d SCOREC_Software ]]
then
   tar -cf FMDB.tar SCOREC_Software/
fi

### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###      remove scotch tar files as they are part of Zoltan
rm -rf scotch* 

### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###      gzip all tar files then move to the results directory
tar_fils=`ls *.tar`
for fil in ${tar_fils}
do
   gzip ${fil}
done

mv -f ${CurrentDirectory}/*.tar.gz ${save_dir}

### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Clean up build directories; otherwise the script can 
###     --- and often does --- find the wrong build dir.
if [[ "${CLEAN_BUILD}" = "clean" ]]
then
    rm -rf ${MOAB_BUILD_DIR}
    rm -rf 24*
    rm -rf MOAB*.h5m
    rm -rf auxil*
    rm -rf SCOREC_Software
    rm -rf fmdbParallel
    rm -rf ${FMDB_BUILD_DIR}
    rm -rf ${IMESHP_BUILD_DIR}
    rm -rf ${ZOLT_PARM_BUILD_DIR}
    rm -rf ${IPCOMM_BUILD_DIR}
    rm -rf ${MESQ_BUILD_DIR}
    rm -rf izoltan*
    rm -rf parme*
    rm -rf zoltan*
    rm -rf Zoltan_*
fi


### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Add line numbers to all *.log files in the storage directory
CurDir=`pwd`
cd ${save_dir}
logfils=`ls *.log`
for lfil in ${logfils}
do
   cat ${lfil} | nl > tmpfil
   rm -rf ${lfil}
   mv tmpfil ${lfil}
done
cd ${CurDir}


### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Create index of results directories
create_html_index


### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Change permissions of results directories
chmod -R 775 ${save_dir}
chgrp -R itaps ${save_dir}

exit 0
