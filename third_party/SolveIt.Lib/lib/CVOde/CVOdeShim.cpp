#include "LLNLTYPS.H"
#include "nvector.h"
#include "CVODE.H"
#include "CVDENSE.H"
#include "CVSPGMR.H"

#undef _llnltyps_h
#undef nvector_h
#undef _cvode_h
#undef _dense_h
#undef _cvdense_h
#undef _iterativ_h
#undef _spgmr_h
#undef _cvspgmr_h
#undef _llnlmath_h

namespace cvode_c {
#include "include/llnltyps.h"
#include "include/nvector.h"
#include "include/cvode.h"
#include "include/cvdense.h"
#include "include/cvspgmr.h"
}

namespace {

template <typename To, typename From>
To CvodeCast(From value)
{
    return reinterpret_cast<To>(value);
}

} // namespace

namespace CVODE {

N_Vector N_VNew(integer n, void *machEnv)
{
    return CvodeCast<N_Vector>(cvode_c::N_VNew(n, machEnv));
}

void N_VFree(N_Vector x)
{
    cvode_c::N_VFree(CvodeCast<cvode_c::N_Vector>(x));
}

void *CVodeMalloc(integer N, RhsFn f, real t0, N_Vector y0, int lmm, int iter,
                  int itol, real *reltol, void *abstol, void *f_data,
                  FILE *errfp, boole optIn, long int iopt[], real ropt[],
                  void *machEnv)
{
    return cvode_c::CVodeMalloc(
        N,
        CvodeCast<cvode_c::RhsFn>(f),
        t0,
        CvodeCast<cvode_c::N_Vector>(y0),
        lmm,
        iter,
        itol,
        CvodeCast<cvode_c::real *>(reltol),
        abstol,
        f_data,
        errfp,
        optIn,
        iopt,
        ropt,
        machEnv);
}

int CVode(void *cvode_mem, real tout, N_Vector yout, real *t, int itask)
{
    return cvode_c::CVode(cvode_mem, tout, CvodeCast<cvode_c::N_Vector>(yout), t, itask);
}

void CVodeFree(void *cvode_mem)
{
    cvode_c::CVodeFree(cvode_mem);
}

void CVDense(void *cvode_mem, CVDenseJacFn djac, void *jac_data)
{
    cvode_c::CVDense(cvode_mem, CvodeCast<cvode_c::CVDenseJacFn>(djac), jac_data);
}

void CVSpgmr(void *cvode_mem, int pretype, int gstype, int maxl, real delt,
             CVSpgmrPrecondFn precond, CVSpgmrPSolveFn psolve, void *P_data)
{
    cvode_c::CVSpgmr(
        cvode_mem,
        pretype,
        gstype,
        maxl,
        delt,
        CvodeCast<cvode_c::CVSpgmrPrecondFn>(precond),
        CvodeCast<cvode_c::CVSpgmrPSolveFn>(psolve),
        P_data);
}

real **denalloc(integer n)
{
    return cvode_c::denalloc(n);
}

integer *denallocpiv(integer n)
{
    return cvode_c::denallocpiv(n);
}

integer gefa(real **a, integer n, integer *p)
{
    return cvode_c::gefa(a, n, p);
}

void gesl(real **a, integer n, integer *p, real *b)
{
    cvode_c::gesl(a, n, p, b);
}

void denfreepiv(integer *p)
{
    cvode_c::denfreepiv(p);
}

void denfree(real **a)
{
    cvode_c::denfree(a);
}

void denzero(real **a, integer n)
{
    cvode_c::denzero(a, n);
}

} // namespace CVODE