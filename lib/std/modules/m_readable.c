/* Do not remove the headers from this file! see /USAGE for more info. */


/*
** From OBJ::description
*/
string the_short();


mapping 	        entries;
mapping                 synonyms;
private string		read_text;


void
set_text( string t )
{
    if (t[0] == '/' && file_size(t))
	read_text = read_file(t);
    else
	read_text = t;
}

mixed query_text()
{
    return read_text;
}

string read()
{
    this_body()->simple_action("$N $vread the $o.", this_object());
    return read_text;
}

string
read_entry( string entry )
{

    entry = entries[entry];
    if(!entry)
    {
	entry = entries[synonyms[entry]];
    }

    if (functionp(entry))
	return evaluate(entry);

    if (stringp(entry) && file_size(entry) > 0)
	return read_file(entry);

    return 0;
}

int
has_entries()
{
    return mapp( entries );
}

/* Parser interaction */
mixed direct_read_obj(object ob) {
    object o;
    
    if (!read_text) {
	if (entries)
	    return "It has a number of entries about various topics.\n";
	return "It appears to be blank.\n";
    }

    o = owner(this_object());
    if (o && o != this_body())
return "#Try asking " + o->query_name() + " nicely if you can read it.\n";

    return 1;
}

mixed direct_read_word_obj(string p, object ob) {
    return direct_read_obj(ob);
}

mixed direct_read_str_word_obj(string str, string p, object ob) {
    if (!entries) {
	if (read_text)
	    return "It's fairly short.  Just read it all.\n";
	return 0;
    }
    return 1;
}

mixed direct_read_word_str_word_obj(string p1, string str, string p2,
  object ob) {
    return direct_read_str_word_obj(str, p2, ob);
}

void set_entries(mapping e)
{
    entries = e;
}

void set_entry_synonyms(mapping s)
{
    synonyms = s;
}

mapping lpscript_attributes() {
    return ([
        "text" : ({ LPSCRIPT_STRING, "setup", "set_text" }),
    ]);
}

