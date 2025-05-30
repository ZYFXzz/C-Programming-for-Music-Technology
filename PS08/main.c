/* Program for a sinewave generation synthesizer
 * which can play simultaneous tones
 *
 * Demonstrates use of NCurses and PortAudio (also libsnd for debugging file)
 */

#include "key_tables.h" /* key to freq functions */
// #include "ncurses_dll.h" // searched online found this is correct way to include ncurses on windows
#include "paUtils.h" /* portaudio function prototypes */
#include "synth.h"   /* struct Synth */
// #include <ncurses.h>   /* This library is for getting input without hitting return */
#include <ncurses\ncurses.h> // the correct way to load ncurses.h, where it is located in a sub folder msys2/mingw64/include/ncurses/ncurses.h
#include <portaudio.h>       /* portaudio */
#include <sndfile.h>         /* libsndfile */
#include <stdatomic.h>       /* permits write/read of "cmd" to be atomic */
#include <stdio.h>
#include <string.h> /* memset() */
#include <unistd.h> /* for usleep(ms) */

/* PortAudio callback function protoype */
static int paCallback(
    const void *inputBuffer,
    void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo *timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData);

/* callback structure */
typedef struct {
    int num_chan;
    Synth *ps;
#if (DB_WAV_OUT)
    SNDFILE *sndfile;
#endif
} Buf;

int main(int argc, char *argv[]) {
    /* instantiate instance of Synth struct */
    Synth synth;
    Synth *ps = &synth;
    /* instantiate instance of portaudio callback data structure */
    Buf buf;
    /* PortAudio data stream */
    PaStream *stream;
#if (DB_WAV_OUT)
    /* libsndfile data structures */
    SF_INFO sfinfo;
#endif

    /* initialize tables */
    init_key2index();

    /* initialize Synth */
    init_synth(ps, SAMP_RATE);

    /* initialize struct for callback */
    buf.num_chan = NUM_CHAN;
    buf.ps = ps;

    /* start up Port Audio */
    stream = startupPa(1, NUM_CHAN,
                       SAMP_RATE, FRAMES_PER_BUFFER, paCallback, &buf);

#if (DB_WAV_OUT)
    /* open debug output file */
    memset(&sfinfo, 0, sizeof(sfinfo));
    sfinfo.samplerate = SAMP_RATE;
    sfinfo.channels = NUM_CHAN;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    if ((buf.sndfile = sf_open("test_file.wav", SFM_WRITE, &sfinfo)) == NULL) {
        fprintf(stderr, "Error: could not open test wav file\n");
        return -1;
    }
#endif

    /* Initialize ncurses
     * to permit interactive character input
     *
     * ncurses loop waits for key press
     * If valid key, then initialize Tone structure
     * and refresh ncurses display buffer
     */
    initscr();                              /* Start curses mode */
    cbreak();                               /* Line buffering disabled */
    noecho();                               /* Comment this out if you want to echo characters when typing */
    printw("Welcome to my synthesizer!\n"); /* line 0 */
    printw("Keyboard to piano key mapping is:\n");
    printw("qwertyQWERTY -> C3 to B3\n");
    printw("asdfghASDFGH -> C4 to B4\n");
    printw("zxcvbnZXCVBN -> C5 to B5\n");
    printw("'.' to stop playing oldest tone\n");
    printw("SpaceBar to quit\n"); /* line 6 */
    printw("\n");                 /* line 7 is blank for now */
    printw("Key: ");              /* line 8 */
    refresh();

    char ch = '\0'; /* Init ch to null character */
    while (1) {
        char msg[32];
        ch = getch(); // wait for next keypress
        if (ch == ' ') {
            break; // quit if space
        } else if (ch == '.') {
            /* remove key from voicing keys */
            ps->cmd = CMD_RM_KEY;
            sprintf(msg, "Removed key. New key: ");
            /* wait for callback to execute command */
            while (ps->cmd > 0)
                usleep(20);
        } else if (!valid_key((int)ch)) {
            /* not a valid synth key */
            sprintf(msg, "Invalid key. New key: ");
        } else {
            /* add key to voicing keys */
            ps->new_key = ch;
            ps->new_freq = key2freq(ch);
            ps->cmd = CMD_ADD_KEY;
            sprintf(msg, "Key   %1c %3.0f, New key: ", ch, ps->new_freq);
            /* wait for callback to execute command */
            while (ps->cmd > 0)
                usleep(20);
        }
        /* print key list on line 7 */
        mvprintw(7, 0, "%d voicing", ps->num_keys);
        for (int i = 0; i < KEYS_VOICED; i++) {
            mvprintw(7, 10 + 4 * i, " %c", (char)ps->tone[i].key);
        }
        mvprintw(8, 0, "%s", msg);
        refresh(); // display NCurses screen buffer on terminal screen
    }

    endwin();

#if (DB_WAV_OUT)
    /* close debugging output wav file */
    sf_close(buf.sndfile);
#endif

    /* shut down Port Audio */
    shutdownPa(stream);

    return 0;
}

/* This routine will be called by the PortAudio engine when audio is needed.
 * It will be called in the "real-time" thread, so don't do anything
 * in the routine that requires significant time or resources.
 */
static int paCallback(
    const void *inputBuffer,
    void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo *timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData) {
    Buf *pb = (Buf *)userData; /* Cast pointer to data passed through stream */
    Synth *ps = pb->ps;        /* struct Synth */
    float *output = (float *)outputBuffer;
    // float *input = (float *)inputBuffer; /* input not used in this code */

    /* synthesize tones */
    // Your Code Here

    // initialization declaration*/
    double *syn_buf;

    /* Get synthesized block */
    syn_buf = synth_block(ps, framesPerBuffer);

    /* Write synthesized samples to output buffer */
    for (unsigned long i = 0; i < framesPerBuffer; i++) {
        // Left channel
        output[i * pb->num_chan] = (float)syn_buf[i];
        // Right channel (same as left for mono signal)
        output[i * pb->num_chan + 1] = (float)syn_buf[i];
    }

#if (DB_WAV_OUT)
    /* write to debugging file */
    sf_writef_float(pb->sndfile, output, framesPerBuffer);
#endif

    return 0;
}
