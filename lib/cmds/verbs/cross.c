/* Do not remove the headers from this file! see /USAGE for more info. */


inherit NVERB_OB;

void do_cross_obj(object ob)
{
    ob->cross();
}

void create() {
    add_rules( ({ "OBJ" }) );
}
