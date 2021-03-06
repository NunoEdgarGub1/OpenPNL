/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                        Intel License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of Intel Corporation may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#include "_cxcore.h"

/****************************************************************************************\
*                             Find sum of pixels in the ROI                              *
\****************************************************************************************/


#define ICX_DEF_SUM_1D_CASE_COI( __op__, src, len, sum, cn )                \
{                                                                           \
    int i;                                                                  \
                                                                            \
    for( i = 0; i <= (len) - 4*(cn); i += 4*(cn) )                          \
        (sum)[0] += __op__((src)[i]) + __op__((src)[i+(cn)]) +              \
                    __op__((src)[i+(cn)*2]) + __op__((src)[i+(cn)*3]);      \
                                                                            \
    for( ; i < (len); i += (cn) )                                           \
        (sum)[0] += __op__((src)[i]);                                       \
}                                                                           \


#define ICX_DEF_SUM_1D_CASE_C1( __op__, src, len, sum ) \
    ICX_DEF_SUM_1D_CASE_COI( __op__, src, len, sum, 1 )


#define ICX_DEF_SUM_1D_CASE_C2( __op__, src, len, sum )                     \
{                                                                           \
    int i;                                                                  \
                                                                            \
    for( i = 0; i <= (len) - 8; i += 8 )                                    \
    {                                                                       \
        (sum)[0] += __op__((src)[i]) + __op__((src)[i+2]) +                 \
                    __op__((src)[i+4]) + __op__((src)[i+6]);                \
        (sum)[1] += __op__((src)[i+1]) + __op__((src)[i+3]) +               \
                    __op__((src)[i+5]) + __op__((src)[i+7]);                \
    }                                                                       \
                                                                            \
    for( ; i < (len); i += 2 )                                              \
    {                                                                       \
        (sum)[0] += __op__((src)[i]);                                       \
        (sum)[1] += __op__((src)[i+1]);                                     \
    }                                                                       \
}                                                                           \


#define ICX_DEF_SUM_1D_CASE_C3( __op__, src, len, sum )                     \
{                                                                           \
    int i;                                                                  \
                                                                            \
    for( i = 0; i <= (len) - 12; i += 12 )                                  \
    {                                                                       \
        (sum)[0] += __op__((src)[i]) + __op__((src)[i+3]) +                 \
                    __op__((src)[i+6]) + __op__((src)[i+9]);                \
        (sum)[1] += __op__((src)[i+1]) + __op__((src)[i+4]) +               \
                    __op__((src)[i+7]) + __op__((src)[i+10]);               \
        (sum)[2] += __op__((src)[i+2]) + __op__((src)[i+5]) +               \
                    __op__((src)[i+8]) + __op__((src)[i+11]);               \
    }                                                                       \
                                                                            \
    for( ; i < (len); i += 3 )                                              \
    {                                                                       \
        (sum)[0] += __op__((src)[i]);                                       \
        (sum)[1] += __op__((src)[i+1]);                                     \
        (sum)[2] += __op__((src)[i+2]);                                     \
    }                                                                       \
}


#define ICX_DEF_SUM_1D_CASE_C4( __op__, src, len, sum )                     \
{                                                                           \
    int i;                                                                  \
                                                                            \
    for( i = 0; i <= (len) - 16; i += 16 )                                  \
    {                                                                       \
        (sum)[0] += __op__((src)[i]) + __op__((src)[i+4]) +                 \
                    __op__((src)[i+8]) + __op__((src)[i+12]);               \
        (sum)[1] += __op__((src)[i+1]) + __op__((src)[i+5]) +               \
                    __op__((src)[i+9]) + __op__((src)[i+13]);               \
        (sum)[2] += __op__((src)[i+2]) + __op__((src)[i+6]) +               \
                    __op__((src)[i+10]) + __op__((src)[i+14]);              \
        (sum)[3] += __op__((src)[i+3]) + __op__((src)[i+7]) +               \
                    __op__((src)[i+11]) + __op__((src)[i+15]);              \
    }                                                                       \
                                                                            \
    for( ; i < (len); i += 4 )                                              \
    {                                                                       \
        (sum)[0] += __op__((src)[i]);                                       \
        (sum)[1] += __op__((src)[i+1]);                                     \
        (sum)[2] += __op__((src)[i+2]);                                     \
        (sum)[3] += __op__((src)[i+3]);                                     \
    }                                                                       \
}


#define CX_SUM_ENTRY_C1( sumtype ) \
    sumtype temp[1] = { 0 };

#define CX_SUM_ENTRY_C2( sumtype ) \
    sumtype temp[2] = { 0, 0 };

#define CX_SUM_ENTRY_C3( sumtype ) \
    sumtype temp[3] = { 0, 0, 0 };

#define CX_SUM_ENTRY_C4( sumtype ) \
    sumtype temp[4] = { 0, 0, 0, 0 };

#define CX_SUM_EXIT_C1(sumtype_final) \
    sum[0] = (sumtype_final)temp[0]

#define CX_SUM_EXIT_C2(sumtype_final) \
    CX_SUM_EXIT_C1(sumtype_final), sum[1] = (sumtype_final)temp[1]

#define CX_SUM_EXIT_C3(sumtype_final) \
    CX_SUM_EXIT_C2(sumtype_final), sum[2] = (sumtype_final)temp[2]

#define CX_SUM_EXIT_C4(sumtype_final) \
    CX_SUM_EXIT_C3(sumtype_final), sum[3] = (sumtype_final)temp[3]


#define ICX_DEF_SUM_NOHINT_FUNC_2D( __op__, _entry_, _exit_, name, flavor,  \
                             cn, srctype, sumtype, sumtype_final )          \
IPCXAPI_IMPL(CxStatus, icx##name##_##flavor##_C##cn##R,( const srctype* src,\
                                    int step, CxSize size, sumtype_final* sum )) \
{                                                                           \
    _entry_( sumtype );                                                     \
    size.width *= cn;                                                       \
                                                                            \
    for( int y = 0; y < size.height; y++, (char*&)src += step )             \
    {                                                                       \
        ICX_DEF_SUM_1D_CASE_C##cn( __op__, src, size.width, temp );         \
    }                                                                       \
                                                                            \
    _exit_(sumtype_final);                                                  \
                                                                            \
    return CX_OK;                                                           \
}


#define ICX_DEF_SUM_HINT_FUNC_2D( __op__, _entry_, _exit_, name, flavor,    \
                             cn, srctype, sumtype, sumtype_final )          \
IPCXAPI_IMPL(CxStatus, icx##name##_##flavor##_C##cn##R,( const srctype* src,\
                                    int step, CxSize size, sumtype_final* sum,\
                                    CxHintAlgorithm /* hint */ ))           \
{                                                                           \
    _entry_( sumtype );                                                     \
    size.width *= cn;                                                       \
                                                                            \
    for( int y = 0; y < size.height; y++, (char*&)src += step )             \
    {                                                                       \
        ICX_DEF_SUM_1D_CASE_C##cn( __op__, src, size.width, temp );         \
    }                                                                       \
                                                                            \
    _exit_(sumtype_final);                                                  \
                                                                            \
    return CX_OK;                                                           \
}


#define ICX_DEF_SUM_0D_CASE_C1( __op__, src, sum )                          \
{                                                                           \
    (sum)[0] += __op__((src)[0]);                                           \
}                                                                           \


#define ICX_DEF_SUM_0D_CASE_C2( __op__, src, sum )                          \
{                                                                           \
    (sum)[0] += __op__((src)[0]);                                           \
    (sum)[1] += __op__((src)[1]);                                           \
}                                                                           \


#define ICX_DEF_SUM_FUNC_2D_COI( __op__, name, flavor, srctype,             \
                                 sumtype, sumtype_final )                   \
static CxStatus CX_STDCALL icx##name##_##flavor##_CnCR( const srctype* src, \
                int step, CxSize size, int cn, int coi, sumtype_final* sum )\
{                                                                           \
    CX_SUM_ENTRY_C1( sumtype );                                             \
                                                                            \
    size.width *= cn;                                                       \
    src += coi - 1;                                                         \
                                                                            \
    for( int y = 0; y < size.height; y++, (char*&)src += step )             \
    {                                                                       \
        ICX_DEF_SUM_1D_CASE_COI( __op__, src, size.width, temp, cn );       \
    }                                                                       \
                                                                            \
    CX_SUM_EXIT_C1(sumtype_final);                                          \
                                                                            \
    return CX_OK;                                                           \
}


#define ICX_DEF_SUM_ALL( __op__, name, flavor, srctype, sumtype, sumtype_final, hint )\
    ICX_DEF_SUM_##hint##_FUNC_2D( __op__, CX_SUM_ENTRY_C1, CX_SUM_EXIT_C1,      \
                         name, flavor, 1, srctype, sumtype, sumtype_final )     \
    ICX_DEF_SUM_##hint##_FUNC_2D( __op__, CX_SUM_ENTRY_C2, CX_SUM_EXIT_C2,      \
                         name, flavor, 2, srctype, sumtype, sumtype_final )     \
    ICX_DEF_SUM_##hint##_FUNC_2D( __op__, CX_SUM_ENTRY_C3, CX_SUM_EXIT_C3,      \
                         name, flavor, 3, srctype, sumtype, sumtype_final )     \
    ICX_DEF_SUM_##hint##_FUNC_2D( __op__, CX_SUM_ENTRY_C4, CX_SUM_EXIT_C4,      \
                         name, flavor, 4, srctype, sumtype, sumtype_final )     \
    ICX_DEF_SUM_FUNC_2D_COI( __op__, name, flavor, srctype, sumtype, sumtype_final )


ICX_DEF_SUM_ALL( CX_NOP, Sum, 8u, uchar, int64, double, NOHINT )
ICX_DEF_SUM_ALL( CX_NOP, Sum, 16s, short, int64, double, NOHINT )
ICX_DEF_SUM_ALL( CX_CAST_64S, Sum, 32s, int, int64, double, NOHINT )
ICX_DEF_SUM_ALL( CX_NOP, Sum, 32f, float, double, double, HINT )
ICX_DEF_SUM_ALL( CX_NOP, Sum, 64f, double, double, double, NOHINT )

#define icxSum_8s_C1R   0
#define icxSum_8s_C2R   0
#define icxSum_8s_C3R   0
#define icxSum_8s_C4R   0
#define icxSum_8s_CnCR  0

CX_DEF_INIT_BIG_FUNC_TAB_2D( Sum, R )
CX_DEF_INIT_FUNC_TAB_2D( Sum, CnCR )

CX_IMPL CxScalar
cxSum( const CxArr* arr )
{
    static CxBigFuncTable sum_tab;
    static CxFuncTable sumcoi_tab;
    static int inittab = 0;

    CxScalar sum = {{ 0, 0, 0, 0 }};

    CX_FUNCNAME("cxSum");

    __BEGIN__;

    int type, coi = 0;
    int mat_step;
    CxSize size;
    CxMat stub, *mat = (CxMat*)arr;

    if( !inittab )
    {
        icxInitSumRTable( &sum_tab );
        icxInitSumCnCRTable( &sumcoi_tab );
        inittab = 1;
    }

    if( !CX_IS_MAT(mat) )
    {
        if( CX_IS_MATND(mat) )
        {
            CxMatND stub;
            CxNArrayIterator iterator;
            int pass_hint;

            CX_CALL( cxInitNArrayIterator( 1, (void**)&mat, 0, &stub, &iterator ));

            type = CX_MAT_TYPE(iterator.hdr[0]->type);

            pass_hint = CX_MAT_DEPTH(type) == CX_32F;

            if( !pass_hint )
            {
                CxFunc2D_1A1P func = (CxFunc2D_1A1P)(sum_tab.fn_2d[type]);
                if( !func )
                    CX_ERROR( CX_StsUnsupportedFormat, "" );
       
                do
                {
                    CxScalar temp = {{ 0, 0, 0, 0 }};
                    IPPI_CALL( func( iterator.ptr[0], CX_STUB_STEP,
                                     iterator.size, temp.val ));
                    sum.val[0] += temp.val[0];
                    sum.val[1] += temp.val[1];
                    sum.val[2] += temp.val[2];
                    sum.val[3] += temp.val[3];
                }
                while( cxNextNArraySlice( &iterator ));
            }
            else
            {
                CxFunc2D_1A1P1I func = (CxFunc2D_1A1P1I)(sum_tab.fn_2d[type]);
                if( !func )
                    CX_ERROR( CX_StsUnsupportedFormat, "" );
       
                do
                {
                    CxScalar temp = {{ 0, 0, 0, 0 }};
                    IPPI_CALL( func( iterator.ptr[0], CX_STUB_STEP,
                                     iterator.size, temp.val, cxAlgHintAccurate ));
                    sum.val[0] += temp.val[0];
                    sum.val[1] += temp.val[1];
                    sum.val[2] += temp.val[2];
                    sum.val[3] += temp.val[3];
                }
                while( cxNextNArraySlice( &iterator ));
            }
            EXIT;
        }
        else
            CX_CALL( mat = cxGetMat( mat, &stub, &coi ));
    }

    type = CX_MAT_TYPE(mat->type);
    size = cxGetMatSize( mat );

    mat_step = mat->step;

    if( CX_IS_MAT_CONT( mat->type ))
    {
        size.width *= size.height;
        
        if( size.width <= CX_MAX_INLINE_MAT_OP_SIZE )
        {
            if( type == CX_32FC1 )
            {
                float* data = mat->data.fl;

                do
                {
                    sum.val[0] += data[size.width - 1];
                }
                while( --size.width );

                EXIT;
            }

            if( type == CX_64FC1 )
            {
                double* data = mat->data.db;

                do
                {
                    sum.val[0] += data[size.width - 1];
                }
                while( --size.width );

                EXIT;
            }
        }
        size.height = 1;
        mat_step = CX_STUB_STEP;
    }

    if( CX_MAT_CN(type) == 1 || coi == 0 )
    {
        int pass_hint = CX_MAT_DEPTH(type) == CX_32F;
        if( !pass_hint )
        {
            CxFunc2D_1A1P func = (CxFunc2D_1A1P)(sum_tab.fn_2d[type]);

            if( !func )
                CX_ERROR( CX_StsBadArg, cxUnsupportedFormat );

            IPPI_CALL( func( mat->data.ptr, mat_step, size, sum.val ));
        }
        else
        {
            CxFunc2D_1A1P1I func = (CxFunc2D_1A1P1I)(sum_tab.fn_2d[type]);

            if( !func )
                CX_ERROR( CX_StsBadArg, cxUnsupportedFormat );

            IPPI_CALL( func( mat->data.ptr, mat_step, size, sum.val, cxAlgHintAccurate ));
        }
    }
    else
    {
        CxFunc2DnC_1A1P func = (CxFunc2DnC_1A1P)(sumcoi_tab.fn_2d[CX_MAT_DEPTH(type)]);

        if( !func )
            CX_ERROR( CX_StsBadArg, cxUnsupportedFormat );

        IPPI_CALL( func( mat->data.ptr, mat_step, size,
                         CX_MAT_CN(type), coi, sum.val ));
    }

    __END__;

    return  sum;
}


#define ICX_DEF_SUM_C1( __op__, name, flavor, srctype, sumtype )        \
    ICX_DEF_SUM_NOHINT_FUNC_2D( __op__, CX_SUM_ENTRY_C1, CX_SUM_EXIT_C1,\
                         name, flavor, 1, srctype, sumtype, int )       \
    ICX_DEF_SUM_FUNC_2D_COI( __op__, name, flavor, srctype, sumtype, int )

ICX_DEF_SUM_C1( CX_NONZERO, CountNonZero, 8u, uchar, int )
ICX_DEF_SUM_C1( CX_NONZERO, CountNonZero, 16s, ushort, int )
ICX_DEF_SUM_C1( CX_NONZERO, CountNonZero, 32s, int, int )
ICX_DEF_SUM_C1( CX_NONZERO_FLT, CountNonZero, 32f, int, int )
ICX_DEF_SUM_C1( CX_NONZERO_FLT, CountNonZero, 64f, int64, int )

#define icxCountNonZero_8s_C1R icxCountNonZero_8u_C1R
#define icxCountNonZero_8s_CnCR icxCountNonZero_8u_CnCR

CX_DEF_INIT_FUNC_TAB_2D( CountNonZero, C1R )
CX_DEF_INIT_FUNC_TAB_2D( CountNonZero, CnCR )


CX_IMPL int
cxCountNonZero( const CxArr* img )
{
    static CxFuncTable nz_tab;
    static CxFuncTable nzcoi_tab;
    static int inittab = 0;

    int count = 0;

    CX_FUNCNAME("cxCountNonZero");

    __BEGIN__;

    int type, coi = 0;
    int mat_step;
    CxSize size;
    CxMat stub, *mat = (CxMat*)img;

    if( !inittab )
    {
        icxInitCountNonZeroC1RTable( &nz_tab );
        icxInitCountNonZeroCnCRTable( &nzcoi_tab );
        inittab = 1;
    }

    if( !CX_IS_MAT(mat) )
    {
        if( CX_IS_MATND(mat) )
        {
            CxMatND stub;
            CxNArrayIterator iterator;
            CxFunc2D_1A1P func;

            CX_CALL( cxInitNArrayIterator( 1, (void**)&mat, 0, &stub, &iterator ));

            type = CX_MAT_TYPE(iterator.hdr[0]->type);

            if( CX_MAT_CN(type) != 1 )
                CX_ERROR( CX_BadNumChannels,
                    "Only single-channel array are supported here" );

            func = (CxFunc2D_1A1P)(nz_tab.fn_2d[CX_MAT_DEPTH(type)]);
            if( !func )
                CX_ERROR( CX_StsUnsupportedFormat, "" );
       
            do
            {
                int temp;
                IPPI_CALL( func( iterator.ptr[0], CX_STUB_STEP,
                                 iterator.size, &temp ));
                count += temp;
            }
            while( cxNextNArraySlice( &iterator ));
            EXIT;
        }
        else
            CX_CALL( mat = cxGetMat( mat, &stub, &coi ));
    }

    type = CX_MAT_TYPE(mat->type);
    size = cxGetMatSize( mat );

    mat_step = mat->step;

    if( CX_IS_MAT_CONT( mat->type ))
    {
        size.width *= size.height;
        size.height = 1;
        mat_step = CX_STUB_STEP;
    }

    if( CX_MAT_CN(type) == 1 || coi == 0 )
    {
        CxFunc2D_1A1P func = (CxFunc2D_1A1P)(nz_tab.fn_2d[CX_MAT_DEPTH(type)]);

        if( CX_MAT_CN(type) != 1 )
            CX_ERROR( CX_BadNumChannels,
            "The function can handle only a single channel at a time (use COI)");

        if( !func )
            CX_ERROR( CX_StsBadArg, cxUnsupportedFormat );

        IPPI_CALL( func( mat->data.ptr, mat_step, size, &count ));
    }
    else
    {
        CxFunc2DnC_1A1P func = (CxFunc2DnC_1A1P)(nzcoi_tab.fn_2d[CX_MAT_DEPTH(type)]);

        if( !func )
            CX_ERROR( CX_StsBadArg, cxUnsupportedFormat );

        IPPI_CALL( func( mat->data.ptr, mat_step, size, CX_MAT_CN(type), coi, &count ));
    }

    __END__;

    return  count;
}

/* End of file. */
