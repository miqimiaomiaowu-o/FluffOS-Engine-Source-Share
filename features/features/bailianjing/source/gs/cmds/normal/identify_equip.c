#define JEWELRY_POLAR_D "/gs/daemons/jewelry_polar.c"

private int _tool_kind(mixed item)
{
    mixed name;
    mixed flag;

    if (!item) return 0;
    name = 0;
    if (!catch(name = item->get_name())) {
        if (name == "鉴定符") return 1;
        if (name == "百练晶") return 2;
    }
    flag = 0;
    if (!catch(flag = item->is_bailianjing_fu()) && flag) return 2;
    return 0;
}

private mixed _item_at(mixed inv, mixed arg)
{
    mixed item;

    item = 0;
    if (!catch(item = inv[arg]) && item) return item;
    return 0;
}

private mixed _tool_from_arg(mixed inv, mixed arg)
{
    mixed item;
    int i;
    int count;

    item = _item_at(inv, arg);
    if (_tool_kind(item)) return item;
    if (_tool_kind(arg)) return arg;

    count = 0;
    if (!catch(count = sizeof(arg)) && count > 0 && count < 8) {
        for (i = 0; i < count; i++) {
            item = _tool_from_arg(inv, arg[i]);
            if (item) return item;
        }
    }
    return 0;
}

private mixed _find_tool(mixed inv)
{
    mixed item;
    mixed keys_arr;
    int i;

    keys_arr = keys(inv);
    for (i = 0; i < sizeof(keys_arr); i++) {
        item = inv[keys_arr[i]];
        if (_tool_kind(item)) return item;
    }
    return 0;
}

private mixed _target_from_arg(mixed inv, mixed arg)
{
    mixed item;
    int i;
    int count;

    item = _item_at(inv, arg);
    if (item && !_tool_kind(item)) return item;
    if (arg && !_tool_kind(arg) && !catch(arg->get_name())) return arg;

    count = 0;
    // ponytail: only unwrap shallow client drag tuples; expand if this client sends deeper protocol data.
    if (!catch(count = sizeof(arg)) && count > 0 && count < 8) {
        for (i = 0; i < count; i++) {
            item = _target_from_arg(inv, arg[i]);
            if (item) return item;
        }
    }
    return 0;
}

mixed main(mixed me, mixed tool_arg, mixed target_arg)
{
    mixed inv;
    mixed tool;
    mixed target;
    mixed msg;
    int kind;

    if (!me) return 0;
    me->send_dialog_ok("BJL identify_equip entered");
    inv = me->query_inventory();

    tool = _tool_from_arg(inv, tool_arg);
    if (!tool) tool = _tool_from_arg(inv, target_arg);
    if (!tool) tool = _find_tool(inv);
    if (!tool) {
        me->send_dialog_ok("你当前没有#R鉴定符#n或#R百练晶#n位于可使用的包裹。");
        return 0;
    }
    kind = _tool_kind(tool);

    target = _target_from_arg(inv, target_arg);
    if (!target) target = _target_from_arg(inv, tool_arg);
    if (!target) {
        me->send_dialog_ok("请选择要使用百练晶的首饰。");
        return 0;
    }

    if (kind != 2) {
        if (!(me->is_deny("_apply_item"))) {
            me->send_dialog_ok("你现在不能使用道具。");
            return 0;
        }
    }

    msg = "";
    if (!(tool->can_be_deal(&msg, 7))) {
        me->send_dialog_ok(msg);
        return 0;
    }

    msg = "/gs/daemons/itemd.c"->is_apply_forbidden(tool, me);
    if (msg) {
        me->send_dialog_ok(msg);
        return 0;
    }

    if (kind == 2) {
        JEWELRY_POLAR_D->use_on_target(me, tool, target);
    } else {
        tool->identify_equip(target);
    }
    return 1;
}
