/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <size.h>
#include <bodyslots.h>

inherit CONTAINER;
inherit M_OPENABLE;
inherit M_WEARABLE;
inherit M_GETTABLE;

void
setup() {
    
    set_id("backpack", "pack");
    set_long("It's a simple leather backpack, with a flap and strap allowing you to close it.");
    set_gettable(1);
    set_objects( ([
		   ]) );
    set_max_capacity(3*MEDIUM);
    set_size(SMALL);
#ifdef USE_BODYSLOTS
    set_slot( TORSO );
#endif
}

	    
void remove()  {
    m_wearable::remove();
    container::remove();
}
