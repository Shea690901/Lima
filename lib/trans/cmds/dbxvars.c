/* Do not remove the headers from this file! see /USAGE for more info. */

// Beek

#include <mudlib.h>
inherit CMD;

private void main(string str) {
    string obname;
    string var;
    string *vars;
    object ob;
    function f;

    if (!str || sscanf(str, "%s %s", obname, var) != 2) {
        write("dbxvars <ob> <var>\n");
        return;
    }
    
    obname = evaluate_path(obname);

    if (!(ob = find_object(obname))) {
        write("Can't find object " + obname + "\n");
        return;
    }

    vars = regexp(variables(ob), var);
    
    if (!sizeof(vars)) {
        write("No matches\n");
        return;
    }

    write("Matches:\n");
    f = unguarded( 1, (: bind( (: fetch_variable :), $(ob)) :));
    foreach (var in vars) {
        printf("%20s: %O\n", var, evaluate(f, var));
    }
    return;
}
