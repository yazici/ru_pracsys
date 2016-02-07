/// autogenerated analytical inverse kinematics code from ikfast program part of OpenRAVE
/// \author Rosen Diankov
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// ikfast version 61 generated on 2013-10-08 17:02:52.109034
/// To compile with gcc:
///     gcc -lstdc++ ik.cpp
/// To compile without any main function as a shared object (might need -llapack):
///     gcc -fPIC -lstdc++ -DIKFAST_NO_MAIN -DIKFAST_CLIBRARY -shared -Wl,-soname,libik.so -o libik.so ik.cpp
#define IKFAST_HAS_LIBRARY
#include "ikfast.h" // found inside share/openrave-X.Y/python/ikfast.h
using namespace ikfast;

// check if the included ikfast version matches what this file was compiled with
#define IKFAST_COMPILE_ASSERT(x) extern int __dummy[(int)x]
IKFAST_COMPILE_ASSERT(IKFAST_VERSION==61);

#include <cmath>
#include <vector>
#include <limits>
#include <algorithm>
#include <complex>

#define IKFAST_STRINGIZE2(s) #s
#define IKFAST_STRINGIZE(s) IKFAST_STRINGIZE2(s)

#ifndef IKFAST_ASSERT
#include <stdexcept>
#include <sstream>
#include <iostream>

#ifdef _MSC_VER
#ifndef __PRETTY_FUNCTION__
#define __PRETTY_FUNCTION__ __FUNCDNAME__
#endif
#endif

#ifndef __PRETTY_FUNCTION__
#define __PRETTY_FUNCTION__ __func__
#endif

#define IKFAST_ASSERT(b) { if( !(b) ) { std::stringstream ss; ss << "ikfast exception: " << __FILE__ << ":" << __LINE__ << ": " <<__PRETTY_FUNCTION__ << ": Assertion '" << #b << "' failed"; throw std::runtime_error(ss.str()); } }

#endif

#if defined(_MSC_VER)
#define IKFAST_ALIGNED16(x) __declspec(align(16)) x
#else
#define IKFAST_ALIGNED16(x) x __attribute((aligned(16)))
#endif

#define IK2PI  ((IkReal)6.28318530717959)
#define IKPI  ((IkReal)3.14159265358979)
#define IKPI_2  ((IkReal)1.57079632679490)

#ifdef _MSC_VER
#ifndef isnan
#define isnan _isnan
#endif
#endif // _MSC_VER

// lapack routines
extern "C" {
  void dgetrf_ (const int* m, const int* n, double* a, const int* lda, int* ipiv, int* info);
  void zgetrf_ (const int* m, const int* n, std::complex<double>* a, const int* lda, int* ipiv, int* info);
  void dgetri_(const int* n, const double* a, const int* lda, int* ipiv, double* work, const int* lwork, int* info);
  void dgesv_ (const int* n, const int* nrhs, double* a, const int* lda, int* ipiv, double* b, const int* ldb, int* info);
  void dgetrs_(const char *trans, const int *n, const int *nrhs, double *a, const int *lda, int *ipiv, double *b, const int *ldb, int *info);
  void dgeev_(const char *jobvl, const char *jobvr, const int *n, double *a, const int *lda, double *wr, double *wi,double *vl, const int *ldvl, double *vr, const int *ldvr, double *work, const int *lwork, int *info);
}

using namespace std; // necessary to get std math routines

#ifdef IKFAST_NAMESPACE
namespace IKFAST_NAMESPACE {
#endif

inline float IKabs(float f) { return fabsf(f); }
inline double IKabs(double f) { return fabs(f); }

inline float IKsqr(float f) { return f*f; }
inline double IKsqr(double f) { return f*f; }

inline float IKlog(float f) { return logf(f); }
inline double IKlog(double f) { return log(f); }

// allows asin and acos to exceed 1
#ifndef IKFAST_SINCOS_THRESH
#define IKFAST_SINCOS_THRESH ((IkReal)0.000001)
#endif

// used to check input to atan2 for degenerate cases
#ifndef IKFAST_ATAN2_MAGTHRESH
#define IKFAST_ATAN2_MAGTHRESH ((IkReal)2e-6)
#endif

// minimum distance of separate solutions
#ifndef IKFAST_SOLUTION_THRESH
#define IKFAST_SOLUTION_THRESH ((IkReal)1e-6)
#endif

inline float IKasin(float f)
{
IKFAST_ASSERT( f > -1-IKFAST_SINCOS_THRESH && f < 1+IKFAST_SINCOS_THRESH ); // any more error implies something is wrong with the solver
if( f <= -1 ) return float(-IKPI_2);
else if( f >= 1 ) return float(IKPI_2);
return asinf(f);
}
inline double IKasin(double f)
{
IKFAST_ASSERT( f > -1-IKFAST_SINCOS_THRESH && f < 1+IKFAST_SINCOS_THRESH ); // any more error implies something is wrong with the solver
if( f <= -1 ) return -IKPI_2;
else if( f >= 1 ) return IKPI_2;
return asin(f);
}

// return positive value in [0,y)
inline float IKfmod(float x, float y)
{
    while(x < 0) {
        x += y;
    }
    return fmodf(x,y);
}

// return positive value in [0,y)
inline double IKfmod(double x, double y)
{
    while(x < 0) {
        x += y;
    }
    return fmod(x,y);
}

inline float IKacos(float f)
{
IKFAST_ASSERT( f > -1-IKFAST_SINCOS_THRESH && f < 1+IKFAST_SINCOS_THRESH ); // any more error implies something is wrong with the solver
if( f <= -1 ) return float(IKPI);
else if( f >= 1 ) return float(0);
return acosf(f);
}
inline double IKacos(double f)
{
IKFAST_ASSERT( f > -1-IKFAST_SINCOS_THRESH && f < 1+IKFAST_SINCOS_THRESH ); // any more error implies something is wrong with the solver
if( f <= -1 ) return IKPI;
else if( f >= 1 ) return 0;
return acos(f);
}
inline float IKsin(float f) { return sinf(f); }
inline double IKsin(double f) { return sin(f); }
inline float IKcos(float f) { return cosf(f); }
inline double IKcos(double f) { return cos(f); }
inline float IKtan(float f) { return tanf(f); }
inline double IKtan(double f) { return tan(f); }
inline float IKsqrt(float f) { if( f <= 0.0f ) return 0.0f; return sqrtf(f); }
inline double IKsqrt(double f) { if( f <= 0.0 ) return 0.0; return sqrt(f); }
inline float IKatan2(float fy, float fx) {
    if( isnan(fy) ) {
        IKFAST_ASSERT(!isnan(fx)); // if both are nan, probably wrong value will be returned
        return float(IKPI_2);
    }
    else if( isnan(fx) ) {
        return 0;
    }
    return atan2f(fy,fx);
}
inline double IKatan2(double fy, double fx) {
    if( isnan(fy) ) {
        IKFAST_ASSERT(!isnan(fx)); // if both are nan, probably wrong value will be returned
        return IKPI_2;
    }
    else if( isnan(fx) ) {
        return 0;
    }
    return atan2(fy,fx);
}

inline float IKsign(float f) {
    if( f > 0 ) {
        return float(1);
    }
    else if( f < 0 ) {
        return float(-1);
    }
    return 0;
}

inline double IKsign(double f) {
    if( f > 0 ) {
        return 1.0;
    }
    else if( f < 0 ) {
        return -1.0;
    }
    return 0;
}

/// solves the forward kinematics equations.
/// \param pfree is an array specifying the free joints of the chain.
IKFAST_API void ComputeW2Fk(const IkReal* j, IkReal* eetrans, IkReal* eerot);
IKFAST_API void ComputeW1Fk(const IkReal* j, IkReal* eetrans, IkReal* eerot);
IKFAST_API void ComputeW0Fk(const IkReal* j, IkReal* eetrans, IkReal* eerot);
IKFAST_API void ComputeE1Fk(const IkReal* j, IkReal* eetrans, IkReal* eerot);
IKFAST_API void ComputeE0Fk(const IkReal* j, IkReal* eetrans, IkReal* eerot);
IKFAST_API void ComputeS1Fk(const IkReal* j, IkReal* eetrans, IkReal* eerot);
IKFAST_API void ComputeS0Fk(const IkReal* j, IkReal* eetrans, IkReal* eerot);
IKFAST_API void ComputeFk(const IkReal* j, IkReal* eetrans, IkReal* eerot);

IKFAST_API int GetNumFreeParameters();
IKFAST_API int* GetFreeParameters();
IKFAST_API int GetNumJoints();

IKFAST_API int GetIkRealSize();

IKFAST_API int GetIkType();
/*
class IKSolver {
public:
IkReal j0,cj0,sj0,htj0,j1,cj1,sj1,htj1,j2,cj2,sj2,htj2,j3,cj3,sj3,htj3,j4,cj4,sj4,htj4,j6,cj6,sj6,htj6,j5,cj5,sj5,htj5,new_r00,r00,rxp0_0,new_r01,r01,rxp0_1,new_r02,r02,rxp0_2,new_r10,r10,rxp1_0,new_r11,r11,rxp1_1,new_r12,r12,rxp1_2,new_r20,r20,rxp2_0,new_r21,r21,rxp2_1,new_r22,r22,rxp2_2,new_px,px,npx,new_py,py,npy,new_pz,pz,npz,pp;
unsigned char _ij0[2], _nj0,_ij1[2], _nj1,_ij2[2], _nj2,_ij3[2], _nj3,_ij4[2], _nj4,_ij6[2], _nj6,_ij5[2], _nj5;

bool ComputeIk(const IkReal* eetrans, const IkReal* eerot, const IkReal* pfree, IkSolutionListBase<IkReal>& solutions);


static inline bool checkconsistency8(const IkReal* Breal)
{
    IkReal norm = 0.1;
    for(int i = 0; i < 7; ++i) {
        norm += IKabs(Breal[i]);
    }
    IkReal tol = 1e-5*norm; // have to increase the threshold since many computations are involved
    return IKabs(Breal[0]*Breal[1]-Breal[2]) < tol && IKabs(Breal[1]*Breal[1]-Breal[3]) < tol && IKabs(Breal[0]*Breal[3]-Breal[4]) < tol && IKabs(Breal[1]*Breal[3]-Breal[5]) < tol && IKabs(Breal[0]*Breal[5]-Breal[6]) < tol;
}
/// \brief Solve the det Ax^2+Bx+C = 0 problem using the Manocha and Canny method (1994)
///
/// matcoeffs is of length 54*3, for 3 matrices
static inline void solvedialyticpoly8qep(const IkReal* matcoeffs, IkReal* rawroots, int& numroots)
{
    const IkReal tol = 128.0*std::numeric_limits<IkReal>::epsilon();
    IkReal IKFAST_ALIGNED16(M[16*16]) = {0};
    IkReal IKFAST_ALIGNED16(A[8*8]);
    IkReal IKFAST_ALIGNED16(work[16*16*15]);
    int ipiv[8];
    int info, coeffindex;
    const int worksize=16*16*15;
    const int matrixdim = 8;
    const int matrixdim2 = 16;
    numroots = 0;
    // first setup M = [0 I; -C -B] and A
    coeffindex = 0;
    for(int j = 0; j < 4; ++j) {
        for(int k = 0; k < 6; ++k) {
            M[matrixdim+(j+4)+2*matrixdim*k] = M[matrixdim+j+2*matrixdim*(k+2)] = -matcoeffs[coeffindex++];
        }
    }
    for(int j = 0; j < 4; ++j) {
        for(int k = 0; k < 6; ++k) {
            M[matrixdim+(j+4)+2*matrixdim*k+matrixdim*2*matrixdim] = M[matrixdim+j+2*matrixdim*(k+2)+matrixdim*2*matrixdim] = -matcoeffs[coeffindex++];
        }
    }
    for(int j = 0; j < 4; ++j) {
        for(int k = 0; k < 6; ++k) {
            A[(j+4)+matrixdim*k] = A[j+matrixdim*(k+2)] = matcoeffs[coeffindex++];
        }
        for(int k = 0; k < 2; ++k) {
            A[j+matrixdim*k] = A[(j+4)+matrixdim*(k+6)] = 0;
        }
    }
    const IkReal lfpossibilities[4][4] = {{1,-1,1,1},{1,0,-2,1},{1,1,2,0},{1,-1,4,1}};
    int lfindex = -1;
    bool bsingular = true;
    do {
        dgetrf_(&matrixdim,&matrixdim,A,&matrixdim,&ipiv[0],&info);
        if( info == 0 ) {
            bsingular = false;
            for(int j = 0; j < matrixdim; ++j) {
                if( IKabs(A[j*matrixdim+j]) < 100*tol ) {
                    bsingular = true;
                    break;
                }
            }
            if( !bsingular ) {
                break;
            }
        }
        if( lfindex == 3 ) {
            break;
        }
        // transform by the linear functional
        lfindex++;
        const IkReal* lf = lfpossibilities[lfindex];
        // have to reinitialize A
        coeffindex = 0;
        for(int j = 0; j < 4; ++j) {
            for(int k = 0; k < 6; ++k) {
                IkReal a = matcoeffs[coeffindex+48], b = matcoeffs[coeffindex+24], c = matcoeffs[coeffindex];
                A[(j+4)+matrixdim*k] = A[j+matrixdim*(k+2)] = lf[0]*lf[0]*a+lf[0]*lf[2]*b+lf[2]*lf[2]*c;
                M[matrixdim+(j+4)+2*matrixdim*k] = M[matrixdim+j+2*matrixdim*(k+2)] = -(lf[1]*lf[1]*a + lf[1]*lf[3]*b + lf[3]*lf[3]*c);
                M[matrixdim+(j+4)+2*matrixdim*k+matrixdim*2*matrixdim] = M[matrixdim+j+2*matrixdim*(k+2)+matrixdim*2*matrixdim] = -(2*lf[0]*lf[1]*a + (lf[0]*lf[3]+lf[1]*lf[2])*b + 2*lf[2]*lf[3]*c);
                coeffindex++;
            }
            for(int k = 0; k < 2; ++k) {
                A[j+matrixdim*k] = A[(j+4)+matrixdim*(k+6)] = 0;
            }
        }
    } while(lfindex<4);

    if( bsingular ) {
        return;
    }
    dgetrs_("No transpose", &matrixdim, &matrixdim2, A, &matrixdim, &ipiv[0], &M[matrixdim], &matrixdim2, &info);
    if( info != 0 ) {
        return;
    }

    // set identity in upper corner
    for(int j = 0; j < matrixdim; ++j) {
        M[matrixdim*2*matrixdim+j+matrixdim*2*j] = 1;
    }
    IkReal IKFAST_ALIGNED16(wr[16]);
    IkReal IKFAST_ALIGNED16(wi[16]);
    IkReal IKFAST_ALIGNED16(vr[16*16]);
    int one=1;
    dgeev_("N", "V", &matrixdim2, M, &matrixdim2, wr, wi,NULL, &one, vr, &matrixdim2, work, &worksize, &info);
    if( info != 0 ) {
        return;
    }
    IkReal Breal[matrixdim-1];
    for(int i = 0; i < matrixdim2; ++i) {
        if( IKabs(wi[i]) < tol*100 ) {
            IkReal* ev = vr+matrixdim2*i;
            if( IKabs(wr[i]) > 1 ) {
                ev += matrixdim;
            }
            // consistency has to be checked!!
            if( IKabs(ev[0]) < tol ) {
                continue;
            }
            IkReal iconst = 1/ev[0];
            for(int j = 1; j < matrixdim; ++j) {
                Breal[j-1] = ev[j]*iconst;
            }
            if( checkconsistency8(Breal) ) {
                if( lfindex >= 0 ) {
                    const IkReal* lf = lfpossibilities[lfindex];
                    rawroots[numroots++] = (wr[i]*lf[0]+lf[1])/(wr[i]*lf[2]+lf[3]);
                }
                else {
                    rawroots[numroots++] = wr[i];
                }
                bool bsmall0=IKabs(ev[0]) > IKabs(ev[2]);
                bool bsmall1=IKabs(ev[0]) > IKabs(ev[1]);
                if( bsmall0 && bsmall1 ) {
                    rawroots[numroots++] = ev[2]/ev[0];
                    rawroots[numroots++] = ev[1]/ev[0];
                }
                else if( bsmall0 && !bsmall1 ) {
                    rawroots[numroots++] = ev[3]/ev[1];
                    rawroots[numroots++] = ev[1]/ev[0];
                }
                else if( !bsmall0 && bsmall1 ) {
                    rawroots[numroots++] = ev[6]/ev[4];
                    rawroots[numroots++] = ev[7]/ev[6];
                }
                else if( !bsmall0 && !bsmall1 ) {
                    rawroots[numroots++] = ev[7]/ev[5];
                    rawroots[numroots++] = ev[7]/ev[6];
                }
            }
        }
    }
}

};*/
class IKSolver {
public:
IkReal j0,cj0,sj0,htj0,j1,cj1,sj1,htj1,j2,cj2,sj2,htj2,j3,cj3,sj3,htj3,j4,cj4,sj4,htj4,j6,cj6,sj6,htj6,j5,cj5,sj5,htj5,new_r00,r00,rxp0_0,new_r01,r01,rxp0_1,new_r02,r02,rxp0_2,new_r10,r10,rxp1_0,new_r11,r11,rxp1_1,new_r12,r12,rxp1_2,new_r20,r20,rxp2_0,new_r21,r21,rxp2_1,new_r22,r22,rxp2_2,new_px,px,npx,new_py,py,npy,new_pz,pz,npz,pp;
unsigned char _ij0[2], _nj0,_ij1[2], _nj1,_ij2[2], _nj2,_ij3[2], _nj3,_ij4[2], _nj4,_ij6[2], _nj6,_ij5[2], _nj5;

bool ComputeIk(const IkReal* eetrans, const IkReal* eerot, const IkReal* pfree, IkSolutionListBase<IkReal>& solutions);
static inline bool checkconsistency8(const IkReal* Breal)
{
    IkReal norm = 0.1;
    for(int i = 0; i < 7; ++i) {
        norm += IKabs(Breal[i]);
    }

    IkReal tol = 1e-5*norm; // have to increase the threshold since many computations are involved

    return IKabs(Breal[0]*Breal[1]-Breal[2]) < tol && IKabs(Breal[1]*Breal[1]-Breal[3]) < tol && IKabs(Breal[0]*Breal[3]-Breal[4]) < tol && IKabs(Breal[1]*Breal[3]-Breal[5]) < tol && IKabs(Breal[0]*Breal[5]-Breal[6]) < tol;
}
/// \brief Solve the det Ax^2+Bx+C = 0 problem using the Manocha and Canny method (1994)
///
/// matcoeffs is of length 54*3, for 3 matrices
static inline void solvedialyticpoly8qep(const IkReal* matcoeffs, IkReal* rawroots, int& numroots)
{
    const IkReal tol = 128.0*std::numeric_limits<IkReal>::epsilon();
    IkReal IKFAST_ALIGNED16(M[16*16]) = {0};
    IkReal IKFAST_ALIGNED16(A[8*8]);
    IkReal IKFAST_ALIGNED16(work[16*16*15]);
    int ipiv[8];
    int info, coeffindex;
    const int worksize=16*16*15;
    const int matrixdim = 8;
    const int matrixdim2 = 16;
    numroots = 0;
    // first setup M = [0 I; -C -B] and A
    coeffindex = 0;
    for(int j = 0; j < 4; ++j) {
        for(int k = 0; k < 6; ++k) {
            M[matrixdim+(j+4)+2*matrixdim*k] = M[matrixdim+j+2*matrixdim*(k+2)] = -matcoeffs[coeffindex++];
        }
    }
    for(int j = 0; j < 4; ++j) {
        for(int k = 0; k < 6; ++k) {
            M[matrixdim+(j+4)+2*matrixdim*k+matrixdim*2*matrixdim] = M[matrixdim+j+2*matrixdim*(k+2)+matrixdim*2*matrixdim] = -matcoeffs[coeffindex++];
        }
    }
    for(int j = 0; j < 4; ++j) {
        for(int k = 0; k < 6; ++k) {
            A[(j+4)+matrixdim*k] = A[j+matrixdim*(k+2)] = matcoeffs[coeffindex++];
        }
        for(int k = 0; k < 2; ++k) {
            A[j+matrixdim*k] = A[(j+4)+matrixdim*(k+6)] = 0;
        }
    }
    const IkReal lfpossibilities[4][4] = {{1,-1,1,1},{1,0,-2,1},{1,1,2,0},{1,-1,4,1}};
    int lfindex = -1;
    bool bsingular = true;
    do {
        dgetrf_(&matrixdim,&matrixdim,A,&matrixdim,&ipiv[0],&info);
        if( info == 0 ) {
            bsingular = false;
            for(int j = 0; j < matrixdim; ++j) {
                if( IKabs(A[j*matrixdim+j]) < 100*tol ) {
                    bsingular = true;
                    break;
                }
            }
            if( !bsingular ) {
                break;
            }
        }
        if( lfindex == 3 ) {
            break;
        }
        // transform by the linear functional
        lfindex++;
        const IkReal* lf = lfpossibilities[lfindex];
        // have to reinitialize A
        coeffindex = 0;
        for(int j = 0; j < 4; ++j) {
            for(int k = 0; k < 6; ++k) {
                IkReal a = matcoeffs[coeffindex+48], b = matcoeffs[coeffindex+24], c = matcoeffs[coeffindex];
                A[(j+4)+matrixdim*k] = A[j+matrixdim*(k+2)] = lf[0]*lf[0]*a+lf[0]*lf[2]*b+lf[2]*lf[2]*c;
                M[matrixdim+(j+4)+2*matrixdim*k] = M[matrixdim+j+2*matrixdim*(k+2)] = -(lf[1]*lf[1]*a + lf[1]*lf[3]*b + lf[3]*lf[3]*c);
                M[matrixdim+(j+4)+2*matrixdim*k+matrixdim*2*matrixdim] = M[matrixdim+j+2*matrixdim*(k+2)+matrixdim*2*matrixdim] = -(2*lf[0]*lf[1]*a + (lf[0]*lf[3]+lf[1]*lf[2])*b + 2*lf[2]*lf[3]*c);
                coeffindex++;
            }
            for(int k = 0; k < 2; ++k) {
                A[j+matrixdim*k] = A[(j+4)+matrixdim*(k+6)] = 0;
            }
        }
    } while(lfindex<4);

    if( bsingular ) {
        return;
    }
    dgetrs_("No transpose", &matrixdim, &matrixdim2, A, &matrixdim, &ipiv[0], &M[matrixdim], &matrixdim2, &info);
    if( info != 0 ) {
        return;
    }

    // set identity in upper corner
    for(int j = 0; j < matrixdim; ++j) {
        M[matrixdim*2*matrixdim+j+matrixdim*2*j] = 1;
    }
    IkReal IKFAST_ALIGNED16(wr[16]);
    IkReal IKFAST_ALIGNED16(wi[16]);
    IkReal IKFAST_ALIGNED16(vr[16*16]);
    int one=1;
    dgeev_("N", "V", &matrixdim2, M, &matrixdim2, wr, wi,NULL, &one, vr, &matrixdim2, work, &worksize, &info);
    if( info != 0 ) {
        return;
    }
    IkReal Breal[matrixdim-1];
    for(int i = 0; i < matrixdim2; ++i) {
        if( IKabs(wi[i]) < tol*100 ) {
            IkReal* ev = vr+matrixdim2*i;
            if( IKabs(wr[i]) > 1 ) {
                ev += matrixdim;
            }
            // consistency has to be checked!!
            if( IKabs(ev[0]) < tol ) {
                continue;
            }
            IkReal iconst = 1/ev[0];
            for(int j = 1; j < matrixdim; ++j) {
                Breal[j-1] = ev[j]*iconst;
            }
            if( checkconsistency8(Breal) ) {
                if( lfindex >= 0 ) {
                    const IkReal* lf = lfpossibilities[lfindex];
                    rawroots[numroots++] = (wr[i]*lf[0]+lf[1])/(wr[i]*lf[2]+lf[3]);
                }
                else {
                    rawroots[numroots++] = wr[i];
                }
                bool bsmall0=IKabs(ev[0]) > IKabs(ev[2]);
                bool bsmall1=IKabs(ev[0]) > IKabs(ev[1]);
                if( bsmall0 && bsmall1 ) {
                    rawroots[numroots++] = ev[2]/ev[0];
                    rawroots[numroots++] = ev[1]/ev[0];
                }
                else if( bsmall0 && !bsmall1 ) {
                    rawroots[numroots++] = ev[3]/ev[1];
                    rawroots[numroots++] = ev[1]/ev[0];
                }
                else if( !bsmall0 && bsmall1 ) {
                    rawroots[numroots++] = ev[6]/ev[4];
                    rawroots[numroots++] = ev[7]/ev[6];
                }
                else if( !bsmall0 && !bsmall1 ) {
                    rawroots[numroots++] = ev[7]/ev[5];
                    rawroots[numroots++] = ev[7]/ev[6];
                }
            }
        }
    }
}};




/// solves the inverse kinematics equations.
/// \param pfree is an array specifying the free joints of the chain.
IKFAST_API bool ComputeIk(const IkReal* eetrans, const IkReal* eerot, const IkReal* pfree, IkSolutionListBase<IkReal>& solutions);

IKFAST_API const char* GetKinematicsHash();

IKFAST_API const char* GetIkFastVersion();

#ifdef IKFAST_NAMESPACE
} // end namespace
#endif

#ifndef IKFAST_NO_MAIN
#include <stdio.h>
#include <stdlib.h>
#ifdef IKFAST_NAMESPACE
using namespace IKFAST_NAMESPACE;
#endif
/*int main(int argc, char** argv)
{
    if( argc != 12+GetNumFreeParameters()+1 ) {
        printf("\nUsage: ./ik r00 r01 r02 t0 r10 r11 r12 t1 r20 r21 r22 t2 free0 ...\n\n"
               "Returns the ik solutions given the transformation of the end effector specified by\n"
               "a 3x3 rotation R (rXX), and a 3x1 translation (tX).\n"
               "There are %d free parameters that have to be specified.\n\n",GetNumFreeParameters());
        return 1;
    }

    IkSolutionList<IkReal> solutions;
    std::vector<IkReal> vfree(GetNumFreeParameters());
    IkReal eerot[9],eetrans[3];
    eerot[0] = atof(argv[1]); eerot[1] = atof(argv[2]); eerot[2] = atof(argv[3]); eetrans[0] = atof(argv[4]);
    eerot[3] = atof(argv[5]); eerot[4] = atof(argv[6]); eerot[5] = atof(argv[7]); eetrans[1] = atof(argv[8]);
    eerot[6] = atof(argv[9]); eerot[7] = atof(argv[10]); eerot[8] = atof(argv[11]); eetrans[2] = atof(argv[12]);
    for(std::size_t i = 0; i < vfree.size(); ++i)
        vfree[i] = atof(argv[13+i]);
    bool bSuccess = ComputeIk(eetrans, eerot, vfree.size() > 0 ? &vfree[0] : NULL, solutions);

    if( !bSuccess ) {
        fprintf(stderr,"Failed to get ik solution\n");
        return -1;
    }

    printf("Found %d ik solutions:\n", (int)solutions.GetNumSolutions());
    std::vector<IkReal> solvalues(GetNumJoints());
    for(std::size_t i = 0; i < solutions.GetNumSolutions(); ++i) {
        const IkSolutionBase<IkReal>& sol = solutions.GetSolution(i);
        printf("sol%d (free=%d): ", (int)i, (int)sol.GetFree().size());
        std::vector<IkReal> vsolfree(sol.GetFree().size());
        sol.GetSolution(&solvalues[0],vsolfree.size()>0?&vsolfree[0]:NULL);
        for( std::size_t j = 0; j < solvalues.size(); ++j)
            printf("%.15f, ", solvalues[j]);
        printf("\n");
    }
    return 0;
}*/

#endif
