#include <iFoo.h>

int main()
{
    iFoo_Instance fooA;
    iFoo_EntityHandle vertA, vertB;
    iFoo_EntitySetHandle setA;
    int err;

    // Operate on implementation A's instance
    iFoo_newMesh("IMPLMUX:A", &fooA, &err, 0);
    iFoo_getRootSet(fooA, &setA, &err);
    iFoo_createVtx(fooA, 1.0, 2.0, 3.0, &vertA, &err);
    iFoo_createVtx(fooA, 4.0, 5.0, 6.0, &vertB, &err);
 
}
