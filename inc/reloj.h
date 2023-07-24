#include <stdbool.h>
#include <stdint.h>

#define MIN_POSPONER 5

typedef struct clock_s *clock_puntero;
typedef struct alarm_s *alarm_t;

typedef void (*Alarm_activate)(clock_puntero clock);

clock_puntero ClockCreate(int tics_por_segundo, Alarm_activate alarma);

bool ClockGetTime(clock_puntero reloj, uint8_t hora[], int size);
bool ClockSetTime(clock_puntero reloj, const uint8_t hora[], int size);

bool ClockNewTick(clock_puntero clock);
void ClockNewSec(clock_puntero clock);
void ClockTenSec(clock_puntero clock);
void ClockNewMin(clock_puntero clock);
void ClockTenMin(clock_puntero clock);
void ClockNewHour(clock_puntero clock);
void ClockTenHour(clock_puntero clock);
void ClockNewDay(clock_puntero clock);

bool ClockGetAlarm(clock_puntero reloj, uint8_t alarma[], int size);
bool ClockSetUpAlarm(clock_puntero reloj, const uint8_t alarma[], int size);

void ClockActivateSnoozeAlarm(clock_puntero clock);
void ClockSnoozeAlarm(clock_puntero clock);
void ClockAlarmDeactivate(clock_puntero clock);
void ClockAlarmActivate(clock_puntero clock);
bool ClockAlarmState(clock_puntero clock);