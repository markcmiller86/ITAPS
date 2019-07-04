#include "TSTTM_unitTest-config.h"

#include "TSTTB.hh"
#include "TSTTM.hh"

//-------------------------------------------------------

//@@ Standard include files

#include <set>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <assert.h>

using std::cout;
using std::endl;
using std::string;

//@@ Typedefs
typedef void* opaque;

//@@ Macros and file static variables.

#define TYPE_HELP do {\
  cout << endl;\
  cout << "This file is intended to test functionality of a TSTT implementation." << endl;\
  cout << "Mesh query, modification, iterators, entity sets, and tags are all" << endl;\
  cout << "tested.  In cases where it's practical to define a priori a correct" << endl;\
  cout << "result for a query, that is done.  In other cases, consistency is the" << endl;\
  cout << "only thing checked.  In still others, the only possible correctness" << endl;\
  cout << "check is that the function took correct input and didn't choke" << endl;\
  cout << "(things like getAdjTable or getGeometricDim, for instance)." << endl;\
  cout << endl;\
  cout << "It is worth noting that, although efforts have been made to make each" << endl;\
  cout << "test as independent as possible, it is unavoidable that a failure" << endl;\
  cout << "during one test may cause failures in subsequent tests, sometimes" << endl;\
  cout << "even across different interface tests.  In light of this, it is best" << endl;\
  cout << "to try to correct errors in the order they occur, or at least to" << endl;\
  cout << "exclude interface tests that cause errors to see if later errors are" << endl;\
  cout << "avoided." << endl;\
  cout << endl;\
  cout << "Correct argument handling, including both pre- and post-conditions," << endl;\
  cout << "is checked elsewhere (TSTTM_precondTest.cc and" << endl;\
  cout << "TSTTM_postcondTest.cc)." << endl;\
} while (0)

#define TYPE_USAGE do {\
  std::cerr << endl;\
  std::cerr << "Usage: " << argv[0] << " [ -h | --help ]" << endl;\
  std::cerr << "   or  " << argv[0] << " [arguments] <mesh_filename>" \
	    << endl << endl;\
  std::cerr << "where help and arguments are:" << endl;\
  std::cerr << " -h       | --help            Display usage information and exit." << endl;\
  std::cerr << " -e       | --error           Test all error throwing." << endl;\
  std::cerr << " -s       | --silent          Display only final result." << endl;\
  std::cerr << " -o<file> | --output=<file>   Output the results to the filename given." << endl << endl;\
} while (0);

#define TYPE_USAGE_HELP do {\
  std::cerr << "All arguments must be given before the mesh filename, where:" << endl;\
  std::cerr << " 'help'    displays this message and ignores all other options," << endl;\
  std::cerr << " 'error'   adds more tests to more thoroughly test TSTT methods" << endl;\
  std::cerr << "           with regard to error-throwing circumstances," << endl;\
  std::cerr << " 'silent'  limits the output to only the final error count, and" << endl;\
  std::cerr << " 'output'  sends the program output to the filename given in the" << endl;\
  std::cerr << "           rest of the argument (the default is 'out_test.txt')." << endl << endl;\
} while (0);

#define TEST(a) do {\
  if (a) iNPassed++;\
  else {\
    qAllPassed = false;\
    iNFailed++;\
    cout << endl << __FILE__ << ":" << __LINE__ << ": " "Failure testing " << #a << endl;\
  }\
} while (0)

#define TEST_STATS do {\
  cout << endl << "Test results so far:  " << iNPassed << " passed, " \
       << iNFailed << " failed." << endl;\
} while(0)

//@ Testing for bad array arguments
#define CATCH_BAD_ARRAY_SIZE catch(TSTTB::Error TErr) {\
  TEST(TErr.getErrorType() == TSTTB::ErrorType_BAD_ARRAY_SIZE);\
}
#define CATCH_NIL_ARRAY catch(TSTTB::Error TErr) {\
  TEST(TErr.getErrorType() == TSTTB::ErrorType_NIL_ARRAY);\
}

#define MUST_THROW(ErrType) catch ( TSTTB::Error& e ) {\
  if ( e.getErrorType() != ErrType ) {\
    e.echo("Unexpected error type!");\
    TEST ( error_throwing );\
  } else\
    iNPassed++;\
}

#define SHOULD_NOT_THROW catch(TSTTB::Error TErr) {\
  TErr.echo("Should not have thrown an error!");\
  TEST ( error_throwing );\
}

#define SHOULD_NOT_THROW_CRITICAL catch(TSTTB::Error TErr) {\
  TErr.echo("Should not have thrown an error!");\
  TEST ( error_throwing );\
  cout << "Cannot continue interface test, exiting...";\
  return;\
}

#define GOT_ALL_ITEMS(_input, _saOutput, _numItems, _qGotAll) do {\
  _qGotAll = true; \
  bool _gotItem; \
  void** _ppvOutput = _saOutput.first(); \
  for ( int _i=0; _i<_numItems && _qGotAll; _i++ ) { \
    _gotItem = false; \
    for ( int _j=0; _j<_numItems && !_gotItem; _j++ ) \
      _gotItem = _gotItem || (_input[_i] == _ppvOutput[_j]); \
    _qGotAll = _qGotAll && _gotItem; \
  } \
} while (0)

#define TOL 1.e-10
#define APPROX_EQUAL(a, b) (((a) < (b) + TOL) && ((b) < (a) + TOL))

// Variables defined to make TEST statments more descriptive
static const bool error_throwing = false;
static const bool data_preparation = false;

#ifdef MAIN_PROGRAM
int iNPassed = 0, iNFailed = 0,
  iNNotImpl = 0, iNIntPassed = 0, iNIntFailed = 0;
bool qAllPassed = true, qErrTests = false, qSilent = false, qOutFile = false;

int a2iAdjTable[4][4];

void *pvRootSet = NULL;
bool a2qLegalTypeAndTopo[5][12];
int a2iEntCount[5][12];
char* astrTypes[] = {"vertex",
			    "edge",
			    "face",
			    "region",
			    "all types"};
char* astrTopologies[] = {"point",
				 "line segment",
				 "polygon",
				 "triangle",
				 "quadrilateral",
				 "polyhedron",
				 "tetrahedron",
				 "pyramid",
				 "prism",
				 "hexahedron",
				 "septahedron",
				 "all topologies"};

TSTTB::TagValueType aTagTypes[] = {TSTTB::TagValueType_INTEGER,
				   TSTTB::TagValueType_DOUBLE,
				   TSTTB::TagValueType_ENTITY_HANDLE,
				   TSTTB::TagValueType_BYTES};
char* astrTagNames[] = {"Int Tag", "Double Tag", "Ent Tag",
			      "Generic Tag"};
int aiTagSizes[] = { 1, 2, 3, 4 };
int aiTagBytes[] = {sizeof(int), sizeof(double),
			   sizeof(void*), sizeof(char)};
#else
extern int iNPassed, iNFailed, iNNotImpl, iNIntPassed, iNIntFailed;
extern bool qAllPassed, qErrTests, qSilent, qOutFile;

extern int a2iAdjTable[4][4];

extern void *pvRootSet;
extern bool a2qLegalTypeAndTopo[5][12];
extern int a2iEntCount[5][12];
extern char* astrTypes[];
extern char* astrTopologies[];
extern TSTTB::TagValueType aTagTypes[];
extern char* astrTagNames[];
extern int aiTagSizes[];
extern int aiTagBytes[];
#endif

void vTestIterWithModify(const int iDim);

