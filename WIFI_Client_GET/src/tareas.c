#include "tareas.h"
#include "simba.h"

static struct event_t event_tarea1;

uint32_t mask = 0x01;


void Tarea1 ()
{
    for (;;)
    {
        event_read (&event_tarea1,&mask, sizeof(mask));
        std_printf ("TAREA 1\r\n");
    }
 

}