/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;

private void main(mixed *arg) {
  string dir;
  foreach(dir in arg[0])
    {
      if (rmdir(dir))
	outf("Removed directory: %s.\n", dir);
      else
	outf("Failed to remove directory: %s.\n", dir);
    }
 
}
