#include "sidl.h"
#include "TSTTB.hh"
#include "TSTTR.hh"
#include "TSTTM_GRUMMP.hh"
#include "TSTTR_GRUMMP.hh"

typedef void* opaque;

int iNPassed = 0, iNFailed = 0;
bool qAllPassed = true;

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

// Variables defined to make TEST statments more descriptive
static const bool error_throwing = false;
static const bool data_preparation = false;

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

int main(int argc, char *argv[])
{
  // Tests for core association functions.

  TSTTR_GRUMMP::Assoc Assoc1;
  Assoc1 = TSTTR_GRUMMP::Assoc::_create();

  TSTTM_GRUMMP::Plane Mesh1, Mesh2, Mesh3;
  Mesh1 = TSTTM_GRUMMP::Plane::_create();
  Mesh2 = TSTTM_GRUMMP::Plane::_create();
  Mesh3 = TSTTM_GRUMMP::Plane::_create();

  Assoc1.createAssociation(Mesh1, Mesh2);
  Assoc1.createAssociation(Mesh1, Mesh3);

  sidl::array<sidl::BaseInterface> saBI_AssocWith1, saBI_AssocWith2, saBI_AssocWith3;
  int iNumAssocWith1, iNumAssocWith2, iNumAssocWith3;

  Assoc1.getAssociatedInterfaces(Mesh1, saBI_AssocWith1, iNumAssocWith1);
  Assoc1.getAssociatedInterfaces(Mesh2, saBI_AssocWith2, iNumAssocWith2);
  Assoc1.getAssociatedInterfaces(Mesh3, saBI_AssocWith3, iNumAssocWith3);

  TEST(iNumAssocWith1 == 2);
  TEST(iNumAssocWith2 == 1);
  TEST(iNumAssocWith3 == 1);
  TEST(Mesh1.isSame(saBI_AssocWith2.get(0)));
  TEST(Mesh1.isSame(saBI_AssocWith3.get(0)));
  TEST((Mesh2.isSame(saBI_AssocWith1.get(0)) &&
	Mesh3.isSame(saBI_AssocWith1.get(1))) ||
       (Mesh3.isSame(saBI_AssocWith1.get(0)) &&
	Mesh2.isSame(saBI_AssocWith1.get(1))));

  Assoc1.destroyAssociation(Mesh3, Mesh1);

  Assoc1.getAssociatedInterfaces(Mesh1, saBI_AssocWith1, iNumAssocWith1);
  Assoc1.getAssociatedInterfaces(Mesh2, saBI_AssocWith2, iNumAssocWith2);
  Assoc1.getAssociatedInterfaces(Mesh3, saBI_AssocWith3, iNumAssocWith3);

  TEST(iNumAssocWith1 == 1);
  TEST(iNumAssocWith2 == 1);
  TEST(iNumAssocWith3 == 0);
  TEST(Mesh1.isSame(saBI_AssocWith2.get(0)));
  TEST(Mesh2.isSame(saBI_AssocWith1.get(0)));

  try {
    Assoc1.setEntEntAssociation(Mesh1, Mesh3, opaque(1), opaque(2));
  } MUST_THROW(TSTTB::ErrorType_INVALID_ARGUMENT);

  try {
    Assoc1.setEntEntAssociation(Mesh1, Mesh2, opaque(1), opaque(3));
    Assoc1.setEntEntAssociation(Mesh1, Mesh2, opaque(1), opaque(4));
  } SHOULD_NOT_THROW;

  opaque oResult = NULL;
  try {
    Assoc1.getEntEntAssociation(Mesh1, Mesh2, opaque(3), oResult);
  } MUST_THROW(TSTTB::ErrorType_INVALID_ARGUMENT);

  try {
    Assoc1.getEntEntAssociation(Mesh1, Mesh2, opaque(1), oResult);
  } MUST_THROW(TSTTB::ErrorType_INVALID_ARGUMENT);

  try {
    Assoc1.getEntEntAssociation(Mesh2, Mesh1, opaque(1), oResult);
  } MUST_THROW(TSTTB::ErrorType_INVALID_ARGUMENT);

  try {
    Assoc1.getEntEntAssociation(Mesh2, Mesh1, opaque(3), oResult);
    TEST(oResult == opaque(1));
  } SHOULD_NOT_THROW;

  try {
    Assoc1.getEntEntAssociation(Mesh2, Mesh1, opaque(4), oResult);
    TEST(oResult == opaque(1));
  } SHOULD_NOT_THROW;

  TEST_STATS;

  exit(0);
}
