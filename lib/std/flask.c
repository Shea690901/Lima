/* Do not remove the headers from this file! see /USAGE for more info. */

inherit CONTAINER;
inherit M_GETTABLE;
inherit M_FLUID_CONTAINER;



void setup()
{
    set_adj("clear", "glass", "blown");
    set_id("flask", "bottle");
    set_long( "It is a thick flask blown from clear glass." );
    set_size(  1  );
    set_max_capacity( 1 );
}



