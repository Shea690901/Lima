/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/obj/admtool/internal/base.c";


nomask string module_name() {
    return "guild";
}

nomask string module_key() {
    return "G";
}

nomask class command_info array module_commands() {
    return ({
	new(class command_info,
	    key : "",
	    desc : "<none so far>")
	    });
}
