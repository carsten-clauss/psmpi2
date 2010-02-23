/* -*- Mode: C; c-basic-offset:4 ; -*- */
/*  
 *  (C) 2001 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 *
 * This file is automatically generated by buildiface 
 * DO NOT EDIT
 */
#include "mpi_fortimpl.h"


/* Begin MPI profiling block */
#if defined(USE_WEAK_SYMBOLS) && !defined(USE_ONLY_MPI_NAMES) 
#if defined(HAVE_MULTIPLE_PRAGMA_WEAK)
extern FORT_DLL_SPEC void FORT_CALL MPI_ADD_ERROR_CODE( MPI_Fint *, MPI_Fint *, MPI_Fint * );
extern FORT_DLL_SPEC void FORT_CALL mpi_add_error_code__( MPI_Fint *, MPI_Fint *, MPI_Fint * );
extern FORT_DLL_SPEC void FORT_CALL mpi_add_error_code( MPI_Fint *, MPI_Fint *, MPI_Fint * );
extern FORT_DLL_SPEC void FORT_CALL mpi_add_error_code_( MPI_Fint *, MPI_Fint *, MPI_Fint * );

#if defined(F77_NAME_UPPER)
#pragma weak MPI_ADD_ERROR_CODE = PMPI_ADD_ERROR_CODE
#pragma weak mpi_add_error_code__ = PMPI_ADD_ERROR_CODE
#pragma weak mpi_add_error_code_ = PMPI_ADD_ERROR_CODE
#pragma weak mpi_add_error_code = PMPI_ADD_ERROR_CODE
#elif defined(F77_NAME_LOWER_2USCORE)
#pragma weak MPI_ADD_ERROR_CODE = pmpi_add_error_code__
#pragma weak mpi_add_error_code__ = pmpi_add_error_code__
#pragma weak mpi_add_error_code_ = pmpi_add_error_code__
#pragma weak mpi_add_error_code = pmpi_add_error_code__
#elif defined(F77_NAME_LOWER_USCORE)
#pragma weak MPI_ADD_ERROR_CODE = pmpi_add_error_code_
#pragma weak mpi_add_error_code__ = pmpi_add_error_code_
#pragma weak mpi_add_error_code_ = pmpi_add_error_code_
#pragma weak mpi_add_error_code = pmpi_add_error_code_
#else
#pragma weak MPI_ADD_ERROR_CODE = pmpi_add_error_code
#pragma weak mpi_add_error_code__ = pmpi_add_error_code
#pragma weak mpi_add_error_code_ = pmpi_add_error_code
#pragma weak mpi_add_error_code = pmpi_add_error_code
#endif



#elif defined(HAVE_PRAGMA_WEAK)

#if defined(F77_NAME_UPPER)
extern FORT_DLL_SPEC void FORT_CALL MPI_ADD_ERROR_CODE( MPI_Fint *, MPI_Fint *, MPI_Fint * );

#pragma weak MPI_ADD_ERROR_CODE = PMPI_ADD_ERROR_CODE
#elif defined(F77_NAME_LOWER_2USCORE)
extern FORT_DLL_SPEC void FORT_CALL mpi_add_error_code__( MPI_Fint *, MPI_Fint *, MPI_Fint * );

#pragma weak mpi_add_error_code__ = pmpi_add_error_code__
#elif !defined(F77_NAME_LOWER_USCORE)
extern FORT_DLL_SPEC void FORT_CALL mpi_add_error_code( MPI_Fint *, MPI_Fint *, MPI_Fint * );

#pragma weak mpi_add_error_code = pmpi_add_error_code
#else
extern FORT_DLL_SPEC void FORT_CALL mpi_add_error_code_( MPI_Fint *, MPI_Fint *, MPI_Fint * );

#pragma weak mpi_add_error_code_ = pmpi_add_error_code_
#endif

#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#if defined(F77_NAME_UPPER)
#pragma _HP_SECONDARY_DEF PMPI_ADD_ERROR_CODE  MPI_ADD_ERROR_CODE
#elif defined(F77_NAME_LOWER_2USCORE)
#pragma _HP_SECONDARY_DEF pmpi_add_error_code__  mpi_add_error_code__
#elif !defined(F77_NAME_LOWER_USCORE)
#pragma _HP_SECONDARY_DEF pmpi_add_error_code  mpi_add_error_code
#else
#pragma _HP_SECONDARY_DEF pmpi_add_error_code_  mpi_add_error_code_
#endif

#elif defined(HAVE_PRAGMA_CRI_DUP)
#if defined(F77_NAME_UPPER)
#pragma _CRI duplicate MPI_ADD_ERROR_CODE as PMPI_ADD_ERROR_CODE
#elif defined(F77_NAME_LOWER_2USCORE)
#pragma _CRI duplicate mpi_add_error_code__ as pmpi_add_error_code__
#elif !defined(F77_NAME_LOWER_USCORE)
#pragma _CRI duplicate mpi_add_error_code as pmpi_add_error_code
#else
#pragma _CRI duplicate mpi_add_error_code_ as pmpi_add_error_code_
#endif
#endif /* HAVE_PRAGMA_WEAK */
#endif /* USE_WEAK_SYMBOLS */
/* End MPI profiling block */


/* These definitions are used only for generating the Fortran wrappers */
#if defined(USE_WEAK_SYMBOLS) && defined(HAVE_MULTIPLE_PRAGMA_WEAK) && \
    defined(USE_ONLY_MPI_NAMES)
extern FORT_DLL_SPEC void FORT_CALL MPI_ADD_ERROR_CODE( MPI_Fint *, MPI_Fint *, MPI_Fint * );
extern FORT_DLL_SPEC void FORT_CALL mpi_add_error_code__( MPI_Fint *, MPI_Fint *, MPI_Fint * );
extern FORT_DLL_SPEC void FORT_CALL mpi_add_error_code( MPI_Fint *, MPI_Fint *, MPI_Fint * );
extern FORT_DLL_SPEC void FORT_CALL mpi_add_error_code_( MPI_Fint *, MPI_Fint *, MPI_Fint * );

#if defined(F77_NAME_UPPER)
#pragma weak mpi_add_error_code__ = MPI_ADD_ERROR_CODE
#pragma weak mpi_add_error_code_ = MPI_ADD_ERROR_CODE
#pragma weak mpi_add_error_code = MPI_ADD_ERROR_CODE
#elif defined(F77_NAME_LOWER_2USCORE)
#pragma weak MPI_ADD_ERROR_CODE = mpi_add_error_code__
#pragma weak mpi_add_error_code_ = mpi_add_error_code__
#pragma weak mpi_add_error_code = mpi_add_error_code__
#elif defined(F77_NAME_LOWER_USCORE)
#pragma weak MPI_ADD_ERROR_CODE = mpi_add_error_code_
#pragma weak mpi_add_error_code__ = mpi_add_error_code_
#pragma weak mpi_add_error_code = mpi_add_error_code_
#else
#pragma weak MPI_ADD_ERROR_CODE = mpi_add_error_code
#pragma weak mpi_add_error_code__ = mpi_add_error_code
#pragma weak mpi_add_error_code_ = mpi_add_error_code
#endif

#endif

/* Map the name to the correct form */
#ifndef MPICH_MPI_FROM_PMPI
#if defined(USE_WEAK_SYMBOLS) && defined(HAVE_MULTIPLE_PRAGMA_WEAK)
/* Define the weak versions of the PMPI routine*/
#ifndef F77_NAME_UPPER
extern FORT_DLL_SPEC void FORT_CALL PMPI_ADD_ERROR_CODE( MPI_Fint *, MPI_Fint *, MPI_Fint * );
#endif
#ifndef F77_NAME_LOWER_2USCORE
extern FORT_DLL_SPEC void FORT_CALL pmpi_add_error_code__( MPI_Fint *, MPI_Fint *, MPI_Fint * );
#endif
#ifndef F77_NAME_LOWER_USCORE
extern FORT_DLL_SPEC void FORT_CALL pmpi_add_error_code_( MPI_Fint *, MPI_Fint *, MPI_Fint * );
#endif
#ifndef F77_NAME_LOWER
extern FORT_DLL_SPEC void FORT_CALL pmpi_add_error_code( MPI_Fint *, MPI_Fint *, MPI_Fint * );

#endif

#if defined(F77_NAME_UPPER)
#pragma weak pmpi_add_error_code__ = PMPI_ADD_ERROR_CODE
#pragma weak pmpi_add_error_code_ = PMPI_ADD_ERROR_CODE
#pragma weak pmpi_add_error_code = PMPI_ADD_ERROR_CODE
#elif defined(F77_NAME_LOWER_2USCORE)
#pragma weak PMPI_ADD_ERROR_CODE = pmpi_add_error_code__
#pragma weak pmpi_add_error_code_ = pmpi_add_error_code__
#pragma weak pmpi_add_error_code = pmpi_add_error_code__
#elif defined(F77_NAME_LOWER_USCORE)
#pragma weak PMPI_ADD_ERROR_CODE = pmpi_add_error_code_
#pragma weak pmpi_add_error_code__ = pmpi_add_error_code_
#pragma weak pmpi_add_error_code = pmpi_add_error_code_
#else
#pragma weak PMPI_ADD_ERROR_CODE = pmpi_add_error_code
#pragma weak pmpi_add_error_code__ = pmpi_add_error_code
#pragma weak pmpi_add_error_code_ = pmpi_add_error_code
#endif /* Test on name mapping */
#endif /* Use multiple pragma weak */

#ifdef F77_NAME_UPPER
#define mpi_add_error_code_ PMPI_ADD_ERROR_CODE
#elif defined(F77_NAME_LOWER_2USCORE)
#define mpi_add_error_code_ pmpi_add_error_code__
#elif !defined(F77_NAME_LOWER_USCORE)
#define mpi_add_error_code_ pmpi_add_error_code
#else
#define mpi_add_error_code_ pmpi_add_error_code_
#endif /* Test on name mapping */

/* This defines the routine that we call, which must be the PMPI version
   since we're renaming the Fortran entry as the pmpi version.  The MPI name
   must be undefined first to prevent any conflicts with previous renamings,
   such as those put in place by the globus device when it is building on
   top of a vendor MPI. */
#undef MPI_Add_error_code
#define MPI_Add_error_code PMPI_Add_error_code 

#else

#ifdef F77_NAME_UPPER
#define mpi_add_error_code_ MPI_ADD_ERROR_CODE
#elif defined(F77_NAME_LOWER_2USCORE)
#define mpi_add_error_code_ mpi_add_error_code__
#elif !defined(F77_NAME_LOWER_USCORE)
#define mpi_add_error_code_ mpi_add_error_code
/* Else leave name alone */
#endif


#endif /* MPICH_MPI_FROM_PMPI */

/* Prototypes for the Fortran interfaces */
#include "fproto.h"
FORT_DLL_SPEC void FORT_CALL mpi_add_error_code_ ( MPI_Fint *v1, MPI_Fint *v2, MPI_Fint *ierr ){
    *ierr = MPI_Add_error_code( *v1, v2 );
}
