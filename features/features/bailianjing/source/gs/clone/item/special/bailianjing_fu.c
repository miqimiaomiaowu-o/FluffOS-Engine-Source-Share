#define JEWELRY_POLAR_D "/gs/daemons/jewelry_polar.c"

inherit "/gs/inherit/item/special_item.c";

private void create()
{
    object me;

    if (strsrch(object_name(this_object()), "#") >= 0) return;

    me = this_object();
    me->set_name("百练晶");
    me->import("百练晶");
    me->set("attrib", 260);
    me->set("sell_value", 0);
    me->setup_special_item();
}

int is_bailianjing_fu()
{
    return 1;
}

mixed apply()
{
    object me;
    int pos;

    me = env_object();
    if (!me) return 0;
    pos = this_object()->query("position");
    me->send(pos, 4579);
    return 0;
}

mixed identify_equip(mixed target)
{
    object me;

    me = env_object();
    if (!me) return 0;
    return JEWELRY_POLAR_D->use_on_target(me, this_object(), target);
}
