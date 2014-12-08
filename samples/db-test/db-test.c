#include <stdio.h>
#include <stdlib.h>

#include <mid/mid.h>
#include <mid/mid-str.h>

#include <db/db.h>

int main(int argc, char* argv[])
{
    mid_t *mid;
    sqlite3 *db;
    
    int rc;
    unsigned int i,j;
    char *sql, *sql2, *sql3;
    char *error = 0;
    FILE *file;
    const char* data = "Callback function called";

    /* Open mid file */
    file = fopen(argv[1],"rb");
    if(file == NULL) {
        perror(argv[1]);
        return -1;
    }
    //parse_filename(file);
    /* Read content */
    mid = read_mid(file);
    fclose(file);

    /* Open database */
    rc = sqlite3_open("test.db", &db);

    /* Database open error check */
	database_open_error(rc, db);

    /* Write database structure */
    sql = "CREATE TABLE midiFile("          \
          "ARTIST                  CHAR,"   \
          "ALBUM                   CHAR,"   \
          "TRACKNUM        UNSIGNED INT,"   \
          "TRACK                   CHAR,"   \
          "BYTE1       UNSIGNED INT,"   \
          "BYTE2                   UNSIGNED INT,"   \
          "DELTA                   UNSIGNED INT,"   \
          "INSTRUMENTS             UNSIGNED INT);";
    
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &error);

    /* Table error check */
    database_general_error(rc, error, 1);

    /* For each track in mid */
    for (i = 0; i < mid->tracks; i++) {
        /* For each event in track */
        for (j = 0; j < mid->track[i].events; j++) {
    
            /* If meta message */
            if (mid->track[i].event[j].msg == NOTE_ON) {
    
                asprintf(&sql2, "INSERT INTO midiFile(BYTE1,BYTE2,DELTA) \
                                VALUES (%u, %u, %u);",mid->track[i].event[j].byte_1, \
                                                      mid->track[i].event[j].byte_2, \
                                                      mid->track[i].event[j].delta);
                
                rc = sqlite3_exec(db, sql2, callback, 0, &error);
                
                free(sql2);
            }
        }
    }
    /* Only checks the last command */
    database_general_error(rc, error, 2);

    /* Create SQL statement */
    /*
    sql3 = "SELECT * from midiFile";
    rc = sqlite3_exec(db, sql3, callback, (void*)data, &error);

    database_general_error(rc, error, 3);

	*/
	
    sqlite3_close(db);    
 
    return 0;
}    