#include <sys/sysctl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <net/if.h>
#include "showhws.h"
extern int MAX_WIDTH ;
extern int MAX_HEIGHT;
 int  get_ram_info()
{
    FILE *meminfo = fopen("/proc/meminfo", "r");
    int  ramKB = 0;
    if(meminfo == NULL) {
        perror("open:");
        return (-1);
    }
    char buff[256];
    while(fgets(buff, sizeof(buff), meminfo))  {
        
        if(sscanf(buff, "MemTotal: %d kB", &ramKB) == 1){
            break;
        }
    }
    if(fclose(meminfo) != 0)  {
        perror("close:");
        return (-1);
    }
    return (ramKB);
}


void fill_cpu_info(cpuinfo_t *pcpu)
{
    FILE *fp = NULL, *fdp = NULL;
    char buff[25], FILE_PATH[70];
    sprintf(FILE_PATH, "/sys/devices/system/cpu/cpu%d/cache/index0/type", pcpu->cpu);
    fp = fopen(FILE_PATH, "r");
    if (fp > 0)
    {
        while (fgets(buff, sizeof(buff), fp))
        {
            if (0 == strncmp("Data", buff, 4))
            {
                sprintf(FILE_PATH, "/sys/devices/system/cpu/cpu%d/cache/index0/size", pcpu->cpu);
                fdp = fopen(FILE_PATH, "r");
                if (fdp > 0)
                {
                    while (fgets(buff, sizeof(buff), fdp))
                    {
                        int dc;
                        if (sscanf(buff, "%dK", &dc) == 1)
                        {
                            pcpu->l1dc = dc;
                        }
                    }
                    fclose(fdp);
                }
            }
        }
        fclose(fp);
    }

    sprintf(FILE_PATH, "/sys/devices/system/cpu/cpu%d/cache/index1/type", pcpu->cpu);
    fp = fopen(FILE_PATH, "r");
    if (fp > 0)
    {
        while (fgets(buff, sizeof(buff), fp))
        {
            if (0 == strncmp("Instruction", buff, 4))
            {
                sprintf(FILE_PATH, "/sys/devices/system/cpu/cpu%d/cache/index1/size", pcpu->cpu);
                fdp = fopen(FILE_PATH, "r");
                if (fdp > 0)
                {
                    while (fgets(buff, sizeof(buff), fdp))
                    {
                        int dc;
                        if (sscanf(buff, "%dK", &dc) == 1)
                        {
                            pcpu->l1ic = dc;
                        }
                    }
                    fclose(fdp);
                }
            }
        }
        fclose(fp);
    }

    sprintf(FILE_PATH, "/sys/devices/system/cpu/cpu%d/cache/index2/size", pcpu->cpu);
    fdp = fopen(FILE_PATH, "r");
    if (fdp > 0)
    {
        while (fgets(buff, sizeof(buff), fdp))
        {
            int dc;
            if (sscanf(buff, "%dK", &dc) == 1)
            {
                pcpu->l2 = dc;
            }
        }
        fclose(fdp);
    }
    sprintf(FILE_PATH, "/sys/devices/system/cpu/cpu%d/cache/index3/size", pcpu->cpu);
    fdp = fopen(FILE_PATH, "r");
    if (fdp > 0)
    {
        while (fgets(buff, sizeof(buff), fdp))
        {
            int dc;
            if (sscanf(buff, "%dK", &dc) == 1)
            {
                pcpu->l3 = dc;
            }
        }
        fclose(fdp);
    }
}


void list_nw_ifaces()
{
  struct if_nameindex *if_nidxs, *intf;
int x=1,i=0;
    if_nidxs = if_nameindex();
    

    if ( if_nidxs != NULL )
    {
        for (intf = if_nidxs; intf->if_index != 0 || intf->if_name != NULL; intf++)
        {
            gotoxy(x, MAX_HEIGHT-5);
            for(i=0;i<MAX_WIDTH/3;i++)
            printf("\e[48;5;225m-");
            draw_sqr(x,MAX_HEIGHT-4,7,3,YELLOW);
            gotoxy(x, MAX_HEIGHT-3);
            printf("\e[48;5;%dm%s\e[48;5;%dm ", BLACK,intf->if_name,YELLOW);
            x+=8;
        }

        if_freenameindex(if_nidxs);
    }
    printf("\e[0m\r\n");

}