#include "showhws.h"
int MAX_WIDTH = (70);
int MAX_HEIGHT = (70);

int num_cpus = 8;
void gotoxy(int x, int y)
{
    printf("\e[%d;%df", y, x);
}
void draw_total_ram()
{
    gotoxy(1, 0);
    printf("\e[48;5;25mRAM %d KB \e[0m", get_ram_info());
}
//void draw_
void draw_sqr(int x, int y, int width, int height, int color)
{

    int i = 0, j = 0;
    for (i = 0; i < width; i++)
    {
        for (j = 0; j < height; j++)
        {
            gotoxy(i + x, y + j);
            printf("\e[48;5;%dm ", color);
        }
    }
}

void add_text(int color, int bg_flag, const char *txt, int box)
{
    gotoxy(2, 45);
    printf("\e[1;%dm%s", color, txt);
    printf("\e[0;%dm%s", color, txt);
}
int prev_x = 2, prev_y = 4;

cpuinfo_t *cpus;

void draw_package(cpuinfo_t *pcpu)
{


    gotoxy(prev_x, PROCESSOR_H + 4);
    printf("%.2fMhz\e[0m", pcpu->clock);

    draw_sqr(prev_x, prev_y, PROCESSOR_W, PROCESSOR_H, BROWN); //main box Processor
    draw_sqr(prev_x + 1, PROCESSOR_H, CORE_W, CORE_H, GREEN);  //cpu core
    gotoxy(prev_x + 2, PROCESSOR_H + 1);
    printf("\e[48:5:162mCore %d\e[0m", pcpu->cpu);
    draw_sqr(prev_x + 1, PROCESSOR_H - CORE_H - 1, CORE_W / 2, CORE_H, BLUE); //cpu cacheL1 Inst
    gotoxy(prev_x + 1, PROCESSOR_H - CORE_H);
    printf("\e[0;1m%d\e[0m", pcpu->l1ic);
    draw_sqr(prev_x + 1 + (CORE_W / 2), PROCESSOR_H - CORE_H - 1, CORE_W / 2, CORE_H, SKYBLUE); //cpu cacheL1 Data
    gotoxy(prev_x + 1 + (CORE_W / 2), PROCESSOR_H - CORE_H);
    printf("\e[0;1m%d\e[0m", pcpu->l1dc);

    draw_sqr(prev_x + 1, PROCESSOR_H - 2 * CORE_H - 1, CORE_W, CORE_H, ROSE); //cpu cacheL2
    gotoxy(prev_x + 1, PROCESSOR_H - 2 * CORE_H);
    printf("\e[0;1mL2 %d\e[0m", pcpu->l2);

    draw_sqr(prev_x + 1, PROCESSOR_H - 3 * CORE_H - 1, CORE_W, CORE_H, YELLOW_LT); //cpu cacheL3
    gotoxy(prev_x + 1, PROCESSOR_H - 3 * CORE_H);
    printf("\e[0;1mL3 %d\e[0m", pcpu->l2);

    prev_x += PROCESSOR_W + 1;

    if (prev_x >= (MAX_WIDTH / 6) * PROCESSOR_W)
    {
        printf("Redraw,Error many CPUs\r\n");
    }
}
void exit_handler(){
    gotoxy(0, 0);
    system("clear");
    printf("\e[0m\r\n");
    exit(1);
}
int main(int argc, char *argv[])
{

    int i = 0, j = 0;

    struct winsize w;
    signal(SIGINT,exit_handler);
    signal(SIGQUIT,exit_handler);
    signal(SIGKILL,exit_handler);
    signal(SIGHUP,exit_handler);
    atexit(exit_handler);
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    MAX_HEIGHT = w.ws_row;
    MAX_WIDTH = w.ws_col - 20;
    draw_sqr(0, 0, MAX_WIDTH, MAX_HEIGHT, WHITE);
    draw_total_ram();
    num_cpus = get_nprocs_conf();
    cpus = calloc(num_cpus , sizeof(cpuinfo_t));

    char buff[25];
    i = 0;
    FILE *fdp = popen("cat /proc/cpuinfo | grep MHz", "r");
    if (fdp > 0)
    {

        while (fgets(buff, sizeof(buff), fdp))
        {
            sscanf(buff, "cpu MHz         :%f", &cpus[i++].clock);
        }

        fclose(fdp);
    }

    for (i = 0; i < num_cpus; i++)
    {
        cpus[i].cpu = i;
        fill_cpu_info(cpus + i);
        draw_package(cpus + i);
    }
    free(cpus);

   
    list_nw_ifaces();
    show_cpu_usage(num_cpus);
    gotoxy(0, MAX_HEIGHT);
    
    exit(0);
    while (i++ < 255)
    {
        printf("\e[38;5;%dm %03d ", i, i);
        if ((i % 20) == 0)
        {
            printf("\r\n");
        }
    }
    i = 0;
    printf("\r\n");
    while (i++ < 255)
    {
        printf("\e[31;5;%dm %03d ", i, i);
        if ((i % 20) == 0)
        {
            printf("\r\n");
        }
    }

    printf("\r\n");
}