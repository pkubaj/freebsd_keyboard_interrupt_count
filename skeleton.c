/*
 * KLD Skeleton
 * Inspired by Andrew Reiter's Daemonnews article
 */

#include <sys/param.h>  /* defines used in kernel.h */
#include <sys/sysctl.h>
#include <sys/module.h>
#include <sys/kernel.h> /* types used in module initialization */
#include <sys/systm.h>

/*
 * Load handler that deals with the loading and unloading of a KLD.
 */

extern char *intrnames;
extern u_long *intrcnt;
extern int nintrcnt;
static u_long *kbd_intr = 0;
static u_int kbd_intr_reset = 0;
static struct bintime kbd_intr_reset_time = { 0 };

static int
sysctl_handle_kbd_intr(SYSCTL_HANDLER_ARGS)
{
  int error;
  u_long value;

  value = **(u_long **)arg1;
  error = sysctl_handle_long(oidp, &value, 0, req);
  if (error != 0 || req->newptr == NULL) return (error);
  if (value < 0) return (EINVAL);
  *(u_long *)arg1 = value;
  return 0;
}

static int
sysctl_handle_kbd_intr_reset(SYSCTL_HANDLER_ARGS)
{
  int error;
  u_int value;

  value = *(u_int *)arg1;
  error = sysctl_handle_int(oidp, &value, 0, req);
  if (error != 0 || req->newptr == NULL) return (error);
  if (value != 1) return (EINVAL);
  *(u_long *)kbd_intr = 0;
  bintime(&kbd_intr_reset_time);
  return 0;
}

SYSCTL_PROC(_debug, OID_AUTO, kbd_intr, CTLFLAG_RD | CTLTYPE_ULONG, &kbd_intr, 0, sysctl_handle_kbd_intr, "LU", "Keyboard interrupt counter");
SYSCTL_PROC(_debug, OID_AUTO, kbd_intr_reset, CTLFLAG_RW | CTLTYPE_UINT, &kbd_intr_reset, 1, sysctl_handle_kbd_intr_reset, "IU", "Keyboard interrupt counter reset");
SYSCTL_LONG(_debug, OID_AUTO, kbd_intr_reset_time, CTLFLAG_RD, &kbd_intr_reset_time.sec, 0, "Keyboard interrupt counter reset time");

static int
skel_loader(struct module *m, int what, void *arg)
{
  int err = 0, word = 0, correct_word = 0;

  switch (what)
  {
    case MOD_LOAD:                /* kldload */

    for(int i = 0; i < nintrcnt; i++)
    {
      if(intrnames[i] == ':') word++;
//      if(strncmp(&intrnames[i], "atkbd0", 6) == 0)
      if(strncmp(&intrnames[i], "xhci0", 5) == 0)
      {
        correct_word = word;
//        for(int j = -6; j < 6; j++) uprintf("%c", intrnames[i+j]);
        for(int j = -9; j < 5; j++) uprintf("%c", intrnames[i+j]);
      }
    }
      uprintf(": %lu\n", intrcnt[correct_word]);
      kbd_intr = &intrcnt[correct_word];
      break;
    case MOD_UNLOAD:
      break;
    default:
      err = EOPNOTSUPP;
      break;
    }
    return(err);
}

/* Declare this module to the rest of the kernel */

static moduledata_t skel_mod = {
  "skel",
  skel_loader,
  NULL
};

DECLARE_MODULE(skeleton, skel_mod, SI_SUB_KLD, SI_ORDER_ANY);
