#include "kernel/timer.h"
#include "kernel/gic.h"
#include "drivers/uart.h"

static volatile uint64_t tick_count = 0;
static uint64_t reload_ticks = 0;

void timer_init(uint32_t interval_ms) {
    // read counter freq
    uint64_t freq;
    asm volatile("mrs %0, cntfrq_el0" : "=r"(freq));

    // calculate the no.of ticks for the desired interval
    reload_ticks = (freq * (uint64_t)interval_ms) / 1000ULL;

    // write countdown value, the timer fires once the counter has advanced by this many ticks from now
    asm volatile("msr cntp_tval_el0, %0" :: "r"(reload_ticks));

    // enable the timer and unmask its interrput (bit 0 = enable, bit 1 = IMASK = 0 means the interrupt is NOT masked)
    asm volatile("msr cntp_ctl_el0, %0" :: "r"((uint64_t)1));

    // enable INTID 30 in the GIC so the timer can signal the CPU
    gic_enable_irq(TIMER_IRQ);
}

void timer_tick(void) {
    tick_count++;
    
    // print a simple tick message
    kprint("tick #");

    // quick inline decimal print for small numbers
    uint64_t n = tick_count;
    char buf[20];
    int i = 0;
    if (n == 0) { buf[i++] = '0'; }
    else {
        while (n > 0) { buf[i++] = '0' + (n % 10); n /= 10; }
        // reverse
        for (int l = 0, r = i - 1; l < r; l++, r--) {
            char t = buf[l]; buf[l] = buf[r]; buf[r] = t;
        }
    }

    buf[i] = '\0';
    kprint(buf);
    kprint("\n");

    // re-arm: write the same countdowm so the timer fires again
    asm volatile("msr cntp_tval_el0, %0" :: "r"(reload_ticks));
}

uint64_t timer_get_ticks(void) {
    return tick_count;
}