#include "iField_dfk.h"
#include <stdio.h>

void foo(IFIELD_DFK_EVAL_SIGNATURE)
{
    printf("I'm in foo\n");
    *err = iBase_SUCCESS;
}

void bar(int *continuous_ness, int *err)
{
    printf("I'm in bar\n");
    *err = iBase_SUCCESS;
}

int main()
{

    int which_func;
    double param_coords[3];
    double dfunc_value;
    int err;
    int continuous_ness;
    int degree;

    /* Set up partial DFK to test interface */
    iField_DFKHandle my_dfk;
    iField_setDfuncEvalFunc(my_dfk, foo, &err);
    iField_setGetContinuousNessFunc(my_dfk, bar, &err);

    /* try the register mechanism too */
    iField_DFKHandle your_dfk;
    iField_registerDFK(your_dfk,
        foo, 0, 0, 0, 0, 0, bar, 0, &err);


    /* Ok, now call some of this DFK's methods */
    iField_DFKEvalDfunc(my_dfk, which_func, param_coords, &dfunc_value, &err);
    iField_DFKGetContinuousNess(my_dfk, &continuous_ness, &err);

    iField_DFKEvalDfunc(your_dfk, which_func, param_coords, &dfunc_value, &err);
    iField_DFKGetContinuousNess(your_dfk, &continuous_ness, &err);

    /* Ok, try a bad call and see if we get an error. */
    iField_DFKGetDegree(your_dfk, &degree, &err);
    printf("Got %s from iField_DFKGetDegree\n", (err==iBase_BAD_DFK)?"iBase_BAD_DFK":"incorrect result");

    return 0;
}
