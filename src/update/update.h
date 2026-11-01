/* In-game updates from the release-publish service. Windows only; see docs/RELEASE.md. */
#ifndef LOFTA_UPDATE_H
#define LOFTA_UPDATE_H

#include <stddef.h>

/* Handles the --update* test flags before any window opens, and cleans up after an update.
 * Returns an exit code, or -1 when the game should start normally. */
int update_run_command_line(int argc, char **argv);

/* Checks now and every few hours on a background thread; a newer build is downloaded and
 * verified first, and only then offered. */
void update_start_background(void);

/* "off", "idle", "checking", "latest", "downloading", "ready" or "failed"; offered gets the
 * version name when one is ready. */
const char *update_status(char *offered, size_t size);

/* The player said yes: the main loop leaves, then update_finish_install swaps the program. */
void update_request_install(void);
void update_shutdown(void);
/* Returns the process exit code. */
int update_finish_install(void);

#endif
