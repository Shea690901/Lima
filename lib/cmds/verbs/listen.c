/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** smell.c
**
*/

inherit VERB_OB;

mixed can_listen_to_obj(object ob)
{
    return 1;
}

mixed direct_listen_to_obj(object ob) 
{
    return 1;
}

mixed can_listen ()
{
  return 1;
}

void do_listen_to_obj(object ob)
{
    if ( !ob->sound() )
    {
	printf("It sounds pretty much like %s.\n", ob->a_short());
    }
}

void do_listen ()
{
  if (!environment (this_body ()) || !environment (this_body ())->sound ())
    {
      printf ("You hear nothing unusual.\n");
    }
}

mixed * query_verb_info()
{
    return ({ ({ "", "to OBJ" }), ({ }) });
}
