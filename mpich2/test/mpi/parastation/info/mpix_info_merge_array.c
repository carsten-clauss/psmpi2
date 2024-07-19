/*
 * ParaStation
 *
 * Copyright (C) 2020-2021 ParTec Cluster Competence Center GmbH, Munich
 * Copyright (C) 2021-2024 ParTec AG, Munich
 *
 * This file may be distributed under the terms of the Q Public License
 * as defined in the file LICENSE.QPL included in the packaging of this
 * file.
 */

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include "mpitest.h"
#include "mpitestconf.h"
#ifdef HAVE_STRING_H
#include <string.h>
#endif

int main(int argc, char *argv[])
{
    int errs = 0;
    MPI_Info info_array[4], info_merge;
    char key[MPI_MAX_INFO_KEY];
    char value[MPI_MAX_INFO_VAL];
    int nkeys;
    int nkeys_expected;
    int count_array_entries;
    int array_entries_expected;

    MTest_Init(&argc, &argv);

    MPI_Info_create(&info_array[0]);
    MPI_Info_create(&info_array[1]);
    MPI_Info_create(&info_array[2]);
    MPI_Info_create(&info_array[3]);

    MPI_Info_set(info_array[0], (char *) "x", (char *) "x0");
    MPI_Info_set(info_array[0], (char *) "y", (char *) "y0");

    MPI_Info_set(info_array[1], (char *) "b", (char *) "b1");
    MPI_Info_set(info_array[1], (char *) "y", (char *) "y1");

    MPI_Info_set(info_array[2], (char *) "z", (char *) "z2");
    MPI_Info_set(info_array[2], (char *) "a", (char *) "a2");
    MPI_Info_set(info_array[2], (char *) "x", (char *) "x2");

    MPI_Info_set(info_array[3], (char *) "z", (char *) "z3");
    MPI_Info_set(info_array[3], (char *) "x", (char *) "x3");
    MPI_Info_set(info_array[3], (char *) "y", (char *) "y3");

    MPIX_Info_merge_from_array(4, info_array, &info_merge);


    /* Do some validity checks: */

    nkeys_expected = 5;
    MPI_Info_get_nkeys(info_merge, &nkeys);
    if (nkeys != nkeys_expected) {
        fprintf(stderr,
                "ERROR: MPI_Info_get_nkeys() returned unexpected number of keys (expected %d / got %d) for info_merge\n",
                nkeys_expected, nkeys);
        errs++;
    }

    count_array_entries = 0;
    array_entries_expected = 3;
    for (int i = 0; i < nkeys; i++) {
        int flag;
        int buflen = MPI_MAX_INFO_VAL;
        MPI_Info_get_nthkey(info_merge, i, key);
        MPI_Info_get_string(info_merge, key, &buflen, value, &flag);
        if (strcmp(value, "mpix_info_array_type") == 0) {
            count_array_entries++;
        }
    }

    if (count_array_entries != array_entries_expected) {
        fprintf(stderr,
                "ERROR: Looping over MPI_Info_get_string() returned unexpected number of array entries (expected %d / got %d) for info_merge\n",
                array_entries_expected, count_array_entries);
        errs++;
    }


    /* Duplicate an info object with array and check the keys: */

    MPI_Info info_merge_array[2];
    MPI_Info info_merge_dup, info_merge_twice;

    MPI_Info_dup(info_merge, &info_merge_dup);
    MPI_Info_get_nkeys(info_merge, &nkeys);

    nkeys_expected = 5;
    MPI_Info_get_nkeys(info_merge, &nkeys);
    if (nkeys != nkeys_expected) {
        fprintf(stderr,
                "ERROR: MPI_Info_get_nkeys() returned unexpected number of keys (expected %d / got %d) for info_merge_dup\n",
                nkeys_expected, nkeys);
        errs++;
    }

    count_array_entries = 0;
    array_entries_expected = 3;
    for (int i = 0; i < nkeys; i++) {
        int flag;
        int buflen = MPI_MAX_INFO_VAL;
        MPI_Info_get_nthkey(info_merge_dup, i, key);
        MPI_Info_get_string(info_merge_dup, key, &buflen, value, &flag);
        if (strcmp(value, "mpix_info_array_type") == 0) {
            count_array_entries++;
        }
    }
    if (count_array_entries != array_entries_expected) {
        fprintf(stderr,
                "ERROR: Looping over MPI_Info_get_string() returned unexpected number of array entries (expected %d / got %d) for info_merge_dup\n",
                array_entries_expected, count_array_entries);
        errs++;
    }


    /* Now merge two info objects with already attached arrays and check what happens: */

    info_merge_array[0] = info_merge;
    info_merge_array[1] = info_merge_dup;
    MPIX_Info_merge_from_array(2, info_merge_array, &info_merge_twice);
    MPI_Info_get_nkeys(info_merge_twice, &nkeys);

    nkeys_expected = 2;
    MPI_Info_get_nkeys(info_merge_twice, &nkeys);
    if (nkeys != nkeys_expected) {
        fprintf(stderr,
                "ERROR: MPI_Info_get_nkeys() returned unexpected number of keys (expected %d / got %d) for info_merge_twice\n",
                nkeys_expected, nkeys);
        errs++;
    }

    count_array_entries = 0;
    array_entries_expected = 2;
    for (int i = 0; i < nkeys; i++) {
        int flag;
        int buflen = MPI_MAX_INFO_VAL;
        MPI_Info_get_nthkey(info_merge_twice, i, key);
        MPI_Info_get_string(info_merge_twice, key, &buflen, value, &flag);
        if (strcmp(value, "mpix_info_array_type") == 0) {
            count_array_entries++;
        }
    }
    if (count_array_entries != array_entries_expected) {
        fprintf(stderr,
                "ERROR: Looping over MPI_Info_get_string() returned unexpected number of array entries (expected %d / got %d) for info_merge_twice\n",
                array_entries_expected, count_array_entries);
        errs++;
    }

    MPI_Info_free(&info_array[0]);
    MPI_Info_free(&info_array[1]);
    MPI_Info_free(&info_array[2]);
    MPI_Info_free(&info_array[3]);
    MPI_Info_free(&info_merge);

    MPI_Info_free(&info_merge_twice);
    MPI_Info_free(&info_merge_dup);

    MTest_Finalize(errs);
    return MTestReturnValue(errs);
}
