/* Do not remove the headers from this file! see /USAGE for more info. */

// An object that can be opened, closed, locked, and unlocked.
//
// NOTE: this should only be mixed into classes that have
//       M_OPENABLE, too.
// NOTE: due to a bug in the driver(?), you must inherit M_OPENABLE
//       _after_ inheriting M_LOCKABLE.
//

string locked, key_type;
string unlock_msg = "$N $vunlock the $o with $p $o1.\n";
string unlock_fail = "$N $vtry to unlock the $o, but $p $o1 doesn't fit.\n";
string lock_msg = "$N $vlock the $o with $p $o1.\n";
string lock_fail = "$N $vtry to lock the $o, but $p $o1 doesn't fit.\n";

string query_locked() { return locked; }
void set_locked(string x) { locked = x; key_type = x; }
int prevent_unlock() { return 0; }

int prevent_open()
{
    if (query_locked())
    {
	this_body()->my_action("The $o is locked.\n", this_object());
	return 1;
    }
    return 0;
}

void unlock_with(object ob)
{
    if (this_object()->prevent_unlock())
	return;

    if (ob->key_type() == key_type)
    {
	this_body()->simple_action(unlock_msg, this_object(), ob);
	locked = 0;
    }
    else
    {
	this_body()->simple_action(unlock_fail, this_object(), ob);
    }
}

void lock_with(object ob) {
    if (ob->key_type() == key_type) {
	this_body()->simple_action(lock_msg, this_object(), ob);
	set_locked(key_type);
    }
    else {
	this_body()->simple_action(lock_fail, this_object(), ob);
    }
}

/*********************************************************/
/*    verb interactions                                   */
