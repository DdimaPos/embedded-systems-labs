#ifndef LAB5_H
#define LAB5_H

// ── Acquisition task ──────────────────────────────────────────────────────────
// DHT11 lib MIN_INTERVAL=2000ms; period must be ≥2000ms to force a real read each cycle.
#define ACQ_PERIOD_MS   2000
#define ACQ_OFFSET_MS   0
#define ACQ_STACK_SIZE  4096
#define ACQ_PRIORITY    3

// ── Conditioning task ─────────────────────────────────────────────────────────
// Runs faster than acquisition to apply anti-bouncing at a finer time
// resolution (DEBOUNCE_COUNT × COND_PERIOD_MS = minimum confirmation window).
#define COND_PERIOD_MS  200
#define COND_OFFSET_MS  100
#define COND_STACK_SIZE 2048
#define COND_PRIORITY   2

// ── Reporting task ────────────────────────────────────────────────────────────
#define RPT_PERIOD_MS   500
#define RPT_OFFSET_MS   250
#define RPT_STACK_SIZE  4096
#define RPT_PRIORITY    1

void lab5Setup();
void lab5Loop();

#endif
