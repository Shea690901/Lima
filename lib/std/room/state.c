/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** room/state.c -- state mechanism for rooms
**
** 96...., Rust: created
*/

private static string array room_state = ({});
private static mapping room_state_extra_longs = ([]);

string array get_room_state_info()
{
  return copy(room_state);
}

string get_state_specific_long()
{
  string retval = "";
  mixed s;

  foreach(string state in room_state)
    {
      if(s = room_state_extra_longs[state])
	retval += evaluate(s)+"\n";
    }
  if(retval == "")
    return "\n";
  return retval;
}

int query_state(string state) {
    return member_array(state + "_on", room_state) != -1;
}

void set_room_state(string state)
{
  room_state -= ({state+"_off",state+"_on"});
  room_state += ({state+"_on"});
}

void clear_room_state(string state)
{
  room_state -= ({state+"_on",state+"_off"});
  room_state += ({state+"_off"});
}

void set_state_description(string state, mixed desc)
{
  if((strlen(state) < 4) || ((state[<3..] != "_on")
			     && (state[<4..] != "_off")))
    {
      error("State description must be state + _on or _off\n");
    }
  if(state[<3] == '_')
    {
      if((member_array(state, room_state) == -1) &&
	 (member_array(state[0..<4]+"_off",room_state) == -1))
	clear_room_state(state[0..<4]);
    }
  else
    {
      if((member_array(state, room_state) == -1) &&
	 (member_array(state[0..<5]+"_on",room_state) == -1))
	clear_room_state(state[0..<5]);
    }
  room_state_extra_longs[state] = desc;
}
