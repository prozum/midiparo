#include "midiogre-search.h"
#include "midiogre-app.h"
#include "midiogre-song.h"

#include <pop/pop.h>

#include <gtk/gtk.h>

#include <string.h>
#include <math.h>

gint search_event(void)
{
    sqlite3 *db;
    gchar *sql_head;
    song_t *song;

    gint instr_classes;
    const gchar *artist_value;
    const gchar *album_value;
    const gchar *title_value;
    gint limit;


    /* Gather search criterias */
    instr_classes = read_instr_classes(app);

    artist_value = gtk_entry_get_text(app->artist_entry);
    if (check_sql(artist_value) == -1) {

        return -1;
    }

    album_value = gtk_entry_get_text(app->album_entry);
    if (check_sql(album_value) == -1) {

        return -1;
    }

    title_value = gtk_entry_get_text(app->title_entry);
    if (check_sql(title_value) == -1) {

        return -1;
    }

    sql_head = g_strdup_printf("SELECT rowid,artist,album,num,title,time,plays,strftime('%%s', import_date),finger_print1,finger_print2,finger_print3,addr FROM songs WHERE artist LIKE '%%%s%%' AND album LIKE '%%%s%%' AND title LIKE '%%%s%%' AND (%d & instr_classes) = %d",
                               artist_value,
                               album_value,
                               title_value,
                               instr_classes,
                               instr_classes);


    /* Get max result from spinbox */
    limit = gtk_spin_button_get_value_as_int(app->result_spinbutton);


    /* Open db */
    sqlite3_open("mid.db", &db);


    /* Search db and add results to song GQueues */
    search_db(app->songs_alpha, db, sql_head, "%s ORDER BY title,album,artist LIMIT %d;", limit);
    search_db(app->songs_new, db, sql_head, "%s ORDER BY import_date DESC LIMIT %d;", limit);
    search_db(app->songs_pop, db, sql_head, "%s ORDER BY title,album,artist;", 0);


    /* Only add songs to fingerprint songbox if a favorite song is chosen. */
    if (app->cur_fav != NULL) {

        /* Finger print only */
        search_db(app->songs_fprnt, db, sql_head, "%s ORDER BY title,album,artist;", 0);
        g_queue_sort(app->songs_fprnt, (GCompareDataFunc)sort_fprnt, app);

        while(app->songs_fprnt->length > limit) {
            song = g_queue_pop_tail(app->songs_fprnt);
            song_free(song);
        }

        /* Both fingerprint and popularity */
        search_db(app->songs_best, db, sql_head, "%s ORDER BY title,album,artist;", 0);
        g_queue_sort(app->songs_best, (GCompareDataFunc)sort_best, app);

        while(app->songs_best->length > limit) {

            song = g_queue_pop_tail(app->songs_best);
            song_free(song);
        }
    }

    /* Popularity */
    g_queue_sort(app->songs_pop, (GCompareDataFunc)song_compare_pop, NULL);
    while(app->songs_pop->length > limit) {
        song = g_queue_pop_tail(app->songs_pop);
        song_free(song);
    }

    g_free(sql_head);
    sqlite3_close(db);

    songbox_update(app->songbox_alpha, app->songs_alpha, limit);
    songbox_update(app->songbox_new, app->songs_new, limit);
    songbox_update(app->songbox_pop, app->songs_pop, limit);
    songbox_update(app->songbox_fprnt, app->songs_fprnt, limit);
    songbox_update(app->songbox_best, app->songs_best, limit);

    return 0;
}

int search_db(GQueue *songs, sqlite3 *db, gchar *head, gchar *body, gint limit)
{
    gint rc;
    gchar *sql;
    gchar *error = 0;


    sql = g_strdup_printf(body,
                          head,
                          limit);


    rc = sqlite3_exec(db, sql, search_handler, songs, &error);

    database_general_error(rc, error, 2);

    free(sql);

    return 0;
}

gint sort_fprnt(gpointer s1, gpointer s2, gpointer a)
{
    song_t *song1 = s1;
    song_t *song2 = s2;
    song_t *song_fav = app->cur_fav;


    if (song1->edit_score == -1) {
        song1->edit_score = finger_prn_cmp(song1->finger_prints, song_fav->finger_prints);
    }

    if (song2->edit_score == -1) {
        song2->edit_score = finger_prn_cmp(song2->finger_prints, song_fav->finger_prints);
    }

    if (song1->edit_score > song2->edit_score) {
        return 1;
    } else if (song1->edit_score == song2->edit_score) {
        return 0;
    } else {
        return -1;
    }
}

gint sort_best(gpointer s1, gpointer s2, gpointer a)
{
    song_t *song1 = s1;
    song_t *song2 = s2;
    song_t *song_fav = app->cur_fav;

    double song1_score = 0, song2_score = 0;

    if (song1->edit_score == -1) {
        song1->edit_score = finger_prn_cmp(song1->finger_prints, song_fav->finger_prints);
    }

    if (song2->edit_score == -1) {
        song2->edit_score = finger_prn_cmp(song2->finger_prints, song_fav->finger_prints);
    }

    song1_score = song_score_all(song1->plays, song1->time_added, song1->edit_score);
    song2_score = song_score_all(song2->plays, song2->time_added, song2->edit_score);

    if (song1_score > song2_score) {
        return -1;
    } else if(song1_score < song2_score) {
        return 1;
    } else {
        return 0;
    }

}

gint search_handler(void *s, int argc, char **argv, char **col_name)
{
    song_t *song;
    GQueue *songs = s;
    uint32_t fingerprints[3];
    song = calloc(1, sizeof(song_t));

    /* Load columns for song row */

    song->id = atoi(argv[0]);

    /* Inforce max string length ARTIST_MAX_NAME */
    argv[0][ARTIST_MAX_NAME] = '\0';
    strcpy(song->artist, argv[1]);

    /* Inforce max string length ALBUM_MAX_NAME */
    argv[1][ALBUM_MAX_NAME] = '\0';
    strcpy(song->album,  argv[2]);

    song->num = atoi(argv[3]);

    /* Inforce max string length TITLE_MAX_NAME */
    argv[1][ALBUM_MAX_NAME] = '\0';
    strcpy(song->title,  argv[4]);

    song->length = atoi(argv[5]);

    song->plays = atoi(argv[6]);

    song->time_added = atoi(argv[7]);

    fingerprints[0] = atoi(argv[8]);
    fingerprints[1] = atoi(argv[9]);
    fingerprints[2] = atoi(argv[10]);
    song->finger_prints = convert_to_f_prn(fingerprints);

    song->addr = malloc(strlen(argv[11])+1);
    strcpy(song->addr,argv[11]);

    song->edit_score = -1;

    /*song->time_added*/

    /* Push column to queue */
    g_queue_push_tail(songs, song);

    return 0;
}

gint read_instr_classes(MidiogreApp *app)
{
    gint i;
    gint rv = 0;

    for (i = 0; i < INSTR_CLASSES; i++) {

        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->instr_buttons[i]))) {

            rv += pow(2, i);
        }
    }

    return rv;
}
