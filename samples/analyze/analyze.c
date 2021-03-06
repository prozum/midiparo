#include <mid/mid.h>
#include <mid/mid-str.h>
#include <analyze/analyze.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main( int argc, char *argv[] )
{
    FILE *mid_file1;
    mid_t *mid1, *tmp_mid1;
    f_prn_t *finger_prints1, *finger_prints2;
    uint8_t i, j;

    uint32_t fprints[3] = {2256978, 7500297, 0};

    /* Open file */
    if((mid_file1 = fopen(argv[1], "rb")) == NULL ) {
        perror(argv[1]);
        return -1;
    }

    /* Read mid */
    tmp_mid1 = read_mid(mid_file1);

    fclose(mid_file1);

    mid1 = merge_tracks(tmp_mid1);

    /* Extract channels */
    finger_prints1 = finger_prn_gen(mid1->tracks->ptr);
    finger_prints2 = convert_to_f_prn(fprints);

    i = finger_prn_cmp(finger_prints1, finger_prints2);

    printf("Compare: %d\n", i);

    for (i = 0; i < FINGER_PRNS; i++) {
        for (j = 0; j < FINGER_PRN_CMP_LEN; j++) {
            printf("%d ", finger_prints1[i].f_prn[j]);
            printf("%d  ", finger_prints2[i].f_prn[j]);
        }

        printf("\n");
    }

    putchar('\n');

    putchar('\n');

    /*Test skyline*/
    
    /* Cleanup */
    free_f_prn(finger_prints1);
    free_f_prn(finger_prints2);
    free_mid(tmp_mid1);
    free_mid(mid1);

    putchar('\n');
    return 0;
}
