#include "db.h"

#include <mid/mid.h>
#include <analyze/analyze.h>
#include <ext/ext.h>

#include <glib.h>
#include <libgen.h>
#include <sqlite3.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int database_open_error(int rc, sqlite3 *db)
{
    if(rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return -1;
    } else {
        fprintf(stdout, "Opened database successfully\n");
        return 0;
    }
}

int database_general_error(int rc, char *error, int type)
{

    if (rc != SQLITE_OK) {

        fprintf(stderr, "SQL error: %s\n", error);
        sqlite3_free(error);
        return -1;

    }

    switch(type) {

        case 1:
            fprintf(stdout, "Table created successfully\n");
            break;

        case 2:
            fprintf(stdout, "Table changed successfully\n");
            break;

        case 3:
            fprintf(stdout, "Data selected successfully\n");
            break;

        default:
            fprintf(stdout, "The operation completed successfully, no idea which though\n");
    }

    return 0;
}

int db_init(sqlite3 *db)
{
    char *sql, *error = 0;
    int rc;

    /* Write database structure */
    sql = "CREATE TABLE IF NOT EXISTS `songs` ("  \
          "`artist`                  VARCHAR(64) DEFAULT \'N/A\',"                                          \
          "`album`                   VARCHAR(64) DEFAULT \'N/A\',"                                          \
          "`num`                    UNSIGNED INT DEFAULT 0,"                                                \
          "`title`                   VARCHAR(64) DEFAULT \'N/A\',"                                          \
          "`instr_classes`          UNSIGNED INT DEFAULT 0,"                                                \
          "`time`                   UNSIGNED INT DEFAULT 0,"                                                \
          "`plays`                      UNSIGNED DEFAULT 0,"                                                \
          "`import_date`               timestamp DEFAULT CURRENT_TIMESTAMP,"                                \
          "`finger_print1`          UNSIGNED INT DEFAULT 0,"   \
          "`finger_print2`          UNSIGNED INT DEFAULT 0,"   \
          "`finger_print3`          UNSIGNED INT DEFAULT 0,"   \
          "`addr`                  VARCHAR(1024) DEFAULT \'N/A\');";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, NULL, 0, &error);

    /* Table error check */
    database_general_error(rc, error, 1);

    return rc;
}

int db_import_mid(sqlite3 *db, char *mid_addr)
{
    FILE * mid_file;
    mid_t *mid, *mid_tmp;
    f_prn_t *f_prns;

    char *sql, *error = 0;
    int rc;
    int i;

    char artist[64];
    char album[64];
    char title[64];
    unsigned  num;
    int instr_classes;
    double time;
    unsigned finger_prints[3] = {0,0,0};

    /* Check string for "'" */
    if (check_sql(mid_addr) == -1) {

        fprintf(stderr,"File contains dangerous character \"\'\"!\nTODO: Proper fix for this.\n");
        return -1;
    }

    /* Open mid file */
    if ((mid_file = fopen(mid_addr, "rb")) == NULL) {

        perror(mid_addr);
        return -1;
    }

    /* Read midi content */
    if ((mid_tmp = read_mid(mid_file)) == NULL) {

        fprintf(stderr,"Could not read mid!\n");
        fclose(mid_file);
        return -1;
    }
    fclose(mid_file);

    /* Merge mid tracks */
    if ((mid = merge_tracks(mid_tmp)) == NULL) {

        fprintf(stderr,"Could not merge tracks!\n");
        free(mid_tmp);
        return -1;

    }
    free_mid(mid_tmp);

    /* Instruments */
    instr_classes = extract_instr_classes(mid);

    /* Mid length (ms) */
    time = extract_time(mid);

    /* Import 3 finger prints */
    //f_prns = finger_prn_gen(mid->tracks->ptr);

    //for (i = 0; i < FINGER_PRNS; i++) {
    //    finger_prints[i] += f_prns[i].f_prn[0]<<25;
    //    finger_prints[i] += f_prns[i].f_prn[1]<<20;
    //    finger_prints[i] += f_prns[i].f_prn[2]<<15;
    //    finger_prints[i] += f_prns[i].f_prn[3]<<10;
    //    finger_prints[i] += f_prns[i].f_prn[4]<<5;
    //    finger_prints[i] += f_prns[i].f_prn[5];
    //}

    //free_mid(mid);
    //for (i = 0; i < FINGER_PRNS; i++) {
    //    free(f_prns[i].f_prn);
    //}
    //free(f_prns);


    /* Exec data import */
    parse_filename(mid_addr, artist, album, &num, title);
    sql = g_strdup_printf("INSERT INTO songs (artist, album, num, title, instr_classes, time, finger_print1, finger_print2, finger_print3,  addr) VALUES ('%s', '%s', %d, '%s', %d, %.0f, %d, %d, %d, '%s');",
                          artist,
                          album,
                          num,
                          title,
                          instr_classes,
                          time,
                          finger_prints[0],
                          finger_prints[1],
                          finger_prints[2],
                          mid_addr);

    rc = sqlite3_exec(db, sql, NULL, 0, &error);
    free(sql);

    database_general_error(rc, error, 2);

    return rc;
}

/** Checks if sql statement contains "'" or """ */
int check_sql(const char *sql)
{
    int i = 0;

    while (sql[i] != '\0') {

        if (sql[i] == '\'' || sql[i] == '\"') {

            return -1;
        }
        i++;
    }

    return 0;
}

/** Parse file name for song data */
void parse_filename (char *file_name, char *artist, char *album, unsigned *num, char *title)
{
    char *file_pnt;

    /* Removes file path */
    file_pnt = basename(file_name);

    /* Removes file extension from last string */
    file_pnt[strlen(file_pnt)-4] = '\0';

    if (4 != sscanf(file_pnt, "%[^-]- %[^-]- %u- %s",
                    artist,
                    album,
                    num,
                    title)) {

        strcpy(title, file_pnt);
        strcpy(artist, "N/A");
        strcpy(album, "N/A");
        *num = 0;
    }
}
