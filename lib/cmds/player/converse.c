/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;
inherit M_INPUT;


#define BLANKLINE "                                                                              "
#define PROMPT "*"


private nomask void handle_piping(string arg)
{
    if( arg == "**" || arg == ".")
    {
	write("Done.\n");
	modal_pop();
	destruct();
	return;
    }

    resend("/cmds/player/say", arg);
}

nomask void start_cmd()
{
    if(!clonep() || (base_name(previous_object()) != base_name()))  {
	write("Illegal attempt to spoof command\n");
	destruct();
	return;
    }

    modal_push((: handle_piping :),
#if 0 // Clients can't handle \r without \n well - Beek
      "\r" BLANKLINE "\r" PROMPT "\r"
#else
      PROMPT "\b"
#endif
    );
}


nomask private void main()
{
    if(!clonep())  {
	out("Entering converse mode. Type '**' or '.' to quit.\n");
	out("-------------------------------------------------\n");
	new(base_name())->start_cmd();
	return;
    }
    destruct();
}

