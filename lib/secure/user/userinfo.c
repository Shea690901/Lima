/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** userinfo.c -- user information
**
** 950821, Deathblade: created.
*/

#include <config.h>
#include <mudlib.h>
#include <security.h>

void new_user_ready();

object query_body();

void save_me();

varargs void modal_push(function input_func,
			mixed prompt,
			int secure,
			function return_to_func
    );
varargs void modal_func(function input_func,
			mixed prompt,
			int secure
    );
void modal_pop();


private string 	email;
private string 	real_name;
private int 	ed_setup;
private string  url;


//temporary new user vars
static private int n_gen = -1;

//### wah! get rid of this. need by sw_body.c; should move to NEW_USER_D
static nomask int query_n_gen() { return n_gen; }


nomask void set_ed_setup(int code)
{
    ed_setup = code;
    save_me();
}

nomask int query_ed_setup()
{
    return ed_setup;
}

//### ACK!  should disappear. need something more secure than this
nomask string query_email()
{
    return email;
}

nomask void set_email(string new_email)
{
//### not secure.
    if ( this_body() != query_body() )
	error("illegal attempt to set email address\n");

    email = new_email;
    save_me();
}

nomask void set_url(string new_url)
{
//### not secure.
    if ( this_body() != query_body() )
	error("illegal attempt to set URL\n");

    url = new_url;
    save_me();
}

nomask string query_url()
{
    return url;
}


/*
** Character creation functions: gather user info
**
** Initiate with a call to begin_info_collection()
*/

private nomask void done_with_inputs()
{
    /*
    ** Done with this series of inputs
    */
    modal_pop();

    /*
    ** Let's move on to introducing the character to the mud.
    */
    if ( file_size(NEW_PLAYER) <= 0 )
    {
	new_user_ready();
	return;
    }

    more_file(NEW_PLAYER, 0, (: new_user_ready :));
}

private nomask void rcv_url(string s)
{
    url = s;
    done_with_inputs();
}

private nomask void rcv_real_name(string str)
{
    real_name = str;
    modal_func((: rcv_url :), "Your home page address (if any): ");
}


private nomask void rcv_email_address(string str)
{
    email = str;

    modal_func((: rcv_real_name :), "Your real name: ");
}

private nomask void rcv_gender(string arg)
{
    arg = lower_case(arg);
    if (arg == "y" || arg == "yes")
    {
	write("Ha, ha, ha. Which one are you?\n");
	return;
    }
    if (arg == "n" || arg == "no")
    {
	write("Well, which one would you have liked to be, then?\n");
	return;
    }
    if ( arg == "f" || arg == "female")
	n_gen = 2;
    else if (arg != "m" && arg != "male")
    {
	write("I've never heard of that gender.  Please try again.\n");
	return;
    }
    else
	n_gen = 1;


    write("\n"
	  "The following info is only seen by you and administrators\n"
	  "  if you prepend a # to your response.\n"
	  "\n"
	  "You cannot gain wizard status without valid responses to these questions:\n"
	);

    modal_func((: rcv_email_address :), "Your email address: ");
}

static nomask void begin_info_collection()
{
    modal_push((: rcv_gender :), "Are you male or female? ");
}
