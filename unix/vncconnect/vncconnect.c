/* Copyright (C) 2002-2005 RealVNC Ltd.  All Rights Reserved.
 * Copyright (C) 2013 D. R. Commander.  All Rights Reserved.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,
 * USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include "vncExt.h"


static char *programName;


static void usage()
{
  fprintf(stderr, "\nUSAGE: %s [-display <d>] [-disconnect] host[:port]\n\n",
          programName);
  fprintf(stderr, "-display <d> = specify the X display of the VNC server session that you wish to\n"
                  "               connect to a listening viewer (for instance, :1).  If this is\n"
                  "               not specified, then the value of the DISPLAY environment\n"
                  "               variable is used.\n");
  fprintf(stderr, "-disconnect = disconnect all listening viewers\n\n");
  exit(1);
}


int main(int argc, char **argv)
{
  char *displayname = NULL;
  Display *dpy;
  int i, disconnect = 0, status = 0;
  Atom prop;

  programName = argv[0];
  
  for (i = 1; i < argc; i++) {
    if (argv[i][0] != '-')
      break;

    if (!strncmp(argv[i], "-disp", 5)) {
      if (++i >= argc) usage();
      displayname = argv[i];
    } else if (!strncmp(argv[i], "-disc", 5)) {
      disconnect = 1;
    }
    else usage();
  }

  if (argc != i+1 && !disconnect)
    usage();

  if (!(dpy = XOpenDisplay(displayname))) {
    fprintf(stderr,"%s: unable to open display \"%s\"\n",
        programName, XDisplayName (displayname));
    exit(1);
  }

  if (disconnect) {
    if (!XVncExtConnect(dpy, "")) {
      fprintf(stderr, "Could not disconnect listening viewers (perhaps there are none)\n");
      status = 1;
    }
  } else {
    if (!XVncExtConnect(dpy, argv[i])) {
      fprintf(stderr, "Reverse connection to %s failed\n", argv[i]);
      status = 1;
    }
  }

  XCloseDisplay(dpy);

  return status;
}
