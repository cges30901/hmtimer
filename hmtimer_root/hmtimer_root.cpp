#include <iostream>
#include <linux/kd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string>
#include <sys/reboot.h>
#include <unistd.h>
using namespace std;

int main()
{
    string command;
    while(cin>>command)
    {
        if(command=="beep")
        {
            int fd = open("/dev/console", O_RDONLY);
            ioctl(fd, KDMKTONE, (200<<16 | 1193180/750));
        }
        else if(command=="shutdown"){
            sync();
            reboot(RB_POWER_OFF);
        }
        else if(command=="reboot"){
            sync();
            reboot(RB_AUTOBOOT);
        }
    }
    return 0;
}
