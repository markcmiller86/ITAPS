#!/bin/bash

# *****************************************************************************
#
# Script: run_iMesh_test
#
# Purpose:
#    This script is used to run iMesh UnitTests for ITAPS implementations
#
# Initial Implementation:
#    November 2009
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
#   Section 3:  GRUMMP Routines
#               get_source_GRUMMP:   get source code from defined location
#               get_builddir_GRUMMP: find directory in which source was built
#               build_check_GRUMMP:  configure, build and verify build 
#
#   Section 4:  MOAB Routines
#               get_source_MOAB:     get source code from defined location    
#               get_builddir_MOAB:   find directory in which source was built
#               build_check_MOAB:    configure, build and verify build 
#
#   Section 5:  FMDB Routines
#               get_source_FMDB:     get source code from defined location
#               get_builddir_FMDB:   find directory in which source was built
#               build_check_FMDB:    configure, build and verify build 
#
#   Section 6:  RefImpl Routines
#               get_source_RefImpl:     get source code from defined location
#               get_builddir_RefImpl:   find directory in which source was built
#               build_check_RefImpl:    configure, build and verify build 
#
#   Section 7:  UnitTest Routines
#               get_source_UnitTest:   get source code from defined location
#               get_builddir_UnitTest: find directory in which source was built
#               build_test_UnitTest:   configure and build source code
#
#   Section 8:  MESQUITE Routines
#               get_source_MESQ:     get source code from defined location
#               get_builddir_MESQ:   find directory in which source was built
#               build_check_MESQ:    configure, build and verify build 
#
#   Section 9:  Swapping Routines
#               get_source_SWAP:     get source code from defined location
#               get_builddir_SWAP:   find directory in which source was built
#               build_check_SWAP:    configure, build and verify build 
#
#   Summary File Routines:
#               create_exec_summary: create PASS/FAIL summary in html
#               create_html_index: create list of *.results files in html
#               create_summary: create test results summary file
#               mail_summary:   email test results summary file
#
#   'Main' Program:
#               Process command line arguments
#               Building GRUMMP
#               Building MOAB
#               Building FMDB
#               Building RefImpl
#               Building unitTest
#               Run unitTest for each implementation
#               Build Mesquite with each implementation
#               Build Swapping with each implementation
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
#   Mark C. Miller, Mon Sep 13 08:47:22 PDT 2010
#   I added full URL to all href constructs so that recipients of emails can
#   just click and follow links.
#
#   Ellen M. Hill, Mon Sep 13 09:47:22 PDT 2010
#   Adding info to access tar files through symbolic links
#   Version numbers will be NULL if symbolic links are used
#
#   Ellen M. Hill, Thu Sep 16 15:43:22 PDT 2010
#   Added RefImpl to tests
#
#   Mark C. Miller, Wed Sep 29 11:20:01 PDT 2010
#   Replaced save_dir with save_dir_href in all href constructs.
#
#   Ellen M. Hill, Wed Oct 27 13:42:22 PDT 2010
#   Fixed error return flags for REFI tests
#   Updated location for Mesquite
#
#   Mark C. Miller, Tue Nov  9 11:55:01 PST 2010
#   Added --no-clean-build and --no-email options.
#
#   Ellen M. Hill, Tue Nov  9 13:42:01 PST 2010
#   Changed location of tarballs being retrieved from the repo to 
#   redmine.scorec.rpi.edu/anonsvn/itaps
#
#   Ellen M. Hill, Tue Nov 16 09:25:23 PST 2010
#   Fixed typos
#   Moved the call to create the HTML summary file outside of the email test
#      in order to have the summary file even if it is not mailed
#
#   Mark C. Miller, Wed Nov 17 09:07:19 PST 2010
#   Inverted sense of --save-load to --no-save-load as default for unitTest
#   has been to turn ON save load and require explicit command line argument
#   now to turn off. Thus, all instances of 'save-load' were replaced with
#   'no-save-load'
#
#   Mark C. Miller, Wed Nov 17 22:56:54 PST 2010
#   Changed where it obtaines unitTest code. svn co from repo of current
#   unitTest code and NOT from Distributions dir. I don't even know why we've
#   got a copy in Distributions dir. Next, --no-<whatever> options didn't work.
#   I made them work. I also adjusted logic so that if -no-<whatever> was
#   specified, it would NOT just skip the download attempt but would also skip
#   attempts to untar and build as well as run iMesh_unitTest. I did all this
#   only for implementations. Didn't touch services. 
#
#   Ellen M. Hill, Wed Dec 22 10:30:37 PST 2010
#   Changed retrieval and build routines for FMDB to reflect the use of the 
#   buildImeshTarballs.sh script
#
#   Ellen M. Hill, Fri Jan 21 15:22:37 PST 2011
#   Changed locations for implementations to reflect new repository organization
#
#   Ellen M. Hill, Tue Jan 25 13:57:37 PST 2011
#   Extract brick.vtk and globe_mats.vtk from repo
#
#   Ellen M. Hill, Mon Jan 31 10:45:53 PST 2011
#   Fixed return value that prevented unitTest from running with MOAB.
#
#   Mark C. Miller, Mon Jan 31 11:03:26 PST 2010
#   Make the second attempt to re-make smarter and do so only if the first
#   was a failure due to impl. spec check.
#
#   Ellen M. Hill, Thu Feb 03 08:21:53 PST 2011
#   Added executive summary of pass/fail test results with link to detailed
#
#   Ellen M. Hill, Tue Feb 08 12:27:41 PST 2011
#   Fixed errors in exec summary initial reports - it's a kludge that will
#   be fixed when this code gets refactored.
#
#   Ellen M. Hill, Tue Feb 15 12:37:41 PST 2011
#   Added clickable links in executive summary for failed tests.
#
#   Ellen M. Hill, Fri Feb 25 14:47:41 PST 2011
#   Refactored code; Added 'DNR' for cases that did not run and N/A for
#   options that are not available (i.e. not all codes use 'make install')
#   Real HTML table used for executive summary; hopefully resolved link issues.
#
#   Ellen M. Hill, Wed Mar 02 15:03:41 PST 2011
#   Added Mesquite results to executive summary
#
#   Ellen M. Hill, Fri Mar 04 10:56:41 PST 2011
#   Adding Swapping to tests; cleaned up executive summary formatting
#
#   Ellen M. Hill, Mon Mar 14 12:51:35 PDT 2011
#   More code refactoring; added 'WARN' to executinve summary; updated FMDB 
#   and Mesquite paths
#
#   Ellen M. Hill, Thu Mar 17 15:30:42 PDT 2011
#   Build HDF5-1.8.6 in /usr/local/itaps/hdf5-1.8.6-serial; added --with-hdf5
#   flag to MOAB configure for this path.
#
#   Ellen M. Hill, Fri Mar 18 09:26:17 PDT 2011
#   Added links to log files for tests that passed in detailed summary.
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
export ITAPS_GRUMMP_VERSION=""
export ITAPS_GRUMMP_SOURCE=""
export ITAPS_GRUMMP_LINK=""
export ITAPS_GRUMMP_PATH="http://tetra.mech.ubc.ca/svn/GRUMMP/trunk"
export GRUMMP_MESHFILE="globe_mats.vtk"

export ITAPS_MOAB_VERSION="nightly"
export ITAPS_MOAB_SOURCE="moab-${ITAPS_MOAB_VERSION}.tar.gz"
export ITAPS_MOAB_LINK=""
export ITAPS_MOAB_PATH="http://gnep.mcs.anl.gov:8010"
export MOAB_MESHFILE="globe_mats.vtk"

export ITAPS_FMDB_VERSION=""
export ITAPS_FMDB_SOURCE="build_FMDB_GMI.sh"
export ITAPS_FMDB_LINK=""
export ITAPS_FMDB_PATH="http://www.scorec.rpi.edu/FMDB/source"
export FMDB_MESHFILE="brick.vtk"

export ITAPS_REFI_VERSION=""
export ITAPS_REFI_SOURCE=""
export ITAPS_REFI_LINK=""
export ITAPS_REFI_PATH="http://tetra.mech.ubc.ca/svn/iMesh-refImpl"
export REFI_MESHFILE="globe_mats.vtk"

export ITAPS_MESQ_VERSION="2.99"
export ITAPS_MESQ_SOURCE="mesquite-${ITAPS_MESQ_VERSION}.tar.gz"
export ITAPS_MESQ_LINK=""
export ITAPS_MESQ_PATH="http://software.sandia.gov/~jakraft"

export ITAPS_SWAP_VERSION=""
export ITAPS_SWAP_SOURCE=""
export ITAPS_SWAP_LINK=""
export ITAPS_SWAP_PATH="https://redmine.scorec.rpi.edu/anonsvn/itaps/software/${ITAPS_BRANCH}/services/Swapping/C_API"

export ZOLTAN_VERSION="3.3"
export ZOLTAN_SOURCE=""
export ZOLTAN_LINK=""
export ZOLTAN_PATH="https://redmine.scorec.rpi.edu/anonsvn/itaps/software/${ITAPS_BRANCH}/release_distros/Zoltan_distrib_${ZOLTAN_VERSION}.tar.gz"

export ITAPS_IMESH_PATH="https://redmine.scorec.rpi.edu/anonsvn/itaps/software/${ITAPS_BRANCH}/interfaces/iMesh/test/unit"

export ITAPS_IMESH_FILES_PATH="https://redmine.scorec.rpi.edu/anonsvn/itaps/software/${ITAPS_BRANCH}/interfaces/iMesh/data"

# *****************************************************************************
#     Set directory names for test execution and results
# *****************************************************************************
export TEST_RESULTS_DIR="/usr/local/itaps/testing/test_results"

if [ ! -d ${TEST_RESULTS_DIR} ]
then
   mkdir ${TEST_RESULTS_DIR}
fi


# *****************************************************************************
#     Set implementaton names
# *****************************************************************************
export IMPL_NAMES="GRUMMP MOAB FMDB REFI"


# *****************************************************************************
#     Set execution options for UnitTest
# *****************************************************************************
export UNIT_IMPL_OPTS="save-load --no-save-load valgrind"


# *****************************************************************************
#     set number of implementations and number of options
# *****************************************************************************
num_impl=4
num_opts=17

max_ind=$((num_impl*num_opts))

# *****************************************************************************
#     Initialize output file array to 'DNR' and set starting indicies
#     for each implementation
# *****************************************************************************
declare -a filarr
declare -a pfarr

indg=0                           #GRUMMP starting index
indm=`expr $indg + $num_opts`    #MOAB starting index
indf=`expr $indm + $num_opts`    #FMDB starting index
indr=`expr $indf + $num_opts`    #RefImpl starting index

for (( i=0; i<${max_ind}; i++ )) 
do 
   filarr[$i]="DNR"
done


# *****************************************************************************
#     Set name of iMesh definition file
# *****************************************************************************
DEFS_FILE="iMesh-Defs.inc"


# *****************************************************************************
#     help/how-to-use routine
# *****************************************************************************
usage()
{
   printf "\n"
   printf "Usage: %s [options]\n" $0
   printf "\n"
   printf "%-15s %s\n" "-h or --help" "Prints this help package and terminates"
   printf "%-15s %s [Default: %s]\n" "--itaps-branch" \
                    "path within repo from which to retrieve source code" \
                    "trunk"
   printf "\n"
   printf "The following flags effect global behavior:\n"
   printf "   %-20s %s [Default: %s]\n" "--no-clean-build" \
                       "Do not remove build directories"  "$CLEAN_BUILD"
   printf "   %-20s %s [Default: %s]\n" "--no-email" \
                       "Do not email test results." "$EMAIL_SUMMARY"
   printf "\n"
   printf "The following flags DISABLE the downloads for each code:\n"
   printf "   %-11s %s [Default: %s]\n" "--no-grummp" "Do NOT download GRUMMP" \
                                     "$GET_GRUMMP"
   printf "   %-11s %s [Default: %s]\n" "--no-moab" "Do NOT download MOAB" \
                                     "$GET_MOAB"
   printf "   %-11s %s [Default: %s]\n" "--no-fmdb" "Do NOT download FMDB" \
                                     "$GET_FMDB"
   printf "   %-11s %s [Default: %s]\n" "--no-refi" "Do NOT download RefImpl" \
                                     "$GET_REFI"
   printf "   %-11s %s [Default: %s]\n" "--no-mesq" "Do NOT download Mesquite" \
                                     "$GET_MESQ"
   printf "   %-11s %s [Default: %s]\n" "--no-swap" "Do NOT download Swapping" \
                                     "$GET_SWAP"
   printf "   %-11s %s [Default: %s]\n" "--no-imesh" \
                                   "Do NOT download iMesh_unitTest" "$GET_IMESH"
   printf "\n"
   printf "The following flags are used to change the default version number "
   printf "for each code:\n"
   printf "   %-16s %s [Default: %s]\n" "--grummp-version" \
                                     "Download GRUMMP version <number>" \
                                     "$ITAPS_GRUMMP_VERSION"
   printf "   %-16s %s [Default: %s]\n" "--moab-version" \
                                     "Download MOAB version <number>" \
                                     "$ITAPS_MOAB_VERSION"
   printf "   %-16s %s [Default: %s]\n" "--fmdb-version" \
                                     "Download FMDB version <number>" \
                                     "$ITAPS_FMDB_VERSION"
   printf "   %-16s %s [Default: %s]\n" "--refi-version" \
                                     "Download RefImpl version <number>" \
                                     "$ITAPS_REFI_VERSION"
   printf "   %-16s %s [Default: %s]\n" "--mesq-version" \
                                     "Download MESQ version <number>" \
                                     "$ITAPS_MESQ_VERSION"
   printf "   %-16s %s [Default: %s]\n" "--swap-version" \
                                     "Download SWAP version <number>" \
                                     "$ITAPS_SWAP_VERSION"
   printf "\n"
   printf "The following flags are used to change the default directory path "
   printf "for each code:\n"
   printf "   %-13s %s [Default: %s]\n" "--grummp-path" "/path/to/GRUMMP" \
                                     "$ITAPS_GRUMMP_PATH"
   printf "   %-13s %s [Default: %s]\n" "--moab-path" "/path/to/MOAB" \
                                     "$ITAPS_MOAB_PATH"
   printf "   %-13s %s [Default: %s]\n" "--fmdb-path" "/path/to/FMDB" \
                                     "$ITAPS_FMDB_PATH"
   printf "   %-13s %s [Default: %s]\n" "--refi-path" "/path/to/RefImpl" \
                                     "$ITAPS_REFI_PATH"
   printf "   %-13s %s [Default: %s]\n" "--mesq-path" "/path/to/MESQ" \
                                     "$ITAPS_MESQ_PATH"
   printf "   %-13s %s [Default: %s]\n" "--swap-path" "/path/to/Swapping" \
                                     "$ITAPS_SWAP_PATH"
   printf "   %-13s %s [Default: %s]\n" "--imesh-path" "/path/to/IMESH_unitTest" \
                                     "$ITAPS_IMESH_PATH"
   printf "\n"
   printf "The following flags are used to change the default input mesh file\n"
   printf "   %-13s %s [Default: %s]\n" "--grummp-file" "Use specified input mesh file" \
                                     "$GRUMMP_MESHFILE"
   printf "   %-13s %s [Default: %s]\n" "--moab-file" "Use specified input mesh file" \
                                     "$MOAB_MESHFILE"
   printf "   %-13s %s [Default: %s]\n" "--fmdb-file" "Use specified input mesh file" \
                                     "$FMDB_MESHFILE"
   printf "   %-13s %s [Default: %s]\n" "--refi-file" "Use specified input mesh file" \
                                     "$REFI_MESHFILE"
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
#     Section 3 - GRUMMP Routines
# -----------------------------------------------------------------------------
# *****************************************************************************
# Function: get_source_GRUMMP
#
# Purpose:  get GRUMMP source code 
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
# Date: Mon Nov 2 2009
#
# *****************************************************************************
function get_source_GRUMMP
{
#     ITAPS_GRUMMP_LINK is a symbolic link to the source code tar file
#     it will contain the name of the actual tar file that then must be
#     retrieved.
    if [[ "${ITAPS_GRUMMP_LINK}" != "" ]]
    then
       wget ${ITAPS_GRUMMP_PATH}/${ITAPS_GRUMMP_LINK}
       ITAPS_GRUMMP_SOURCE=`cat ${ITAPS_GRUMMP_LINK} | sed -e 's/link //'`
    fi

#     initialize
    timelimit=0
    retval=0

#     get source code and check for successful completion
    echo "Getting GRUMMP from ${ITAPS_GRUMMP_PATH} . . . "

    if [[ "${ITAPS_GRUMMP_SOURCE}" = "" ]]
    then
       if [[ ! -d GRUMMP ]]
       then
          mkdir GRUMMP
       fi
       svn co ${ITAPS_GRUMMP_PATH} ./GRUMMP 1> GRUMMP.checkout 2> GRUMMP.checkout.log
       grummp_file="GRUMMP.checkout"
       sleep 30
       if [[ -z GRUMMP.checkout ]]
       then
          retval=20
       else
          endlog=`tail --lines=1 GRUMMP.checkout | cut -d" " -f3`
          while [[ "$endlog" != "revision" ]]
          do
             sleep 60
             endlog=`tail --lines=1 GRUMMP.checkout | cut -d" " -f3`
             timelimit=`expr $timelimit + 1`
             if [[ "$timelimit" == "2" ]]
             then
                echo "Problem with SVN checkout of GRUMMP source code"
                retval=20
                break
             fi
          done
       fi
    else
       wget ${ITAPS_GRUMMP_PATH}/${ITAPS_GRUMMP_SOURCE} 1> GRUMMP.checkout 2> GRUMMP.checkout.log
       grummp_file="GRUMMP.checkout.log"
       sleep 30
       if [[ -z GRUMMP.checkout.log ]]
       then
          retval=20
       else
          endlog1=`tail --lines=2 GRUMMP.checkout.log | cut -d" " -f6`
          endlog2=`tail --lines=2 GRUMMP.checkout.log | cut -d" " -f7`
          while [[ "$endlog1" != "saved"  &&  "$endlog2" != "saved" ]]
          do
             sleep 60
             endlog1=`tail --lines=2 GRUMMP.checkout.log | cut -d" " -f6`
             endlog2=`tail --lines=2 GRUMMP.checkout.log | cut -d" " -f7`
             timelimit=`expr $timelimit + 1`
             if [[ "$timelimit" == "2" ]]
             then
                echo "Problem checking out GRUMMP source code"
                retval=20
                break
             fi
          done
       fi
    fi

#     remove GRUMMP.checkout and/or GRUMMP.checkout.log if it is an empty file
    find . -name 'GRUMMP.checkout' -size 0 -exec rm -f {} \;
    find . -name 'GRUMMP.checkout.log' -size 0 -exec rm -f {} \;
   
#
#     set error flag for summary file
#     create tar file of source code if downloaded with svn
   if [[ "$retval" = "0" ]]
   then
      if [ "${ITAPS_GRUMMP_SOURCE}" = "" ] 
      then
         tar -cf GRUMMP.tar GRUMMP/
      fi
      filarr[$indg]=""
      if [[ ! -z "$(cat ${grummp_file})" ]]
      then
         numwarn=`grep -i -e "warn" ${grummp_file}|wc -l`
         if [[ "$numwarn" != "0" ]]
         then
            filarr[$indg]="WARN:${grummp_file}"
         fi
      fi
      indg=`expr $indg + 1`
   else
      filarr[$indg]="$grummp_file"
      indg=`expr $indg + 1`
      filarr[$indg]="DNR"
      indg=`expr $indg + 1`
      filarr[$indg]="DNR"
      indg=`expr $indg + 1`
      filarr[$indg]="DNR"
      indg=`expr $indg + 1`
      filarr[$indg]="DNR"
      indg=`expr $indg + 1`
    fi

    return $retval
}


# *****************************************************************************
# Function: get_builddir_GRUMMP
#
# Purpose: Find the GRUMMP directory name into which the source code 
#          was untarred
#
# Programmer: Ellen Hill 
# Date: Mon Nov 2 2009
#
# *****************************************************************************
function get_builddir_GRUMMP
{
    GRUMMP_BUILD_DIR=""
    files=`ls`
    for grummp_dir in ${files} 
    do
       if [[ -d ${grummp_dir} ]]
       then
          case ${grummp_dir} in
             *GR* | gr*)
                 export GRUMMP_BUILD_DIR=`pwd`/${grummp_dir}
                 break
                 ;;
               *)
                 ;;
          esac
       fi
    done
    if [[ -z ${GRUMMP_BUILD_DIR} ]]
    then
       echo "Cannot find GRUMMP source code directory; error return"
       return 55
    else
       echo " GRUMMP build directory: ${GRUMMP_BUILD_DIR}"
       return 0
    fi
}


# *************************************************************************** #
# Function: build_check_GRUMMP
#
# Purpose: Builds GRUMMP implementation
#          Configure and make GRUMMP with outputs going into log and err
#             files, respectively
#          directories containing the iMesh-Defs.h and libiMesh_GRUMMP.a 
#             are found and LD_LIBRARY_PATH updated to include them
#          run internal tests to verify correct build
#
# Programmer: Ellen Hill (modified from build_visit Function 8.17)
# Date: Mon Nov 2 2009
#
#     Mark C. Miller, Mon Mar 14 15:17:09 PDT 2011
#     Added logic to cat config.log to GRUMMP.config.log
# *************************************************************************** #
function build_check_GRUMMP
{
    GRUMMP_DEFS_DIR=""
    GRUMMP_LIB_DIR=""

    echo "Configuring ITAPS_GRUMMP. . ."
    GRUMMP_INSTALL_DIR="$GRUMMP_BUILD_DIR/iMeshDir"
    cd $GRUMMP_BUILD_DIR || echo "Cannot cd to $GRUMMP_BUILD_DIR build dir."
    ./configure --enable-imesh --prefix=$GRUMMP_INSTALL_DIR \
             1> GRUMMP.config.log 2> GRUMMP.config.err
    retval=$?
    cat config.log >> GRUMMP.config.log
  
#     save GRUMMP config error return value & file name (if non-zero length)
#     if GRUMMP config failed, set GRUMMP make to DNR and GRUMMP make-install
#     to N/A (not an option)
    if [[ "$retval" = "0" ]] 
    then
       filarr[$indg]=""
       if [[ ! -z "$(cat GRUMMP.config.err)" ]]
       then
          numwarn=`grep -i -e "warn" GRUMMP.config.err|wc -l`
          if [[ "$numwarn" != "0" ]]
          then
             filarr[$indg]="WARN:GRUMMP.config.err"
          fi
       fi
       indg=`expr $indg + 1`
    else
       echo "GRUMMP configure failed, Giving up"
       filarr[$indg]="GRUMMP.config.err"
       indg=`expr $indg + 1`
       filarr[$indg]="DNR"
       indg=`expr $indg + 1`
       filarr[$indg]="DNR"
       indg=`expr $indg + 1`
       filarr[$indg]="DNR"
       indg=`expr $indg + 1`
       return 30
    fi

    echo "Building GRUMMP. . . (~10 minutes)"
    make 1> GRUMMP.make.log 2> GRUMMP.make.err 
    retval=$?

#     save GRUMMP make error return value & file name (if non-zero length)
#     if GRUMMP make failed, set GRUMMP make-install to DNR
    if [[ "$retval" = "0" ]] 
    then
       filarr[$indg]=""
       if [[ ! -z "$(cat GRUMMP.make.err)" ]]
       then
          numwarn=`grep -i -e "warn" GRUMMP.make.err|wc -l`
          if [[ "$numwarn" != "0" ]]
          then
             filarr[$indg]="WARN:GRUMMP.make.err"
          fi
       fi
       indg=`expr $indg + 1`
    else
       echo "GRUMMP make failed, Giving up"
       filarr[$indg]="GRUMMP.make.err"
       indg=`expr $indg + 1`
       filarr[$indg]="DNR"
       indg=`expr $indg + 1`
       filarr[$indg]="DNR"
       indg=`expr $indg + 1`
       return 35
    fi

#     run make-install
    echo "Installing GRUMMP. . ."
    make install 1> GRUMMP.mkinstall.log 2> GRUMMP.mkinstall.err 
    retval=$?

    if [[ "$retval" = "0" ]] 
    then
       filarr[$indg]=""
       if [[ ! -z "$(cat GRUMMP.mkinstall.err)" ]]
       then
          numwarn=`grep -i -e "warn" GRUMMP.mkinstall.err|wc -l`
          if [[ "$numwarn" != "0" ]]
          then
             filarr[$indg]="WARN:GRUMMP.mkinstall.err"
          fi
       fi
       indg=`expr $indg + 1`
    else
       echo "GRUMMP make install failed, Giving up"
       filarr[$indg]="GRUMMP.mkinstall.err"
       indg=`expr $indg + 1`
       filarr[$indg]="DNR"
       indg=`expr $indg + 1`
       return 40
    fi

    echo "Done building GRUMMP"

#     filter out messages for creating lib files
    if [[ -s "GRUMMP.make.err" ]]
    then
       apply_error_filters "GRUMMP.make.err"
    fi

#     find directory containing iMesh-Defs.inc file
    defs_dirs=`find ${GRUMMP_INSTALL_DIR} -name ${DEFS_FILE}`
    if [[ "${defs_dirs}" == "" ]] 
    then
       echo "Cannot find iMesh-Defs.inc for GRUMMP, error return"
       return 50
    fi
    GRUMMP_DEFS_DIR=`dirname ${defs_dirs}`

#     find directory containing libiMesh_GRUMMP.a file
    libs_dirs=`find ${GRUMMP_INSTALL_DIR} -name libiMesh_GRUMMP.a`
    if [[ ${libs_dirs} == "" ]] 
    then
       echo "Cannot find libiMesh for GRUMMP, error return"
       return 50
    fi
    GRUMMP_LIBDIR=`dirname ${libs_dirs}`

    echo " GRUMMP ${DEFS_FILE} is stored in directory:"
    echo "    ${GRUMMP_DEFS_DIR}"
    export LD_LIBRARY_PATH="$GRUMMP_LIBDIR"

#     run internal tests to determine if build was successful
    make test > grummp_results
    
    passed=0
    failed=0

    while read InputLine
    do
       tst_res=`echo $InputLine | awk '/success$/ {print "success"}'`
       if [[ "$tst_res" == "success" ]]
       then
          passed=`expr $passed + 1`
       else
          failed=`expr $failed + 1`
       fi
    done < grummp_results

    if [[ $failed > 0 ]]
    then
       filarr[$indg]="grummp_results"
       indg=`expr $indg + 1`
    else
       filarr[$indg]=""
       indg=`expr $indg + 1`
    fi

    num_tsts=`expr $passed + $failed`
    if [[ $passed > 0 ]]
    then
       echo " GRUMMP Check Test Results: $passed/$num_tsts PASSED"
       return 0
    fi
    return $failed
}


# *****************************************************************************
#     Section 4 - MOAB Routines
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
# Date: Mon Nov 9 2009
#
# *****************************************************************************
function get_source_MOAB
{
    echo "Getting ${ITAPS_MOAB_SOURCE} from ${ITAPS_MOAB_PATH}. . . "
    wget ${ITAPS_MOAB_PATH}/${ITAPS_MOAB_SOURCE} \
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
    if [[ "$retval" = "0" ]]
    then
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
    else
       filarr[$indm]="MOAB.checkout.log"
       indm=`expr $indm + 1`
       filarr[$indm]="DNR"
       indm=`expr $indm + 1`
       filarr[$indm]="DNR"
       indm=`expr $indm + 1`
       filarr[$indm]="DNR"
       indm=`expr $indm + 1`
       filarr[$indm]="DNR"
       indm=`expr $indm + 1`
    fi
    
    return $retval
}


# *****************************************************************************
# Function: get_builddir_MOAB
#
# Purpose: Find the MOAB directory name into which the source code 
#          was untarred
#
# Programmer: Ellen Hill 
# Date: Mon Nov 9 2009
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
# Date: Mon Nov 9 2009
#
#     Mark C. Miller, Mon Mar 14 15:17:09 PDT 2011
#     Added logic to cat config.log to MOAB.confi.log
# *************************************************************************** #
function build_check_MOAB
{
    MOAB_DEFS_DIR=""

    echo "Configuring ITAPS_MOAB. . ."
    MOAB_INSTALL_DIR="$MOAB_BUILD_DIR/iMeshDir"
    cd $MOAB_BUILD_DIR || echo "Cannot cd to $MOAB_BUILD_DIR build dir."
    ./configure --enable-imesh --with-zlib --with-hdf5=/usr/local/itaps/hdf5-1.8.6-serial \
                --prefix=$MOAB_INSTALL_DIR \
                1> MOAB.config.log 2> MOAB.config.err
    retval=$?
    cat config.log >> MOAB.config.log

#     save MOAB config error return value & file name (if non-zero length)
#     if MOAB config failed, set MOAB make to N/A and MOAB make-install to DNR
    if [[ "$retval" = "0" ]] 
    then
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
    else
       echo "MOAB configure failed, Giving up"
       filarr[$indm]="MOAB.config.err"
       indm=`expr $indm + 1`
       filarr[$indm]="DNR"
       indm=`expr $indm + 1`
       filarr[$indm]="DNR"
       indm=`expr $indm + 1`
       filarr[$indm]="DNR"
       indm=`expr $indm + 1`
       return 30
    fi

    echo "Building MOAB. . . (~10 minutes)"
    make 1> MOAB.make.log 2> MOAB.make.err
    retval=$?

#     save MOAB make error return value & file name (if non-zero length)
    if [[ "$retval" = "0" ]] 
    then
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
    else
       echo "MOAB make failed, Giving up"
       filarr[$indm]="MOAB.make.err"
       indm=`expr $indm + 1`
       filarr[$indm]="DNR"
       indm=`expr $indm + 1`
       filarr[$indm]="DNR"
       indm=`expr $indm + 1`
       return 40
    fi

    echo "Installing MOAB. . . "
    make install 1> MOAB.mkinstall.log 2> MOAB.mkinstall.err
    retval=$?

#     save MOAB make-install error return value & file name (if non-zero length)
    if [[ "$retval" = "0" ]] 
    then
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
    else
       echo "MOAB make-install failed, Giving up"
       filarr[$indm]="MOAB.mkinstall.err"
       indm=`expr $indm + 1`
       filarr[$indm]="DNR"
       indm=`expr $indm + 1`
       return 40
    fi

    echo "Done building MOAB"

#     find directory containing iMesh-Defs.inc file
    defs_dirs=`find ${MOAB_INSTALL_DIR} -name ${DEFS_FILE}`
    if [[ ${defs_dirs} == "" ]] 
    then
       echo "Cannot find ${DEFS_FILE} for MOAB, error return"
       return 50
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
       filarr[$indm]="moab_results"
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
# Programmer: Ellen Hill (modified from build_visit script)
# Date: Wed Dec 2 2009
#
# *****************************************************************************
function get_source_FMDB
{
    echo "Getting ${ITAPS_FMDB_SOURCE} from ${ITAPS_FMDB_PATH}. . . "
    wget ${ITAPS_FMDB_PATH}/${ITAPS_FMDB_SOURCE} \
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
    if [[ "$retval" = "0" ]]
    then
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
    else
       filarr[$indf]="FMDB.checkout.log"
       indf=`expr $indf + 1`
       filarr[$indf]="DNR"
       indf=`expr $indf + 1`
       filarr[$indf]="DNR"
       indf=`expr $indf + 1`
       filarr[$indf]="DNR"
       indf=`expr $indf + 1`
       filarr[$indf]="DNR"
       indf=`expr $indf + 1`
    fi

#     change permissions to make script executable
    chmod +x ${ITAPS_FMDB_SOURCE}

    return $retval
}


# *************************************************************************** #
# Function: build_check_FMDB
#
# Purpose: Builds ITAPS FMDB implementation
#
# Programmer: Ellen Hill (modified from build_visit Function 8.15)
# Date: Mon Nov 9 2009
#
#     Mark C. Miller, Mon Mar 14 15:17:09 PDT 2011
#     Added logic to cat config.log to FMDB.config.log
# *************************************************************************** #
function build_check_FMDB
{

    echo "Building ITAPS_FMDB. . ."

    ./${ITAPS_FMDB_SOURCE} 1> FMDB.config.log 2> FMDB.config.err
    retval=$?

#     save FMDB error return value & file name (if non-zero length)
#     the build script controls config & make for FMDB so all messages 
#     are in the *.config.* files; so the same file is used for config/make
#     and make-install; no option for make-test
    if [[ "$retval" = "0" ]] 
    then
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
    else
       echo "FMDB configure failed, Giving up"
       filarr[$indf]="FMDB.config.err"
       indf=`expr $indf + 1`
       filarr[$indf]="DNR"
       indf=`expr $indf + 1`
       filarr[$indf]="DNR"
       indf=`expr $indf + 1`
       filarr[$indf]="N/A"
       indf=`expr $indf + 1`
       return 40
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

#     find directory containing iMesh-Defs.inc file
    defs_dirs=`find ${FMDB_INSTALL_DIR} -name ${DEFS_FILE}`
    if [[ ${defs_dirs} == "" ]] 
    then
       echo "Cannot find ${DEFS_FILE} for FMDB, error return"
       return 50
    fi
    FMDB_DEFS_DIR=`dirname ${defs_dirs}`

    echo " FMDB ${DEFS_FILE} is stored in directory:"
    echo "    ${FMDB_DEFS_DIR}"

    export LD_LIBRARY_PATH="${FMDB_DEFS_DIR}":"${LD_LIBRARY_PATH}"
    return 0
}


# *****************************************************************************
#     Section 6 - RefImpl Routines
# -----------------------------------------------------------------------------
# *****************************************************************************
# Function: get_source_REFI
#
# Purpose:  download RefImpl source code from distribution URL
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
# Date: Tue Sept 14 2010
#
# *****************************************************************************
function get_source_REFI
{
#     ITAPS_REFI_LINK is a symbolic link to the source code tar file
#     it will contain the name of the actual tar file that then must be
#     retrieved.
    if [[ "${ITAPS_REFI_LINK}" != "" ]]
    then
       wget ${ITAPS_REFI_PATH}/${ITAPS_REFI_LINK}
       ITAPS_REFI_SOURCE=`cat ${ITAPS_REFI_LINK} | sed -e 's/link //'`
    fi

    echo "Getting RefImpl from ${ITAPS_REFI_PATH}. . . "
    if [[ "${ITAPS_REFI_SOURCE}" = "" ]]
    then
       if [[ ! -d RefImpl ]]
       then
          mkdir RefImpl
       fi
       svn co ${ITAPS_REFI_PATH} ./RefImpl 1>REFI.checkout 2> REFI.checkout.log
       refi_file="REFI.checkout"
       sleep 30
       if [[ -z ${refi_file} ]]
       then
          retval=20
       else
          endlog=`tail --lines=1 ${refi_file} | cut -d" " -f3`
          while [[ "$endlog" != "revision" ]]
          do
             sleep 60
             endlog=`tail --lines=1 ${refi_file} | cut -d" " -f3`
             timelimit=`expr $timelimit + 1`
             if [[ "$timelimit" == "2" ]]
             then
                echo "Problem with SVN checkout of RefImpl source code"
                retval=20
                break
             fi
          done
       fi
    else
       wget ${ITAPS_REFI_PATH}/${ITAPS_REFI_SOURCE} 1> REFI.checkout 2> REFI.checkout.log
       refi_file="REFI.checkout.log"
       sleep 30
       if [[ -z ${refi_file} ]]
       then
          retval=20
       else
          endlog1=`tail --lines=2 ${refi_file} | cut -d" " -f6`
          endlog2=`tail --lines=2 ${refi_file} | cut -d" " -f7`
          while [[ "$endlog1" != "saved"  &&  "$endlog2" != "saved" ]]
          do
             sleep 60
             endlog1=`tail --lines=2 ${refi_file} | cut -d" " -f6`
             endlog2=`tail --lines=2 ${refi_file} | cut -d" " -f7`
             timelimit=`expr $timelimit + 1`
             if [[ "$timelimit" == "2" ]]
             then
                echo "Problem checking out RefImpl source code"
                retval=20
                break
             fi
          done
       fi
    fi

#     remove REFI.checkout if it is an empty file
    find . -name 'REFI.checkout' -size 0 -exec rm -f {} \;
    find . -name 'REFI.checkout.log' -size 0 -exec rm -f {} \;

#
#     set error flag for summary file
#     make tar file of soruce code from svn download
    if [[ "$retval" = "0" ]]
    then
       if [[ "${ITAPS_REFI_SOURCE}" = "" ]]
       then
          tar -cf RefImpl.tar RefImpl/
       fi
       filarr[$indr]=""
       if [[ ! -z "$(cat ${refi_file})" ]]
       then
          numwarn=`grep -i -e "warn" ${refi_file}|wc -l`
          if [[ "$numwarn" != "0" ]]
          then
             filarr[$indr]="WARN:${refi_file}"
          fi
       fi
       indr=`expr $indr + 1`
    else
       filarr[$indr]="${refi_file}"
       indr=`expr $indr + 1`
       filarr[$indr]="DNR"
       indr=`expr $indr + 1`
       filarr[$indr]="DNR"
       indr=`expr $indr + 1`
       filarr[$indr]="DNR"
       indr=`expr $indr + 1`
       filarr[$indr]="DNR"
       indr=`expr $indr + 1`
    fi

    return $retval
}


# *****************************************************************************
# Function: get_builddir_REFI
#
# Purpose: Find the REFI directory name into which the source code 
#          was untarred
#
# Programmer: Ellen Hill 
# Date: Tue Sept 14 2010
#
# *****************************************************************************
function get_builddir_REFI
{
    REFI_BUILD_DIR=""
    files=`ls`
    for refi_dir in ${files} 
    do
       if [[ -d ${refi_dir} ]]
       then
          case ${refi_dir} in
             *Ref* | *ref*)
                 export REFI_BUILD_DIR=`pwd`/${refi_dir}
                 break
                 ;;
               *)
                 ;;
          esac
       fi
    done
    if [[ -z $REFI_BUILD_DIR ]]
    then
       echo "Cannot find RefImpl source code directory; error return"
       return 55
    else
       echo " RefImpl build directory: ${REFI_BUILD_DIR}"
       return 0
    fi
}


# *************************************************************************** #
# Function: build_check_REFI
#
# Purpose: Builds ITAPS Ref implementation
#
# Programmer: Ellen Hill 
# Date: Tue Sept 14 2010
#
#     Mark C. Miller, Mon Mar 14 15:17:09 PDT 2011
#     Added logic to cat config.log to REFI.config.log
# *************************************************************************** #
function build_check_REFI
{
    REFI_DEFS_DIR=""

    echo "Configuring RefImpl. . ."
    REFI_INSTALL_DIR=$REFI_BUILD_DIR/iMeshDir
    cd $REFI_BUILD_DIR || echo "Cannot cd to $REFI_BUILD_DIR build dir."
    ./configure --prefix=$REFI_INSTALL_DIR 1> REFI.config.log 2> REFI.config.err
    retval=$?
    cat config.log >> REFI.config.log

#     save REFI config error return value & file name (if non-zero length)
#     if REFI config failed, set REFI make to N/A and REFI make-install to N/A
    if [[ "$retval" = "0" ]] 
    then
       filarr[$indr]=""
       if [[ ! -z "$(cat REFI.config.err)" ]]
       then
          numwarn=`grep -i -e "warn" REFI.config.err|wc -l`
          if [[ "$numwarn" != "0" ]]
          then
             filarr[$indr]="WARN:REFI.config.err"
          fi
       fi
       indr=`expr $indr + 1`
    else
       echo "RefImpl configure failed, Giving up"
       filarr[$indr]="REFI.config.err"
       indr=`expr $indr + 1`
       filarr[$indr]="DNR"
       indr=`expr $indr + 1`
       filarr[$indr]="DNR"
       indr=`expr $indr + 1`
       filarr[$indr]="N/A"
       indr=`expr $indr + 1`
       return 30
    fi

    echo "Building RefImpl. . . "
    make 1> REFI.make.log 2> REFI.make.err
    retval=$?

#     save REFI make error return value & file name (if non-zero length)
#     if REFI make failed, set REFI make-install to N/A
    if [[ "$retval" = "0" ]] 
    then
       filarr[$indr]=""
       if [[ ! -z "$(cat REFI.make.err)" ]]
       then
          numwarn=`grep -i -e "warn" REFI.make.err|wc -l`
          if [[ "$numwarn" != "0" ]]
          then
             filarr[$indr]="WARN:REFI.make.err"
          fi
       fi
       indr=`expr $indr + 1`
    else
       echo "RefImpl make failed, Giving up"
       filarr[$indr]="REFI.make.err"
       indr=`expr $indr + 1`
       filarr[$indr]="DNR"
       indr=`expr $indr + 1`
       filarr[$indr]="N/A"
       indr=`expr $indr + 1`
       return 35
    fi

#     filter out messages for creating lib files
    if [[ -s "REFI.make.err" ]]
    then
       apply_error_filters "REFI.make.err"
    fi

    echo "Installing RefImpl. . . "
    make install 1> REFI.mkinstall.log 2> REFI.mkinstall.err
    retval=$?

#     save REFI make-install error return value & file name (if non-zero length)
    if [[ "$retval" = "0" ]] 
    then
       filarr[$indr]=""
       if [[ ! -z "$(cat REFI.mkinstall.err)" ]]
       then
          numwarn=`grep -i -e "warn" REFI.mkinstall.err|wc -l`
          if [[ "$numwarn" != "0" ]]
          then
             filarr[$indr]="WARN:REFI.mkinstall.err"
          fi
       fi
       indr=`expr $indr + 1`
    else
       echo "RefImpl make failed, Giving up"
       filarr[$indr]="REFI.mkinstall.err"
       indr=`expr $indr + 1`
       filarr[$indr]="N/A"
       indr=`expr $indr + 1`
       return 40
    fi

    echo "Done building RefImpl"
#
#     set make-check option
    filarr[$indr]="N/A"
    indr=`expr $indr + 1`

#     find directory containing iMesh-Defs.inc file
    defs_dirs=`find ${REFI_INSTALL_DIR} -name ${DEFS_FILE}`
    if [[ "${defs_dirs}" == "" ]] 
    then
       echo "Cannot find iMesh-Defs.inc for RefImpl, error return"
       return 50
    fi
    REFI_DEFS_DIR=`dirname ${defs_dirs}`

#     find directory containing libiMesh_RefImpl.a file
    libs_dirs=`find ${REFI_INSTALL_DIR} -name libiMesh_RefImpl.a`
    if [[ ${libs_dirs} == "" ]] 
    then
       echo "Cannot find libiMesh for RefImpl, error return"
       return 50
    fi
    REFI_LIBDIR=`dirname ${libs_dirs}`

    echo " RefImpl ${DEFS_FILE} is stored in directory:"
    echo "    ${REFI_DEFS_DIR}"
    export LD_LIBRARY_PATH="$REFI_LIBDIR":"$LD_LIBRARY_PATH"
  
    return 0
}


# *****************************************************************************
#     Section 8 - UnitTest Routines
# -----------------------------------------------------------------------------
# *****************************************************************************
# Function: get_source_UnitTest
#
# Purpose:  get iMesh unitTest source code
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
# Date: Tue Nov 3 2009
#
# *****************************************************************************
function get_source_UnitTest
{
    echo "Getting iMesh_unitTest sources from ${ITAPS_IMESH_PATH}. . . "
    svn co ${ITAPS_IMESH_PATH} iMesh_unitTest_dir \
         1> IMESH.checkout 2> IMESH.checkout.err

    retval=$?
    if [[ -e IMESH.checkout.err && -z "$(cat IMESH.checkout.err)" ]]
    then
        rm -f IMESH.checkout.err
        retval=0
    fi

#   remove IMESH.checkout if it is an empty file
    if [[ -z "$(cat IMESH.checkout)" ]]
    then
        rm -f IMESH.checkout
    fi

#
#     make tar files of iMesh source code 
    tar -cf iMesh_unitTest.tar iMesh_unitTest_dir/

    return $retval
}


# *****************************************************************************
# Function: get_builddir_UnitTest
#
# Purpose: Find the IMESH directory name into which the source code was untarred
#
# Programmer: Ellen Hill 
# Date: Mon Nov 2 2009
#
# *****************************************************************************
function get_builddir_UnitTest
{
    export IMESH_BUILD_DIR="$(pwd)/iMesh_unitTest_dir"
}


# *************************************************************************** #
# Function: build_test_UnitTest
#
# Purpose: Builds and runs iMesh unitTests
#          creates required directory conf, for configure related files, 
#             if it does not already exist
#
# Programmer: Ellen Hill
# Date: Tue Nov 3 2009
#
#   Mark C. Miller, Wed Jan 26 10:53:45 PST 2011
#   Add logic to re-attempt the make but with specification check disabled.
#
#   Mark C. Miller, Wed Jan 26 10:55:58 PST 2011
#   Doh! Forgot to append make output to log files in above change.
#
#   Mark C. Miller, Wed Mar  9 12:35:57 PST 2011
#   Grep for 'speckcheck' make target in make output.
# *************************************************************************** #
function build_test_UnitTest
{
    cd $IMESH_BUILD_DIR || echo "Cannot cd to $IMESH_BUILD_DIR build dir."

#      if a conf subdirectory does not exist; create one and move the config
#      related files into it.  Configure is expecting to find its files in the
#      conf sub-directory.
    if [[ ! -d conf ]]
    then
       mkdir conf
       mv config.guess config.sub install-sh conf/
    fi

    #
    # save input arguments
    #
    IMPL_NAME=$1
    IMPL_BUILD_DIR=$2
    IMPL_MESH_FILE=$3

    #
    # if build directory is NULL, return error
    #
    if [[ "${IMPL_BUILD_DIR}" = "" ]]
    then
       return 55
    fi

    #
    # inititalize return value
    #
    retval=0

    #
    # inititalize pass/fail values
    #
    config_err=999
    make_err=999
    nock_err=999
    svl_err=999
    nosvl_err=999
    valg_err=999

    #
    # remove any existing files
    #
    if [[ -e Makefile ]]
    then
       make distclean
    fi

    echo ""
    echo "Configuring UnitTest tests for ${IMPL_NAME} . . ."
    ./configure IMESH_DEFS=$IMPL_BUILD_DIR/${DEFS_FILE}  \
            1> imesh_${IMPL_NAME}.config.log 2> imesh_${IMPL_NAME}.config.err
    if [[ $? != 0 ]] ; then
       echo "iMesh configure failed for ${IMPL_NAME}, Giving up"
       config_err=1
       return 30
    else
       config_err=0
    fi

    echo "Building UnitTest tests for ${IMPL_NAME} . . ."
    make  1> imesh_${IMPL_NAME}.make.log 2> imesh_${IMPL_NAME}.make.err
    if [[ $? != 0 ]] ; then
       make_err=1
       if [[ -n "$(grep speccheck imesh_${IMPL_NAME}.make.err | grep 'Error 4')" ]] ; then
          echo "Spec-check failed for ${IMPL_NAME}, trying with spec-check disabled . . ."
          make  ITAPS_NOCKSPEC=1 \
                1>> imesh_${IMPL_NAME}.make.log 2>> imesh_${IMPL_NAME}.make.err
          if [[ $? != 0 ]] ; then
             echo "iMesh_unitTest make failed for ${IMPL_NAME}, even with spec-check disabled, Giving up"
             nock_err=1
             return 35
          else
             nock_err=0
          fi
       else
          echo "iMesh_unitTest make failed for ${IMPL_NAME}, Giving up"
          return 35
       fi
    else
       make_err=0
    fi

##     get input file
##     if not available, copy from one level up
    if [[ ! -r ${IMPL_MESH_FILE} ]]
    then
       cp ../${IMPL_MESH_FILE}* .
    fi

##     run unit tests and send results to output file
    for impl_opt in ${UNIT_IMPL_OPTS}
    do
        case $impl_opt in
           save*) 
                  echo "Running unitTests for" ${IMPL_NAME}
                  ./iMesh_unitTest ${IMPL_MESH_FILE} \
                     1> imesh_${IMPL_NAME}.out 2> imesh_${IMPL_NAME}.msg
                  retval=$?
                  if [[ $retval != 0 ]]
                  then
                     svl_err=1
                  else
                     svl_err=0
                  fi
                  ;;
         --no-s*) 
                  echo "Running unitTests with --no-save-load for" ${IMPL_NAME}
                  ./iMesh_unitTest --no-save-load ${IMPL_MESH_FILE} \
                      1> imesh_${IMPL_NAME}.no-save-load.out \
                      2> imesh_${IMPL_NAME}.no-save-load.msg
                  retval=$?
                  if [[ $retval != 0 ]]
                  then
                     nosvl_err=1
                  else
                     nosvl_err=0
                  fi
                  ;;
         valgri*) 
                  echo "Running unitTests with valgrind for" ${IMPL_NAME}
                  valgrind --leak-check=full --show-reachable=yes \
                     ./iMesh_unitTest ${IMPL_MESH_FILE} \
                        1> imesh_${IMPL_NAME}.valg.out \
                        2> imesh_${IMPL_NAME}.valg.msg
                  retval=$?
                  if [[ $retval != 0 ]]
                  then
                     valg_err=1
                  else
                     valg_err=0
                  fi
                  ;;
        esac
    done

    echo "Done running UnitTest tests for ${IMPL_NAME}"
    echo ""

    return $retval
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

    echo "Configuring Mesquite with $IMPL_NAME . . ."
    cd $MESQ_BUILD_DIR || echo "Cannot cd to $MESQ_BUILD_DIR build dir."
    ./configure --with-imesh=$IMPL_DIR/$DEFS_FILE 1> MESQ_$IMPL_NAME.config.log \
                                         2> MESQ_$IMPL_NAME.config.err
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

#     remove erroneous error messages
    apply_error_filters MESQ_$IMPL_NAME.make.err

#     run 'make check'
    make check > MESQ_$IMPL_NAME.results
    
#
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
    done < MESQ_$IMPL_NAME.results

    apply_error_filters MESQ_$IMPL_NAME.results

    num_tsts=`expr $passed + $failed`
#
#     set check error flag
    if [[ $failed > 0 ]]
    then
       check_err=1
    else
       check_err=0
    fi

    if [[ $passed > 0 ]]
    then
       echo " Mesquite/$impl_name Check Test Results: $passed/$num_tsts PASSED"
       return 0
    fi

    return $failed
}


# *****************************************************************************
#     Section 9 - Swapping Routines
# -----------------------------------------------------------------------------
# *****************************************************************************
# Function: get_source_SWAP
#
# Purpose:  get swapping source code
#
#           The checkout file has the status of the file checkout.  When
#           complete the last line contains the word 'revision'.  Completion of
#           the file checkout is determined when the word 'revision' is found.
#
#           NOTE:  If the file cannot be download within 2 minutes an error
#                  message is written and immediate return from this routine
#
# Programmer: Ellen Hill
# Date: Fri Mar 4 2011
#
# *****************************************************************************
function get_source_SWAP
{
    retval=0

    echo "Getting Swapping sources from ${ITAPS_SWAP_PATH}. . . "
    svn co ${ITAPS_SWAP_PATH} SwapDir 1> SWAP.checkout 2> SWAP.checkout.err

    retval=$?

    sleep 30
    if [[ -z SWAP.checkout ]]
    then
       retval=20
    else
       endlog=`tail --lines=1 SWAP.checkout | cut -d" " -f3`
       while [[ "$endlog" != "revision" ]]
       do
          sleep 60
          endlog=`tail --lines=1 SWAP.checkout | cut -d" " -f3`
          timelimit=`expr $timelimit + 1`
          if [[ "$timelimit" == "2" ]]
          then
             echo "Problem with SVN checkout of Swapping source code"
             retval=20
             break
          fi
       done
    fi

#   remove SWAP.checkout if it is an empty file
    if [[ -z "$(cat SWAP.checkout)" ]]
    then
        rm -f SWAP.checkout
    fi

#
#      create tar file of Swap source code
    tar -cf Swap.tar SwapDir/

    return $retval
}


# *****************************************************************************
# Function: get_builddir_SWAP
#
# Purpose: Find the directory name into which the source code was untarred
#
# Programmer: Ellen Hill 
# Date: Fri Mar 4 2011
#
# *****************************************************************************
function get_builddir_SWAP
{
    export SWAP_BUILD_DIR="$(pwd)/SwapDir"
}


# *************************************************************************** #
# Function: build_check_SWAP
#
# Purpose: Builds Swapping with each implementation
#          Configure and make SWAP with outputs going into log and err
#             files, respectively
#          Run 'make test' to verify successful build
#
# Programmer: Ellen Hill
# Date: Fri Mar 4 2011
#
# *************************************************************************** #
function build_check_SWAP
{
#
#     get arguments
    IMPL_NAME=$1
    IMPL_DIR=$2
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

    echo "Configuring Swapping with $IMPL_NAME . . ."
    cd $SWAP_BUILD_DIR || echo "Cannot cd to $SWAP_BUILD_DIR build dir."
    if [[ "${IMPL_NAME}" = "MOAB" ]]
    then
       ./configure IMESH_DEFS=$IMPL_DIR/${DEFS_FILE} --enable-tets-from-faces \
           1> SWAP_$IMPL_NAME.config.log  2> SWAP_$IMPL_NAME.config.err
    else
       ./configure IMESH_DEFS=$IMPL_DIR/${DEFS_FILE}  \
           1> SWAP_$IMPL_NAME.config.log  2> SWAP_$IMPL_NAME.config.err
    fi

    if [[ $? != 0 ]] 
    then
       echo "Swapping configure failed with $IMPL_DIR, Giving up"
       config_err=1
       return 30
    else
       config_err=0
    fi

    echo "Building Swapping with $IMPL_NAME. . . "
    make 1> SWAP_$IMPL_NAME.make.log 2> SWAP_$IMPL_NAME.make.err 
    if [[ $? != 0 ]] 
    then
       echo "Swapping make failed with $IMPL_NAME, Giving up"
       make_err=1
       return 35
    else
       make_err=0
    fi

    export LD_LIBRARY_PATH="$SWAP_BUILD_DIR":"$LD_LIBRARY_PATH"
    echo "Done building Swapping with $IMPL_NAME"

#     remove erroneous error messages
    apply_error_filters SWAP_$IMPL_NAME.make.err

#     run 'make test'
    make test > SWAP_$IMPL_NAME.results
    
#
#     Next to last line in results file has average elapsed time
#     if that value is 0 there is a failure; otherwise success
    while read InputLine
    do
       case $InputLine in
          Ave.*) timval=`echo $InputLine | cut -d" " -f5`
                 if [[ "$timval" = "0" ]]
                 then
                    check_err=1
                 else
                    check_err=0
                 fi
                 break
                 ;;
              *)
                 ;;
       esac
    done < SWAP_$IMPL_NAME.results

    return 0
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
   EXEC_SUMM="UnitTest_exec.`date '+%Y%m%d'`.html"

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
   printf "<caption>ITAPS ${ITAPS_BRANCH} Test Results Summary</caption>\n" >> $EXEC_SUMM
   printf "\n" >> $EXEC_SUMM

#
#      set pass/fail array
   for ((kk=0; kk<${max_ind}; kk++ ))
   do
      case ${filarr[$kk]} in
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
                *) pfarr[$kk]="FAIL"
                   ;;
      esac
   done

#
#     print table column headings and test results
   printf "<tr class=\"toprow\">\n" >> $EXEC_SUMM
   printf "   <th>test</th>\n" >> $EXEC_SUMM
   printf "   <th>GRUMMP</th>\n" >> $EXEC_SUMM
   printf "   <th>MOAB</th>\n" >> $EXEC_SUMM
   printf "   <th>FMDB</th>\n" >> $EXEC_SUMM
   printf "   <th>RefImpl</th>\n" >> $EXEC_SUMM
   printf "</tr>\n" >> $EXEC_SUMM
   printf "\n" >> $EXEC_SUMM

   indx=0
   for (( indx=0; indx<$num_opts; indx++ ))
   do
      indx1=`expr $indx + $num_opts`
      indx2=`expr $indx1 + $num_opts`
      indx3=`expr $indx2 + $num_opts`

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
         5) optnam="configure"         #start of iMesh UnitTest
            ;;
         6) optnam="make-spec-ck"
            ;;
         7) optnam="make-no-spec-ck"
            ;;
         8) optnam="run-save-load"
            ;;
         9) optnam="run-no-save-load"
            ;;
        10) optnam="run-valgrind"
            ;;
        11) optnam="configure"         #start of Mesquite Tests
            ;;
        12) optnam="make"
            ;;
        13) optnam="run"
            ;;
        14) optnam="configure"         #start of Swapping Tests
            ;;
        15) optnam="make"
            ;;
        16) optnam="run"
            ;;
      esac
#
#     print header for each section of tests
      if [[ $indx -eq 0 ]]
      then
         echo "<th colspan=\"5\">- - - - - Implementation Tests - - - - -</th>" >> $EXEC_SUMM
      elif [[ $indx -eq 5 ]]
      then
         echo "<th colspan=\"5\">- - - - - iMesh UnitTest Tests - - - - -</th>" >> $EXEC_SUMM
      elif [[ $indx -eq 11 ]]
      then
         echo "<th colspan=\"5\">- - - - - Mesquite Tests - - - - -</th>" >> $EXEC_SUMM
      elif [[ $indx -eq 14 ]]
      then
         echo "<th colspan=\"5\">- - - - - Swapping Tests - - - - -</th>" >> $EXEC_SUMM
      fi

      printf "\n" >> $EXEC_SUMM
      printf "<tr>\n" >> $EXEC_SUMM
      printf "   <td class=\"opts\">%s</td>\n" ${optnam} >> $EXEC_SUMM
#
#     print clickable links for those tests that failed

      if [[ "${pfarr[$indx]}" = "PASS"  || "${pfarr[$indx]}" = "N/A" || "${pfarr[$indx]}" = "DNR" ]]
      then
         printf "   <td class=\"dats\">%s</td>\n" ${pfarr[$indx]} >> $EXEC_SUMM
      else
         filnam="<a href=https://itaps.mcs.anl.gov/${save_dir_href}/${filarr[$indx]}>${pfarr[$indx]}</a>"
         printf "   <td class=\"dats\">%s %s</td>\n" ${filnam} >> $EXEC_SUMM
      fi

      if [[ "${pfarr[$indx1]}" = "PASS"  || "${pfarr[$indx1]}" = "N/A" || "${pfarr[$indx1]}" = "DNR" ]]
      then
         printf "   <td class=\"dats\">%s</td>\n" ${pfarr[$indx1]} >> $EXEC_SUMM
      else
         filnam="<a href=https://itaps.mcs.anl.gov/${save_dir_href}/${filarr[$indx1]}>${pfarr[$indx1]}</a>"
         printf "   <td class=\"dats\">%s %s</td>\n" ${filnam} >> $EXEC_SUMM
      fi

      if [[ "${pfarr[$indx2]}" = "PASS"  || "${pfarr[$indx2]}" = "N/A" || "${pfarr[$indx2]}" = "DNR" ]]
      then
         printf "   <td class=\"dats\">%s</td>\n" ${pfarr[$indx2]} >> $EXEC_SUMM
      else
         filnam="<a href=https://itaps.mcs.anl.gov/${save_dir_href}/${filarr[$indx2]}>${pfarr[$indx2]}</a>"
         printf "   <td class=\"dats\">%s %s</td>\n" ${filnam} >> $EXEC_SUMM
      fi

      if [[ "${pfarr[$indx3]}" = "PASS"  || "${pfarr[$indx3]}" = "N/A" || "${pfarr[$indx3]}" = "DNR" ]]
      then
         printf "   <td class=\"dats\">%s</td>\n" ${pfarr[$indx3]} >> $EXEC_SUMM
      else
         filnam="<a href=https://itaps.mcs.anl.gov/${save_dir_href}/${filarr[$indx3]}>${pfarr[$indx3]}</a>"
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
   echo "Creating unitTest summary file"
   BUILD_DIRS="${GRUMMP_BUILD_DIR} ${MOAB_BUILD_DIR} ${FMDB_BUILD_DIR} ${REFI_BUILD_DIR}"

   tmpfile="tmp.file"
   SUMFILE="UnitTest_summary.`date '+%Y%m%d'`.html"
   
   CurDir=`pwd`

   CLOGCNT=0
   CERRCNT=0
   DWRNCNT=0
   DERRCNT=0

   printf "<html>\n" > $SUMFILE
   printf "<head></head>\n" >> $SUMFILE
   printf "<PRE>\n" >> $SUMFILE
   printf "<body>\n" >> $SUMFILE
   printf "ITAPS ${ITAPS_BRANCH} Error Summary for `date`<br>\n" >> $SUMFILE
   printf "%s<br>" "-----------------------------------------------" >> $SUMFILE
   printf "Operating system: `uname -a`<br>\n" >> $SUMFILE
   printf "Execution Directory:      %s<br>\n" $CurDir >> $SUMFILE
   printf "Results Directory:        %s<br>\n" $save_dir >> $SUMFILE
   printf "%s<br>\n" "-----------------------------------------------" >> $SUMFILE
   printf "iMesh Implementation(s) Compile Summary:<br>\n" >> $SUMFILE
   printf "                           Status: Warnings: Errors<br>\n" >> $SUMFILE
#
#    get configure and build related error messages for each implementation
   for idir in ${BUILD_DIRS} 
   do
      impl_name=`basename ${idir}`
      printf "\n"  >> $SUMFILE
      printf "Building Implementation: %s\n" $impl_name >> $SUMFILE

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

         printf "   %-29s" ${fnam} >> $SUMFILE
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

   printf "<br>\n" >> $SUMFILE
   printf "%2d test with errors<br>" $CERRCNT  >> $SUMFILE
   printf "%2d test succeeded<br>" $CLOGCNT >> $SUMFILE
   printf "%2d test total<br>" $CTOTCNT >> $SUMFILE
   printf "<br>" >> $SUMFILE

   printf "%s<br>" "-----------------------------------------------" >> $SUMFILE
#
#    iMesh_unitTest Compile Summary
   printf "iMesh_unitTest Compile Summary:<br>\n" >> $SUMFILE
   printf "                                    Status: Warnings: Errors<br>\n" >> $SUMFILE

   cd ${IMESH_BUILD_DIR}
   errfils=`ls *.err`
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
      if [ -s "${IMESH_BUILD_DIR}/${errf}" ] 
      then
         errinfo="<a href=https://itaps.mcs.anl.gov/${save_dir_href}/${errf}>${fnam}</a>"
         CERRCNT=`expr $CERRCNT + 1`
         FailedCount=`expr $FailedCount + 1`
         DWRNNUM=0
         DERRNUM=0
         DERRNM1=0
         DWRNNUM=`grep -i -e " warning: " ${IMESH_BUILD_DIR}/${errf}|wc -l`
         DERRNUM=`grep -i -e " error: " ${IMESH_BUILD_DIR}/${errf}|wc -l`
         DERRNM1=`grep -i -e " error " ${IMESH_BUILD_DIR}/${errf}|wc -l`
         DWRNCNT=`expr $DWRNCNT + $DWRNNUM`
         DERRCNT=`expr $DERRCNT + $DERRNUM + $DERRNM1`
      else
         CLOGCNT=`expr $CLOGCNT + 1`
         SuccessCount=`expr $SuccessCount + 1`
         filinfo="<a href=https://itaps.mcs.anl.gov/${save_dir_href}/${fnam}.log>${fnam}</a>"
      fi

      printf "   %-32s" ${fnam} >> $SUMFILE
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
#     iMesh_unitTest Execution Summary
   printf "%s<br>" "-----------------------------------------------" >> $SUMFILE
   printf "iMesh_unitTest (save-load) Execution Summary: <br>" >> $SUMFILE

   for impl in ${IMPL_NAMES}
   do
      printf "<br>\n" >> $SUMFILE
      final_found=0
      if [[ -r ${IMESH_BUILD_DIR}/imesh_${impl}.out ]]
      then
         errf="<a href=https://itaps.mcs.anl.gov/${save_dir_href}/imesh_${impl}.out>imesh_${impl}.out</a>"
         echo "  **$impl Unit Test" >> $SUMFILE
         echo "     " >> $SUMFILE

         tail -8 ${IMESH_BUILD_DIR}/imesh_${impl}.out > ${tmpfile}
         final_found=`grep FINAL ${tmpfile} | wc -l`
         if [[ "$final_found" = "0" ]]
         then 
            echo "     FINAL RESULTS info missing for ${impl}" >> $SUMFILE
            echo "     See: imesh_${impl}.out or imesh_${impl}.msg" >> $SUMFILE
         fi

         if [[ -s ${tmpfile} ]]
         then
            echo "     For detailed results see: $errf" >> $SUMFILE
            echo "     " >> $SUMFILE
            echo "     Last 8 lines of file imesh_${impl}.out" >> $SUMFILE
            while read InputLine
            do
              echo "        ${InputLine}" >> ${SUMFILE}
            done < ${tmpfile}
         fi
      else
         echo "  **$impl Unit Test:" >> $SUMFILE
         echo "     Not run - Check error files" >> $SUMFILE
      fi
      rm -rf ${tmpfile}
   done
#
#     iMesh_unitTest with --no-save-load Execution Summary
   printf "%s<br>" "-----------------------------------------------" >> $SUMFILE
   printf "iMesh_unitTest with --no-save-load Execution Summary: <br>" >> $SUMFILE

   for impl in ${IMPL_NAMES}
   do
      printf "<br>\n" >> $SUMFILE
      if [[ -r ${IMESH_BUILD_DIR}/imesh_${impl}.no-save-load.out ]]
      then
         errf="<a href=https://itaps.mcs.anl.gov/${save_dir_href}/imesh_${impl}.no-save-load.out>imesh_${impl}.no-save-load.out</a>"
         echo "  **$impl Unit Test with --no-save-load" >> $SUMFILE
         echo "     " >> $SUMFILE

         tail -12 ${IMESH_BUILD_DIR}/imesh_${impl}.no-save-load.out > ${tmpfile}
         if [[ -s ${tmpfile} ]]
         then
            echo "     For detailed results see: $errf" >> $SUMFILE
            echo "     Last 12 lines of file imesh_${impl}.no-save-load.out" >> $SUMFILE
            while read InputLine
            do
               echo "        ${InputLine}" >> ${SUMFILE}
            done < ${tmpfile}
         fi
      else
         echo "  **$impl Unit Test with --no-save-load:" >> $SUMFILE
         echo "     Not run - Check error files" >> $SUMFILE
      fi
      rm -rf ${tmpfile}
   done
#
#     iMesh_unitTest with Valgrind Execution Summary
   printf "%s<br>" "-----------------------------------------------" >> $SUMFILE
   printf "iMesh_unitTest with Valgrind Execution Summary: <br>" >> $SUMFILE

   for impl in ${IMPL_NAMES}
   do
      printf "<br>\n" >> $SUMFILE
      if [[ -r ${IMESH_BUILD_DIR}/imesh_${impl}.valg.msg ]]
      then
         errf="<a href=https://itaps.mcs.anl.gov/${save_dir_href}/imesh_${impl}.valg.msg>imesh_${impl}.valg.msg</a>"
         echo "  **$impl Unit Test with Valgrind" >> $SUMFILE
         echo "     " >> $SUMFILE
         
         errline=`grep ERROR ${IMESH_BUILD_DIR}/imesh_${impl}.valg.msg`
         echo "     ${errline}" >> $SUMFILE

         tail -12 ${IMESH_BUILD_DIR}/imesh_${impl}.valg.msg > ${tmpfile}
         if [[ -s ${tmpfile} ]]
         then
            echo "     For detailed results see: $errf" >> $SUMFILE
            echo "     " >> $SUMFILE
            echo "     Last 12 lines of file imesh_${impl}.valg.msg" >> $SUMFILE
            while read InputLine
            do
               echo "        ${InputLine}" >> ${SUMFILE}
            done < ${tmpfile}
         fi
      else
         echo "  **$impl Unit Test with Valgrind:" >> $SUMFILE
         echo "     Not run - Check error files" >> $SUMFILE
      fi
      rm -rf ${tmpfile}
   done
   printf "%s<br>" "-----------------------------------------------" >> $SUMFILE
#
####     Mesquite Results
   printf "Mesquite Compile Summary:<br>\n" >> $SUMFILE
   printf "                           Status: Warnings: Errors<br>\n" >> $SUMFILE

   cd ${MESQ_BUILD_DIR}
   errfils=`ls *.err`
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

      printf "   %-29s" ${fnam} >> $SUMFILE
      if [[ "$FailedCount" != "0" && "$DERRCNT" = "0" ]] ; then
         printf "Warning:    %4d   :   %4d" $DWRNCNT $DERRCNT >> $SUMFILE
         printf "   ${errinfo}<br>" >> $SUMFILE
      elif [[ "$FailedCount" != "0" && "$DERRCNT" != "0" ]] ; then
         printf " Failed:    %4d   :   %4d" $DWRNCNT $DERRCNT >> $SUMFILE
         echo "   ${errinfo}" >> $SUMFILE
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

   printf "%s<br>" "-----------------------------------------------" >> $SUMFILE
   printf "Mesquite Execution Summary: <br>" >> $SUMFILE

   for impl in ${IMPL_NAMES}
   do
      if [ -r ${MESQ_BUILD_DIR}/MESQ_${impl}.results ]
      then
         errval=`grep FAIL ${MESQ_BUILD_DIR}/MESQ_${impl}.results|wc -l`
         printf "<br>\n" >> $SUMFILE
         errf="<a href=https://itaps.mcs.anl.gov/${save_dir_href}/MESQ_${impl}.results>MESQ_${impl}.results</a>"
         echo "  **$impl Test" >> $SUMFILE
         echo "     Number of failed tests: $errval" >> $SUMFILE
         echo "     For detailed results see: $errf" >> $SUMFILE
         echo "     " >> $SUMFILE
      else
         echo "     Error running Mesquite with ${impl}; check error files" >> $SUMFILE
      fi
   done
      
   printf "%s<br>\n" "-----------------------------------------------" >> $SUMFILE

#
####     Swapping Results
   printf "Swapping Compile Summary:<br>\n" >> $SUMFILE
   printf "                           Status: Warnings: Errors<br>\n" >> $SUMFILE

   cd ${SWAP_BUILD_DIR}
   errfils=`ls *.err`
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
      if [ -s "${SWAP_BUILD_DIR}/${errf}" ] 
      then
         errinfo="<a href=https://itaps.mcs.anl.gov/${save_dir_href}/${errf}>${fnam}</a>"
         CERRCNT=`expr $CERRCNT + 1`
         FailedCount=`expr $FailedCount + 1`
         DWRNNUM=0
         DERRNUM=0
         DERRNM1=0
         DWRNNUM=`grep -i -e " warning: " ${SWAP_BUILD_DIR}/${errf}|wc -l`
         DERRNUM=`grep -i -e " error: " ${SWAP_BUILD_DIR}/${errf}|wc -l`
         DERRNM1=`grep -i -e " error " ${SWAP_BUILD_DIR}/${errf}|wc -l`
         DWRNCNT=`expr $DWRNCNT + $DWRNNUM`
         DERRCNT=`expr $DERRCNT + $DERRNUM + $DERRNM1`
      else
         CLOGCNT=`expr $CLOGCNT + 1`
         SuccessCount=`expr $SuccessCount + 1`
         filinfo="<a href=https://itaps.mcs.anl.gov/${save_dir_href}/${fnam}.log>${fnam}</a>"
      fi

      printf "   %-29s" ${fnam} >> $SUMFILE
      if [[ "$FailedCount" != "0" && "$DERRCNT" = "0" ]] ; then
         printf "Warning:    %4d   :   %4d" $DWRNCNT $DERRCNT >> $SUMFILE
         printf "   ${errinfo}<br>" >> $SUMFILE
      elif [[ "$FailedCount" != "0" && "$DERRCNT" != "0" ]] ; then
         printf " Failed:    %4d   :   %4d" $DWRNCNT $DERRCNT >> $SUMFILE
         echo "   ${errinfo}" >> $SUMFILE
      elif [ "$SuccessCount" != "0" ] ; then
         printf " Passed ${filinfo}<br>" >> $SUMFILE
      else
         printf "N/A<br>" >> $SUMFILE
      fi
   done   #  end of errfils loop

   printf "<br>" >> $SUMFILE
   printf "%2d test with errors<br>" $CERRCNT  >> $SUMFILE
   printf "%2d test succeeded<br>" $CLOGCNT >> $SUMFILE
   printf "%2d test total<br>" $CTOTCNT >> $SUMFILE
   printf "<br>" >> $SUMFILE

   printf "%s<br>" "-----------------------------------------------" >> $SUMFILE
   printf "Swapping Execution Summary: <br>" >> $SUMFILE

   for impl in ${IMPL_NAMES}
   do
      if [ -r ${SWAP_BUILD_DIR}/SWAP_${impl}.results ]
      then
         errval=`grep nan ${SWAP_BUILD_DIR}/SWAP_${impl}.results|wc -l`
         printf "<br>\n" >> $SUMFILE
         errf="<a href=https://itaps.mcs.anl.gov/${save_dir_href}/SWAP_${impl}.results>SWAP_${impl}.results</a>"
         echo "  **$impl Test" >> $SUMFILE
         echo "     Number of failed tests: $errval" >> $SUMFILE
         echo "     For detailed results see: $errf" >> $SUMFILE
         echo "     " >> $SUMFILE
      else
         echo "     Error running Swapping with ${impl}; check error files" >> $SUMFILE
      fi
   done
      
   printf "%s<br>\n" "-----------------------------------------------" >> $SUMFILE
   printf "</body>\n" >> $SUMFILE
   printf "</PRE>\n" >> $SUMFILE
   printf "</html>\n" >> $SUMFILE

   chmod 664 $SUMFILE
   chgrp itaps $SUMFILE
   echo "Done creating unitTest summary file"
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
      INDX_FILE="INDEX.html"
   else
      INDX_FILE="INDEX_RC.html"
   fi

   printf "<html>\n" > $INDX_FILE
   printf "<head></head>\n" >> $INDX_FILE
   printf "<PRE>\n" >> $INDX_FILE
   printf "<body>\n" >> $INDX_FILE
   printf "  Index of Test Results Files<br>" >> $INDX_FILE
   printf " `date`<br>\n" >> $INDX_FILE
   printf "%s<br>" "-----------------------------------------------" >> $INDX_FILE

   if [ "${ITAPS_BRANCH}" = "trunk" ]
   then
      tst_dirs=`ls -d -r ${TEST_RESULTS_DIR}/results*`
   else
      tst_dirs=`ls -d -r ${TEST_RESULTS_DIR}/results_rc*`
   fi
   for tstdir in ${tst_dirs}
   do
      errfiles=`ls ${tstdir}/*.err`
      failed=0
      for errf in ${errfiles}
      do
         if [[ -s ${errf} ]]
         then
            failed=`expr $failed + 1`
         fi
      done
      results_file=`ls ${tstdir}/UnitTest_summary*.html`
      result_name=`basename ${results_file}`
      result_path=`basename ${tstdir}`
      smryfile="<a href=${result_path}/${result_name}>${result_name}</a>"
      if [[ "${failed}" = "0" ]]
      then
         printf "${smryfile}   PASS <br>" >> $INDX_FILE
      else
         printf "${smryfile}   FAIL <br>" >> $INDX_FILE
      fi
   done

   printf "%s<br>\n" "-----------------------------------------------" >> $INDX_FILE
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

   SUBJECT="iMesh ${ITAPS_BRANCH} Test Summary `date '+%D'`"

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
GET_GRUMMP="yes"
GET_MOAB="yes"
GET_FMDB="yes"
GET_REFI="yes"
GET_MESQ="yes"
GET_SWAP="yes"
GET_IMESH="yes"


### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Process command line arguments
next_arg=""

for arg in $@
do
   if test -n "${next_arg}"
   then
      case ${next_arg} in
         grummp-vers) ITAPS_GRUMMP_VERSION="${arg}"
                      ;;
         grummp-path) ITAPS_GRUMMP_PATH="${arg}"
                      ;;
         grummp-file) GRUMMP_MESHFILE="${arg}"
                      ;;
           moab-vers) ITAPS_MOAB_VERSION="${arg}"
                      ;;
           moab-path) ITAPS_MOAB_PATH="${arg}"
                      ;;
           moab-file) MOAB_MESHFILE="${arg}"
                      ;;
           fmdb-vers) ITAPS_FMDB_VERSION="${arg}"
                      ;;
           fmdb-path) ITAPS_FMDB_PATH="${arg}"
                      ;;
           fmdb-file) FMDB_MESHFILE="${arg}"
                      ;;
           refi-vers) ITAPS_REFI_VERSION="${arg}"
                      ;;
           refi-path) ITAPS_REFI_PATH="${arg}"
                      ;;
           refi-file) REFI_MESHFILE="${arg}"
                      ;;
           mesq-vers) ITAPS_MESQ_VERSION="${arg}"
                      ;;
           mesq-path) ITAPS_MESQ_PATH="${arg}"
                      ;;
           swap-vers) ITAPS_SWAP_VERSION="${arg}"
                      ;;
           swap-path) ITAPS_SWAP_PATH="${arg}"
                      ;;
          imesh-path) ITAPS_IMESH_PATH="${arg}"
                      ;;
        itaps-branch) ITAPS_BRANCH="${arg}"
                      ;;
     esac
     next_arg=""
     continue
   fi

   case ${arg} in
           -h | --help) usage
                        exit 99
                        ;;
        --itaps-branch) next_arg="itaps-branch"
                        ;;
      --grummp-version) next_arg="grummp-vers"
                        ;;
        --moab-version) next_arg="moab-vers"
                        ;;
        --fmdb-version) next_arg="fmdb-vers"
                        ;;
        --refi-version) next_arg="refi-vers"
                        ;;
        --mesq-version) next_arg="mesq-vers"
                        ;;
        --swap-version) next_arg="swap-vers"
                        ;;
       --imesh-version) next_arg="imesh-vers"
                        ;;
         --grummp-path) next_arg="grummp-path"
                        ;;
           --moab-path) next_arg="moab-path"
                        ;;
           --fmdb-path) next_arg="fmdb-path"
                        ;;
           --refi-path) next_arg="refi-path"
                        ;;
           --mesq-path) next_arg="mesq-path"
                        ;;
           --swap-path) next_arg="swap-path"
                        ;;
          --imesh-path) next_arg="imesh-path"
                        ;;
         --grummp-file) next_arg="grummp-file"
                        ;;
           --moab-file) next_arg="moab-file"
                        ;;
           --fmdb-file) next_arg="fmdb-file"
                        ;;
           --refi-file) next_arg="refi-file"
                        ;;
           --no-grummp) GET_GRUMMP="no"
                        ;;
             --no-moab) GET_MOAB="no"
                        ;;
             --no-fmdb) GET_FMDB="no"
                        ;;
             --no-refi) GET_REFI="no"
                        ;;
             --no-mesq) GET_MESQ="no"
                        ;;
             --no-swap) GET_SWAP="no"
                        ;;
            --no-imesh) GET_IMESH="no"
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
###     Set ITAPS_BRANCH to 'trunk' if not set on command line
if [ "X${ITAPS_BRANCH}" = "X" ]
then
   ITAPS_BRANCH=trunk
fi

### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Reset path names based on ITAPS_BRANCH
export ITAPS_SWAP_PATH="https://redmine.scorec.rpi.edu/anonsvn/itaps/software/${ITAPS_BRANCH}/services/Swapping/C_API"

export ZOLTAN_PATH="https://redmine.scorec.rpi.edu/anonsvn/itaps/software/${ITAPS_BRANCH}/release_distros/Zoltan_distrib_${ZOLTAN_VERSION}.tar.gz"

export ITAPS_IMESH_PATH="https://redmine.scorec.rpi.edu/anonsvn/itaps/software/${ITAPS_BRANCH}/interfaces/iMesh/test/unit"

export ITAPS_IMESH_FILES_PATH="https://redmine.scorec.rpi.edu/anonsvn/itaps/software/${ITAPS_BRANCH}/interfaces/iMesh/data"

### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Set name of error log file; initialize total error counter
ERRORLOG="`pwd`/ErrorLog.UnitTest"
echo "           Error Log for ITAPS ${ITAPS_BRANCH} UnitTests" > $ERRORLOG
echo "             `date '+%c'`" >> $ERRORLOG
errTot=0

### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Set current directory
CurrentDirectory=`pwd`

### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Get data files from repo, remove *.checkout file if empty
if [[ ! -r brick.vtk ]]
then
   echo "Getting data file brick.vtk from ${ITAPS_IMESH_FILES_PATH}"
   wget ${ITAPS_IMESH_FILES_PATH}/brick.vtk 1> BRICK.checkout 2> BRICK.checkout.log
   find . -name 'BRICK.checkout' -size 0 -exec rm -f {} \;
fi

if [[ ! -r globe_mats.vtk ]]
then
   echo "Getting data file globe_mats.vtk from ${ITAPS_IMESH_FILES_PATH}"
   wget ${ITAPS_IMESH_FILES_PATH}/globe_mats.vtk 1> GLOBE.checkout 2> GLOBE.checkout.log
   find . -name 'GLOBE.checkout' -size 0 -exec rm -f {} \;
fi

### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Building ${impl_name}
err_get_GRUMMP=0
err_untar_GRUMMP=0
err_dir_GRUMMP=0
err_have_GRUMMP=1

if [[ "${GET_GRUMMP}" = "yes" ]]
then
   echo " "
   echo "  - - - - - - - - - - - - - - - - - - - - - - - - - - "

   get_source_GRUMMP
   err_get_GRUMMP="$?"
   checkHandleError "$err_get_GRUMMP" "GRUMMP source code"

   if [[ "${err_get_GRUMMP}" = "0" ]]
   then
      uncompress_untar ${ITAPS_GRUMMP_SOURCE}
      err_untar_GRUMMP="$?"
      checkHandleError "$err_untar_GRUMMP" "GRUMMP untar"
   fi

   if [[ "${err_untar_GRUMMP}" = "0" ]]
   then
      get_builddir_GRUMMP
      err_dir_GRUMMP="$?"
      checkHandleError "$err_dir_GRUMMP" "GRUMMP build directory"
   fi

   if [[ "${err_dir_GRUMMP}" = "0" ]]
   then
      build_check_GRUMMP
      retval=$?
      err_have_GRUMMP=$retval
   
      case $retval in
         30) checkHandleError $retval "GRUMMP config" \
                             "${GRUMMP_BUILD_DIR}/GRUMMP.config.err"
             ;;
         35) checkHandleError $retval "GRUMMP make" \
                             "${GRUMMP_BUILD_DIR}/GRUMMP.make.err"
             ;;
         50) checkHandleError $retval "GRUMMP lib"
             ;;
      esac
   fi
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
      uncompress_untar ${ITAPS_MOAB_SOURCE}
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
      uncompress_untar ${ITAPS_FMDB_SOURCE}
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
###     Building RefImpl
cd ${CurrentDirectory}

err_get_REFI=0
err_untar_REFI=0
err_dir_REFI=0
err_have_REFI=1

if [[ "${GET_REFI}" = "yes" ]]
then
   echo " "
   echo "  - - - - - - - - - - - - - - - - - - - - - - - - - - "

   get_source_REFI
   err_get_REFI="$?"
   checkHandleError "$err_get_REFI" "REFI source code"

   if [[ "${err_get_REFI}" = "0" ]]
   then
      uncompress_untar ${ITAPS_REFI_SOURCE}
      err_untar_REFI="$?"
      checkHandleError "$err_untar_REFI" "REFI untar"
   fi
   
   if [[ "${err_untar_REFI}" = "0" ]]
   then
      get_builddir_REFI
      err_dir_REFI="$?"
      checkHandleError "$err_dir_REFI" "REFI build directory"
   fi
   
   if [[ "${err_dir_REFI}" = "0" ]]
   then
      build_check_REFI
      retval=$?
      err_have_REFI=$retval
   
      case $retval in
         30) checkHandleError $retval "REFI config" \
                             "${REFI_BUILD_DIR}/REFI.config.err"
             ;;
         35) checkHandleError $retval "REFI make" \
                             "${REFI_BUILD_DIR}/REFI.make.err"
             ;;
         40) checkHandleError $retval "REFI install" \
                             "${REFI_BUILD_DIR}/REFI.mkinstall.err"
             ;;
         50) checkHandleError $retval "REFI lib"
             ;;
      esac
   fi
fi


### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Building UnitTest
cd ${CurrentDirectory}

err_get_iMesh=0
err_dir_iMesh=0

if [[ "${GET_IMESH}" = "yes" ]]
then
   echo " "
   echo "  - - - - - - - - - - - - - - - - - - - - - - - - - - "

   get_source_UnitTest
   err_get_iMesh="$?"
   checkHandleError "$err_get_iMesh" "unitTest source code"

   if [[ "${err_get_iMesh}" = "0" ]]
   then
      get_builddir_UnitTest
      err_dir_iMesh="$?"
      checkHandleError "$err_dir_iMesh" "unitTest build directory"
   fi
fi

### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Running UnitTest for each implementation
cd ${CurrentDirectory}

if [[ "${err_dir_iMesh}" = "0" ]]
then

   for impl_name in ${IMPL_NAMES}
   do
      case ${impl_name} in
         GRUMMP) impl_dir=${GRUMMP_DEFS_DIR}
                 impl_inp=${GRUMMP_MESHFILE}
                 indu=$indg
                 ;;
           MOAB) impl_dir=${MOAB_DEFS_DIR}
                 impl_inp=${MOAB_MESHFILE}
                 indu=$indm
                 ;;
           FMDB) impl_dir=${FMDB_DEFS_DIR}
                 impl_inp=${FMDB_MESHFILE}
                 indu=$indf
                 ;;
           REFI) impl_dir=${REFI_DEFS_DIR}
                 impl_inp=${REFI_MESHFILE}
                 indu=$indr
                 ;;
      esac

      build_test_UnitTest ${impl_name} ${impl_dir} ${impl_inp}
      retval=$?

# - - - -   ck: config errors
      if [[ ${config_err} -eq 0 ]]
      then
         filarr[$indu]=""
         if [[ ! -z "$(cat imesh_${impl_name}.config.err)" ]]
         then
            numwarn=`grep -i -e "warn" imesh_${impl_name}.config.err|wc -l`
            if [[ "$numwarn" != "0" ]]
            then
               filarr[$indu]="WARN:imesh_${impl_name}.config.err"
            fi
         fi
      elif [[ ${config_err} -eq 1 ]]
      then
         filarr[$indu]="imesh_${impl_name}.config.err"
      elif [[ ${config_err} -eq 999 ]]
      then
         filarr[$indu]="DNR"
      fi
      indu=`expr $indu + 1`

# - - - -   ck: make errors
      if [[ ${make_err} -eq 0 ]]
      then
         filarr[$indu]=""
         if [[ ! -z "$(cat imesh_${impl_name}.make.err)" ]]
         then
            numwarn=`grep -i -e "warn" imesh_${impl_name}.make.err|wc -l`
            if [[ "$numwarn" != "0" ]]
            then
               filarr[$indu]="WARN:imesh_${impl_name}.make.err"
            fi
         fi
      elif [[ ${make_err} -eq 1 ]]
      then
         filarr[$indu]="imesh_${impl_name}.make.err"
      elif [[ ${make_err} -eq 999 ]]
      then
         filarr[$indu]="DNR"
      fi
      indu=`expr $indu + 1`

# - - - -   ck: make-no-spec errors
      if [[ ${nock_err} -eq 0 ]]
      then
         filarr[$indu]=""
         if [[ ! -z "$(cat imesh_${impl_name}.make.err)" ]]
         then
            numwarn=`grep -i -e "warn" imesh_${impl_name}.make.err|wc -l`
            if [[ "$numwarn" != "0" ]]
            then
               filarr[$indu]="WARN:imesh_${impl_name}.make.err"
            fi
         fi
      elif [[ ${nock_err} -eq 1 ]]
      then
         filarr[$indu]="imesh_${impl_name}.make.err"
      elif [[ ${nock_err} -eq 999 ]]
      then
         filarr[$indu]="DNR"
      fi
      indu=`expr $indu + 1`

# - - - -   ck: save-load errors
      if [[ ${svl_err} -eq 0 ]]
      then
         filarr[$indu]=""
      elif [[ ${svl_err} -eq 1 ]]
      then
         if [[ -z "$(cat imesh_${impl_name}.msg)" ]]
         then
            filarr[$indu]="imesh_${impl_name}.out"
         else
            filarr[$indu]="imesh_${impl_name}.msg"
         fi
      elif [[ ${svl_err} -eq 999 ]]
      then
         filarr[$indu]="DNR"
      fi
      indu=`expr $indu + 1`

# - - - -   ck: no-save-load errors
      if [[ ${nosvl_err} -eq 0 ]]
      then
         filarr[$indu]=""
      elif [[ ${nosvl_err} -eq 1 ]]
      then
         if [[ -z "$(cat imesh_${impl_name}.no-save-load.msg)" ]]
         then
            filarr[$indu]="imesh_${impl_name}.no-save-load.out"
         else
            filarr[$indu]="imesh_${impl_name}.no-save-load.msg"
         fi
      elif [[ ${nosvl_err} -eq 999 ]]
      then
         filarr[$indu]="DNR"
      fi
      indu=`expr $indu + 1`

# - - - -   ck: valgrind errors
      if [[ ${valg_err} -eq 0 ]]
      then
         filarr[$indu]=""
      elif [[ ${valg_err} -eq 1 ]]
      then
         if [[ -z "$(cat imesh_${impl_name}.valg.msg)" ]]
         then
            filarr[$indu]="imesh_${impl_name}.valg.out"
         else
            filarr[$indu]="imesh_${impl_name}.valg.msg"
         fi
      elif [[ ${valg_err} -eq 999 ]]
      then
         filarr[$indu]="DNR"
      fi
      indu=`expr $indu + 1`

# - - - -     reset implementation indicies
      case ${impl_name} in
         GRUMMP) indg=$indu
                 ;;
           MOAB) indm=$indu
                 ;;
           FMDB) indf=$indu
                 ;;
           REFI) indr=$indu
                 ;;
      esac

      case $retval in
         30) checkHandleError $retval "unitTest/${impl_name} config" \
                             "${IMESH_BUILD_DIR}/imesh_${impl_name}.config.err"
             ;;
         35) checkHandleError $retval "unitTest/${impl_name} make" \
                             "${IMESH_BUILD_DIR}/imesh_${impl_name}.make.err"
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
            GRUMMP) impl_dir=${GRUMMP_DEFS_DIR}
                    indq=${indg}
                    ;;
              MOAB) impl_dir=${MOAB_DEFS_DIR}
                    indq=${indm}
                    ;;
              FMDB) impl_dir=${FMDB_DEFS_DIR}
                    indq=${indf}
                    ;;
              REFI) impl_dir=${REFI_DEFS_DIR}
                    indq=${indr}
                    ;;
         esac

         build_check_MESQ ${impl_name} ${impl_dir}
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
            filarr[$indq]="MESQ_${impl_name}.config.err"
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
            filarr[$indq]="MESQ_${impl_name}.make.err"
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
            filarr[$indq]="MESQ_${impl_name}.results"
         elif [[ ${check_err} -eq 999 ]]
         then
            filarr[$indq]="DNR"
         fi
         indq=`expr $indq + 1`

# - - - -    reset implementation indicies
         case ${impl_name} in
            GRUMMP) indg=${indq}
                    ;;
              MOAB) indm=${indq}
                    ;;
              FMDB) indf=${indq}
                    ;;
              REFI) indr=${indq}
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
###     Build Swapping with each implementation

cd ${CurrentDirectory}

err_get_SWAP=0
err_untar_SWAP=0
err_dir_SWAP=0

if [[ "${GET_SWAP}" = "yes" ]]
then
   get_source_SWAP ${ITAPS_SWAP_SOURCE}
   err_get_SWAP="$?"
   checkHandleError "$err_get_SWAP" "Swapping source code"

   if [[ "${err_get_SWAP}" = "0" ]]
   then
      uncompress_untar ${ITAPS_SWAP_SOURCE}
      err_untar_SWAP="$?"
      checkHandleError "$err_untar_SWAP" "Swapping untar"
   fi

   if [[ "${err_untar_SWAP}" = "0" ]]
   then
      get_builddir_SWAP
      err_dir_SWAP="$?"
      checkHandleError "$err_dir_SWAP" "Swapping build directory"
   fi

   if [[ "${err_dir_SWAP}" = "0" ]]
   then
      for impl_name in ${IMPL_NAMES}
      do
         case ${impl_name} in

            GRUMMP) impl_dir=${GRUMMP_DEFS_DIR}
                    inds=${indg}
                    ;;
              MOAB) impl_dir=${MOAB_DEFS_DIR}
                    inds=${indm}
                    ;;
              FMDB) impl_dir=${FMDB_DEFS_DIR}
                    inds=${indf}
                    ;;
              REFI) impl_dir=${REFI_DEFS_DIR}
                    inds=${indr}
                    ;;
         esac

         build_check_SWAP ${impl_name} ${impl_dir}
         retval=$?

# - - - -   ck: config errors
         if [[ ${config_err} -eq 0 ]]
         then
            filarr[$inds]=""
            if [[ ! -z "$(cat SWAP_${impl_name}.config.err)" ]]
            then
               numwarn=`grep -i -e "warn" SWAP_${impl_name}.config.err|wc -l`
               if [[ "$numwarn" != "0" ]]
               then
                  filarr[$inds]="WARN:SWAP_${impl_name}.config.err"
               fi
            fi
         elif [[ ${config_err} -eq 1 ]]
         then
            filarr[$inds]="SWAP_${impl_name}.config.err"
         elif [[ ${config_err} -eq 999 ]]
         then
            filarr[$inds]="DNR"
         fi
         inds=`expr $inds + 1`

# - - - -   ck: make errors
         if [[ ${make_err} -eq 0 ]]
         then
            filarr[$inds]=""
            if [[ ! -z "$(cat SWAP_${impl_name}.make.err)" ]]
            then
               numwarn=`grep -i -e "warn" SWAP_${impl_name}.make.err|wc -l`
               if [[ "$numwarn" != "0" ]]
               then
                  filarr[$inds]="WARN:SWAP_${impl_name}.make.err"
               fi
            fi
         elif [[ ${make_err} -eq 1 ]]
         then
            filarr[$inds]="SWAP_${impl_name}.make.err"
         elif [[ ${make_err} -eq 999 ]]
         then
            filarr[$inds]="DNR"
         fi
         inds=`expr $inds + 1`

# - - - -   ck: check errors
         if [[ ${check_err} -eq 0 ]]
         then
            filarr[$inds]=""
            if [[ ! -z "$(cat SWAP_${impl_name}.results)" ]]
            then
               numwarn=`grep -i -e "warn" SWAP_${impl_name}.results|wc -l`
               if [[ "$numwarn" != "0" ]]
               then
                  filarr[$inds]="WARN:SWAP_${impl_name}.results"
               fi
            fi
         elif [[ ${check_err} -eq 1 ]]
         then
            filarr[$inds]="SWAP_${impl_name}.results"
         elif [[ ${check_err} -eq 999 ]]
         then
            filarr[$inds]="DNR"
         fi
         inds=`expr $inds + 1`

         case $retval in
            30) checkHandleError $retval "Swapping/${impl_name} config" \
                                "${SWAP_BUILD_DIR}/SWAP_${impl_name}.config.err"
                ;;
            35) checkHandleError $retval "Swapping/${impl_name} make" \
                                "${SWAP_BUILD_DIR}/SWAP_${impl_name}.make.err"
                ;;
            55) checkHandleError $retval \
                           "Swapping/${impl_name}; problem with build of ${impl_name}"
                ;;
             *) checkHandleError $retval "Return value from Swapping/${impl_name}"
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
   save_dir="${TEST_RESULTS_DIR}/results.`date '+%Y%m%d'`"
   save_dir_href="results.`date '+%Y%m%d'`"
else
   save_dir="${TEST_RESULTS_DIR}/results_rc.`date '+%Y%m%d'`"
   save_dir_href="results_rc.`date '+%Y%m%d'`"
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
if [[ "${ITAPS_GRUMMP_LINK}" != "" ]]
then
   rm -rf ${ITAPS_GRUMMP_LINK}
fi

if [[ "${ITAPS_MOAB_LINK}" != "" ]]
then
   rm -rf ${ITAPS_MOAB_LINK}
fi

if [[ "${ITAPS_FMDB_LINK}" != "" ]]
then
   rm -rf ${ITAPS_FMDB_LINK}
fi

if [[ "${ITAPS_REFI_LINK}" != "" ]]
then
   rm -rf ${ITAPS_REFI_LINK}
fi

if [[ "${ITAPS_MESQ_LINK}" != "" ]]
then
   rm -rf ${ITAPS_MESQ_LINK}
fi

if [[ "${ITAPS_SWAP_LINK}" != "" ]]
then
   rm -rf ${ITAPS_SWAP_LINK}
fi


### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Store error files in results sub-directory with dates for those
###     cases that were run
if [[ "${GET_GRUMMP}" = "yes" ]]
then
   mv -f ${GRUMMP_BUILD_DIR}/GRUMMP*.err  ${save_dir}
   mv -f ${GRUMMP_BUILD_DIR}/GRUMMP*.log  ${save_dir}
   mv -f ${GRUMMP_BUILD_DIR}/grummp_results  ${save_dir}
fi

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

if [[ "${GET_REFI}" = "yes" ]]
then
   mv -f ${REFI_BUILD_DIR}/REFI*.err  ${save_dir}
   mv -f ${REFI_BUILD_DIR}/REFI*.log  ${save_dir}
fi

if [[ "${GET_IMESH}" = "yes" ]]
then
   mv -f ${IMESH_BUILD_DIR}/imesh*.err  ${save_dir}
   mv -f ${IMESH_BUILD_DIR}/imesh*.log  ${save_dir}
   mv -f ${IMESH_BUILD_DIR}/imesh*.msg  ${save_dir}
   mv -f ${IMESH_BUILD_DIR}/imesh*.out  ${save_dir}
fi

if [[ "${GET_MESQ}" = "yes" ]]
then
   mv -f ${MESQ_BUILD_DIR}/MESQ*.err ${save_dir}
   mv -f ${MESQ_BUILD_DIR}/MESQ*.log ${save_dir}
   mv -f ${MESQ_BUILD_DIR}/MESQ*.results ${save_dir}
fi

if [[ "${GET_SWAP}" = "yes" ]]
then
   mv -f ${SWAP_BUILD_DIR}/SWAP*.err ${save_dir}
   mv -f ${SWAP_BUILD_DIR}/SWAP*.log ${save_dir}
   mv -f ${SWAP_BUILD_DIR}/SWAP*.results ${save_dir}
fi

mv -f ${CurrentDirectory}/buildF* ${save_dir}
mv -f ${CurrentDirectory}/build_F* ${save_dir}
mv -f ${CurrentDirectory}/*.cron* ${save_dir}
mv -f ${CurrentDirectory}/*.checkout* ${save_dir}
mv -f ${CurrentDirectory}/*.err ${save_dir}
mv -f ${CurrentDirectory}/*.log ${save_dir}
mv -f ${CurrentDirectory}/*.vtk ${save_dir}
mv -f ${CurrentDirectory}/*.html ${save_dir}
mv -f ${CurrentDirectory}/Error* ${save_dir}

### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###      create tar file for SCOREC files
if [[ -d SCOREC_Software ]]
then
   tar -cf FMDB.tar SCOREC_Software/
fi

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
    rm -rf ${GRUMMP_BUILD_DIR}
    rm -rf ${MOAB_BUILD_DIR}
    rm -rf SCOREC_Software
    rm -rf fmdbSerial
    rm -rf ${FMDB_BUILD_DIR}
    rm -rf ${REFI_BUILD_DIR}
    rm -rf ${IMESH_BUILD_DIR}
    rm -rf ${MESQ_BUILD_DIR}
    rm -rf ${SWAP_BUILD_DIR}
fi


### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Create index of results directories
create_html_index


### - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
###     Change permissions of results directories
chmod -R 775 ${save_dir}
chgrp -R itaps ${save_dir}

exit 0
