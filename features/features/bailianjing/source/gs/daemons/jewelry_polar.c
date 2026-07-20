#define JEWELRY_POLAR_D "/gs/daemons/jewelry_polar.c"
#define CONVERT_D "/gs/daemons/upgrade/convert_jewelry_attrib.c"
#define ITEMD "/gs/daemons/itemd.c"

#define ITEM_NAME "百练晶"
#define SUCCESS_RATE_PERCENT 30
#define ENHANCE_ADD_VALUE 1

#define TOKEN_ITEM "bailianjing/item"
#define TOKEN_IID  "bailianjing/item_iid"
#define TOKEN_MARK "bailianjing/mark"
#define ENHANCE_COUNT_KEY "bailianjing/enhance_count"
#define EXTRA_DESC_KEY "bailianjing/orig_extra_desc"
#define EXTRA_DESC_MARK "bailianjing/has_extra_desc"
#define CONFIRM_ITEM "bailianjing/confirm_item"
#define CONFIRM_TARGET "bailianjing/confirm_target"
#define CONFIRM_KEY "bailianjing/confirm_key"
#define CONFIRM_BASE "bailianjing/confirm_base"

mixed apply_task_item(object me, mixed arg);

private string query_marker()
{
    return "bailianjing-jewelry-polar-v1-20260629";
}

private void _msg(object me, string text)
{
    if (!me || !text) return;
    if (catch(me->send_dialog_ok(text))) {
    }
}

private mixed _safe_item_name(mixed item)
{
    mixed name;

    if (!item) return 0;
    name = 0;
    if (catch(name = item->get_name())) name = 0;
    return name;
}

private int _to_int(mixed value)
{
    int n;

    if (!value) return 0;
    n = 0;
    if (catch(n = value + 0)) return 0;
    return n;
}

private mixed _append_inventory_items(mixed out, mixed inv)
{
    mixed item;
    int i;

    if (!out) out = ({});
    if (!inv) return out;
    if (catch(sizeof(inv))) return out;
    for (i = 0; i < sizeof(inv); i++) {
        item = 0;
        if (catch(item = inv[i])) item = 0;
        if (item) out += ({ item });
    }
    return out;
}

private mixed _inventory(object me)
{
    mixed out;
    mixed inv;
    mixed db;

    if (!me) return 0;
    out = ({});

    inv = 0;
    if (!catch(inv = me->query_inventory())) out = _append_inventory_items(out, inv);
    inv = 0;
    if (!catch(inv = me->query_inventory_arr())) out = _append_inventory_items(out, inv);
    inv = 0;
    if (!catch(inv = me->get_inventory_objs())) out = _append_inventory_items(out, inv);
    inv = 0;
    if (!catch(inv = me->query("inventory"))) out = _append_inventory_items(out, inv);
    inv = 0;
    if (!catch(inv = me->lite_query("inventory"))) out = _append_inventory_items(out, inv);
    db = 0;
    if (!catch(db = me->query_entire_dbase()) && db) {
        inv = 0;
        if (!catch(inv = db["inventory"])) out = _append_inventory_items(out, inv);
    }

    if (sizeof(out) > 0) return out;
    return 0;
}

private mixed _find_owner_token(object me)
{
    mixed items;
    mixed item;
    mixed name;
    int i;

    if (!me) return 0;
    items = 0;
    if (catch(items = _inventory(me)) || !items) return 0;
    for (i = 0; i < sizeof(items); i++) {
        item = items[i];
        if (!item) continue;
        if (!catch(item->is_bailianjing_fu()) && item->is_bailianjing_fu()) return item;
        name = _safe_item_name(item);
        if (name == ITEM_NAME) return item;
    }
    return 0;
}

private void _consume_item(mixed item)
{
    if (!item) return;
    if (!catch(item->cost_amount(1))) return;
    if (!catch(item->destruct())) return;
}

private void _clear_confirm(object me)
{
    if (!me) return;
    if (catch(me->delete_temp(CONFIRM_ITEM))) {
    }
    if (catch(me->delete_temp(CONFIRM_TARGET))) {
    }
    if (catch(me->delete_temp(CONFIRM_KEY))) {
    }
    if (catch(me->delete_temp(CONFIRM_BASE))) {
    }
}

private mixed _submit_item_at(mixed items, int idx)
{
    mixed ks;

    if (!items) return 0;
    if (!catch(ks = keys(items))) {
        if (sizeof(ks) <= idx) return 0;
        return ks[idx];
    }
    if (!catch(items + "")) return 0;
    if (catch(sizeof(items))) return 0;
    if (sizeof(items) <= idx) return 0;
    return items[idx];
}

private int _looks_like_item(mixed item)
{
    mixed name;

    if (!item) return 0;
    name = 0;
    if (catch(name = item->get_name())) return 0;
    return name ? 1 : 0;
}

private mixed _query_submit_items(object me)
{
    mixed items;

    if (!me) return 0;
    items = 0;
    if (catch(items = me->query_temp("submit_items"))) items = 0;
    return items;
}

private mixed _pick_submit_target(object me, mixed a, mixed b, mixed c, mixed d, mixed e)
{
    mixed item;

    item = _submit_item_at(a, 0);
    if (_looks_like_item(item)) return item;
    item = _submit_item_at(b, 0);
    if (_looks_like_item(item)) return item;
    item = _submit_item_at(c, 0);
    if (_looks_like_item(item)) return item;
    item = _submit_item_at(d, 0);
    if (_looks_like_item(item)) return item;
    item = _submit_item_at(e, 0);
    if (_looks_like_item(item)) return item;

    item = _submit_item_at(_query_submit_items(me), 0);
    if (_looks_like_item(item)) return item;
    return 0;
}

private int _query_jewelry_score(mixed target)
{
    mixed max_times;

    if (!target) return 0;
    max_times = 0;
    if (catch(max_times = CONVERT_D->query_jewelry_max_convert_times(target))) return 0;
    return _to_int(max_times);
}

private int _query_path_int(mixed target, string key)
{
    mixed value;

    if (!target || !key) return 0;
    value = 0;
    if (catch(value = target->query(key))) return 0;
    return _to_int(value);
}

private int _query_jewelry_level(mixed target)
{
    mixed value;
    int level;

    if (!target) return 80;

    value = 0;
    if (!catch(value = target->query_req_level(0))) {
        level = _to_int(value);
        if (level > 0) return level;
    }
    value = 0;
    if (!catch(value = target->query("req_level"))) {
        level = _to_int(value);
        if (level > 0) return level;
    }
    value = 0;
    if (!catch(value = target->query("level"))) {
        level = _to_int(value);
        if (level > 0) return level;
    }
    value = 0;
    if (!catch(value = target->query("item_level"))) {
        level = _to_int(value);
        if (level > 0) return level;
    }
    value = 0;
    if (!catch(value = target->query("equip_level"))) {
        level = _to_int(value);
        if (level > 0) return level;
    }
    return 80;
}

private int _query_real_jewelry_level(mixed target)
{
    mixed value;
    int level;

    if (!target) return 0;

    value = 0;
    if (!catch(value = target->query_req_level(0))) {
        level = _to_int(value);
        if (level > 0) return level;
    }
    value = 0;
    if (!catch(value = target->query("req_level"))) {
        level = _to_int(value);
        if (level > 0) return level;
    }
    value = 0;
    if (!catch(value = target->query("level"))) {
        level = _to_int(value);
        if (level > 0) return level;
    }
    value = 0;
    if (!catch(value = target->query("item_level"))) {
        level = _to_int(value);
        if (level > 0) return level;
    }
    value = 0;
    if (!catch(value = target->query("equip_level"))) {
        level = _to_int(value);
        if (level > 0) return level;
    }
    return 0;
}

private int _max_enhance_count_by_level(int level)
{
    if (level <= 100) return 3;
    if (level <= 110) return 5;
    if (level <= 120) return 8;
    if (level <= 130) return 10;
    if (level <= 140) return 15;
    if (level <= 150) return 20;
    if (level <= 160) return 25;
    if (level <= 170) return 30;
    return 35;
}

private int _single_attr_cap_by_level(int level)
{
    if (level <= 80) return 20;
    if (level <= 90) return 22;
    if (level <= 100) return 25;
    if (level <= 110) return 27;
    if (level <= 120) return 30;
    if (level <= 130) return 32;
    if (level <= 140) return 35;
    if (level <= 150) return 38;
    if (level <= 160) return 41;
    return 44;
}

private int _is_single_attr(string base)
{
    return base == "str" || base == "con" || base == "dex" || base == "wiz";
}

private int _is_ignore_resist_single(string base)
{
    return base == "ignore_resist_metal" || base == "ignore_resist_wood" ||
        base == "ignore_resist_water" || base == "ignore_resist_fire" ||
        base == "ignore_resist_earth" || base == "ignore_resist_frozen" ||
        base == "ignore_resist_sleep" || base == "ignore_resist_forgotten" ||
        base == "ignore_resist_confusion" || base == "ignore_resist_poison" ||
        base == "ignore_resist_repress" || base == "ignore_resist_melt" ||
        base == "ignore_resist_cage" || base == "ignore_resist_lock" ||
        base == "ignore_resist_lost";
}

private int _is_resist_except_single(string base)
{
    return base == "resist_frozen" || base == "resist_sleep" ||
        base == "resist_forgotten" || base == "resist_confusion" ||
        base == "resist_poison" || base == "resist_repress" ||
        base == "resist_melt" || base == "resist_cage" ||
        base == "resist_lock" || base == "resist_lost";
}

private int _is_resist_polar_single(string base)
{
    return base == "resist_metal" || base == "resist_wood" ||
        base == "resist_water" || base == "resist_fire" ||
        base == "resist_earth";
}

private int _itemd_attr_cap(mixed target, string base)
{
    mixed value;
    int level;
    int cap;

    if (!target || !base || base == "") return 0;
    level = _query_real_jewelry_level(target);
    if (level <= 0) return 0;

    value = 0;
    if (catch(value = ITEMD->get_attrib_max_value(target, base, level))) return 0;
    cap = _to_int(value);
    if (cap > 0) return cap;
    return 0;
}

private int _attr_cap(mixed target, string base)
{
    int cap;

    if (!base || base == "") return 0;
    if (base == "all_polar") return 10;
    cap = _itemd_attr_cap(target, base);
    if (cap > 0) return cap;
    if (_is_single_attr(base)) return _single_attr_cap_by_level(_query_jewelry_level(target));
    if (base == "all_skill") return 10;
    if (base == "ignore_all_resist" || base == "ignore_all_resist_except") return 20;
    if (_is_ignore_resist_single(base)) return 30;
    if (base == "all_resist_except") return 15;
    if (_is_resist_except_single(base)) return 20;
    if (_is_resist_polar_single(base)) return 15;
    return 0;
}

private string _attr_show_name(string base)
{
    if (base == "all_polar") return "所有相性";
    if (base == "metal") return "金相性";
    if (base == "wood") return "木相性";
    if (base == "water") return "水相性";
    if (base == "fire") return "火相性";
    if (base == "earth") return "土相性";
    if (base == "str") return "力量";
    if (base == "con") return "体质";
    if (base == "dex") return "敏捷";
    if (base == "wiz") return "灵力";
    if (base == "accurate") return "准确";
    if (base == "mana" || base == "max_mana") return "法力";
    if (base == "life" || base == "max_life") return "气血";
    if (base == "ignore_all_resist") return "忽视所有抗性";
    if (base == "ignore_all_resist_except") return "忽视所有抗异常";
    if (base == "ignore_resist_metal") return "忽视目标抗金";
    if (base == "ignore_resist_wood") return "忽视目标抗木";
    if (base == "ignore_resist_water") return "忽视目标抗水";
    if (base == "ignore_resist_fire") return "忽视目标抗火";
    if (base == "ignore_resist_earth") return "忽视目标抗土";
    if (base == "ignore_resist_frozen") return "忽视目标抗冰冻";
    if (base == "ignore_resist_sleep") return "忽视目标抗昏睡";
    if (base == "ignore_resist_forgotten") return "忽视目标抗遗忘";
    if (base == "ignore_resist_confusion") return "忽视目标抗混乱";
    if (base == "ignore_resist_poison") return "忽视目标抗中毒";
    if (base == "ignore_resist_repress") return "忽视目标抗压制";
    if (base == "ignore_resist_melt") return "忽视目标抗消融";
    if (base == "ignore_resist_cage") return "忽视目标抗牢笼";
    if (base == "ignore_resist_lock") return "忽视目标抗锁灵";
    if (base == "ignore_resist_lost") return "忽视目标抗迷失";
    if (base == "resist_metal") return "抗金";
    if (base == "resist_wood") return "抗木";
    if (base == "resist_water") return "抗水";
    if (base == "resist_fire") return "抗火";
    if (base == "resist_earth") return "抗土";
    if (base == "resist_frozen") return "抗冰冻";
    if (base == "resist_sleep") return "抗昏睡";
    if (base == "resist_forgotten") return "抗遗忘";
    if (base == "resist_confusion") return "抗混乱";
    if (base == "resist_poison") return "抗中毒";
    if (base == "resist_repress") return "抗压制";
    if (base == "resist_melt") return "抗消融";
    if (base == "resist_cage") return "抗牢笼";
    if (base == "resist_lock") return "抗锁灵";
    if (base == "resist_lost") return "抗迷失";
    if (base == "all_resist_except") return "所有抗异常";
    if (base == "all_skill") return "所有技能";
    return base;
}

private mixed _add_candidate_key(mixed target, mixed keys, string key, string base)
{
    int value;
    int cap;

    if (!keys) keys = ({});
    if (!key || key == "") return keys;
    value = _query_path_int(target, key);
    if (value <= 0) return keys;
    cap = _attr_cap(target, base);
    if (cap <= 0) return keys;
    if (value >= cap) return keys;
    keys += ({ ({ key, base }) });
    return keys;
}

private mixed _add_candidate_base(mixed target, mixed keys, string base)
{
    if (!base || base == "") return keys;

    keys = _add_candidate_key(target, keys, base, base);
    keys = _add_candidate_key(target, keys, "prop/" + base, base);
    keys = _add_candidate_key(target, keys, "prop1/" + base, base);
    keys = _add_candidate_key(target, keys, "prop2/" + base, base);
    keys = _add_candidate_key(target, keys, "prop3/" + base, base);
    keys = _add_candidate_key(target, keys, "prop4/" + base, base);
    keys = _add_candidate_key(target, keys, "prop5/" + base, base);
    return keys;
}

private mixed _find_random_attr_key(mixed target)
{
    mixed keys;

    if (!target) return 0;

    keys = ({});
    keys = _add_candidate_base(target, keys, "all_polar");
    keys = _add_candidate_base(target, keys, "metal");
    keys = _add_candidate_base(target, keys, "wood");
    keys = _add_candidate_base(target, keys, "water");
    keys = _add_candidate_base(target, keys, "fire");
    keys = _add_candidate_base(target, keys, "earth");
    keys = _add_candidate_base(target, keys, "str");
    keys = _add_candidate_base(target, keys, "con");
    keys = _add_candidate_base(target, keys, "dex");
    keys = _add_candidate_base(target, keys, "wiz");
    keys = _add_candidate_base(target, keys, "accurate");
    keys = _add_candidate_base(target, keys, "mana");
    keys = _add_candidate_base(target, keys, "life");
    keys = _add_candidate_base(target, keys, "max_mana");
    keys = _add_candidate_base(target, keys, "max_life");
    keys = _add_candidate_base(target, keys, "ignore_all_resist");
    keys = _add_candidate_base(target, keys, "ignore_all_resist_except");
    keys = _add_candidate_base(target, keys, "ignore_resist_metal");
    keys = _add_candidate_base(target, keys, "ignore_resist_wood");
    keys = _add_candidate_base(target, keys, "ignore_resist_water");
    keys = _add_candidate_base(target, keys, "ignore_resist_fire");
    keys = _add_candidate_base(target, keys, "ignore_resist_earth");
    keys = _add_candidate_base(target, keys, "ignore_resist_frozen");
    keys = _add_candidate_base(target, keys, "ignore_resist_sleep");
    keys = _add_candidate_base(target, keys, "ignore_resist_forgotten");
    keys = _add_candidate_base(target, keys, "ignore_resist_confusion");
    keys = _add_candidate_base(target, keys, "ignore_resist_poison");
    keys = _add_candidate_base(target, keys, "ignore_resist_repress");
    keys = _add_candidate_base(target, keys, "ignore_resist_melt");
    keys = _add_candidate_base(target, keys, "ignore_resist_cage");
    keys = _add_candidate_base(target, keys, "ignore_resist_lock");
    keys = _add_candidate_base(target, keys, "ignore_resist_lost");
    keys = _add_candidate_base(target, keys, "resist_metal");
    keys = _add_candidate_base(target, keys, "resist_wood");
    keys = _add_candidate_base(target, keys, "resist_water");
    keys = _add_candidate_base(target, keys, "resist_fire");
    keys = _add_candidate_base(target, keys, "resist_earth");
    keys = _add_candidate_base(target, keys, "resist_frozen");
    keys = _add_candidate_base(target, keys, "resist_sleep");
    keys = _add_candidate_base(target, keys, "resist_forgotten");
    keys = _add_candidate_base(target, keys, "resist_confusion");
    keys = _add_candidate_base(target, keys, "resist_poison");
    keys = _add_candidate_base(target, keys, "resist_repress");
    keys = _add_candidate_base(target, keys, "resist_melt");
    keys = _add_candidate_base(target, keys, "resist_cage");
    keys = _add_candidate_base(target, keys, "resist_lock");
    keys = _add_candidate_base(target, keys, "resist_lost");
    keys = _add_candidate_base(target, keys, "all_resist_except");
    keys = _add_candidate_base(target, keys, "all_skill");

    if (!keys || sizeof(keys) <= 0) return 0;
    return keys[random(sizeof(keys))];
}

private int _query_enhance_count(mixed target)
{
    mixed count;

    if (!target) return 0;
    count = 0;
    if (catch(count = target->query(ENHANCE_COUNT_KEY))) count = 0;
    return _to_int(count);
}

private void _clear_count_desc(mixed target)
{
    mixed extra;
    mixed mark;

    if (!target) return;
    mark = 0;
    if (catch(mark = target->query(EXTRA_DESC_MARK))) mark = 0;
    if (!mark) return;
    extra = "";
    if (catch(extra = target->query(EXTRA_DESC_KEY))) extra = "";
    if (!extra) extra = "";
    catch(target->set("extra_desc", extra));
    catch(target->set(EXTRA_DESC_KEY, 0));
    catch(target->set(EXTRA_DESC_MARK, 0));
}

private mixed _check_target(mixed target)
{
    mixed key;
    string base;
    int jewelry_score;
    mixed is_jewelry;

    if (!target) return ({0, "请先提交要强化的首饰。"});
    jewelry_score = _query_jewelry_score(target);
    is_jewelry = 0;
    if (catch(is_jewelry = target->is_jewelry())) is_jewelry = 0;
    if (jewelry_score <= 0 && !is_jewelry) {
        return ({0, "百练晶只能对首饰使用。"});
    }
    key = _find_random_attr_key(target);
    if (!key) return ({0, "这件首饰没有可强化的属性。"});
    base = key[1];
    return ({1, key[0], base, 0, 0});
}

private int _add_attr(mixed target, string key, string base, int add)
{
    int old_value;
    int cap;
    int new_value;

    if (!target || !key) return 0;
    old_value = _query_path_int(target, key);
    if (old_value <= 0) return 0;
    new_value = old_value + add;
    cap = _attr_cap(target, base);
    if (cap <= 0) return 0;
    if (old_value >= cap) return 0;
    if (new_value > cap) new_value = cap;
    catch(target->set(key, new_value));
    return 1;
}

private int _success_rate(mixed target, string key, string base)
{
    int value;
    int cap;
    int rate;

    if (!target || !key || !base) return SUCCESS_RATE_PERCENT;
    value = _query_path_int(target, key);
    cap = _attr_cap(target, base);
    if (cap <= 0 || value <= 0) return SUCCESS_RATE_PERCENT;
    rate = 80 - value * 70 / cap;
    if (rate < 5) rate = 5;
    if (rate > 80) rate = 80;
    return rate;
}

private void _refresh_target(object me, mixed target)
{
    mixed pos;

    if (!me || !target) return;
    pos = 0;
    if (catch(pos = target->query("position"))) pos = 0;
    if (!pos && catch(pos = target->get_pos())) pos = 0;

    if (pos) {
        if (!catch(me->refresh_item(pos))) return;
    }
    if (catch(me->refresh_item(target))) {
    }
}

private void _return_to_bag(object me, mixed item)
{
    mixed pos;

    if (!me || !_looks_like_item(item)) return;
    if (catch(item->move_to_bag(me))) {
        if (catch(item->move(me))) {
        }
    }
    pos = 0;
    if (catch(pos = item->query("position"))) pos = 0;
    if (pos && !catch(me->refresh_item(pos))) return;
    if (catch(me->refresh_item(item))) {
    }
}

private mixed _menu_action(string fn)
{
    return (: call_other, JEWELRY_POLAR_D, fn :);
}

private void _activate_menu_session(object me, mixed menu_ob)
{
    if (!me || !menu_ob) return;
    if (catch(me->set_temp("ingore_distance_npc", 1))) {
    }
    if (catch(me->set_temp("current_npc", menu_ob))) {
    }
    if (catch(me->set_temp("click_npc", menu_ob))) {
    }
    if (catch(menu_ob->set_temp("ingore_distance_npc", 1))) {
    }
    if (catch(me->start_talk_to(menu_ob))) {
    }
}

private void _bind_menu(object me, string key, string fn)
{
    mixed action;

    if (!me || !key || key == "") return;
    action = _menu_action(fn);
    if (catch(me->set_temp("current_menu/" + key, action))) {
    }
    if (catch(me->set_temp("menu/" + key, action))) {
    }
}

private void _bind_menu_on(object ob, string key, string fn)
{
    mixed action;

    if (!ob || !key || key == "") return;
    action = _menu_action(fn);
    if (catch(ob->set_temp("current_menu/" + key, action))) {
    }
    if (catch(ob->set_temp("menu/" + key, action))) {
    }
}

private void _bind_default_menu(object me)
{
    mixed action;

    if (!me) return;
    action = _menu_action("menu_default_action");
    if (catch(me->set_temp("default_action", action))) {
    }
    if (catch(me->set_temp("extra_para_action", action))) {
    }
}

private void _bind_default_menu_on(object ob)
{
    mixed action;

    if (!ob) return;
    action = _menu_action("menu_default_action");
    if (catch(ob->set_temp("default_action", action))) {
    }
    if (catch(ob->set_temp("extra_para_action", action))) {
    }
}

private mixed _menu_object(object me)
{
    return me;
}

private void _prepare_menu_object(object me)
{
    if (!me) return;
    if (catch(me->set_temp("ingore_distance_npc", 1))) {
    }
}

private void _bind_confirm_actions(object me)
{
    _bind_menu(me, "bailianjing_confirm", "menu_confirm_enhance");
    _bind_menu(me, "确定", "menu_confirm_enhance");
    _bind_menu(me, "确认", "menu_confirm_enhance");
    _bind_menu(me, "$确定", "menu_confirm_enhance");
    _bind_menu(me, "$确认", "menu_confirm_enhance");
    _bind_menu(me, "bailianjing_cancel", "menu_cancel_enhance");
    _bind_menu(me, "取消", "menu_cancel_enhance");
    _bind_menu(me, "$取消", "menu_cancel_enhance");
}

private void _bind_confirm_actions_on(object ob)
{
    _bind_menu_on(ob, "bailianjing_confirm", "menu_confirm_enhance");
    _bind_menu_on(ob, "确定", "menu_confirm_enhance");
    _bind_menu_on(ob, "确认", "menu_confirm_enhance");
    _bind_menu_on(ob, "$确定", "menu_confirm_enhance");
    _bind_menu_on(ob, "$确认", "menu_confirm_enhance");
    _bind_menu_on(ob, "bailianjing_cancel", "menu_cancel_enhance");
    _bind_menu_on(ob, "取消", "menu_cancel_enhance");
    _bind_menu_on(ob, "$取消", "menu_cancel_enhance");
}

private void _show_submit_menu(object me)
{
    mixed content;
    mixed action;

    if (!me) return;

    content = "#R百练晶#n\n提交一件首饰后，将根据首饰当前属性接近满值的程度计算成功率；属性越接近满值，成功率越低。\n[提交首饰/$提交|请提交一件首饰,1,0][取消/离开]";

    _bind_menu(me, "$提交", "submit_item");
    _bind_menu(me, "$提交物品", "submit_item");
    _bind_menu(me, "提交首饰", "submit_item");
    action = _menu_action("submit_item");
    if (catch(me->set_temp("menu/$提交", action))) {
    }
    if (catch(me->set_temp("current_menu/$提交", action))) {
    }

    me->set_menu(me, "百练晶", content, _menu_action("menu_default_action"));
    _activate_menu_session(me, me);
    _bind_default_menu(me);
    _bind_menu(me, "$提交", "submit_item");
    _bind_menu(me, "$提交物品", "submit_item");
    _bind_menu(me, "提交首饰", "submit_item");
    if (catch(me->set_temp("menu/$提交", action))) {
    }
    if (catch(me->set_temp("current_menu/$提交", action))) {
    }
}

private mixed _use_checked_target(object me, mixed token_item, mixed target, string key, string base, int count, int max_count, int reopen_menu, int restore_target)
{
    int roll;
    int rate;
    int old_value;
    int new_value;
    string attr_name;
    string item_name;

    if (!me) return 0;
    if (!token_item) {
        _msg(me, "百练晶已经失效，请重新使用道具。");
        return 1;
    }

    rate = _success_rate(target, key, base);
    old_value = _query_path_int(target, key);
    _consume_item(token_item);
    roll = random(100);
    if (roll >= rate) {
        if (restore_target) _return_to_bag(me, target);
        _clear_count_desc(target);
        _refresh_target(me, target);
        _msg(me, "很遗憾，你的首饰未能吸收百练晶中的精华，首饰的属性没有得到提升。");
        return 1;
    }
    if (!_add_attr(target, key, base, ENHANCE_ADD_VALUE)) {
        if (restore_target) _return_to_bag(me, target);
        _clear_count_desc(target);
        _refresh_target(me, target);
        _msg(me, "很遗憾，你的首饰未能吸收百练晶中的精华，首饰的属性没有得到提升。");
        return 1;
    }
    new_value = _query_path_int(target, key);
    attr_name = _attr_show_name(base);
    item_name = _safe_item_name(target);
    if (!item_name || item_name == "") item_name = "首饰";
    _clear_count_desc(target);
    if (restore_target) _return_to_bag(me, target);
    _refresh_target(me, target);
    _msg(me, sprintf("你的#R%s#n吸收了百练晶中的精华，#R%s %d 增加#n提升至#R%s %d 增加#n。", item_name, attr_name, old_value, attr_name, new_value));
    return 1;
}

private mixed _show_confirm_menu(object me, mixed token_item, mixed target)
{
    mixed ret;
    string key;
    string base;
    string name;
    int rate;
    mixed content;
    mixed menu_ob;

    if (!me) return 0;
    if (!token_item) {
        _msg(me, "百练晶已经失效，请重新使用道具。");
        return 1;
    }
    ret = _check_target(target);
    if (!ret || ret[0] != 1) {
        if (ret && sizeof(ret) > 1) _msg(me, ret[1]);
        else _msg(me, "目标不能使用百练晶。");
        return 1;
    }

    key = ret[1];
    base = ret[2];
    rate = _success_rate(target, key, base);
    name = _safe_item_name(target);
    if (!name || name == "") name = "目标首饰";

    catch(me->set_temp(CONFIRM_ITEM, token_item));
    catch(me->set_temp(CONFIRM_TARGET, target));
    catch(me->set_temp(CONFIRM_KEY, key));
    catch(me->set_temp(CONFIRM_BASE, base));

    menu_ob = _menu_object(me);
    _prepare_menu_object(me);
    content = sprintf("[@确定/bailianjing_confirm#DLG:1#prompt:#r使用#R百练晶#n后 #R%s#n将立即进行#R1次#n随机属性强化，本次强化的成功率为#R%d#n，是否继续？]",
        name, rate);
    _bind_confirm_actions(me);
    me->set_menu(menu_ob, "百练晶强化确认", content, _menu_action("menu_default_action"));
    _activate_menu_session(me, menu_ob);
    _bind_default_menu(me);
    _bind_default_menu_on(menu_ob);
    _bind_confirm_actions(me);
    _bind_confirm_actions_on(menu_ob);
    return 1;
}

private mixed _use_on_target(object me, mixed token_item, mixed target, int reopen_menu, int restore_target)
{
    mixed ret;
    string key;
    string base;
    int count;
    int max_count;

    if (!me) return 0;
    if (!token_item) {
        _msg(me, "百练晶已经失效，请重新使用道具。");
        return 1;
    }

    ret = _check_target(target);
    if (!ret || ret[0] != 1) {
        if (restore_target) _return_to_bag(me, target);
        if (ret && sizeof(ret) > 1) _msg(me, ret[1]);
        else _msg(me, "目标不能使用百练晶。");
        if (reopen_menu) _show_submit_menu(me);
        return 1;
    }

    key = ret[1];
    base = ret[2];
    count = ret[3];
    max_count = ret[4];
    return _use_checked_target(me, token_item, target, key, base, count, max_count, reopen_menu, restore_target);
}

private mixed _submit_restore_items(object me, mixed npc, mixed items, string tag, int reopen_menu)
{
    mixed token_item;
    mixed target;

    if (!me) return 1;
    token_item = 0;
    if (catch(token_item = me->query_temp(TOKEN_ITEM))) token_item = 0;
    if (!token_item) token_item = _find_owner_token(me);
    target = _pick_submit_target(me, items, npc, 0, 0, 0);
    if (!target) {
        _msg(me, "请提交要强化的首饰。");
        if (reopen_menu) _show_submit_menu(me);
        return 1;
    }
    return _use_on_target(me, token_item, target, reopen_menu, 1);
}

mixed open_merge_dialog(object me, object item)
{
    mixed iid;

    if (!me) return 0;
    if (item) {
        iid = 0;
        if (catch(iid = item->get_iid())) iid = 0;
        if (catch(me->set_temp(TOKEN_ITEM, item))) {
        }
        if (iid && catch(me->set_temp(TOKEN_IID, iid))) {
        }
        if (catch(me->set_temp(TOKEN_MARK, query_marker()))) {
        }
    }
    _show_submit_menu(me);
    return 0;
}

mixed prepare_select_target(object me, object item)
{
    mixed iid;

    if (!me) return 0;
    if (item) {
        iid = 0;
        if (catch(iid = item->get_iid())) iid = 0;
        if (catch(me->set_temp(TOKEN_ITEM, item))) {
        }
        if (iid && catch(me->set_temp(TOKEN_IID, iid))) {
        }
        if (catch(me->set_temp(TOKEN_MARK, query_marker()))) {
        }
    }
    return 1;
}

mixed use_enhance_item(object me, string unused, object item)
{
    if (me) _msg(me, "请将百练晶拖到要强化的首饰上。");
    return 0;
}

mixed apply_to(object me, object item)
{
    if (me) _msg(me, "请将百练晶拖到要强化的首饰上。");
    return 0;
}

mixed use_on(object me, object item, object target)
{
    return _show_confirm_menu(me, item, target);
}

mixed use_on_target(object me, object item, mixed target)
{
    return _show_confirm_menu(me, item, target);
}

mixed menu_default_action(object me, mixed npc, mixed arg, mixed para)
{
    mixed token_item;
    mixed chk;
    mixed key;

    if (!me) return 1;
    key = para;
    if (!key || key == "") key = arg;
    if (key == "bailianjing_confirm" || key == "确定" || key == "确认" || key == "$确定" || key == "$确认") {
        return call_other(JEWELRY_POLAR_D, "menu_confirm_enhance", me, npc, arg, para);
    }
    if (key == "bailianjing_cancel" || key == "取消" || key == "$取消" || key == "离开") {
        return call_other(JEWELRY_POLAR_D, "menu_cancel_enhance", me, npc, arg, para);
    }

    token_item = _find_owner_token(me);
    if (!arg || !_looks_like_item(arg)) {
        _msg(me, "请将百练晶拖到要强化的首饰上。");
        return 1;
    }
    chk = _check_target(arg);
    if (!chk || chk[0] != 1) {
        if (chk && sizeof(chk) > 1) _msg(me, chk[1]);
        return 1;
    }
    return _show_confirm_menu(me, token_item, arg);
}

mixed apply_task_item(object me, mixed arg)
{
    mixed token_item;
    mixed target;
    mixed chk;

    if (!me) return 0;

    if (arg && !catch(arg->is_bailianjing_fu()) && arg->is_bailianjing_fu()) {
        _msg(me, "请将百练晶拖到要强化的首饰上。");
        return 0;
    }

    token_item = 0;
    target = 0;

    chk = _check_target(arg);
    if (chk && chk[0]) {
        target = arg;
        if (!catch(token_item = me->query_temp(TOKEN_ITEM)) && token_item) {
        } else {
            token_item = _find_owner_token(me);
        }
    } else {
        token_item = arg;
    }

    if (!target) {
        return 0;
    }
    return _show_confirm_menu(me, token_item, target);
}

mixed menu_confirm_enhance(object me, mixed npc, mixed arg, mixed para)
{
    mixed token_item;
    mixed target;
    string key;
    string base;
    mixed ret;
    int count;
    int max_count;

    if (!me) return 1;
    token_item = 0;
    target = 0;
    key = 0;
    base = 0;
    if (catch(token_item = me->query_temp(CONFIRM_ITEM))) token_item = 0;
    if (catch(target = me->query_temp(CONFIRM_TARGET))) target = 0;
    if (catch(key = me->query_temp(CONFIRM_KEY))) key = 0;
    if (catch(base = me->query_temp(CONFIRM_BASE))) base = 0;
    _clear_confirm(me);
    if (!token_item) token_item = _find_owner_token(me);
    if (!target || !key || !base) {
        _msg(me, "本次强化确认已经失效，请重新使用百练晶。");
        return 1;
    }
    ret = _check_target(target);
    if (!ret || ret[0] != 1) {
        if (ret && sizeof(ret) > 1) _msg(me, ret[1]);
        else _msg(me, "目标不能使用百练晶。");
        return 1;
    }
    count = ret[3];
    max_count = ret[4];
    return _use_checked_target(me, token_item, target, key, base, count, max_count, 0, 0);
}

mixed menu_cancel_enhance(object me, mixed npc, mixed arg, mixed para)
{
    _clear_confirm(me);
    return 1;
}

mixed submit_item(object me, mixed npc, mixed items, mixed para)
{
    return _submit_restore_items(me, npc, items, "submit_item", 1);
}

mixed menu_submit_item(object me, mixed npc, mixed items, mixed para)
{
    return _submit_restore_items(me, npc, items, "menu_submit_item", 1);
}

mixed submit_items(object me, mixed npc, mixed items, mixed para)
{
    return _submit_restore_items(me, npc, items, "submit_items", 1);
}

mixed submit_items_succ(object me, mixed npc, mixed items, mixed para, mixed arg4, mixed arg5)
{
    return _submit_restore_items(me, npc, items, "submit_items_succ", 1);
}

mixed do_submit(object me, mixed npc, mixed items, mixed para)
{
    return _submit_restore_items(me, npc, items, "do_submit", 1);
}

mixed do_submit_item(object me, mixed npc, mixed items, mixed para)
{
    return _submit_restore_items(me, npc, items, "do_submit_item", 1);
}

void create()
{
}
