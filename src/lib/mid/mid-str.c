#include "mid-str.h"

#include "mid.h"

#include <stdio.h>

const char *instruments[] = {
    "Acoustic grand piano",
    "Bright acoustic piano",
    "Electric grand piano",
    "Honky tonk piano",
    "Electric piano 1",
    "Electric piano 2",
    "Harpsichord",
    "Clavi",
    "Celesta",
    "Glockenspiel",
    "Music box",
    "Vibraphone",
    "Marimba",
    "Xylophone",
    "Tubular bells",
    "Dulcimer",
    "Drawbar organ",
    "Percussive organ",
    "Rock organ",
    "Church organ",
    "Reed organ",
    "Accordion",
    "Harmonica",
    "Tango accordion",
    "Acoustic guitar nylon",
    "Acoustic guitar steel",
    "Electric guitar jazz",
    "Electric guitar clean",
    "Electric guitar muted",
    "Overdriven guitar",
    "Distortion guitar",
    "Guitar harmonics",
    "Acoustic bass",
    "Electric bass finger",
    "Electric bass pick",
    "Fretless bass",
    "Slap bass 1",
    "Slap bass 2",
    "Synth bass 1",
    "Synth bass 2",
    "Violin",
    "Viola",
    "Cello",
    "Contrabass",
    "Tremolo strings",
    "Pizzicato strings",
    "Orchestral harp",
    "Timpani",
    "String ensemble 1",
    "String ensemble 2",
    "Synthstrings 1",
    "Synthstrings 2",
    "Choir aahs",
    "Voice oohs",
    "Synth voice",
    "Orchestra hit",
    "Trumpet",
    "Trombone",
    "Tuba",
    "Muted trumpet",
    "French horn",
    "Brass section",
    "Synthbrass 1",
    "Synthbrass 2",
    "Soprano sax",
    "Alto sax",
    "Tenor sax",
    "Baritone sax",
    "Oboe",
    "English horn",
    "Bassoon",
    "Clarinet",
    "Piccolo",
    "Flute",
    "Recorder",
    "Pan flute",
    "Blown bottle",
    "Shakuhachi",
    "Whistle",
    "Ocarina",
    "Lead 1 square",
    "Lead 2 sawtooth",
    "Lead 3 calliope",
    "Lead 4 chiff",
    "Lead 5 charang",
    "Lead 6 voice",
    "Lead 7 fifths",
    "Lead 8 bass lead",
    "Pad 1 new age",
    "Pad 2 warm",
    "Pad 3 polysynth",
    "Pad 4 choir",
    "Pad 5 bowed",
    "Pad 6 metallic",
    "Pad 7 halo",
    "Pad 8 sweep",
    "Fx 1 rain",
    "Fx 2 soundtrack",
    "Fx 3 crystal",
    "Fx 4 atmosphere",
    "Fx 5 brightness",
    "Fx 6 goblins",
    "Fx 7 echoes",
    "Fx 8 sci fi",
    "Sitar",
    "Banjo",
    "Shamisen",
    "Koto",
    "Kalimba",
    "Bag pipe",
    "Fiddle",
    "Shanai",
    "Tinkle bell",
    "Agogo",
    "Steel drums",
    "Woodblock",
    "Taiko drum",
    "Melodic tom",
    "Synth drum",
    "Reverse cymbal",
    "Guitar fret noise",
    "Breath noise",
    "Seashore",
    "Bird tweet",
    "Telephone ring",
    "Helicopter",
    "Applause",
    "Gunshot"
};

void print_header(mid_t *mid)
{
    printf("\n<###HEAD INFO###>\n");
    printf("Format:\t\t%i\n",mid->format);
    printf("Tracks:\t\t%i\n",mid->tracks);
    printf("Division:\t%i\n",mid->division);
}

void print_tracks(mid_t *mid)
{
    int i;

    printf("\n<###TRACK INFO###>\n");
    for (i = 0; i < mid->tracks; i++) {
        printf("Track: \t%u\n",i+1);
        printf("Track bytes: \t%u\n",mid->track[i].bytes);
        printf("Track events: \t%u\n",mid->track[i].events);
    }
}

void print_event(mid_t *mid, unsigned int t, unsigned int e)
{
    int i;

    printf("\n<###EVENT INFO###>\n");
    printf("Track %2u | Event %2u\n",t,e);
    printf("Msg : %x\n",mid->track[t].event[e].msg);
    printf("Byte 1: %x\n",mid->track[t].event[e].byte_1);
    printf("Byte 2: %x\n",mid->track[t].event[e].byte_2);
    printf("Delta: %x\n",mid->track[t].event[e].delta);

    /* Meta message data */
    if (mid->track[t].event[e].msg == SYS_RESET) {
        printf("Data: ");
        for (i = 0; i < mid->track[t].event[e].byte_2; i++ ) {
            printf("%x ",mid->track[t].event[e].data[i]);
        }
    /* System exclusive message */
    } else if (mid->track[t].event[e].msg == SYSEX_START) {
        printf("Data: ");
        for (i = 0; i < mid->track[t].event[e].byte_2; i++ ) {
            printf("%x ",mid->track[t].event[e].data[i]);
        }
    }
    printf("\n");
}
