/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
// Takes the specified cloned object, dests it, updates its base
// file (NOT its inherits), reclones it and replaces it to its
// original environment.
// Usage: renew <object>


#include <commands.h>

inherit CMD;

private void main(mixed *arg) {
    object ob, env;
    string file;
    string data;
    
    ob = arg[0];
    file = base_name(ob);
    sscanf(file, "%s.c", file);
    if (file_size(file+".c")<0) {
	outf("renew: file '" + file + ".c' does not exist (or is a directory).\n");
	return 0;
    }
    env = environment(ob);
    data = ob->save_to_string(1);
    ob->remove();
    outf("Updating '" + file + ".c' ...\n");
    if (CMD_OB_UPDATE->do_update(file + ".c", 3) < time())
        out(file + ": No update necessary.\n");
    ob = new(file);
    ob->load_from_string(data, 1);
    ob->move(env);
}
