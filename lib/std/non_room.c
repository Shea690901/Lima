/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust wrote this, it's a lot like room.c, except it's
// for things like furniture and vehichles, which aren't rooms,
// but are locations.


#include <setbit.h>
#include <move.h>

inherit COMPLEX_CONTAINER;
inherit M_ITEMS;
inherit __DIR__ "room/exits";
inherit M_EXIT_OBJ;

string stat_me()
{
    return sprintf("Location: %s [ %s ]\n\n",
		   short(), implode(query_exit_directions(1), ", ")) +
	complex_container::stat_me();
}


int is_player_inside();

mixed direct_go_wrd_obj(string s, object ob)
{
    if (!is_player_inside())
      if (environment(this_body()) != environment(this_object()))
        return 0;
    return ::direct_go_wrd_obj(s, ob);
}

mixed direct_go_obj(object ob)
{
    if (!is_player_inside())
      if (environment(this_body()) != environment(this_object()))
        return 0;
   return ::direct_go_obj(ob);
}

//:FUNCTION can_hold_water
//Return 1 if the object can hold water
/* by default, locations can hold water */
int can_hold_water()
{
    return 1;
}

void create()
{
    complex_container::create();
    exits::create();
}

void mudlib_setup()
{
    ::mudlib_setup();
    set_light(DEFAULT_LIGHT_LEVEL);
    set_max_capacity(VERY_LARGE+LARGE);
    add_id_no_plural("here");
    set_preposition("on");
    if(!sizeof(get_relations()))
    {
	set_relations("on");
    }
    set_default_exit((: "You're not going anywhere until you get out of the " 
		      + short() + ".\n" :));
}


string get_location_description()
{
  return environment(this_object())->long_without_object(this_object());
}

//:FUNCTION as_location_short
// Since locations double as objects and places to be, this is the short
// That goes at the top of a room description.  Usually it's the same as
// the room plus a modifier, like:  Grand Hall, in the camero

string get_brief()
{
  return sprintf("%s, %s the %s", environment(this_object())->short(),
		 query_prep(), short());
}

//:FUNCTION do_looking
//print out the description of the current room
void do_looking(int force_long_desc)
{
//### how to use force_long_desc ??

    if ( wizardp() && get_user_variable("show_loc") )
    {
	printf("[%s]\n", file_name(this_object()));
    }

    if ( environment(this_object())->query_light() < 1 )
    {
	write("Someplace dark\nIt is dark here.\n");
#ifdef ZORKMUD
	write("You might get eaten by a grue.\n");
#endif
    }
    else
    {
#ifdef OBVIOUS_EXITS
	printf("%s [exits: %s]\n", get_brief(), show_exits());
#else
	printf("%s\n", get_brief());
#endif

	write(get_location_description());
    }
}

int is_player_inside() {
    return (environment(this_body()) == this_object());
}

//:FUNCTION parent_environment_accessible
//Return 1 if the outside should be included in the parse
//Non-rooms default to returning 1
int parent_environment_accessible() {
    return 1;
}

