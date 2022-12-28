/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */
/*
 *
 *  (C) 2015 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

/*  Extended version of ANL's acc-pairtype test by ParTec. */

/*  Portions of this code were written/modified by ParTec AG
 *  Copyright (C) 2017-2021 ParTec Cluster Competence Center GmbH, Munich
 *  Copyright (C) 2021-2023 ParTec AG, Munich
 */

#include "mpi.h"
#include <stdio.h>
#include <assert.h>
#include "mpitest.h"

#define DATA_SIZE 25

typedef struct double_int {
    double a;
    int b;
} double_int_t;

int main(int argc, char *argv[])
{
    MPI_Win win;
    int errors = 0;
    int wrank, nproc, i;
    double_int_t *orig_buf;
    double_int_t *tar_buf;
    double_int_t *res_buf;
    MPI_Datatype vector_dtp;
    int disp;
    int qdisp;
    MPI_Aint qsize;
    void *ptr = NULL;
    void *qptr = NULL;
    MPI_Comm comm_shmem;
    int comm_shmem_rank;
    int comm_shmem_size;

    MTest_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &wrank);

    if (nproc != 2) {
	printf("This program needs exactly np = 2 processes! Calling MPI_Abort()...\n");
	MPI_Abort(MPI_COMM_WORLD, 1);
    }

    MPI_Comm_split_type(MPI_COMM_WORLD, MPI_COMM_TYPE_SHARED, 0, MPI_INFO_NULL, &comm_shmem);

    MPI_Comm_size(comm_shmem, &comm_shmem_size);
    MPI_Comm_rank(comm_shmem, &comm_shmem_rank);

    if(comm_shmem_size == 1) {
        goto exit;
    }
    
    assert(sizeof(MPI_Aint) == sizeof(long));
    assert(sizeof(size_t) == sizeof(unsigned long));

    MPI_Alloc_mem(sizeof(double_int_t) * DATA_SIZE, MPI_INFO_NULL, &orig_buf);
    MPI_Alloc_mem(sizeof(double_int_t) * DATA_SIZE, MPI_INFO_NULL, &res_buf);

    disp = sizeof(double_int_t);

    if(comm_shmem_rank == 1) {
        MPI_Win_allocate_shared(sizeof(double_int_t) * DATA_SIZE, disp, MPI_INFO_NULL, comm_shmem, &ptr, &win);
        assert(ptr != NULL);
        MPI_Win_shared_query(win, 1, &qsize, &qdisp, &qptr);
        assert(qptr == ptr);
        tar_buf = qptr;
    } else {
        MPI_Win_allocate_shared(0, disp, MPI_INFO_NULL, comm_shmem, &ptr, &win);
        MPI_Win_shared_query(win, 1, &qsize, &qdisp, &qptr);
        assert(qptr != NULL);
        tar_buf = qptr;
    }

    assert(qsize == sizeof(double_int_t) * DATA_SIZE);
    assert(qdisp == disp);

    for (i = 0; i < DATA_SIZE; i++) {
        orig_buf[i].a = 1.0;
        orig_buf[i].b = 1;
        tar_buf[i].a = 0.5;
        tar_buf[i].b = 0;
        res_buf[i].a = 0.0;
        res_buf[i].b = -1;
    }

    MPI_Type_vector(5 /* count */ , 3 /* blocklength */ , 5 /* stride */ , MPI_DOUBLE_INT, &vector_dtp);
    MPI_Type_commit(&vector_dtp);

    if (comm_shmem_rank == 0) {
        MPI_Win_lock(MPI_LOCK_SHARED, 1, 0, win);
        MPI_Get_accumulate(orig_buf, 1, vector_dtp, res_buf, 1, vector_dtp, 1, 0, 1, vector_dtp, MPI_MAXLOC, win);
        MPI_Win_unlock(1, win);
    }

    MPI_Win_fence(0, win);

    if (comm_shmem_rank == 1) {
        for (i = 0; i < DATA_SIZE; i++) {
            if (i % 5 < 3) {
                if (tar_buf[i].a != 1.0 || tar_buf[i].b != 1) {
                    printf("tar_buf[i].a = %f (expected 1.0) | tar_buf[i].b = %d (expected 1)\n", tar_buf[i].a, tar_buf[i].b);
                    errors++;
                }
            }
            else {
                if (tar_buf[i].a != 0.5 || tar_buf[i].b != 0) {
                    printf("tar_buf[i].a = %f (expected 0.5) | tar_buf[i].b = %d (expected 0)\n", tar_buf[i].a, tar_buf[i].b);
                    errors++;
                }
            }
        }
    }

    if (comm_shmem_rank == 0) {
        for (i = 0; i < DATA_SIZE; i++) {
            if (i % 5 < 3) {
                if (res_buf[i].a != 0.5 || res_buf[i].b != 0) {
                    printf("res_buf[i].a = %f (expected 0.5) | res_buf[i].b = %d (expected 0)\n", res_buf[i].a, res_buf[i].b);
                    errors++;
                }
            }
            else {
                if (res_buf[i].a != 0.0 || res_buf[i].b != -1) {
                    printf("res_buf[i].a = %f (expected 0.0) | res_buf[i].b = %d (expected -1)\n", res_buf[i].a, res_buf[i].b);
                    errors++;
                }
            }
        }
    }


    MPI_Type_free(&vector_dtp);

    MPI_Barrier(MPI_COMM_WORLD);

    for (i = 0; i < DATA_SIZE; i++) {
        orig_buf[i].a = 0.0;
        orig_buf[i].b = 2;
        res_buf[i].a = 0.0;
        res_buf[i].b = -1;
    }

    MPI_Win_fence(0, win);

    if (comm_shmem_rank == 0) {
        MPI_Win_lock(MPI_LOCK_SHARED, 1, 0, win);
        MPI_Get_accumulate(orig_buf, DATA_SIZE, MPI_DOUBLE_INT, res_buf, DATA_SIZE, MPI_DOUBLE_INT, 1, 0, DATA_SIZE, MPI_DOUBLE_INT, MPI_MINLOC, win);
        MPI_Win_unlock(1, win);
    }

    MPI_Win_fence(0, win);

    if (comm_shmem_rank == 1) {
        for (i = 0; i < DATA_SIZE; i++) {
            if (tar_buf[i].a != 0.0 || tar_buf[i].b != 2) {
                printf("res_buf[i].a = %f (expected 0.0) | tar_buf[i].b = %d (expected 2)\n", tar_buf[i].a, tar_buf[i].b);
                errors++;
            }
        }
    }

    if (comm_shmem_rank == 0) {
        for (i = 0; i < DATA_SIZE; i++) {
            if (i % 5 < 3) {
                if (res_buf[i].a != 1.0 || res_buf[i].b != 1) {
                    printf("res_buf[i].a = %f (expected 1.0) | res_buf[i].b = %d (expected 1)\n", res_buf[i].a, res_buf[i].b);
                    errors++;
                }
            }
            else {
                if (res_buf[i].a != 0.5 || res_buf[i].b != 0) {
                    printf("res_buf[i].a = %f (expected 0.5) | res_buf[i].b = %d (expected 0)\n", res_buf[i].a, res_buf[i].b);
                    errors++;
                }
            }
        }
    }

    MPI_Win_free(&win);

    MPI_Free_mem(orig_buf);
    MPI_Free_mem(res_buf);

exit:
    MPI_Comm_free(&comm_shmem);

    MTest_Finalize(errors);

    return MTestReturnValue(errors);
}
