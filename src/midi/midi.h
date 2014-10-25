#ifndef __MIDI_H__
#define __MIDI_H__

#include <stdint.h>

#if defined(_WIN32)
#include <stddef.h>
#endif

#define HEADER_SIGNATURE 0x4D546864
#define HEADER_LENGTH    0x00000006
#define TRACK_SIGNATURE  0x4D54726b

typedef struct header_s {
    uint16_t format;    // Midi format
    uint16_t tracks;    // Number of tracks
    uint16_t division;  // Time division
} header_t;

typedef struct event_s {
    uint8_t type;     // Event type see: event_type
    uint8_t para_1;   // First parameter
    uint8_t para_2;   // Second parameter
    uint8_t delta;    // Delta time
    uint8_t *data;    // Meta event data
} event_t;

typedef struct track_s {

    /* track length in bytes */
    uint32_t len;

    /* how many elements the track contains */
    uint32_t num;

    event_t *events;
} track_t;


enum midi_format {
    SINGLE_TRACK,
    MULTI_TRACK_SYNC,
    MULTI_TRACK_ASYNC
};

enum event_type {               // Parameter1	Parameter2
    NOTE_OFF            = 0x80, // Note			Velocity
    NOTE_ON             = 0x90, // Note			Velocity
    POLY_KEY_PRESS      = 0xa0, // Note			Pressure
    CONTROLLER_CHANGE   = 0xb0, // Controls1	Controls2
    PROGRAM_CHANGE      = 0xc0, // Program		NONE
    CHANNEL_PRESSURE    = 0xd0, // Pressure		NONE
    PITCH_BEND          = 0xe0, // Pitch LSB	Pitch MSB
    SYS_EX_MESSAGE      = 0xf0, // Vendor ID	N/A
    META_EVENT          = 0xff	// Meta event	Length
};

enum meta_type {
    SEQ_NUMBER          = 0x00,
    TEXT_EVENT          = 0x01,
    COPYRIGHT_NOTICE    = 0x02,
    TRACK_NAME          = 0x03,
    INSTRUMENT_NAME     = 0x04,
    LYRIC_TEXT          = 0x05,
    MARKER_TEXT         = 0x06,
    CUE_POINT           = 0x07,
    MIDI_CPA            = 0x20,
    END_OF_TRACK        = 0x2F,
    TEMPO_SETTING       = 0x51,
    SMPTE_OFFSET        = 0x54,
    TIME_SIG            = 0x58,
    KEY_SIG             = 0x59,
    SEQ_SPECIFIC_EVENT  = 0x7f
};

enum node {
    C0,  CH0,  D0,  DH0,  E0,  F0,  FH0,  G0,  GH0,  A0,  AH0,  B0,
    C1,  CH1,  D1,  DH1,  E1,  F1,  FH1,  G1,  GH1,  A1,  AH1,  B1,
    C2,  CH2,  D2,  DH2,  E2,  F2,  FH2,  G2,  GH2,  A2,  AH2,  B2,
    C3,  CH3,  D3,  DH3,  E3,  F3,  FH3,  G3,  GH3,  A3,  AH3,  B3,
    C4,  CH4,  D4,  DH4,  E4,  F4,  FH4,  G4,  GH4,  A4,  AH4,  B4,
    C5,  CH5,  D5,  DH5,  E5,  F5,  FH5,  G5,  GH5,  A5,  AH5,  B5,
    C6,  CH6,  D6,  DH6,  E6,  F6,  FH6,  G6,  GH6,  A6,  AH6,  B6,
    C7,  CH7,  D7,  DH7,  E7,  F7,  FH7,  G7,  GH7,  A7,  AH7,  B7,
    C8,  CH8,  D8,  DH8,  E8,  F8,  FH8,  G8,  GH8,  A8,  AH8,  B8,
    C9,  CH9,  D9,  DH9,  E9,  F9,  FH9,  G9,  GH9,  A9,  AH9,  B9,
    C10, CH10, D10, DH10, E10, F10, FH10, G10
};

enum instrument {
    ACOUSTIC_GRAND_PIANO,
    BRIGHT_ACOUSTIC_PIANO,
    ELECTRIC_GRAND_PIANO,
    HONKY_TONK_PIANO,
    ELECTRIC_PIANO_1,
    ELECTRIC_PIANO_2,
    HARPSICHORD,
    CLAVI,
    CELESTA,
    GLOCKENSPIEL,
    MUSIC_BOX,
    VIBRAPHONE,
    MARIMBA,
    XYLOPHONE,
    TUBULAR_BELLS,
    DULCIMER,
    DRAWBAR_ORGAN,
    PERCUSSIVE_ORGAN,
    ROCK_ORGAN,
    CHURCH_ORGAN,
    REED_ORGAN,
    ACCORDION,
    HARMONICA,
    TANGO_ACCORDION,
    ACOUSTIC_GUITAR_NYLON,
    ACOUSTIC_GUITAR_STEEL,
    ELECTRIC_GUITAR_JAZZ,
    ELECTRIC_GUITAR_CLEAN,
    ELECTRIC_GUITAR_MUTED,
    OVERDRIVEN_GUITAR,
    DISTORTION_GUITAR,
    GUITAR_HARMONICS,
    ACOUSTIC_BASS,
    ELECTRIC_BASS_FINGER,
    ELECTRIC_BASS_PICK,
    FRETLESS_BASS,
    SLAP_BASS_1,
    SLAP_BASS_2,
    SYNTH_BASS_1,
    SYNTH_BASS_2,
    VIOLIN,
    VIOLA,
    CELLO,
    CONTRABASS,
    TREMOLO_STRINGS,
    PIZZICATO_STRINGS,
    ORCHESTRAL_HARP,
    TIMPANI,
    STRING_ENSEMBLE_1,
    STRING_ENSEMBLE_2,
    SYNTHSTRINGS_1,
    SYNTHSTRINGS_2,
    CHOIR_AAHS,
    VOICE_OOHS,
    SYNTH_VOICE,
    ORCHESTRA_HIT,
    TRUMPET,
    TROMBONE,
    TUBA,
    MUTED_TRUMPET,
    FRENCH_HORN,
    BRASS_SECTION,
    SYNTHBRASS_1,
    SYNTHBRASS_2,
    SOPRANO_SAX,
    ALTO_SAX,
    TENOR_SAX,
    BARITONE_SAX,
    OBOE,
    ENGLISH_HORN,
    BASSOON,
    CLARINET,
    PICCOLO,
    FLUTE,
    RECORDER,
    PAN_FLUTE,
    BLOWN_BOTTLE,
    SHAKUHACHI,
    WHISTLE,
    OCARINA,
    LEAD_1_SQUARE,
    LEAD_2_SAWTOOTH,
    LEAD_3_CALLIOPE,
    LEAD_4_CHIFF,
    LEAD_5_CHARANG,
    LEAD_6_VOICE,
    LEAD_7_FIFTHS,
    LEAD_8_BASS_LEAD,
    PAD_1_NEW_AGE,
    PAD_2_WARM,
    PAD_3_POLYSYNTH,
    PAD_4_CHOIR,
    PAD_5_BOWED,
    PAD_6_METALLIC,
    PAD_7_HALO,
    PAD_8_SWEEP,
    FX_1_RAIN,
    FX_2_SOUNDTRACK,
    FX_3_CRYSTAL,
    FX_4_ATMOSPHERE,
    FX_5_BRIGHTNESS,
    FX_6_GOBLINS,
    FX_7_ECHOES,
    FX_8_SCI_FI,
    SITAR,
    BANJO,
    SHAMISEN,
    KOTO,
    KALIMBA,
    BAG_PIPE,
    FIDDLE,
    SHANAI,
    TINKLE_BELL,
    AGOGO,
    STEEL_DRUMS,
    WOODBLOCK,
    TAIKO_DRUM,
    MELODIC_TOM,
    SYNTH_DRUM,
    REVERSE_CYMBAL,
    GUITAR_FRET_NOISE,
    BREATH_NOISE,
    SEASHORE,
    BIRD_TWEET,
    TELEPHONE_RING,
    HELICOPTER,
    APPLAUSE,
    GUNSHOT
};

uintptr_t *ffread(FILE *file, long int offset ,size_t buf_size);

int count_events(uint8_t *data, uint32_t len);

header_t *read_header(FILE *file);
track_t *read_tracks(FILE *file, uint16_t n);
event_t *read_events(uint8_t *data, uint16_t n);

void write_header(FILE *midi_file, header_t header);
void write_tracks(FILE *midi_file, track_t track);
void free_tracks(track_t *tracks, uint16_t n);

#endif
