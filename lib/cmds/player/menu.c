/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
inherit CMD;

private void
main()
{
  clone_object(PLAYER_MENU)->start_menu();
}
