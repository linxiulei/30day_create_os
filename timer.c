#include "bootpack.h"

#define PIT_CTRL  0x0043
#define PIT_CNT0  0x0040

struct TIMERCTL timerctl;

#define TIMER_FLAGS_ALLOC   1
#define TIMER_FLAGS_USING   2

void init_pit(void)
{
   int i;
   io_out8(PIT_CTRL, 0x34);
   io_out8(PIT_CNT0, 0x9c);
   io_out8(PIT_CNT0, 0x2e);
   timerctl.count = 0;
   //timerctl.next = 0xffffffff; 
   //timerctl.using = 0;
   //for (i = 0; i < MAX_TIMER; i++) {
   //    timerctl.timers0[i].flags = 0;
   //}
   return;
}

void inthandler20(int *esp)
{
    io_out8(PIC0_OCW2, 0x60);
    timerctl.count++;
    return;
}
