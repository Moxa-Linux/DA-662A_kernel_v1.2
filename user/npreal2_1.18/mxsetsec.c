#include "nport.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define     ER_ARG  -10

typedef struct _CONINFO
{
    int idx;
    char ipaddr[40];
    char data[10];
    char cmd[10];
    char fifo[10];
    int secure;
    char tty[10];
    char cout[10];
    int num;
	char scope_id[10];
}
CONINFO;

char svrList[256][40];
int total[256];
int idx, change_flag;
CONINFO	info[256];

int SetSecure(int index);
int SelectNPort();

char* GetIP(unsigned long ip, char *ret)
{
    struct in_addr ad;

    ad.s_addr = ip;
    sprintf(ret, "%s", inet_ntoa(ad));

    return;
}

int SetSecure(int index)
{
    int i, j, s, start, ret, page=0;
    struct in_addr ad;
    unsigned long ip;
    char	c[5];

    for (i = 0; i < 256; i++)
    {
        if (!strcmp(info[i].ipaddr, svrList[index]))
            break;
    }

    while (1)
    {
	int tot=0;
        system("clear");
        printf("Set Security Parameter ...\n");
        printf("<<Port Selection>>\n");
        printf("\n[Index]\t%-40s\t[Port]\t[Security]\n", "[Server IP]");
        s = 0;
        while (!strcmp(svrList[index], info[s+i].ipaddr))
        {
	    tot++;
            if (info[s+i].num > (page+1)*16 || info[s+i].num <= page*16)
	    {
		s++;
                continue;
	    }
            if (info[s+i].secure)
                printf("  (%d)\t%-40s\t  %d\t  Yes", s+1, info[s+i].ipaddr, info[s+i].num);
            else
                printf("  (%d)\t%-40s\t  %d\t  No",  s+1, info[s+i].ipaddr, info[s+i].num);
	    printf("\n");
            s++;
        }

        if(info[i].secure == 0)
            printf("  (a)\tEnable all port\n");
        else
            printf("  (a)\tDisable all port\n");

	if (tot > 16)
        {
            if (page == 0)
		printf("  (n)\tnext page\n");
            if (page == 1)
                printf("  (p)\tprevious page\n");
        }
        printf("  (q)\tback to model selection\n");
        printf("\nPlease select a port you want to change security setting: ");

        scanf("%s", c);

        if ((c[0] == 'q') || (c[0] == 'Q'))
        {
            break;
        }
        if ((c[0] == 'n') || (c[0] == 'N'))
        {
            if (page == 0 && tot>16)
                page++; 
        }
        if ((c[0] == 'p') || (c[0] == 'P'))
        {
            if (page == 1 && tot>16)
                page--; 
        }
        else if ((c[0] == 'a') || (c[0] == 'A'))
        {
            int secure = !info[i].secure;
            for(j=0; j<tot; j++)
            {
                info[i+j].secure = secure;
            }
            change_flag = 1;
        }
        else if ((atoi(c) > 0) && (atoi(c) <= s))
        {
            j = atoi(c) - 1;
            change_flag = 1;
            if (info[i+j].secure)
                info[i+j].secure = 0;
            else
                info[i+j].secure = 1;
        }
    }

    return 1;
}

int SelectNPort()
{

    int i, ret;
    struct in_addr ad;
    char c[5];

    system("clear");
    printf("Set Security Parameter ...\n");
    printf("<<Model Selection>>\n");

    printf("\n[Index]\t%-40s\t[Port(s)]\n", "[Server IP]");
    for (i=0; i<idx; i++)
    {
        printf("  (%d)\t%-40s\t  %d\n", i+1, svrList[i], total[i]);
    }

	printf("  (q)\tExit \n");
    printf("\nPlease select a model you want to set up: ");
    scanf("%s", c);

    if ((c[0] == 'q') || (c[0] == 'Q'))
        return -1;
    if (atoi(c)<=0 || atoi(c)>idx)
        return 0;
    else
    {
        ret = SetSecure(atoi(c)-1);
    //    SelectNPort();
    }
    return 0;
}

int main(int arg, char *argv[])
{
    int i, j;
    int len, daemon, num, ret;
    char *tmpstr, *tmp, *os;
    char token[40], tty[10], cout[10], major[20], del[16], sec[10], index[10];
    char data[10], cmd[10], fifo[10], scope[10];
    FILE *f, *ft;

    idx = 0;
    daemon = 0;
    change_flag = 0;
    tmpstr = (char *)malloc(1024);
    len = 1024;
    tmp = (char *)malloc(20);

    memset(scope, 0x0, sizeof(scope));
    memset(svrList, 0x0, 256*40);
    memset(total, 0x0, 256*sizeof(int));
    memset(info, 0x0, 256*sizeof(CONINFO));
    sprintf(tmpstr, "%s/mxcfmat", DRIVERPATH);
    system(tmpstr);

#if 0 // Jared, 2013-07-30
    sprintf(tmpstr, "%s/npreal2d.cf", DRIVERPATH);
#else
    sprintf(tmpstr, "/etc/npreal2d/npreal2d.cf");
#endif
    f = fopen (tmpstr, "r");
    if (f == NULL)
    {
        printf("file open error\n");
        free(tmpstr);
        free(tmp);
        return(0);
    }

    /* print the list of installed server */
    for (;;)
    {
        if (getline (&tmpstr, &len, f) < 0)
        {
            break;
        }
        if (strstr(tmpstr, "#") != NULL)
        {
            continue;
        }
        memset(major, '\0', 20);
        sscanf(tmpstr, "%s", major);
        if (strstr(major, "ttymajor") != NULL ||
                strstr(major, "calloutmajor") != NULL )
        {
            continue;
        }

        sscanf(tmpstr, "%s%s%s%s%s%s%s%s%s", index, token, data, cmd, fifo, sec, tty, cout, scope);
        num = atoi(index);
        for (i=0; i<idx; i++)
        {
            if (!strcmp(svrList[i],token))
            {
                total[i]++;
                info[num].idx = num;
                strcpy(info[num].ipaddr, token);
                strcpy(info[num].data, data);
                strcpy(info[num].cmd, cmd);
                strcpy(info[num].fifo, fifo);
                info[num].secure = atoi(sec);
                strcpy(info[num].tty, tty);
                strcpy(info[num].cout, cout);
                info[num].num = total[i];
                strcpy(info[num].scope_id, scope);
                break;
            }
        }
        if (i == idx)
        {
            strcpy(svrList[idx], token);
            total[idx]++;
            info[num].idx = num;
            strcpy(info[num].ipaddr, token);
            strcpy(info[num].data, data);
            strcpy(info[num].cmd, cmd);
            strcpy(info[num].fifo, fifo);
            info[num].secure = atoi(sec);
            strcpy(info[num].tty, tty);
            strcpy(info[num].cout, cout);
            info[num].num = total[i];
            strcpy(info[num].scope_id, scope);
            idx++;
        }
    }
    fclose (f);

    if (idx == 0)
    {
        printf("No NPort server is installed.\n\n");
        free(tmpstr);
        free(tmp);
        return 0;
    }

    while(SelectNPort() == 0)
    {}

    printf("Exit!!\n\n");

#if 0 // Jared, 2013-07-30
    sprintf(tmpstr, "%s/npreal2d.cf", DRIVERPATH);
#else
    sprintf(tmpstr, "/etc/npreal2d/npreal2d.cf");
#endif
    f = fopen (tmpstr, "r");
    if (f == NULL)
    {
        printf("file open error\n");
        free(tmpstr);
        free(tmp);
        return(0);
    }
    ft = fopen ("/tmp/nprtmp_cf", "w");
    if (ft == NULL)
    {
        printf("file open error\n");
        free(tmpstr);
        free(tmp);
        return(0);
    }

    for (;;)
    {
        if (getline (&tmpstr, &len, f) < 0)
        {
            break;
        }
        if (strstr(tmpstr, "#") != NULL)
        {
            fputs (tmpstr, ft);
            continue;
        }
        memset(major, '\0', 20);
        sscanf(tmpstr, "%s", major);
        if (strstr(major, "ttymajor") != NULL ||
                strstr(major, "calloutmajor") != NULL )
        {
            fputs (tmpstr, ft);
            continue;
        }
        for (i=0; i<256; i++)
        {
            if (strlen(info[i].ipaddr) > 0)
	    {
                sprintf (tmpstr, "%d\t%s\t%s\t%s\t%s\t%d %s\t%s\t%s\n", info[i].idx, info[i].ipaddr,
                info[i].data, info[i].cmd, info[i].fifo, info[i].secure, info[i].tty, info[i].cout, info[i].scope_id);
                fputs (tmpstr, ft);

                /* daemon is a flag which is used to delete the */
                /* daemon start string in /etc/rc.d/rc.local */
                if (change_flag)
                    daemon = 1;
            }
        }
        break;
    }

    fclose(ft);
    fclose (f);

    os = "linux";
    f = fopen ("/etc/redhat-release", "r");
    if (f != NULL)
    {
        fclose(f);
        os = "linux";
    }
    else
    {
        f = fopen ("/etc/SuSE-release", "r");
        if (f != NULL)
        {
            fclose(f);
            os = "SuSE";
        }
        else
        {
            f = fopen ("/etc/debian_version", "r");
            if (f != NULL)
            {
                os = "debian";
            }
        }
    }

    if (!daemon)
    {
        if (os == "linux")
        {
            system("grep -v mxloadsvr /etc/rc.d/rc.local > /tmp/nprtmp_rclocal");
            system("cp -f /tmp/nprtmp_rclocal /etc/rc.d/rc.local > /dev/null 2>&1");
            system("rm -f /tmp/nprtmp_rclocal");

        }
        else if (os == "debian")
        {
            system("grep -v mxloadsvr /etc/init.d/npreals > /tmp/nprtmp_rclocal");
            system("cp -f /tmp/nprtmp_rclocal /etc/init.d/npreals > /dev/null 2>&1");
            system("rm -f /tmp/nprtmp_rclocal");
            system("update-rc.d npreals defaults 90");

        }
        else if (os == "SuSE")
        {
            system("grep -v mxloadsvr /etc/rc.d/boot.local > /tmp/nprtmp_rclocal");
            system("cp -f /tmp/nprtmp_rclocal /etc/rc.d/boot.local > /dev/null 2>&1");
            system("rm -f /tmp/nprtmp_rclocal");

        }
    }

#if 0 // Jared, 2013-07-30
    sprintf(tmpstr, "cp -f /tmp/nprtmp_cf %s/npreal2d.cf", DRIVERPATH);
#else
    sprintf(tmpstr, "cp -f /tmp/nprtmp_cf /etc/npreal2d/npreal2d.cf");
#endif
    system(tmpstr);
    system("rm -f /tmp/nprtmp_cf");

    sprintf(tmpstr, "%s/mxloadsvr", DRIVERPATH);
    system(tmpstr);
    if (os == "linux")
    {
        system("chmod +x /etc/rc.d/rc.local");
    }
    else if (os == "debian")
    {
        system("chmod +x /etc/init.d/npreals");
    }
    else if (os == "SuSE")
    {
        system("chmod +x /etc/rc.d/boot.local");
    }

    free(tmpstr);
    free(tmp);
    return 0;
}


