/* Do not remove the headers from this file! see /USAGE for more info. */

// By Ohara

inherit HIDDEN_DOOR;


string extra_long_stuff()
{
    if( query_closed())
	return "";
    return capitalize( the_short()) + " is open.\n";
}


int door_check()
{
    string ret = environment( this_object())->get_default_exit();
    if( query_closed())
    {
	write( ret );
	return 0;
    }
    return 1;
}
