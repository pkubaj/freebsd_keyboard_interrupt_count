#include <errno.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <stdio.h>

int main()
{
  u_long kbd_intr = 0, time = 0;
  size_t size = sizeof(kbd_intr);
  int err;
  u_int intr_reset = 1;
  
  err = sysctlbyname("debug.kbd_intr", &kbd_intr, &size, NULL, 0);
  if(err == -1)
  {
    perror("Error: ");
    return errno;
  }
  printf("Interrupt count: %lu\n", kbd_intr);
  
  err = sysctlbyname("debug.kbd_intr_reset", NULL, NULL, &intr_reset, sizeof(intr_reset));
  if(err == -1)
  {
    perror("Error: ");
    return errno;
  }

  err = sysctlbyname("debug.kbd_intr_reset_time", &time, &size, NULL, 0);
  if(err == -1)
  {
    perror("Error: ");
    return errno;
  }
  printf("Reset time: %lu\n", time);
  return 0;
}
