/* Do not remove the headers from this file! see /USAGE for more info. */

//  a room.c
// Rust@ZorkMUD,   Jan 12, 1994
// Nevyn@GUE       1-14-94
// Cleave@ZorkMUD 2-25-94 Added long(), long default.
// Cleave@ZorkMUD 2-25-94 Added code to go through and expand the
//   items array for ([ ({"tree","trees"}) : "Trees are here.\n"])
//   into separate maps.
// Randor@SE made some changes and added arg and function types
// Beek@ZorkMud   4-6-94  Added the way cool exit code.  Look at the
//      lfun docs.
// Rust 4-13-94	pretty much redid all of Randor's code for various reasons...
// Also wrote in_room stuff to make looking in rooms more interesting
// Rust 4-20-94  Beek-ized items mapping.
// Beek & Rust have both done a bit of work on this since....
// August 24 - Beek added light

#include <mudlib.h>
#include <playerflags.h>
#include <setbit.h>
#include <move.h>

inherit CONTAINER;
inherit M_ITEMS;



// global vars

static int DEF_ITEM = 1;
int rbits;
int xtra_flags;
string base;
string remote_desc;
mapping exits = ([]);
mapping items = ([]);
mapping exit_msg = ([]);
mapping enter_msg = ([ ]);
int total_light;

int query_lit() { return total_light + query_light(); }

/* Never, EVER, under pain of death, call this function.  For
   internal mudlib use ONLY! */
void adjust_light(int x) {
    total_light += x;
}

int stat_me() {
    printf("Room: %s [ %s ]\nItems: %s\nLong: %s\n",
      short(), implode(keys(exits),", "), implode(keys(items), ", "),
      container::long() );
    container::stat_me();
    return 1;
}

void set_brief(string str) {
    set_proper_name(str);
}

/* by default, rooms can hold water */
int can_hold_water()
{
    return 1;
}

void create(){
    container::create();
    items::items_init();

    base = file_name(this_object());
    base = base[0..strsrch(base, '/', -1)];
    set_max_capacity( 1000000 );
    add_id_no_plural("ground");
    add_id_no_plural("here");
    add_id("environment");
    add_id("room");
}

mapping get_exits()
{
    return exits;
}

string get_exit(string dir) {
    mixed tmp;
    int i;

    tmp = exits[dir];
    if (!tmp) return 0;
    if (stringp(tmp)) return tmp;
    for (i=0; i<sizeof(tmp); i++) {
	if (stringp(tmp[i])) return tmp[i];
    }
    return 0;
}

string show_exits()
{
    string exit_str;
    string* exit_names;
    exit_names = keys(exits);
    exit_str = ((sizeof(exit_names)) ? implode(exit_names,", ") : "none");
    return exit_str;
}

void do_looking()
{
    if ( wizardp(this_user()) &&
      this_body()->query_shell_ob()->get_variable("show_loc") )
    {
	printf("[%s]\n", file_name(this_object()));
    }

    if ( query_lit() < 1 )
    {
	write("Someplace dark\nIt is dark here.\nYou might get eaten by a grue.\n");
    }
    else
    {
#ifdef OBVIOUS_EXITS
	printf("%s [exits: %s]\n", short(), show_exits());
#else
	printf("%s\n", short());
#endif

	if (!this_body()->test_flag(F_BRIEF)) 
	    write(wrap(long() + "\n"));

	write("\n");
    }
}

string show_objects()
{
    object *obs;
    string user_show;
    string obj_show;
    string str;
    int n;

    obs = all_inventory() - ({ this_body() });
    n = sizeof(obs);
    user_show = "";
    obj_show = "";

    while (n--) {
	if (obs[n]->is_living()) {
	    user_show += obs[n]->in_room_desc() + "\n";
	} else {
	    if (!duplicatep(obs[n])) {
		if (obs[n]->test_flag(INVIS)) continue;
		if (str = obs[n]->show_in_room()) {
		    obj_show += str + "\n";
		}
		if (obs[n]->inventory_visible())
		    obj_show += obs[n]->inventory_recurse();
	    }
	}
    }

    if (user_show != "") obj_show += "\n"+user_show;
    return wrap(obj_show);
}

string query_enter_msg(string arg){
    return enter_msg[arg]; 
}

nomask string long()
{
    return sprintf("%s%s", container::simple_long(), show_objects());
}


int move_player(string dest, string arg){
    object env;
    int r;
    string txt;
    string *msgs;

    if ((r = this_body()->move(dest)) != MOVE_OK)
	if ((r = this_body()->move(evaluate_path(base + dest))) != MOVE_OK) {
	    notify_fail("You remain where you are.\n");
	    return r;
	}
    env = environment(this_body());
    if (this_body()->test_flag(F_DISPLAY_PATH))
	printf("[ %s ]\n", file_name(env));
    txt = exit_msg[arg];
    if (!arg) arg = "somewhere";
    if (!txt) {
	msgs = this_body()->get_player_message("enter", arg);
	tell_room(env, msgs[1], ({ this_body() }));
    } else {
	tell_room(env, txt, ({ this_body() }) );
    }
    txt = env->query_enter_msg(arg);
    if (!txt) {
	msgs = this_body()->get_player_message("leave", arg);
	tell_room(this_object(), msgs[1], ({ this_body() }));
    } else {
	tell_room(this_object(), txt, ({ this_body() }) );
    }
    return r;
}

/*
** really funky destination evaluation code.
** Read the lfun docs under "exits".
*/
int evaluate_destination(mixed dest, string arg) {
    int i,n;
    mixed foo,tmp;
    if (pointerp(dest)) {
	i = 0;
	n = sizeof(dest);
	while (i<n) {
	    foo = dest[i]; /* optimization */
	    if (intp(foo) && !foo) return 0;
	    if (intp(foo)) i+=foo;
	    if (stringp(foo)) {
		if (!move_player(foo, arg)) return 1;
		i++;
	    }
	    if (functionp(foo)) {
		tmp = evaluate(foo);
		if (!tmp) return 0;
		if (stringp(tmp)) {
		    if (!move_player(tmp, arg)) return 1;
		    i++;
		} else i+=tmp;
	    }
	}
	return 1;
    } else {
	return !move_player(dest, arg);
    }
}

int go_somewhere(string arg){
    string dest;
    int ret;

    if (!(dest = exits[arg])){
	if (dest = exits["DEFAULT"]) notify_fail(dest);
	else notify_fail("You can't go that way!\n");
	return 0;
    }
    this_object()->pre_exit();
    if (ret = evaluate_destination(dest, arg)) this_object()->post_exit();
    return ret;
}

void remove()
{
    destruct(this_object());
}


void add_exits(mapping new_exits){
    if (!mapp(new_exits)) return;
    exits += new_exits;
}

void delete_exits(mapping old){
    if (!mapp(old)) return;
    exits -= old;
}

void set_exits( mapping new_exits )
{
    if(mapp(new_exits)) exits = new_exits;
}


string query_name(){ return "the ground"; }

string
remote_look( object o )
{
    // This isn't right.  exits don't necessarily match full filenames
    if (member_array(file_name(this_object()), values(o->query_exits())) != -1
      && remote_desc )
    {
	// if well lit....
	return remote_desc;
    }
    return "You can't seem to make out anything.\n";
}

string query_base() { return base; }
