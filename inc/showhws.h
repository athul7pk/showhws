#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/sysinfo.h>
#include  <signal.h>
#define PROCESSOR_W 14
#define PROCESSOR_H 17
#define CORE_W (PROCESSOR_W - 2)
#define CORE_H (PROCESSOR_H / 5)
typedef enum color
{
    BLACK = 0,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    BROWN = 52,
    SKYBLUE = 81,
    ROSE = 163,
    YELLOW_LT = 221,
    WHITE = 255,
} color_t;
typedef struct cpuinfo
{
    int cpu;
    int l1dc;
    int l1ic;
    int l2;
    int l3;
    int numcache;
    float clock;
} cpuinfo_t;
typedef struct cpu_stat
{
    long double user, nice, system, idle, loadavg, user1, nice1, system1, idle1;
} cpu_stat_t;

void gotoxy(int x, int y);
void draw_sqr(int x, int y, int width, int height, int color);
int get_ram_info();
void fill_cpu_info(cpuinfo_t *pcpu);
void draw_package(cpuinfo_t *pcpu);
void list_nw_ifaces();
void show_cpu_usage(int numcpus);