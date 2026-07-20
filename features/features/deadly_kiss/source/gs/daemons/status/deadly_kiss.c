// ËÀÍö²øÃà×´Ì¬£ºÆÕÍ¨ÎïÀí¹¥»÷ÓëÆÕÍ¨·¨Êõ¹¥»÷¾ù´¥·¢2´Î¹¥»÷¡£

mixed status_id;

mixed sf_action_translate(mixed me, mixed para)
{
    mixed cmd, action, child_action;

    if (!is_object(me)) return 0;
    cmd = me->lite_query_temp("got_command");
    if (!is_mapping(cmd)) return 0;
    action = cmd["action"];
    child_action = cmd["child_action"];
    if (child_action == 50) return 0;
    if (action == 2) {
        cmd["action"] = 47;
        me->lite_set_temp("got_command", cmd);
        return 1;
    }
    return 0;
}

mixed sf_change_to_double_magic_hit(mixed me, mixed para)
{
    mixed cmd;

    if (!is_object(me)) return 0;
    if (!me->is_in_status("deadly_kiss")) return 0;
    cmd = me->lite_query_temp("got_command");
    if (!is_mapping(cmd)) return 0;
    if (cmd["action"] != 3) return 0;
    cmd["double_hit_times"] = 2;
    me->lite_set_temp("got_command", cmd);
    me->set_temp("deadly_kiss/magic_used", 1);
    me->set_temp("deadly_kiss/magic_para", cmd["para"]);
    me->set_temp("deadly_kiss/magic_victim_id", cmd["victim_id"]);
    return "fantianyin";
}

mixed sf_when_active_double_magic(mixed me, mixed para)
{
    mixed cmd;

    if (!is_object(me)) return 0;
    if (!me->query_temp("deadly_kiss/magic_used")) return 0;
    if (sizeof(para) < 2) return 0;
    cmd = para[1];
    if (!is_mapping(cmd)) return 0;
    cmd["para"] = me->query_temp("deadly_kiss/magic_para");
    cmd["victim_id"] = me->query_temp("deadly_kiss/magic_victim_id");
    cmd["no_cost_mana"] = 1;
    cmd["double_hit_times"] = 2;
    return cmd;
}

mixed sf_do_action_finished(mixed me, mixed para)
{
    if (!is_object(me)) return 0;
    if (!me->query_temp("deadly_kiss/magic_used")) return 0;
    me->delete_temp("deadly_kiss/magic_used");
    me->delete_temp("deadly_kiss/magic_para");
    me->delete_temp("deadly_kiss/magic_victim_id");
    me->remove_status("deadly_kiss");
    return 1;
}

mixed apply_status(mixed me, mixed value, mixed round_num, mixed para)
{
    mixed filter;

    if (!is_object(me)) return 0;
    me->remove_status("yangjing_xurui");
    if (me->is_interface_existed(this_object(), me)) {
        me->unregister_interface(this_object(), me);
    }
    filter = ([
        "sf_action_translate" : (: call_other, this_object(), "sf_action_translate", me :),
        "sf_change_to_double_magic_hit" : (: call_other, this_object(), "sf_change_to_double_magic_hit", me :),
        "sf_when_active_double_magic" : (: call_other, this_object(), "sf_when_active_double_magic", me :),
        "sf_do_action_finished" : (: call_other, this_object(), "sf_do_action_finished", me :)
    ]);
    me->register_interface(this_object(), me, filter);
    return 1;
}

mixed clear_status(mixed me)
{
    if (!is_object(me)) return 0;
    me->delete_temp("deadly_kiss/magic_used");
    me->delete_temp("deadly_kiss/magic_para");
    me->delete_temp("deadly_kiss/magic_victim_id");
    if (me->is_interface_existed(this_object(), me)) {
        me->unregister_interface(this_object(), me);
    }
    return 1;
}

mixed create()
{
    status_id = "/gs/daemons/statusd.c"->register_status("deadly_kiss", 27, 1);
    "/gs/daemons/statusd.c"->register_entry("deadly_kiss", this_object(), 1);
    return 1;
}
