/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;

private void main(string array argv)
{
    mixed result;


    result = CMD_D->find_cmd_in_path(argv[0], 
				     this_user()->query_shell_ob()->query_path());

    if ( result == -1 )
    {
	out("A file with that name exists in your path, but doesn't have a main() function.\n");
	return;
    }

    if ( !result )
    {
	out("No such command found in your path.\n");
	return;
    }
    outf("%s is found in: %s\n", argv[0], result[1]);
    return;
}

