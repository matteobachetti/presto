#include "presto.h"

/*
 * The following are the parameters that are accepted in a
 * par file when trying to determine a pulsar ephemeris.
 *
  PEPOCH   Epoch of period/frequency parameters and position (MJD)
  F0       Pulsar rotation frequency (s-2)
  F        Alternative for F0
  F1       Pulsar rotation frequency derivative (s^-2)
  F2       Pulsar rotation frequency second derivative
  P0       Pulsar period (s).
  P        Alternative for P0
  P1       Pulsar period derivative (10^-15).
  DM       Dispersion measure (pc cm^-3)
  A1       Projected pulsar semi-major axis of 1st orbit
  E        Eccentricity of 1st orbit
  T0       Epoch of periastron passage of 1st orbit (MJD)
  TASC     Epoch of ascending node passage (MJD)
  PB       Period of 1st orbit (days)
  OM       Longitude of periastron passage, 2st orbit (deg)
  EPS1     First Laplace parameter [eccentricity times sin(omega)]
  EPS2     Second Laplace parameter [eccentricity times cos(omega)]
  EPS1DOT  Time derivative of EPS1
  EPS2DOT  Time derivative of EPS2
  OMDOT    Rate of periastron advance (deg/yr) 
  OM2DOT   Second time derivative of angle of periastron (rad/s^2)
  PBDOT    Rate of change of orbital period (10^-12) 
  XDOT     Rate of change of projected semi-major axis (-12)
  X2DOT    Second time derivative of projected semi-major axis (1/s)
  EDOT     Rate of change of eccentricity (-12)
*/

/* Test routine
int main(int argc, char *argv[])
{
  int retval;
  double epoch=54000.0;
  psrparams pp;
  
  retval = get_psr_from_parfile(argv[1], epoch, &pp);
  exit(0);
}
*/

#define DEBUGOUT 0

int get_psr_from_parfile(char *parfilenm, double epoch, psrparams *psr)
/* Converts info from a "par" file to the "current" epoch.  */
/* Returned values go in *psr.  The name of the parfile is  */
/* in *parfilenm. epoch is the time in question in MJD.     */
/* The int returned is 1 if successful, 0 otherwise.        */
{
  FILE *parfile;
  int binary=0;
  double difft=0.0, f=0.0, fd=0.0;
  double eps1=0.0, eps2=0.0, eps1d=0.0, eps2d=0.0;
  char line[80], *keyword, *value;

  psr->f = psr->fd = psr->p = psr->pd = psr->dm = 0.0;
  psr->orb.p = psr->orb.pd = psr->orb.x = psr->orb.e = 0.0;
  psr->orb.w = psr->orb.wd = psr->orb.t = 0.0;
  psr->ntype = 0;

  parfile = chkfopen(parfilenm, "r");
  while (fgets(line, 80, parfile)){
    keyword = strtok(line, " \t\n");
    if (strncmp("PSR", keyword, 80)==0){
      strncpy(psr->jname, strtok(NULL, " \t\n"), 20);
      if (DEBUGOUT) printf("The pulsar is '%s'\n", psr->jname);
    } else if (strncmp("RAJ", keyword, 80)==0){
      int h, m;
      double s;
      ra_dec_from_string(strtok(NULL, " \t\n"), &h, &m, &s);
      psr->ra2000 = hms2rad(h, m, s);
      if (DEBUGOUT) printf("The RA  is %d %d %f (%f)\n", h, m, s, psr->ra2000);
    } else if (strncmp("DECJ", keyword, 80)==0){
      int d, m;
      double s;
      ra_dec_from_string(strtok(NULL, " \t\n"), &d, &m, &s);
      psr->dec2000 = dms2rad(d, m, s);
      if (DEBUGOUT) printf("The DEC is %d %d %f (%f)\n", d, m, s, psr->dec2000);
    } else if (strncmp("F0", keyword, 80)==0 ||
	       strncmp("F", keyword, 80)==0){
      value = strtok(NULL, " \t\n");
      f = strtod(value, &value);
      if (DEBUGOUT) printf("The freq is %.15g\n", f);
    } else if (strncmp("P0", keyword, 80)==0 ||
	       strncmp("P", keyword, 80)==0){
      value = strtok(NULL, " \t\n");
      f = 1.0/strtod(value, &value);
      if (DEBUGOUT) printf("The period is %.15g\n", 1.0/f);
    } else if (strncmp("F1", keyword, 80)==0){
      value = strtok(NULL, " \t\n");
      fd = strtod(value, &value);
      if (DEBUGOUT) printf("The f-dot is %.15g\n", fd);
    } else if (strncmp("P1", keyword, 80)==0){
      value = strtok(NULL, " \t\n");
      fd = -strtod(value, &value)*f*f;
      if (DEBUGOUT) printf("The p-dot is %.15g\n", -fd/(f*f));
    } else if (strncmp("F2", keyword, 80)==0){
      value = strtok(NULL, " \t\n");
      psr->fdd = strtod(value, &value);
      psr->pdd = (2.0*(fd*fd)/f - psr->fdd)/(f*f);
      if (DEBUGOUT) printf("The f-dotdot is %.15g\n", psr->fdd);
    } else if (strncmp("PEPOCH", keyword, 80)==0){
      value = strtok(NULL, " \t\n");
      psr->timepoch = strtod(value, &value);
      difft = (epoch - psr->timepoch)*SECPERDAY;
      psr->f = f + fd*difft + 0.5*psr->fdd*difft*difft;
      psr->fd = fd + psr->fdd*difft;
      psr->p = 1.0/psr->f;
      psr->pd = -psr->fd*psr->p*psr->p;
      psr->pdd = (2.0*(fd*fd)/f - psr->fdd)/(f*f);
    } else if (strncmp("DM", keyword, 80)==0){
      value = strtok(NULL, " \t\n");
      psr->dm = strtod(value, &value);
      if (DEBUGOUT) printf("The DM is %.15g\n", psr->dm);
    } else if (strncmp("BINARY", keyword, 80)==0){
      binary = 1;
      value = strtok(NULL, " \t\n");
      psr->ntype += 8;
      if (DEBUGOUT) printf("This is a binary PSR ('%s')...\n", value);
    } else if (strncmp("PB", keyword, 80)==0){
      value = strtok(NULL, " \t\n");
      psr->orb.p = strtod(value, &value)*SECPERDAY;
      if (DEBUGOUT) printf("  P_orb  = %.15g\n", psr->orb.p);
    } else if (strncmp("PBDOT", keyword, 80)==0){
      value = strtok(NULL, " \t\n");
      psr->orb.pd = strtod(value, &value)*1.0E-12;
      psr->orb.p += psr->orb.pd*difft;
      if (DEBUGOUT) printf("  P_orb-dot  = %.15g\n", psr->orb.pd);
    } else if (strncmp("OM", keyword, 80)==0){
      value = strtok(NULL, " \t\n");
      psr->orb.w = strtod(value, &value);
      if (DEBUGOUT) printf("  w_orb  = %.15g\n", psr->orb.w);
    } else if (strncmp("OMDOT", keyword, 80)==0){
      value = strtok(NULL, " \t\n");
      psr->orb.wd = strtod(value, &value)/SECPERJULYR;
      psr->orb.w += psr->orb.wd*difft;
      if (DEBUGOUT) printf("  w_orb-dot  = %.15g\n", psr->orb.wd);
    } else if (strncmp("A1", keyword, 80)==0){
      value = strtok(NULL, " \t\n");
      psr->orb.x = strtod(value, &value);
      if (DEBUGOUT) printf("  x_orb  = %.15g\n", psr->orb.x);
    } else if (strncmp("XDOT", keyword, 80)==0){
      double xd;
      value = strtok(NULL, " \t\n");
      xd = strtod(value, &value)*1.0E-12;
      psr->orb.x += xd*difft;
      if (DEBUGOUT) printf("  x_orb-dot  = %.15g\n", xd);
    } else if (strncmp("E", keyword, 80)==0){
      value = strtok(NULL, " \t\n");
      psr->orb.e = strtod(value, &value);
      if (DEBUGOUT) printf("  e_orb  = %.15g\n", psr->orb.e);
    } else if (strncmp("EDOT", keyword, 80)==0){
      double ed;
      value = strtok(NULL, " \t\n");
      ed = strtod(value, &value)*1.0E-12;
      psr->orb.e += ed*difft;
      if (DEBUGOUT) printf("  e_orb-dot  = %.15g\n", ed);
    } else if (strncmp("T0", keyword, 80)==0 ||
	       strncmp("TASC", keyword, 80)==0){
      value = strtok(NULL, " \t\n");
      psr->orb.t = strtod(value, &value);
      if (DEBUGOUT) printf("  T_orb  = %.15g\n", psr->orb.t);
    } else if (strncmp("EPS1", keyword, 80)==0){
      value = strtok(NULL, " \t\n");
      eps1 = strtod(value, &value);
      if (DEBUGOUT) printf("  EPS1  = %.15g\n", eps1);
    } else if (strncmp("EPS2", keyword, 80)==0){
      value = strtok(NULL, " \t\n");
      eps2 = strtod(value, &value);
      if (DEBUGOUT) printf("  EPS2  = %.15g\n", eps2);
    } else if (strncmp("EPS1DOT", keyword, 80)==0){
      value = strtok(NULL, " \t\n");
      eps1d = strtod(value, &value);
      eps1 += eps1d*difft;
      if (DEBUGOUT) printf("  EPS1DOT  = %.15g\n", eps1d);
    } else if (strncmp("EPS2DOT", keyword, 80)==0){
      value = strtok(NULL, " \t\n");
      eps2d = strtod(value, &value);
      eps2 += eps1d*difft;
      if (DEBUGOUT) printf("  EPS2DOT  = %.15g\n", eps2d);
    }
  }
  if (binary){
    if (eps1 != 0.0 && eps2 !=0.0){
      /* Convert Laplace-Lagrange params to e and w */
      /* Warning!  This is presently untested!      */
      psr->orb.e = sqrt(eps1*eps1 + eps2*eps2);
      psr->orb.w = atan2(eps1, eps2);
      psr->orb.t += psr->orb.p*SECPERDAY/TWOPI*psr->orb.w;
      psr->orb.w *= RADTODEG;
    }
    /* psr->orb.t is in seconds, _not_ MJD.  It represents the */
    /* time in sec _since_ the last periastron passage, _not_  */
    /* when the next periastron will occur....                 */
    psr->orb.t = fmod((epoch-psr->orb.t)*SECPERDAY, psr->orb.p);
    if (psr->orb.t < 0.0)
      psr->orb.t += psr->orb.p;
    psr->orb.w = fmod(psr->orb.w, 360.0);
  }
  fclose(parfile);
  return 1;
}