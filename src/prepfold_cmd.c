/*****
  command line parser -- generated by clig (Version: 1.0.1)

  The command line parser `clig':
  (C) 1995 Harald Kirsch (kir@iitb.fhg.de)
*****/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* mainly a hack to make things work on SUNOS with gcc */
extern double strtod(const char *, char **);
extern long int strtol(const char *, char **, int);

#include "prepfold_cmd.h"

char *Program;

/*@-null*/

static Cmdline cmd = {
  /***** -pkmb: Raw data in Parkes Multibeam format */
  /* pkmbP = */ FALSE,
  /***** -ebpp: Raw data in Efflesberg-Berkeley Pulsar Processor format */
  /* ebppP = */ FALSE,
  /***** -nobary: Do not barycenter the data */
  /* nobaryP = */ FALSE,
  /***** -DE405: Use the DE405 ephemeris for barycentering instead of DE200 (the default) */
  /* de405P = */ FALSE,
  /***** -p: The folding period (s) */
  /* pP = */ FALSE,
  /* p = */ (double)0,
  /* pC = */ 0,
  /***** -pd: The folding period derivative (s/s) */
  /* pdotP = */ TRUE,
  /* pdot = */ 0,
  /* pdotC = */ 1,
  /***** -f: The folding frequency (hz) */
  /* freqP = */ FALSE,
  /* freq = */ (double)0,
  /* freqC = */ 0,
  /***** -fd: The folding frequency derivative (hz/s) */
  /* dfdtP = */ TRUE,
  /* dfdt = */ 0,
  /* dfdtC = */ 1,
  /***** -n: The number of bins in the profile.  Defaults to the number of sampling bins which correspond to one folded period */
  /* proflenP = */ FALSE,
  /* proflen = */ (int)0,
  /* proflenC = */ 0,
  /***** -psr: Name of pulsar to fold (do not include J or B) */
  /* psrnameP = */ FALSE,
  /* psrname = */ (char*)0,
  /* psrnameC = */ 0,
  /***** -rzwcand: The candidate number to fold from 'infile'_rzw.cand */
  /* rzwcandP = */ FALSE,
  /* rzwcand = */ (int)0,
  /* rzwcandC = */ 0,
  /***** -rzwfile: Name of the rzw search file to use (include the full name of the file) */
  /* rzwfileP = */ FALSE,
  /* rzwfile = */ (char*)0,
  /* rzwfileC = */ 0,
  /***** -bincand: Fold a binary pulsar but take the input data from this candidate number in 'infile'_bin.cand */
  /* bincandP = */ FALSE,
  /* bincand = */ (int)0,
  /* bincandC = */ 0,
  /***** -onoff: A list of white-space separated pairs of numbers from 0.0 to 1.0 that designate barycentric times in our data set when we will actually keep the data. (i.e. '-onoff 0.1 0.4 0.7 0.9' means that we will fold the data set during the barycentric times 0.1-0.4 and 0.7-0.9 of the total time length of the data set) */
  /* onoffP = */ FALSE,
  /* onoff = */ (char*)0,
  /* onoffC = */ 0,
  /***** -bin: Fold a binary pulsar.  Must include all of the following parameters */
  /* binaryP = */ FALSE,
  /***** -pb: The orbital period (s) */
  /* pbP = */ FALSE,
  /* pb = */ (double)0,
  /* pbC = */ 0,
  /***** -x: The projected orbital semi-major axis (lt-sec) */
  /* asinicP = */ FALSE,
  /* asinic = */ (double)0,
  /* asinicC = */ 0,
  /***** -e: The orbital eccentricity */
  /* eP = */ TRUE,
  /* e = */ 0,
  /* eC = */ 1,
  /***** -To: The time of periastron passage (MJD) */
  /* ToP = */ FALSE,
  /* To = */ (double)0,
  /* ToC = */ 0,
  /***** -w: Longitude of periastron (deg) */
  /* wP = */ FALSE,
  /* w = */ (double)0,
  /* wC = */ 0,
  /***** -wdot: Rate of advance of periastron (deg/yr) */
  /* wdotP = */ TRUE,
  /* wdot = */ 0,
  /* wdotC = */ 1,
  /***** uninterpreted rest of command line */
  /* argc = */ 0,
  /* argv = */ (char**)0,
  /***** the original command line concatenated */
  /* full_cmd_line = */ NULL
};

/*@=null*/

/***** let LCLint run more smoothly */
/*@-predboolothers*/
/*@-boolops*/


/**********************************************************************/
int
getIntOpt(int argc, char **argv, int i, int *value, int force)
{
  char *end;

  if( ++i>=argc ) goto nothingFound;

  *value = (int)strtol(argv[i], &end, 10);

  /***** check for conversion error */
  if( end==argv[i] ) goto nothingFound;

  /***** check for surplus non-whitespace */
  while( isspace(*end) ) end+=1;
  if( *end ) goto nothingFound;

  return i;

nothingFound:
  if( !force ) return i-1;

  fprintf(stderr, 
	  "%s: missing or malformed integer value after option `%s'\n",
	  Program, argv[i-1]);
    exit(EXIT_FAILURE);
}
/**********************************************************************/
int
getIntOpts(int argc, char **argv, int i, 
	   int **values,
	   int cmin, int cmax)
/*****
  We want to find at least cmin values and at most cmax values.
  cmax==-1 then means infinitely many are allowed.
*****/
{
  int alloced, used;
  char *end;

  if( i+cmin >= argc ) {
    fprintf(stderr, 
	    "%s: option `%s' wants at least %d parameters\n",
	    Program, argv[i], cmin);
    exit(EXIT_FAILURE);
  }

  /***** 
    alloc a bit more than cmin values. It does not hurt to have room
    for a bit more values than cmax.
  *****/
  alloced = cmin + 4;
  *values = (int*)calloc((size_t)alloced, sizeof(int));
  if( ! *values ) {
outMem:
    fprintf(stderr, 
	    "%s: out of memory while parsing option `%s'\n",
	    Program, argv[i]);
    exit(EXIT_FAILURE);
  }

  for(used=0; (cmax==-1 || used<cmax) && used+i+1<argc; used++) {
    if( used==alloced ) {
      alloced += 8;
      *values = realloc(*values, alloced*sizeof(int));
      if( !*values ) goto outMem;
    }

    (*values)[used] = (int)strtol(argv[used+i+1], &end, 10);

    /***** check for conversion error */
    if( end==argv[used+i+1] ) break;

    /***** check for surplus non-whitespace */
    while( isspace(*end) ) end+=1;
    if( *end ) break;
  }
    
  if( used<cmin ) {
    fprintf(stderr, 
	    "%s: parameter `%s' of `%s' should be an "
	    "integer value\n",
	    Program, argv[i+used+1], argv[i]);
    exit(EXIT_FAILURE);
  }

  return i+used;
}
/**********************************************************************/
int
getdoubleOpt(int argc, char **argv, int i, double *value, int force)
{
  char *end;

  if( ++i>=argc ) goto nothingFound;

  *value = (double)strtod(argv[i], &end);

  /***** check for conversion error */
  if( end==argv[i] ) goto nothingFound;

  /***** check for surplus non-whitespace */
  while( isspace(*end) ) end+=1;
  if( *end ) goto nothingFound;

  return i;

nothingFound:
  if( !force ) return i-1;

  fprintf(stderr,
	  "%s: missing or malformed double value after option `%s'\n",
	  Program, argv[i-1]);
  exit(EXIT_FAILURE);
 
}
/**********************************************************************/
int
getdoubleOpts(int argc, char **argv, int i, 
	   double **values,
	   int cmin, int cmax)
/*****
  We want to find at least cmin values and at most cmax values.
  cmax==-1 then means infinitely many are allowed.
*****/
{
  int alloced, used;
  char *end;

  if( i+cmin >= argc ) {
    fprintf(stderr, 
	    "%s: option `%s' wants at least %d parameters\n",
	    Program, argv[i], cmin);
    exit(EXIT_FAILURE);
  }

  /***** 
    alloc a bit more than cmin values.
  *****/
  alloced = cmin + 4;
  *values = (double*)calloc((size_t)alloced, sizeof(double));
  if( ! *values ) {
outMem:
    fprintf(stderr, 
	    "%s: out of memory while parsing option `%s'\n",
	    Program, argv[i]);
    exit(EXIT_FAILURE);
  }

  for(used=0; (cmax==-1 || used<cmax) && used+i+1<argc; used++) {
    if( used==alloced ) {
      alloced += 8;
      *values = realloc(*values, alloced*sizeof(double));
      if( !*values ) goto outMem;
    }

    (*values)[used] = (double)strtod(argv[used+i+1], &end);

    /***** check for conversion error */
    if( end==argv[used+i+1] ) break;

    /***** check for surplus non-whitespace */
    while( isspace(*end) ) end+=1;
    if( *end ) break;
  }
    
  if( used<cmin ) {
    fprintf(stderr, 
	    "%s: parameter `%s' of `%s' should be a "
	    "double value\n",
	    Program, argv[i+used+1], argv[i]);
    exit(EXIT_FAILURE);
  }

  return i+used;
}
/**********************************************************************/
int
getStringOpt(int argc, char **argv, int i, char **value, int force)
{
  if( ++i>=argc ) {
    fprintf(stderr, "%s: missing string after option `%s'\n",
            Program, argv[i-1]);
    exit(EXIT_FAILURE);
  }
  
  if( !force && argv[i+1][0] == '-' ) return i-1;
  *value = argv[i];
  return i;
}
/**********************************************************************/
int
getStringOpts(int argc, char **argv, int i, 
	   char*  **values,
	   int cmin, int cmax)
/*****
  We want to find at least cmin values and at most cmax values.
  cmax==-1 then means infinitely many are allowed.
*****/
{
  int alloced, used;

  if( i+cmin >= argc ) {
    fprintf(stderr, 
	    "%s: option `%s' wants at least %d parameters\n",
	    Program, argv[i], cmin);
    exit(EXIT_FAILURE);
  }

  alloced = cmin + 4;
    
  *values = (char**)calloc((size_t)alloced, sizeof(char*));
  if( ! *values ) {
outMem:
    fprintf(stderr, 
	    "%s: out of memory during parsing of option `%s'\n",
	    Program, argv[i]);
    exit(EXIT_FAILURE);
  }

  for(used=0; (cmax==-1 || used<cmax) && used+i+1<argc; used++) {
    if( used==alloced ) {
      alloced += 8;
      *values = realloc(*values, alloced*sizeof(char*));
      if( !*values ) goto outMem;
    }

    if( argv[used+i+1][0]=='-' ) break;
    (*values)[used] = argv[used+i+1];
  }
    
  if( used<cmin ) {
    fprintf(stderr, 
    "%s: less than %d parameters for option `%s', only %d found\n",
	    Program, cmin, argv[i], used);
    exit(EXIT_FAILURE);
  }

  return i+used;
}
/**********************************************************************/
void
checkIntLower(char *opt, int *values, int count, int max)
{
  int i;

  for(i=0; i<count; i++) {
    if( values[i]<=max ) continue;
    fprintf(stderr, 
	    "%s: parameter %d of option `%s' greater than max=%d\n",
	    Program, i+1, opt, max);
    exit(EXIT_FAILURE);
  }
}
/**********************************************************************/
void
checkIntHigher(char *opt, int *values, int count, int min)
{
  int i;

  for(i=0; i<count; i++) {
    if( values[i]>=min ) continue;
    fprintf(stderr, 
	    "%s: parameter %d of option `%s' smaller than min=%d\n",
	    Program, i+1, opt, min);
    exit(EXIT_FAILURE);
  }
}
/**********************************************************************/
void
checkdoubleLower(char *opt, double *values, int count, double max)
{
  int i;

  for(i=0; i<count; i++) {
    if( values[i]<=max ) continue;
    fprintf(stderr, 
	    "%s: parameter %d of option `%s' greater than max=%f\n",
	    Program, i+1, opt, max);
    exit(EXIT_FAILURE);
  }
}
/**********************************************************************/
void
checkdoubleHigher(char *opt, double *values, int count, double min)
{
  int i;

  for(i=0; i<count; i++) {
    if( values[i]>=min ) continue;
    fprintf(stderr, 
	    "%s: parameter %d of option `%s' smaller than min=%f\n",
	    Program, i+1, opt, min);
    exit(EXIT_FAILURE);
  }
}
/**********************************************************************/
static char *
catArgv(int argc, char **argv)
{
  int i;
  size_t l;
  char *s, *t;

  for(i=0, l=0; i<argc; i++) l += (1+strlen(argv[i]));
  s = malloc(l);
  if( !s ) {
    fprintf(stderr, "%s: out of memory\n", Program);
    exit(EXIT_FAILURE);
  }
  strcpy(s, argv[0]);
  t = s;
  for(i=1; i<argc; i++) {
    t = t+strlen(t);
    *t++ = ' ';
    strcpy(t, argv[i]);
  }
  return s;
}
/**********************************************************************/
void
showOptionValues(void)
{
  int i;

  printf("Full command line is:\n`%s'\n", cmd.full_cmd_line);

  /***** -pkmb: Raw data in Parkes Multibeam format */
  if( !cmd.pkmbP ) {
    printf("-pkmb not found.\n");
  } else {
    printf("-pkmb found:\n");
  }

  /***** -ebpp: Raw data in Efflesberg-Berkeley Pulsar Processor format */
  if( !cmd.ebppP ) {
    printf("-ebpp not found.\n");
  } else {
    printf("-ebpp found:\n");
  }

  /***** -nobary: Do not barycenter the data */
  if( !cmd.nobaryP ) {
    printf("-nobary not found.\n");
  } else {
    printf("-nobary found:\n");
  }

  /***** -DE405: Use the DE405 ephemeris for barycentering instead of DE200 (the default) */
  if( !cmd.de405P ) {
    printf("-DE405 not found.\n");
  } else {
    printf("-DE405 found:\n");
  }

  /***** -p: The folding period (s) */
  if( !cmd.pP ) {
    printf("-p not found.\n");
  } else {
    printf("-p found:\n");
    if( !cmd.pC ) {
      printf("  no values\n");
    } else {
      printf("  value = `%f'\n", cmd.p);
    }
  }

  /***** -pd: The folding period derivative (s/s) */
  if( !cmd.pdotP ) {
    printf("-pd not found.\n");
  } else {
    printf("-pd found:\n");
    if( !cmd.pdotC ) {
      printf("  no values\n");
    } else {
      printf("  value = `%f'\n", cmd.pdot);
    }
  }

  /***** -f: The folding frequency (hz) */
  if( !cmd.freqP ) {
    printf("-f not found.\n");
  } else {
    printf("-f found:\n");
    if( !cmd.freqC ) {
      printf("  no values\n");
    } else {
      printf("  value = `%f'\n", cmd.freq);
    }
  }

  /***** -fd: The folding frequency derivative (hz/s) */
  if( !cmd.dfdtP ) {
    printf("-fd not found.\n");
  } else {
    printf("-fd found:\n");
    if( !cmd.dfdtC ) {
      printf("  no values\n");
    } else {
      printf("  value = `%f'\n", cmd.dfdt);
    }
  }

  /***** -n: The number of bins in the profile.  Defaults to the number of sampling bins which correspond to one folded period */
  if( !cmd.proflenP ) {
    printf("-n not found.\n");
  } else {
    printf("-n found:\n");
    if( !cmd.proflenC ) {
      printf("  no values\n");
    } else {
      printf("  value = `%d'\n", cmd.proflen);
    }
  }

  /***** -psr: Name of pulsar to fold (do not include J or B) */
  if( !cmd.psrnameP ) {
    printf("-psr not found.\n");
  } else {
    printf("-psr found:\n");
    if( !cmd.psrnameC ) {
      printf("  no values\n");
    } else {
      printf("  value = `%s'\n", cmd.psrname);
    }
  }

  /***** -rzwcand: The candidate number to fold from 'infile'_rzw.cand */
  if( !cmd.rzwcandP ) {
    printf("-rzwcand not found.\n");
  } else {
    printf("-rzwcand found:\n");
    if( !cmd.rzwcandC ) {
      printf("  no values\n");
    } else {
      printf("  value = `%d'\n", cmd.rzwcand);
    }
  }

  /***** -rzwfile: Name of the rzw search file to use (include the full name of the file) */
  if( !cmd.rzwfileP ) {
    printf("-rzwfile not found.\n");
  } else {
    printf("-rzwfile found:\n");
    if( !cmd.rzwfileC ) {
      printf("  no values\n");
    } else {
      printf("  value = `%s'\n", cmd.rzwfile);
    }
  }

  /***** -bincand: Fold a binary pulsar but take the input data from this candidate number in 'infile'_bin.cand */
  if( !cmd.bincandP ) {
    printf("-bincand not found.\n");
  } else {
    printf("-bincand found:\n");
    if( !cmd.bincandC ) {
      printf("  no values\n");
    } else {
      printf("  value = `%d'\n", cmd.bincand);
    }
  }

  /***** -onoff: A list of white-space separated pairs of numbers from 0.0 to 1.0 that designate barycentric times in our data set when we will actually keep the data. (i.e. '-onoff 0.1 0.4 0.7 0.9' means that we will fold the data set during the barycentric times 0.1-0.4 and 0.7-0.9 of the total time length of the data set) */
  if( !cmd.onoffP ) {
    printf("-onoff not found.\n");
  } else {
    printf("-onoff found:\n");
    if( !cmd.onoffC ) {
      printf("  no values\n");
    } else {
      printf("  value = `%s'\n", cmd.onoff);
    }
  }

  /***** -bin: Fold a binary pulsar.  Must include all of the following parameters */
  if( !cmd.binaryP ) {
    printf("-bin not found.\n");
  } else {
    printf("-bin found:\n");
  }

  /***** -pb: The orbital period (s) */
  if( !cmd.pbP ) {
    printf("-pb not found.\n");
  } else {
    printf("-pb found:\n");
    if( !cmd.pbC ) {
      printf("  no values\n");
    } else {
      printf("  value = `%f'\n", cmd.pb);
    }
  }

  /***** -x: The projected orbital semi-major axis (lt-sec) */
  if( !cmd.asinicP ) {
    printf("-x not found.\n");
  } else {
    printf("-x found:\n");
    if( !cmd.asinicC ) {
      printf("  no values\n");
    } else {
      printf("  value = `%f'\n", cmd.asinic);
    }
  }

  /***** -e: The orbital eccentricity */
  if( !cmd.eP ) {
    printf("-e not found.\n");
  } else {
    printf("-e found:\n");
    if( !cmd.eC ) {
      printf("  no values\n");
    } else {
      printf("  value = `%f'\n", cmd.e);
    }
  }

  /***** -To: The time of periastron passage (MJD) */
  if( !cmd.ToP ) {
    printf("-To not found.\n");
  } else {
    printf("-To found:\n");
    if( !cmd.ToC ) {
      printf("  no values\n");
    } else {
      printf("  value = `%f'\n", cmd.To);
    }
  }

  /***** -w: Longitude of periastron (deg) */
  if( !cmd.wP ) {
    printf("-w not found.\n");
  } else {
    printf("-w found:\n");
    if( !cmd.wC ) {
      printf("  no values\n");
    } else {
      printf("  value = `%f'\n", cmd.w);
    }
  }

  /***** -wdot: Rate of advance of periastron (deg/yr) */
  if( !cmd.wdotP ) {
    printf("-wdot not found.\n");
  } else {
    printf("-wdot found:\n");
    if( !cmd.wdotC ) {
      printf("  no values\n");
    } else {
      printf("  value = `%f'\n", cmd.wdot);
    }
  }
  if( !cmd.argc ) {
    printf("no remaining parameters in argv\n");
  } else {
    printf("argv =");
    for(i=0; i<cmd.argc; i++) {
      printf(" `%s'", cmd.argv[i]);
    }
    printf("\n");
  }
}
/**********************************************************************/

void
usage(void)
{
  fprintf(stderr, "usage: %s%s", Program, "\
 [-pkmb] [-ebpp] [-nobary] [-DE405] [-p p] [-pd pdot] [-f freq] [-fd dfdt] [-n proflen] [-psr psrname] [-rzwcand rzwcand] [-rzwfile rzwfile] [-bincand bincand] [-onoff onoff] [-bin] [-pb pb] [-x asinic] [-e e] [-To To] [-w w] [-wdot wdot] infile\n\
    Prepares a raw, multichannel, radio data file and folds it looking for the correct dispersion measure.\n\
     -pkmb: Raw data in Parkes Multibeam format\n\
     -ebpp: Raw data in Efflesberg-Berkeley Pulsar Processor format\n\
   -nobary: Do not barycenter the data\n\
    -DE405: Use the DE405 ephemeris for barycentering instead of DE200 (the default)\n\
        -p: The folding period (s)\n\
            1 double precision value between 0 and oo\n\
       -pd: The folding period derivative (s/s)\n\
            1 double precision value\n\
            default: `0'\n\
        -f: The folding frequency (hz)\n\
            1 double precision value between 0 and oo\n\
       -fd: The folding frequency derivative (hz/s)\n\
            1 double precision value\n\
            default: `0'\n\
        -n: The number of bins in the profile.  Defaults to the number of sampling bins which correspond to one folded period\n\
            1 integer value\n\
      -psr: Name of pulsar to fold (do not include J or B)\n\
            1 string value\n\
  -rzwcand: The candidate number to fold from 'infile'_rzw.cand\n\
            1 integer value between 1 and oo\n\
  -rzwfile: Name of the rzw search file to use (include the full name of the file)\n\
            1 string value\n\
  -bincand: Fold a binary pulsar but take the input data from this candidate number in 'infile'_bin.cand\n\
            1 integer value between 1 and oo\n\
    -onoff: A list of white-space separated pairs of numbers from 0.0 to 1.0 that designate barycentric times in our data set when we will actually keep the data. (i.e. '-onoff 0.1 0.4 0.7 0.9' means that we will fold the data set during the barycentric times 0.1-0.4 and 0.7-0.9 of the total time length of the data set)\n\
            1 string value\n\
      -bin: Fold a binary pulsar.  Must include all of the following parameters\n\
       -pb: The orbital period (s)\n\
            1 double precision value between 0 and oo\n\
        -x: The projected orbital semi-major axis (lt-sec)\n\
            1 double precision value between 0 and oo\n\
        -e: The orbital eccentricity\n\
            1 double precision value between 0 and 0.9999999\n\
            default: `0'\n\
       -To: The time of periastron passage (MJD)\n\
            1 double precision value between 0 and oo\n\
        -w: Longitude of periastron (deg)\n\
            1 double precision value between 0 and 360\n\
     -wdot: Rate of advance of periastron (deg/yr)\n\
            1 double precision value\n\
            default: `0'\n\
    infile: Input data file name (without a suffix).  Currently, only PKMB format is supported.\n\
            1 string value\n\
version: 06Nov99\n\
");
  exit(EXIT_FAILURE);
}
/**********************************************************************/
Cmdline *
parseCmdline(int argc, char **argv)
{
  int i, keep;

  Program = argv[0];
  cmd.full_cmd_line = catArgv(argc, argv);
  for(i=1, cmd.argc=1; i<argc; i++) {
    if( 0==strcmp("--", argv[i]) ) {
      while( ++i<argc ) argv[cmd.argc++] = argv[i];
      continue;
    }

    if( 0==strcmp("-pkmb", argv[i]) ) {
      cmd.pkmbP = TRUE;
      continue;
    }

    if( 0==strcmp("-ebpp", argv[i]) ) {
      cmd.ebppP = TRUE;
      continue;
    }

    if( 0==strcmp("-nobary", argv[i]) ) {
      cmd.nobaryP = TRUE;
      continue;
    }

    if( 0==strcmp("-DE405", argv[i]) ) {
      cmd.de405P = TRUE;
      continue;
    }

    if( 0==strcmp("-p", argv[i]) ) {
      cmd.pP = TRUE;
      keep = i;
      i = getdoubleOpt(argc, argv, i, &cmd.p, 1);
      cmd.pC = i-keep;
      checkdoubleHigher("-p", &cmd.p, cmd.pC, 0);
      continue;
    }

    if( 0==strcmp("-pd", argv[i]) ) {
      cmd.pdotP = TRUE;
      keep = i;
      i = getdoubleOpt(argc, argv, i, &cmd.pdot, 1);
      cmd.pdotC = i-keep;
      continue;
    }

    if( 0==strcmp("-f", argv[i]) ) {
      cmd.freqP = TRUE;
      keep = i;
      i = getdoubleOpt(argc, argv, i, &cmd.freq, 1);
      cmd.freqC = i-keep;
      checkdoubleHigher("-f", &cmd.freq, cmd.freqC, 0);
      continue;
    }

    if( 0==strcmp("-fd", argv[i]) ) {
      cmd.dfdtP = TRUE;
      keep = i;
      i = getdoubleOpt(argc, argv, i, &cmd.dfdt, 1);
      cmd.dfdtC = i-keep;
      continue;
    }

    if( 0==strcmp("-n", argv[i]) ) {
      cmd.proflenP = TRUE;
      keep = i;
      i = getIntOpt(argc, argv, i, &cmd.proflen, 1);
      cmd.proflenC = i-keep;
      continue;
    }

    if( 0==strcmp("-psr", argv[i]) ) {
      cmd.psrnameP = TRUE;
      keep = i;
      i = getStringOpt(argc, argv, i, &cmd.psrname, 1);
      cmd.psrnameC = i-keep;
      continue;
    }

    if( 0==strcmp("-rzwcand", argv[i]) ) {
      cmd.rzwcandP = TRUE;
      keep = i;
      i = getIntOpt(argc, argv, i, &cmd.rzwcand, 1);
      cmd.rzwcandC = i-keep;
      checkIntHigher("-rzwcand", &cmd.rzwcand, cmd.rzwcandC, 1);
      continue;
    }

    if( 0==strcmp("-rzwfile", argv[i]) ) {
      cmd.rzwfileP = TRUE;
      keep = i;
      i = getStringOpt(argc, argv, i, &cmd.rzwfile, 1);
      cmd.rzwfileC = i-keep;
      continue;
    }

    if( 0==strcmp("-bincand", argv[i]) ) {
      cmd.bincandP = TRUE;
      keep = i;
      i = getIntOpt(argc, argv, i, &cmd.bincand, 1);
      cmd.bincandC = i-keep;
      checkIntHigher("-bincand", &cmd.bincand, cmd.bincandC, 1);
      continue;
    }

    if( 0==strcmp("-onoff", argv[i]) ) {
      cmd.onoffP = TRUE;
      keep = i;
      i = getStringOpt(argc, argv, i, &cmd.onoff, 1);
      cmd.onoffC = i-keep;
      continue;
    }

    if( 0==strcmp("-bin", argv[i]) ) {
      cmd.binaryP = TRUE;
      continue;
    }

    if( 0==strcmp("-pb", argv[i]) ) {
      cmd.pbP = TRUE;
      keep = i;
      i = getdoubleOpt(argc, argv, i, &cmd.pb, 1);
      cmd.pbC = i-keep;
      checkdoubleHigher("-pb", &cmd.pb, cmd.pbC, 0);
      continue;
    }

    if( 0==strcmp("-x", argv[i]) ) {
      cmd.asinicP = TRUE;
      keep = i;
      i = getdoubleOpt(argc, argv, i, &cmd.asinic, 1);
      cmd.asinicC = i-keep;
      checkdoubleHigher("-x", &cmd.asinic, cmd.asinicC, 0);
      continue;
    }

    if( 0==strcmp("-e", argv[i]) ) {
      cmd.eP = TRUE;
      keep = i;
      i = getdoubleOpt(argc, argv, i, &cmd.e, 1);
      cmd.eC = i-keep;
      checkdoubleLower("-e", &cmd.e, cmd.eC, 0.9999999);
      checkdoubleHigher("-e", &cmd.e, cmd.eC, 0);
      continue;
    }

    if( 0==strcmp("-To", argv[i]) ) {
      cmd.ToP = TRUE;
      keep = i;
      i = getdoubleOpt(argc, argv, i, &cmd.To, 1);
      cmd.ToC = i-keep;
      checkdoubleHigher("-To", &cmd.To, cmd.ToC, 0);
      continue;
    }

    if( 0==strcmp("-w", argv[i]) ) {
      cmd.wP = TRUE;
      keep = i;
      i = getdoubleOpt(argc, argv, i, &cmd.w, 1);
      cmd.wC = i-keep;
      checkdoubleLower("-w", &cmd.w, cmd.wC, 360);
      checkdoubleHigher("-w", &cmd.w, cmd.wC, 0);
      continue;
    }

    if( 0==strcmp("-wdot", argv[i]) ) {
      cmd.wdotP = TRUE;
      keep = i;
      i = getdoubleOpt(argc, argv, i, &cmd.wdot, 1);
      cmd.wdotC = i-keep;
      continue;
    }

    if( argv[i][0]=='-' ) {
      fprintf(stderr, "\n%s: unknown option `%s'\n\n",
              Program, argv[i]);
      usage();
    }
    argv[cmd.argc++] = argv[i];
  }/* for i */


  /*@-mustfree*/
  cmd.argv = argv+1;
  /*@=mustfree*/
  cmd.argc -= 1;

  if( 1>cmd.argc ) {
    fprintf(stderr, "%s: there should be at least 1 non-option argument(s)\n",
            Program);
    exit(EXIT_FAILURE);
  }
  if( 1<cmd.argc ) {
    fprintf(stderr, "%s: there should be at most 1 non-option argument(s)\n",
            Program);
    exit(EXIT_FAILURE);
  }
  /*@-compmempass*/  return &cmd;
}

