/* The updater alone, as a console program, for end-to-end runs on real Windows: the same
 * update_win.c and relhub.c the game links, without a window. See tools/relhub_e2e.py. */
#include <stdio.h>

#include "update.h"

int main(int argc, char **argv)
{
    int status = update_run_command_line(argc, argv);

    if (status < 0) {
        fprintf(stderr, "usage: %s --update=URL [--update-channel=C] [--update-keys=K] ...\n", argv[0]);
        return 2;
    }
    return status;
}
