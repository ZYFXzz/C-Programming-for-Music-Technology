#include "synth.h"
#include <math.h> //for sin()
#include <stdio.h>
#include <stdlib.h> //for exit()

void init_synth(Synth *ps, int in_samp_rate) {
    Tone *pt = &ps->tone[0]; /* Tone structure */
    /* initialize class data */
    ps->samp_rate = in_samp_rate;
    ps->num_keys = 0; // number of keys voiced
    for (int i = 0; i < KEYS_VOICED; i++) {
        // space, which is printed in Ncurses loop
        pt[i].key = ' ';
    }
}

void execute_cmd(Synth *ps, int in_cmd) {
    switch (in_cmd) {
    case (CMD_ADD_KEY):
        add_key(ps, ps->new_key, ps->new_freq);
        break;
    case (CMD_RM_KEY):
        rm_key(ps);
        break;
    }
}

double *synth_block(Synth *ps, int framesPerBuffer) {
    // Your Code Here
    if (ps->cmd > 0) {
        int local_cmd = ps->cmd;
        execute_cmd(ps, local_cmd);
        ps->cmd = 0;
    }
    Tone *pt = &ps->tone[0];

    /* Synthesize a block of audio samples */
    for (int i = 0; i < framesPerBuffer; i++) {
        double v = 0.0; /* Initialize output value to zero */

        /* Loop through all active tones */
        for (int n = 0; n < ps->num_keys; n++) {
            if (pt[n].phase_inc > -1) {
                /* Calculate sine wave sample with attack and decay envelope */
                v += FS_AMPL * sin(pt[n].phase) * (1.0 - pt[n].attack_amp) * pt[n].decay_amp;

                /* Increment phase */
                pt[n].phase += pt[n].phase_inc;

                /* Wrap phase to keep within 0 to 2*PI */
                if (pt[n].phase >= 2.0 * PI) {
                    pt[n].phase -= 2.0 * PI;
                }

                /* Update attack */
                pt[n].attack_amp *= pt[n].attack_factor;

                /* Update decay */
                pt[n].decay_amp *= pt[n].decay_factor;

                /* Stop tone if amplitude drops below drop level */
                if (pt[n].decay_amp < DROP_LEVEL) {
                    pt[n].phase_inc = -1;
                }
            }
        }

        /* Store the synthesized sample */
        ps->output[i] = v;
    }
    return (&ps->output[0]);
}

void init_key(Synth *ps, int new_key, double new_freq) {
    /* add new key information at num_keys-1 index in Tone array */
    int n;
    double fs, f0;
    Tone *pt = &ps->tone[0];

    n = ps->num_keys - 1; /* from number to index */
    if (n < 0) {
        /* since init_key() is called after ++num_keys, this should not occur */
        fprintf(stderr, "ERROR in init_key(): num_keys %d\n", ps->num_keys);
        exit(-1);
    }
    // Your code here
    f0 = new_freq;
    fs = ps->samp_rate;

    /* Initialize the values of the Tone array at index n */
    pt[n].key = new_key;
    pt[n].f0 = new_freq;
    pt[n].phase_inc = 2.0 * PI * f0 / fs;
    pt[n].phase = 0.0;
    pt[n].attack_factor = ATTACK_FACTOR;
    pt[n].decay_factor = DECAY_FACTOR;
    pt[n].attack_amp = 1.0;
    pt[n].decay_amp = 1.0;
}

void add_key(Synth *ps, int in_new_key, double in_new_freq) {
    ps->new_key = in_new_key;
    ps->new_freq = in_new_freq;
    /* increment number of keys */
    if (++(ps->num_keys) > KEYS_VOICED) {
        /* list is full, so remove oldest key by shifting key list */
        shift_keys(ps);
        ps->num_keys = KEYS_VOICED;
    }
    /* enter new key info */
    init_key(ps, in_new_key, in_new_freq);
}

void rm_key(Synth *ps) {
    /* remove oldest key by shifting key list down */
    shift_keys(ps);
    /* decreast number of keys voicing */
    /* number of keys can never be less than zero */
    if (--(ps->num_keys) < 0) {
        ps->num_keys = 0;
    }
}

void shift_keys(Synth *ps) {
    Tone *pt = &ps->tone[0];
    /* shift tone array values down by one place */
    for (int i = 0; i < KEYS_VOICED - 1; i++) {
        pt[i].key = pt[i + 1].key;
        pt[i].f0 = pt[i + 1].f0;
        pt[i].phase_inc = pt[i + 1].phase_inc;
        pt[i].phase = pt[i + 1].phase;
        pt[i].attack_factor = pt[i + 1].attack_factor;
        pt[i].decay_factor = pt[i + 1].decay_factor;
        pt[i].attack_amp = pt[i + 1].attack_amp;
        pt[i].decay_amp = pt[i + 1].decay_amp;
    }
    /* top array entry is initialized to inactive */
    int n = KEYS_VOICED - 1;
    pt[n].key = ' ';
    pt[n].phase_inc = -1;
}
