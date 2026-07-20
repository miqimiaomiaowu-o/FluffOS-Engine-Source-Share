#define ITEM_NAME "宠物同源丹"
#define ITEM_COLOR "金色"
#define ITEM_ATTRIB_MASK 260
#define ITEM_CONSUME_AMOUNT 1

#define PETD "/gs/daemons/petd.c"
#define FORMULAD "/gs/daemons/formulad.c"
#define TASK_EASYUTILD "/gs/daemons/task_easy_utild.c"
#define CHANNELD "/gs/daemons/channeld.c"
#define AUTO_HEARTBEAT_KEY_PREFIX "pet_growth_random_yu_auto_"
#define PROP_COLOR "color"
#define PROP_ATTRIB "attrib"
#define PROP_GOLD_COIN "gold_coin"
#define PET_BASE_PROP_PREFIX "qingtian_pet_growth_random_yu_base_"
#define PET_BASE_MARK_PREFIX "qingtian_pet_growth_random_yu_base_mark_"
#define SESSION_PET "pet_growth_random_yu/pet"
#define SESSION_MASK "pet_growth_random_yu/mask"
#define SESSION_SELECT_PET_PREFIX "pet_growth_random_yu/select_pet_"
#define SESSION_PREVIEW_TYPE_PREFIX "pet_growth_random_yu/preview_type_"
#define SESSION_PREVIEW_DELTA_PREFIX "pet_growth_random_yu/preview_delta_"
#define SESSION_PREVIEW_LOCK_PREFIX "pet_growth_random_yu/preview_lock_"
#define SESSION_PREVIEW_COUNT "pet_growth_random_yu/preview_count"
#define SESSION_NOTICE "pet_growth_random_yu/notice"
#define SESSION_AUTO_RUNNING "pet_growth_random_yu/auto_running"
#define SESSION_AUTO_TIMES "pet_growth_random_yu/auto_times"
#define SESSION_AUTO_SPENT "pet_growth_random_yu/auto_spent"
#define MAX_SELECT_PETS 30
#define PREVIEW_COUNT 5
#define LOCK_COST_YUANBAO 100
#define MAX_LOCK_COUNT 3
#define REROLL_COST_YUANBAO 100
#define AUTO_RANDOM_MAX_TIMES 300
#define AUTO_RANDOM_DELAY 1

#define PREVIEW_TYPE_LIFE 1
#define PREVIEW_TYPE_MANA 2
#define PREVIEW_TYPE_SPEED 3
#define PREVIEW_TYPE_PHY 4
#define PREVIEW_TYPE_MAG 5

#define TASKD "/gs/daemons/taskd.c"
#define TRADE_CHECK_LEVEL 9
#define TASK_STATE_KEY "坐牢"
#define STATUS_FORBIDDEN_MESSAGE "当前状态无法进行此操作。"

#define ACTION_LOCK_LIFE 1
#define ACTION_LOCK_MANA 2
#define ACTION_LOCK_SPEED 3
#define ACTION_LOCK_PHY 4
#define ACTION_LOCK_MAG 5
#define ACTION_APPLY 100
#define ACTION_CANCEL 999

#define LOCK_LIFE 1
#define LOCK_MANA 2
#define LOCK_SPEED 4
#define LOCK_PHY 8
#define LOCK_MAG 16
#define LOCK_ALL 31

#define FORMULA_CALCULATE_TYPE 55

inherit "/gs/inherit/item/charge_item.c";
inherit "/gs/feature/item/charge_item_consume_log.c";

object auto_reroll_user;
string auto_reroll_key;

void apply_pet_choose_lock(object user, object pet, object item, int mask, mixed action);
void _auto_reroll_tick(object user);
mixed _auto_heart_beat(mixed now);
void _show_preview_menu(object user, object pet);

mixed menu_default_action(object me, mixed npc, mixed para, mixed text);
mixed menu_action_lock_life(object me, mixed npc);
mixed menu_action_lock_mana(object me, mixed npc);
mixed menu_action_lock_speed(object me, mixed npc);
mixed menu_action_lock_phy(object me, mixed npc);
mixed menu_action_lock_mag(object me, mixed npc);
mixed menu_action_lock_line_1(object me, mixed npc);
mixed menu_action_lock_line_2(object me, mixed npc);
mixed menu_action_lock_line_3(object me, mixed npc);
mixed menu_action_lock_line_4(object me, mixed npc);
mixed menu_action_lock_line_5(object me, mixed npc);
mixed menu_action_confirm_lock_1(object me, mixed npc);
mixed menu_action_confirm_lock_2(object me, mixed npc);
mixed menu_action_confirm_lock_3(object me, mixed npc);
mixed menu_action_confirm_lock_4(object me, mixed npc);
mixed menu_action_confirm_lock_5(object me, mixed npc);
mixed menu_action_reroll(object me, mixed npc);
mixed menu_action_confirm_reroll(object me, mixed npc);
mixed menu_action_auto_reroll(object me, mixed npc);
mixed menu_action_start_once(object me, mixed npc);
mixed menu_action_confirm(object me, mixed npc);
mixed menu_action_apply(object me, mixed npc);
mixed menu_action_back_preview(object me, mixed npc);
mixed menu_action_cancel(object me, mixed npc);
mixed menu_action_pick_pet_1(object me, mixed npc);
mixed menu_action_pick_pet_2(object me, mixed npc);
mixed menu_action_pick_pet_3(object me, mixed npc);
mixed menu_action_pick_pet_4(object me, mixed npc);
mixed menu_action_pick_pet_5(object me, mixed npc);
mixed menu_action_pick_pet_6(object me, mixed npc);
mixed menu_action_pick_pet_7(object me, mixed npc);
mixed menu_action_pick_pet_8(object me, mixed npc);
mixed menu_action_pick_pet_9(object me, mixed npc);
mixed menu_action_pick_pet_10(object me, mixed npc);
mixed menu_action_pick_pet_11(object me, mixed npc);
mixed menu_action_pick_pet_12(object me, mixed npc);
mixed menu_action_pick_pet_13(object me, mixed npc);
mixed menu_action_pick_pet_14(object me, mixed npc);
mixed menu_action_pick_pet_15(object me, mixed npc);
mixed menu_action_pick_pet_16(object me, mixed npc);
mixed menu_action_pick_pet_17(object me, mixed npc);
mixed menu_action_pick_pet_18(object me, mixed npc);
mixed menu_action_pick_pet_19(object me, mixed npc);
mixed menu_action_pick_pet_20(object me, mixed npc);
mixed menu_action_pick_pet_21(object me, mixed npc);
mixed menu_action_pick_pet_22(object me, mixed npc);
mixed menu_action_pick_pet_23(object me, mixed npc);
mixed menu_action_pick_pet_24(object me, mixed npc);
mixed menu_action_pick_pet_25(object me, mixed npc);
mixed menu_action_pick_pet_26(object me, mixed npc);
mixed menu_action_pick_pet_27(object me, mixed npc);
mixed menu_action_pick_pet_28(object me, mixed npc);
mixed menu_action_pick_pet_29(object me, mixed npc);
mixed menu_action_pick_pet_30(object me, mixed npc);

private string _lock_text(int mask, int bit)
{
    return (mask & bit) ? "已锁" : "未锁";
}

private int _toggle_lock(int mask, int bit)
{
    if (mask & bit) return mask - bit;
    return mask + bit;
}

private int _growth_delta()
{
    int roll;

    roll = random(100);
    if (roll < 60) return random(11) - 5;
    if (roll < 90) return random(6);
    if (roll < 95) return random(6) + 5;
    if (roll < 98) return random(6) + 10;
    if (roll < 99) return random(6) + 15;
    return 20;
}

private int _pet_text_has(object pet, string text)
{
    mixed value;

    if (!pet || !text || text == "") return 0;
    value = 0;
    if (!catch(value = pet->get_name()) && strsrch(sprintf("%O", value), text) >= 0) return 1;
    value = 0;
    if (!catch(value = pet->query("type")) && strsrch(sprintf("%O", value), text) >= 0) return 1;
    value = 0;
    if (!catch(value = pet->query("pet_type")) && strsrch(sprintf("%O", value), text) >= 0) return 1;
    value = 0;
    if (!catch(value = pet->query("rank")) && strsrch(sprintf("%O", value), text) >= 0) return 1;
    value = 0;
    if (!catch(value = pet->query("pet_rank")) && strsrch(sprintf("%O", value), text) >= 0) return 1;
    value = 0;
    if (!catch(value = pet->query("kind")) && strsrch(sprintf("%O", value), text) >= 0) return 1;
    return 0;
}

private int _method_true(object pet, string fn)
{
    mixed value;

    if (!pet || !fn || fn == "") return 0;
    value = 0;
    if (catch(value = call_other(pet, fn))) return 0;
    return value ? 1 : 0;
}

private int _pet_growth_delta_max(object pet)
{
    if (!pet) return 10;
    if (_method_true(pet, "is_shenshou") ||
        _method_true(pet, "is_yuanling") ||
        _method_true(pet, "is_xianyuan") ||
        _pet_text_has(pet, "神兽") ||
        _pet_text_has(pet, "元灵") ||
        _pet_text_has(pet, "仙元")) {
        return 20;
    }
    if (_method_true(pet, "is_bianyi") ||
        _method_true(pet, "is_mutant") ||
        _pet_text_has(pet, "变异")) {
        return 15;
    }
    return 10;
}

private int _growth_delta_by_pet(object pet)
{
    return _growth_delta();
}

private int _growth_type()
{
    return random(5) + 1;
}

private int _random_preview_count()
{
    int roll;

    roll = random(100);
    if (roll < 80) return random(2) + 1;
    if (roll < 95) return random(2) + 3;
    return 5;
}

private string _growth_key_by_type(int type)
{
    if (type == PREVIEW_TYPE_LIFE) return "life";
    if (type == PREVIEW_TYPE_MANA) return "mana";
    if (type == PREVIEW_TYPE_SPEED) return "speed";
    if (type == PREVIEW_TYPE_PHY) return "phy";
    if (type == PREVIEW_TYPE_MAG) return "mag";
    return "";
}

private string _growth_name_by_type(int type)
{
    if (type == PREVIEW_TYPE_LIFE) return "血量成长";
    if (type == PREVIEW_TYPE_MANA) return "法力成长";
    if (type == PREVIEW_TYPE_SPEED) return "速度成长";
    if (type == PREVIEW_TYPE_PHY) return "物攻成长";
    if (type == PREVIEW_TYPE_MAG) return "法攻成长";
    return "未知成长";
}

private string _preview_lock_text(int locked)
{
    return locked ? "#R锁定#n" : "#G未锁定#n";
}

private int _pay_yuanbao(object user, int amount, string action)
{
    mixed coin;

    if (!user) return 0;
    coin = 0;
    if (catch(coin = user->safe_query(PROP_GOLD_COIN))) {
        coin = 0;
        if (catch(coin = user->query(PROP_GOLD_COIN))) coin = 0;
    }
    if (coin < amount) {
        user->send_dialog_ok(sprintf("元宝不足，本次%s需要#R%d#n元宝。", action, amount));
        return 0;
    }
    if (catch(user->set(PROP_GOLD_COIN, coin - amount))) {
        user->send_dialog_ok("扣除元宝失败，请稍后再试。");
        return 0;
    }
    if (catch(user->send_update_object(user->get_id(), ({ "silver_coin", "gold_coin", "limit_trade_coin" })))) {
    }
    return 1;
}

private mixed _menu_action(string fn)
{
    return (: call_other, this_object(), fn :);
}

private void _bind_menu(object user, string key, string fn)
{
    mixed action;

    if (!user || !key || key == "") return;
    action = _menu_action(fn);
    if (catch(user->set_temp("current_menu/" + key, action))) {
    }
    if (catch(user->set_temp("menu/" + key, action))) {
    }
        if (catch(user->set_temp("menu/" + ITEM_NAME + "/" + key, action))) {
    }
}

private void _bind_default_menu(object user)
{
    mixed action;

    if (!user) return;
    action = _menu_action("menu_default_action");
    if (catch(user->set_temp("default_action", action))) {
    }
    if (catch(user->set_temp("extra_para_action", action))) {
    }
}

private void _activate_menu_session(object user)
{
    if (!user) return;
    if (catch(user->set_temp("ingore_distance_npc", 1))) {
    }
    if (catch(user->set_temp("current_npc", user))) {
    }
    if (catch(user->set_temp("click_npc", user))) {
    }
    if (catch(user->start_talk_to(user))) {
    }
}

private void _clear_lock_session(object user)
{
    int i;

    if (!user) return;
    if (catch(user->delete_temp(SESSION_PET))) {
    }
    if (catch(user->delete_temp(SESSION_MASK))) {
    }
    if (catch(user->delete_temp(SESSION_NOTICE))) {
    }
    if (catch(user->delete_temp(SESSION_PREVIEW_COUNT))) {
    }
    if (catch(user->delete_temp(SESSION_AUTO_RUNNING))) {
    }
    if (catch(user->delete_temp(SESSION_AUTO_TIMES))) {
    }
    if (catch(user->delete_temp(SESSION_AUTO_SPENT))) {
    }
    for (i = 1; i <= PREVIEW_COUNT; i++) {
        if (catch(user->delete_temp(SESSION_PREVIEW_TYPE_PREFIX + i))) {
        }
        if (catch(user->delete_temp(SESSION_PREVIEW_DELTA_PREFIX + i))) {
        }
        if (catch(user->delete_temp(SESSION_PREVIEW_LOCK_PREFIX + i))) {
        }
    }
    for (i = 1; i <= MAX_SELECT_PETS; i++) {
        if (catch(user->delete_temp(SESSION_SELECT_PET_PREFIX + i))) {
        }
    }
}

private mixed _session_pet(object user)
{
    mixed pet;

    if (!user) return 0;
    pet = 0;
    if (catch(pet = user->query_temp(SESSION_PET))) pet = 0;
    if (!pet || !user->has_pet(pet)) return 0;
    return pet;
}

private int _item_in_user_bag(object user)
{
    if (!user) return 0;
    return env_object() == user;
}

private int _ensure_item_in_user_bag(object user)
{
    if (_item_in_user_bag(user)) return 1;
    if (user) {
        if (catch(user->destroy_menu())) {
        }
        user->send_dialog_ok(ITEM_NAME + "已经不在背包中，本次操作无法继续。");
    }
    return 0;
}

private int _session_mask(object user)
{
    mixed mask;

    if (!user) return 0;
    mask = 0;
    if (catch(mask = user->query_temp(SESSION_MASK))) mask = 0;
    return mask;
}

private void _set_lock_session(object user, object pet, int mask)
{
    if (!user || !pet) return;
    if (catch(user->set_temp(SESSION_PET, pet))) {
    }
    if (catch(user->set_temp(SESSION_MASK, mask))) {
    }
}

private int _preview_type(object user, int slot)
{
    mixed value;

    if (!user || slot <= 0 || slot > PREVIEW_COUNT) return 0;
    value = 0;
    if (catch(value = user->query_temp(SESSION_PREVIEW_TYPE_PREFIX + slot))) value = 0;
    if (!value) return 0;
    return value;
}

private int _preview_delta(object user, int slot)
{
    mixed value;

    if (!user || slot <= 0 || slot > PREVIEW_COUNT) return 0;
    value = 0;
    if (catch(value = user->query_temp(SESSION_PREVIEW_DELTA_PREFIX + slot))) value = 0;
    if (!value) return 0;
    return value;
}

private int _preview_locked(object user, int slot)
{
    mixed value;

    if (!user || slot <= 0 || slot > PREVIEW_COUNT) return 0;
    value = 0;
    if (catch(value = user->query_temp(SESSION_PREVIEW_LOCK_PREFIX + slot))) value = 0;
    if (!value) return 0;
    return value;
}

private int _preview_type_count(object user, int type)
{
    int i;
    int count;

    count = 0;
    if (!user || type <= 0) return 0;
    for (i = 1; i <= PREVIEW_COUNT; i++) {
        if (_preview_type(user, i) == type) count++;
    }
    return count;
}

private int _preview_type_count_except(object user, int type, int except_slot)
{
    int i;
    int count;

    count = 0;
    if (!user || type <= 0) return 0;
    for (i = 1; i <= PREVIEW_COUNT; i++) {
        if (i == except_slot) continue;
        if (_preview_type(user, i) == type) count++;
    }
    return count;
}

private int _has_three_same_preview(object user)
{
    if (!user) return 0;
    if (_preview_type_count(user, PREVIEW_TYPE_LIFE) >= 3) return 1;
    if (_preview_type_count(user, PREVIEW_TYPE_MANA) >= 3) return 1;
    if (_preview_type_count(user, PREVIEW_TYPE_SPEED) >= 3) return 1;
    if (_preview_type_count(user, PREVIEW_TYPE_PHY) >= 3) return 1;
    if (_preview_type_count(user, PREVIEW_TYPE_MAG) >= 3) return 1;
    return 0;
}

private int _max_same_preview_count(object user)
{
    int max_count;
    int count;

    max_count = 0;
    if (!user) return 0;
    count = _preview_type_count(user, PREVIEW_TYPE_LIFE);
    if (count > max_count) max_count = count;
    count = _preview_type_count(user, PREVIEW_TYPE_MANA);
    if (count > max_count) max_count = count;
    count = _preview_type_count(user, PREVIEW_TYPE_SPEED);
    if (count > max_count) max_count = count;
    count = _preview_type_count(user, PREVIEW_TYPE_PHY);
    if (count > max_count) max_count = count;
    count = _preview_type_count(user, PREVIEW_TYPE_MAG);
    if (count > max_count) max_count = count;
    return max_count;
}

private int _max_perfect_percent_preview(object user)
{
    int i;
    int percent;
    int max_percent;

    max_percent = 0;
    if (!user) return 0;
    for (i = 1; i <= PREVIEW_COUNT; i++) {
        percent = _preview_delta(user, i) * 100 / 30;
        if (percent > max_percent) max_percent = percent;
    }
    return max_percent;
}

private string _safe_user_name(object user)
{
    mixed name;

    name = 0;
    if (user && !catch(name = user->get_name()) && name && name != "") return name;
    return "某位道友";
}

private void _send_pet_growth_announce(string msg)
{
    if (!msg || msg == "") return;
    if (catch(CHANNELD->do_channel(7, "", msg))) {
    }
}

private void _announce_preview_awards(object user)
{
    int same_count;
    int perfect_percent;
    string name;

    if (!user) return;
    name = _safe_user_name(user);
    same_count = _max_same_preview_count(user);
    if (same_count >= 3) {
        _send_pet_growth_announce(sprintf("#Y%s#n玩家，勘破天机在进行宠物同源时获得了#R%d#n条相同属性！真是鸿运加身！", name, same_count));
    }
    perfect_percent = _max_perfect_percent_preview(user);
    if (perfect_percent >= 100) {
        _send_pet_growth_announce(sprintf("#Y%s#n玩家，道韵天成在进行宠物同源时获得了完美度#R%d%%#n的属性值！快去恭贺他吧！", name, perfect_percent));
    }
}

private int _has_any_preview(object user)
{
    int i;

    if (!user) return 0;
    for (i = 1; i <= PREVIEW_COUNT; i++) {
        if (_preview_type(user, i) > 0) return 1;
    }
    return 0;
}

private int _finish_invalid_preview_session(object user)
{
    if (!user) return 1;
    if (!_has_any_preview(user)) {
        _clear_lock_session(user);
        user->send_dialog_ok("宠物已经失效，请重新使用" + ITEM_NAME + "。");
        return 1;
    }
    _clear_lock_session(user);
    if (_item_in_user_bag(user)) {
        consume_amount(ITEM_CONSUME_AMOUNT);
        charge_item_consume_log(user);
        if (catch(user->destroy_menu())) {
        }
        user->send_dialog_ok("宠物已经失效，本次同源属性已经作废，宠物同源丹已经消失。");
        return 1;
    }
    if (catch(user->destroy_menu())) {
    }
    user->send_dialog_ok(ITEM_NAME + "已经不在背包中，本次操作无法继续。");
    return 1;
}

private int _preview_count(object user)
{
    mixed value;

    if (!user) return PREVIEW_COUNT;
    value = 0;
    if (catch(value = user->query_temp(SESSION_PREVIEW_COUNT))) value = 0;
    if (!value || value <= 0) return PREVIEW_COUNT;
    if (value > PREVIEW_COUNT) return PREVIEW_COUNT;
    return value;
}

private void _set_preview_count(object user, int count)
{
    if (!user) return;
    if (count < 1) count = 1;
    if (count > PREVIEW_COUNT) count = PREVIEW_COUNT;
    if (catch(user->set_temp(SESSION_PREVIEW_COUNT, count))) {
    }
}

private int _random_preview_type_for_slot(object user, int slot)
{
    int i;
    int type;

    if (!user || random(100) < 10) return _growth_type();
    for (i = 0; i < 10; i++) {
        type = _growth_type();
        if (_preview_type_count_except(user, type, slot) <= 0) return type;
    }
    return _growth_type();
}

private string _colored_growth_name(object user, int type)
{
    string name;

    name = _growth_name_by_type(type);
    if (_preview_type_count(user, type) >= 2) return "#Y" + name + "#n";
    return name;
}

private string _colored_delta(int delta)
{
    if (delta > 0) return sprintf("+%d", delta);
    if (delta < 0) return sprintf("#R%d#n", delta);
    return "0";
}

private int _perfect_percent(int delta)
{
    if (delta <= 0) return 0;
    return delta * 100 / 30;
}

private string _colored_perfect_text(int delta)
{
    int percent;

    if (delta < 0) return "下降";
    if (delta == 0) return "无变化";
    percent = _perfect_percent(delta);
    if (percent >= 80) return sprintf("完美度#R%d%%#n", percent);
    return sprintf("完美度%d%%", percent);
}

private string _preview_line_label(object user, int slot)
{
    int type;
    int delta;

    type = _preview_type(user, slot);
    delta = _preview_delta(user, slot);
    if (type <= 0) {
        return sprintf("%d. #Y等待随机#n（未生成）", slot);
    }
    return sprintf(
        "%d. %s %s（%s）",
        slot,
        _colored_growth_name(user, type),
        _colored_delta(delta),
        _colored_perfect_text(delta)
    );
}

private void _set_preview_slot(object user, int slot, int type, int delta, int locked)
{
    if (!user || slot <= 0 || slot > PREVIEW_COUNT) return;
    if (catch(user->set_temp(SESSION_PREVIEW_TYPE_PREFIX + slot, type))) {
    }
    if (catch(user->set_temp(SESSION_PREVIEW_DELTA_PREFIX + slot, delta))) {
    }
    if (catch(user->set_temp(SESSION_PREVIEW_LOCK_PREFIX + slot, locked))) {
    }
}

private void _random_preview_slot(object user, int slot)
{
    _set_preview_slot(user, slot, _random_preview_type_for_slot(user, slot), _growth_delta_by_pet(_session_pet(user)), 0);
}

private void _set_preview_pet(object user, object pet)
{
    if (!user || !pet) return;
    if (catch(user->set_temp(SESSION_PET, pet))) {
    }
}

private int _has_unlocked_preview(object user)
{
    int i;

    if (!user) return 0;
    for (i = 1; i <= PREVIEW_COUNT; i++) {
        if (!_preview_locked(user, i)) return 1;
    }
    return 0;
}

private int _locked_preview_count(object user)
{
    int i;
    int count;

    count = 0;
    if (!user) return 0;
    for (i = 1; i <= PREVIEW_COUNT; i++) {
        if (_preview_locked(user, i)) count++;
    }
    return count;
}

private int _lock_cost_yuanbao(object user)
{
    int count;
    int cost;

    count = _locked_preview_count(user);
    cost = LOCK_COST_YUANBAO;
    while (count > 0) {
        cost = cost * 2;
        count--;
    }
    return cost;
}

private void _set_select_pet_slot(object user, int slot, object pet)
{
    if (!user || slot <= 0 || slot > MAX_SELECT_PETS) return;
    if (catch(user->set_temp(SESSION_SELECT_PET_PREFIX + slot, pet))) {
    }
}

private mixed _query_select_pet_slot(object user, int slot)
{
    mixed pet;

    if (!user || slot <= 0 || slot > MAX_SELECT_PETS) return 0;
    pet = 0;
    if (catch(pet = user->query_temp(SESSION_SELECT_PET_PREFIX + slot))) pet = 0;
    if (!pet || !user->has_pet(pet)) return 0;
    return pet;
}

private string _safe_pet_name(object pet, int slot)
{
    mixed name;

    name = 0;
    if (pet && !catch(name = pet->get_name()) && name && name != "") return name;
    return sprintf("宠物%d", slot);
}

private int _safe_pet_level(object pet)
{
    mixed level;

    level = 0;
    if (pet && !catch(level = pet->query("level")) && level) return level;
    return 0;
}

private string _pet_select_label(object pet, int slot)
{
    string name;
    int level;

    name = _safe_pet_name(pet, slot);
    level = _safe_pet_level(pet);
    if (level > 0) return sprintf("%d. %s        %d级 %s", slot, name, level, name);
    return sprintf("%d. %s", slot, name);
}

private void _set_notice(object user, string msg)
{
    if (!user) return;
    if (!msg || msg == "") {
        if (catch(user->delete_temp(SESSION_NOTICE))) {
        }
        return;
    }
    if (catch(user->set_temp(SESSION_NOTICE, msg))) {
    }
}

private string _take_notice(object user)
{
    mixed msg;

    if (!user) return "";
    msg = "";
    if (catch(msg = user->query_temp(SESSION_NOTICE))) msg = "";
    if (catch(user->delete_temp(SESSION_NOTICE))) {
    }
    if (!msg) return "";
    return msg;
}

private int _query_auto_value(object user, string key)
{
    mixed value;

    if (!user || !key || key == "") return 0;
    value = 0;
    if (catch(value = user->query_temp(key))) value = 0;
    if (!value) return 0;
    return value;
}

private void _set_auto_value(object user, string key, int value)
{
    if (!user || !key || key == "") return;
    if (catch(user->set_temp(key, value))) {
    }
}

private void _stop_auto_random(object user)
{
    if (auto_reroll_key && auto_reroll_key != "") {
        if (catch(TASK_EASYUTILD->remove_heart_beat_entry(auto_reroll_key))) {
        }
    }
    auto_reroll_user = 0;
    auto_reroll_key = 0;
    if (!user) return;
    if (catch(user->delete_temp(SESSION_AUTO_RUNNING))) {
    }
}

private string _auto_heartbeat_key(object user)
{
    return sprintf("%s%O", AUTO_HEARTBEAT_KEY_PREFIX, user);
}

private int _register_auto_heartbeat(object user)
{
    int next_time;

    if (!user) return 0;
    auto_reroll_user = user;
    auto_reroll_key = _auto_heartbeat_key(user);
    next_time = time() + AUTO_RANDOM_DELAY;
    if (catch(TASK_EASYUTILD->remove_heart_beat_entry(auto_reroll_key))) {
    }
    if (catch(TASK_EASYUTILD->register_heart_beat_entry(auto_reroll_key, next_time, (: _auto_heart_beat :)))) {
        auto_reroll_user = 0;
        auto_reroll_key = 0;
        return 0;
    }
    return 1;
}

private mixed _query_user_pets(object user)
{
    mixed pets;

    if (!user) return 0;
    pets = 0;
    if (!catch(pets = user->query_all_sorted_pet_objs()) && pets && sizeof(pets) > 0) return pets;
    pets = 0;
    if (!catch(pets = user->query_all_pet_objs()) && pets && sizeof(pets) > 0) return pets;
    return 0;
}

private void _bind_pet_slot(object user, int slot, string label)
{
    string fn;
    string key;

    if (!user || slot <= 0 || slot > MAX_SELECT_PETS) return;
    key = sprintf("pgr_pet_%d", slot);
    fn = sprintf("menu_action_pick_pet_%d", slot);
    _bind_menu(user, key, fn);
    if (label && label != "") _bind_menu(user, label, fn);
}

private void _show_pet_select_menu(object user)
{
    mixed pets;
    mixed pet;
    object old_pet;
    string content;
    string label;
    int i;
    int count;

    if (!user) return;
    if (!_ensure_item_in_user_bag(user)) return;

    old_pet = _session_pet(user);
    if (old_pet && user->has_pet(old_pet)) {
        _show_preview_menu(user, old_pet);
        return;
    }
    if (_has_any_preview(user)) {
        _finish_invalid_preview_session(user);
        return;
    }

    pets = _query_user_pets(user);
    if (!pets || sizeof(pets) <= 0) {
        user->send_dialog_ok("你没有宠物，无法使用" + ITEM_NAME + "。");
        return;
    }

    _clear_lock_session(user);
    content = "#R" + ITEM_NAME + "#n请选择要进行同源的宠物：";
    count = sizeof(pets);
    if (count > MAX_SELECT_PETS) count = MAX_SELECT_PETS;

    for (i = 0; i < count; i++) {
        pet = pets[i];
        if (!pet || !user->has_pet(pet)) continue;
        label = _pet_select_label(pet, i + 1);
        _set_select_pet_slot(user, i + 1, pet);
        content += sprintf("[%s/pgr_pet_%d]", label, i + 1);
        _bind_pet_slot(user, i + 1, label);
    }
    content += "[取消/pgr_cancel]";
    _bind_menu(user, "pgr_cancel", "menu_action_cancel");
    _bind_menu(user, "取消", "menu_action_cancel");

    user->set_menu(user, ITEM_NAME, content, _menu_action("menu_default_action"));
    _activate_menu_session(user);
    _bind_default_menu(user);

    for (i = 1; i <= count; i++) {
        pet = _query_select_pet_slot(user, i);
        if (!pet) continue;
        _bind_pet_slot(user, i, _pet_select_label(pet, i));
    }
    _bind_menu(user, "pgr_cancel", "menu_action_cancel");
    _bind_menu(user, "取消", "menu_action_cancel");
}

private int _query_base_growth(object pet, string key)
{
    string prop;
    string mark;
    mixed base_value;

    prop = PET_BASE_PROP_PREFIX + key;
    mark = PET_BASE_MARK_PREFIX + key;
    base_value = pet->query(prop);
    if (base_value || pet->query(mark)) {
        pet->set(mark, 1);
        return base_value;
    }
    if (!base_value) {
        base_value = pet->get_basic_effect(key);
        if (!base_value) base_value = 0;
        pet->set(prop, base_value);
        pet->set(mark, 1);
    }

    return base_value;
}

private string _effect_field_by_key(string key)
{
    if (key == "life") return "life_effect";
    if (key == "mana") return "mana_effect";
    if (key == "speed") return "speed_effect";
    if (key == "phy") return "phy_effect";
    if (key == "mag") return "mag_effect";
    return "";
}

private void _sync_growth_effect_field(object pet, string key)
{
    string field;
    int value;

    field = _effect_field_by_key(key);
    if (!pet || !field || field == "") return;
    value = pet->get_basic_effect(key);
    pet->set(field, value);
}

private void _sync_growth_effect_fields(object pet)
{
    if (!pet) return;
    _sync_growth_effect_field(pet, "life");
    _sync_growth_effect_field(pet, "mana");
    _sync_growth_effect_field(pet, "speed");
    _sync_growth_effect_field(pet, "phy");
    _sync_growth_effect_field(pet, "mag");
}

private void _add_growth(object pet, string key, int delta)
{
    int value;
    mixed init_effect;

    value = _query_base_growth(pet, key) + delta;
    pet->set_basic_effect(key, value);
    pet->set("basic_effect/" + key, value);
    pet->set(_effect_field_by_key(key), value);
    init_effect = pet->query("init_basic_effect");
    if (!is_mapping(init_effect)) init_effect = ([]);
    init_effect[key] = value;
    pet->set("init_basic_effect", init_effect);
    pet->set("init_basic_effect/" + key, value);
}

private void _show_preview_menu(object user, object pet)
{
    string content;
    string label;
    string notice;
    int i;
    int type;
    int delta;
    int locked;

    if (!user || !pet || !user->has_pet(pet)) return;

    _set_preview_pet(user, pet);
    content = sprintf("#R%s#n当前选择宠物：#Y%s#n\n点击开始同源后随机生成#R1-%d#n条属性；一颗丹只随机一次。确定保留属性时扣除#R%d#n元宝；取消则本丹消失。",
        ITEM_NAME,
        _safe_pet_name(pet, 1),
        PREVIEW_COUNT,
        REROLL_COST_YUANBAO);
    notice = _take_notice(user);
    if (notice && notice != "") content += "\n" + notice;

    if (!_has_any_preview(user)) {
        content += "[开始同源/pgr_start_once][取消/pgr_cancel]";
        _bind_menu(user, "pgr_start_once", "menu_action_start_once");
        _bind_menu(user, "开始同源", "menu_action_start_once");
    } else {
        for (i = 1; i <= _preview_count(user); i++) {
            label = _preview_line_label(user, i);
            content += "\n" + label;
        }
        content += "[确定/pgr_confirm][取消/pgr_cancel]";
    }
    _bind_menu(user, "pgr_confirm", "menu_action_confirm");
    _bind_menu(user, "pgr_cancel", "menu_action_cancel");
    _bind_menu(user, "确定", "menu_action_confirm");
    _bind_menu(user, "取消", "menu_action_cancel");

    user->set_menu(user, ITEM_NAME, content, _menu_action("menu_default_action"));
    _activate_menu_session(user);
    _bind_default_menu(user);

    if (!_has_any_preview(user)) {
        _bind_menu(user, "pgr_start_once", "menu_action_start_once");
    }
    _bind_menu(user, "pgr_confirm", "menu_action_confirm");
    _bind_menu(user, "pgr_cancel", "menu_action_cancel");
}

private void _show_lock_confirm_menu(object user, object pet, int slot)
{
    string content;
    int type;
    int delta;
    int cost;

    if (!user || !pet || !user->has_pet(pet)) return;
    if (_locked_preview_count(user) >= MAX_LOCK_COUNT) {
        user->send_dialog_ok(sprintf("最多只能锁定#R%d#n条同源预览属性。", MAX_LOCK_COUNT));
        _show_preview_menu(user, pet);
        return;
    }
    type = _preview_type(user, slot);
    delta = _preview_delta(user, slot);
    if (type <= 0 || delta <= 0) {
        user->send_dialog_ok("该预览属性尚未生成，请先进行同源转属。");
        _show_preview_menu(user, pet);
        return 1;
    }
    cost = _lock_cost_yuanbao(user);
    content = sprintf(
        "#R%s#n你将锁定第#R%d#n条属性：%s %s（%s），本次操作将扣除#R%d#n元宝，请确认操作。"
        "[确认/pgr_confirm_lock_%d][取消/pgr_back_preview]",
        ITEM_NAME,
        slot,
        _colored_growth_name(user, type),
        _colored_delta(delta),
        _colored_perfect_text(delta),
        cost,
        slot
    );
    _bind_menu(user, sprintf("pgr_confirm_lock_%d", slot), sprintf("menu_action_confirm_lock_%d", slot));
    _bind_menu(user, "pgr_back_preview", "menu_action_back_preview");
    _bind_menu(user, "确认", sprintf("menu_action_confirm_lock_%d", slot));
    _bind_menu(user, "取消", "menu_action_back_preview");
    user->set_menu(user, ITEM_NAME, content, _menu_action("menu_default_action"));
    _activate_menu_session(user);
    _bind_default_menu(user);
    _bind_menu(user, sprintf("pgr_confirm_lock_%d", slot), sprintf("menu_action_confirm_lock_%d", slot));
    _bind_menu(user, "pgr_back_preview", "menu_action_back_preview");
}

private void _show_reroll_confirm_menu(object user, object pet)
{
    string content;

    if (!user || !pet || !user->has_pet(pet)) return;
    content = sprintf(
        "#R%s#n你将进行同源转属，刷新未锁定的同源预览属性，本次操作将扣除#R%d#n元宝，请确认操作。"
        "[确认/pgr_confirm_reroll][取消/pgr_back_preview]",
        ITEM_NAME,
        REROLL_COST_YUANBAO
    );
    _bind_menu(user, "pgr_confirm_reroll", "menu_action_confirm_reroll");
    _bind_menu(user, "pgr_back_preview", "menu_action_back_preview");
    _bind_menu(user, "确认", "menu_action_confirm_reroll");
    _bind_menu(user, "取消", "menu_action_back_preview");
    user->set_menu(user, ITEM_NAME, content, _menu_action("menu_default_action"));
    _activate_menu_session(user);
    _bind_default_menu(user);
    _bind_menu(user, "pgr_confirm_reroll", "menu_action_confirm_reroll");
    _bind_menu(user, "pgr_back_preview", "menu_action_back_preview");
}

private void _start_preview_menu(object user, object pet)
{
    int i;

    if (!user || !pet || !user->has_pet(pet)) return;
    _clear_lock_session(user);
    _set_preview_pet(user, pet);
    _show_preview_menu(user, pet);
}

private mixed _toggle_preview_line(object user, int slot)
{
    object pet;
    int locked;

    pet = _session_pet(user);
    if (!pet) {
        return _finish_invalid_preview_session(user);
    }
    if (!_has_any_preview(user)) {
        user->send_dialog_ok("请先进行同源转属生成预览属性。");
        _show_preview_menu(user, pet);
        return 1;
    }

    locked = _preview_locked(user, slot);
    if (!locked) {
        _show_lock_confirm_menu(user, pet, slot);
        return 1;
    }
    _set_preview_slot(user, slot, _preview_type(user, slot), _preview_delta(user, slot), locked ? 0 : 1);
    _show_preview_menu(user, pet);
    return 1;
}

private mixed _confirm_lock_line(object user, int slot)
{
    object pet;
    int cost;

    pet = _session_pet(user);
    if (!pet) {
        return _finish_invalid_preview_session(user);
    }
    if (_preview_locked(user, slot)) {
        _show_preview_menu(user, pet);
        return 1;
    }
    if (_preview_type(user, slot) <= 0 || _preview_delta(user, slot) <= 0) {
        user->send_dialog_ok("该预览属性尚未生成，请先进行同源转属。");
        _show_preview_menu(user, pet);
        return 1;
    }
    if (_locked_preview_count(user) >= MAX_LOCK_COUNT) {
        user->send_dialog_ok(sprintf("最多只能锁定#R%d#n条同源预览属性。", MAX_LOCK_COUNT));
        _show_preview_menu(user, pet);
        return 1;
    }
    cost = _lock_cost_yuanbao(user);
    if (!_pay_yuanbao(user, cost, "锁定")) {
        _show_preview_menu(user, pet);
        return 1;
    }
    _set_preview_slot(user, slot, _preview_type(user, slot), _preview_delta(user, slot), 1);
    _show_preview_menu(user, pet);
    return 1;
}

private mixed _request_reroll_preview(object user)
{
    object pet;

    pet = _session_pet(user);
    if (!pet) {
        return _finish_invalid_preview_session(user);
    }
    if (!_has_unlocked_preview(user)) {
        user->send_dialog_ok("当前没有未锁定的预览属性，无法进行同源转属。");
        _show_preview_menu(user, pet);
        return 1;
    }
    _show_reroll_confirm_menu(user, pet);
    return 1;
}

private mixed _confirm_reroll_preview(object user)
{
    object pet;
    int i;

    pet = _session_pet(user);
    if (!pet) {
        return _finish_invalid_preview_session(user);
    }
    if (!_has_unlocked_preview(user)) {
        user->send_dialog_ok("当前没有未锁定的预览属性，无法进行同源转属。");
        _show_preview_menu(user, pet);
        return 1;
    }

    if (!_pay_yuanbao(user, REROLL_COST_YUANBAO, "同源转属")) {
        _show_preview_menu(user, pet);
        return 1;
    }
    for (i = 1; i <= PREVIEW_COUNT; i++) {
        if (!_preview_locked(user, i)) _random_preview_slot(user, i);
    }
    _announce_preview_awards(user);
    _show_preview_menu(user, pet);
    return 1;
}

private mixed _start_once_preview(object user)
{
    object pet;
    int i;
    int count;

    if (!_ensure_item_in_user_bag(user)) return 1;
    pet = _session_pet(user);
    if (!pet) {
        return _finish_invalid_preview_session(user);
    }
    if (_has_any_preview(user)) {
        user->send_dialog_ok("本次同源已经生成过属性，请确定或取消。");
        _show_preview_menu(user, pet);
        return 1;
    }
    count = _random_preview_count();
    _set_preview_count(user, count);
    for (i = 1; i <= count; i++) {
        _random_preview_slot(user, i);
    }
    _announce_preview_awards(user);
    _show_preview_menu(user, pet);
    return 1;
}

private mixed _auto_reroll_preview(object user)
{
    object pet;
    int i;
    int times;
    int spent;

    pet = _session_pet(user);
    if (!pet) {
        return _finish_invalid_preview_session(user);
    }
    if (!_has_unlocked_preview(user)) {
        user->send_dialog_ok("当前没有未锁定的预览属性，无法进行自动转属。");
        _show_preview_menu(user, pet);
        return 1;
    }
    _stop_auto_random(user);
    if (_has_three_same_preview(user)) {
        _set_notice(user, "#Y当前同源预览已经出现三条同属性，无需继续自动转属。#n");
        _show_preview_menu(user, pet);
        return 1;
    }

    times = 0;
    spent = 0;
    while (times < AUTO_RANDOM_MAX_TIMES) {
        if (!_has_unlocked_preview(user)) {
            _set_notice(user, "当前没有未锁定的预览属性，自动转属已停止。");
            _show_preview_menu(user, pet);
            return 1;
        }
        if (!_pay_yuanbao(user, REROLL_COST_YUANBAO, "自动转属")) {
            _set_notice(user, sprintf("#Y自动转属已停止，共转属#R%d#n次，消耗#R%d#n元宝。#n", times, spent));
            _show_preview_menu(user, pet);
            return 1;
        }
        for (i = 1; i <= PREVIEW_COUNT; i++) {
            if (!_preview_locked(user, i)) _random_preview_slot(user, i);
        }
        _announce_preview_awards(user);
        times++;
        spent += REROLL_COST_YUANBAO;
        if (_has_three_same_preview(user)) {
            _set_notice(user, sprintf("#Y自动转属完成，共转属#R%d#n次，消耗#R%d#n元宝，已出现三条同属性。#n", times, spent));
            _show_preview_menu(user, pet);
            return 1;
        }
    }

    _set_notice(user, sprintf("#Y自动转属达到#R%d#n次保护上限，共消耗#R%d#n元宝，暂未出现三条同属性。#n", AUTO_RANDOM_MAX_TIMES, spent));
    _show_preview_menu(user, pet);
    return 1;
}

void _auto_reroll_tick(object user)
{
    object pet;
    int i;
    int times;
    int spent;

    if (!user || !_query_auto_value(user, SESSION_AUTO_RUNNING)) return;
    pet = _session_pet(user);
    if (!pet) {
        _stop_auto_random(user);
        _clear_lock_session(user);
        return;
    }
    if (!_has_unlocked_preview(user)) {
        _stop_auto_random(user);
        _set_notice(user, "当前没有未锁定的预览属性，自动转属已停止。");
        _show_preview_menu(user, pet);
        return;
    }
    if (_has_three_same_preview(user)) {
        _stop_auto_random(user);
        _set_notice(user, "#Y自动转属已停止，当前已出现三条同属性。#n");
        _show_preview_menu(user, pet);
        return;
    }

    times = _query_auto_value(user, SESSION_AUTO_TIMES);
    spent = _query_auto_value(user, SESSION_AUTO_SPENT);
    if (times >= AUTO_RANDOM_MAX_TIMES) {
        _stop_auto_random(user);
        _set_notice(user, sprintf("#Y自动转属达到#R%d#n次保护上限，共消耗#R%d#n元宝，暂未出现三条同属性。#n", AUTO_RANDOM_MAX_TIMES, spent));
        _show_preview_menu(user, pet);
        return;
    }
    if (!_pay_yuanbao(user, REROLL_COST_YUANBAO, "自动转属")) {
        _stop_auto_random(user);
        _set_notice(user, sprintf("#Y自动转属已停止，共转属#R%d#n次，消耗#R%d#n元宝。#n", times, spent));
        _show_preview_menu(user, pet);
        return;
    }

    for (i = 1; i <= PREVIEW_COUNT; i++) {
        if (!_preview_locked(user, i)) _random_preview_slot(user, i);
    }
    _announce_preview_awards(user);
    times++;
    spent += REROLL_COST_YUANBAO;
    _set_auto_value(user, SESSION_AUTO_TIMES, times);
    _set_auto_value(user, SESSION_AUTO_SPENT, spent);

    if (_has_three_same_preview(user)) {
        _stop_auto_random(user);
        _set_notice(user, sprintf("#Y自动转属完成，共转属#R%d#n次，消耗#R%d#n元宝，已出现三条同属性。#n", times, spent));
        _show_preview_menu(user, pet);
        return;
    }

    _set_notice(user, sprintf("#Y自动转属中：第#R%d#n次，已消耗#R%d#n元宝。#n", times, spent));
    _show_preview_menu(user, pet);
    if (!_register_auto_heartbeat(user)) {
        _stop_auto_random(user);
        _set_notice(user, "#Y自动转属暂时无法继续，请重新点击。#n");
        _show_preview_menu(user, pet);
    }
}

mixed _auto_heart_beat(mixed now)
{
    if (!auto_reroll_user) return 1;
    _auto_reroll_tick(auto_reroll_user);
    return 1;
}

private mixed _back_preview(object user)
{
    object pet;

    pet = _session_pet(user);
    if (!pet) {
        return _finish_invalid_preview_session(user);
    }
    _show_preview_menu(user, pet);
    return 1;
}

private mixed _confirm_preview_growth(object user)
{
    object pet;
    int i;
    int type;
    int delta;
    int life_delta;
    int mana_delta;
    int speed_delta;
    int phy_delta;
    int mag_delta;
    string msg;

    if (!_ensure_item_in_user_bag(user)) return 1;
    pet = _session_pet(user);
    if (!pet) {
        return _finish_invalid_preview_session(user);
    }
    if (!_has_any_preview(user)) {
        user->send_dialog_ok("请先点击开始同源生成预览属性。");
        _show_preview_menu(user, pet);
        return 1;
    }
    if (!_pay_yuanbao(user, REROLL_COST_YUANBAO, "确认同源")) {
        _show_preview_menu(user, pet);
        return 1;
    }

    life_delta = 0;
    mana_delta = 0;
    speed_delta = 0;
    phy_delta = 0;
    mag_delta = 0;
    msg = sprintf("恭喜你，#Y%s#n的基础成长获得提升：", pet->get_name());
    for (i = 1; i <= PREVIEW_COUNT; i++) {
        type = _preview_type(user, i);
        delta = _preview_delta(user, i);
        if (type <= 0) continue;
        msg += sprintf("\n%s %s（%s）", _colored_growth_name(user, type), _colored_delta(delta), _colored_perfect_text(delta));
        if (type == PREVIEW_TYPE_LIFE) life_delta += delta;
        if (type == PREVIEW_TYPE_MANA) mana_delta += delta;
        if (type == PREVIEW_TYPE_SPEED) speed_delta += delta;
        if (type == PREVIEW_TYPE_PHY) phy_delta += delta;
        if (type == PREVIEW_TYPE_MAG) mag_delta += delta;
    }

    if (life_delta != 0) {
        _add_growth(pet, "life", life_delta);
    }
    if (mana_delta != 0) {
        _add_growth(pet, "mana", mana_delta);
    }
    if (speed_delta != 0) {
        _add_growth(pet, "speed", speed_delta);
    }
    if (phy_delta != 0) {
        _add_growth(pet, "phy", phy_delta);
    }
    if (mag_delta != 0) {
        _add_growth(pet, "mag", mag_delta);
    }
    FORMULAD->calculate(pet, FORMULA_CALCULATE_TYPE);
    _sync_growth_effect_fields(pet);
    pet->update_char();
    PETD->send_update_pet(user, pet, 0);
    if (catch(PETD->send_pet_update_improvement(user, pet, 0))) {
    }
    if (catch(user->send_update_pet(user->get_pet_no(pet), pet->get_id(), "basic_effect"))) {
    }
    if (catch(user->send_update_pet(user->get_pet_no(pet), pet->get_id(), "init_basic_effect"))) {
    }
    if (catch(user->send_update_pet(user->get_pet_no(pet), pet->get_id(), "life_effect"))) {
    }
    if (catch(user->send_update_pet(user->get_pet_no(pet), pet->get_id(), "mana_effect"))) {
    }
    if (catch(user->send_update_pet(user->get_pet_no(pet), pet->get_id(), "speed_effect"))) {
    }
    if (catch(user->send_update_pet(user->get_pet_no(pet), pet->get_id(), "phy_effect"))) {
    }
    if (catch(user->send_update_pet(user->get_pet_no(pet), pet->get_id(), "mag_effect"))) {
    }
    consume_amount(ITEM_CONSUME_AMOUNT);
    charge_item_consume_log(user);
    _clear_lock_session(user);
    user->send_dialog_ok("同源成功！\n" + msg);
    return 1;
}

private mixed _toggle_session_lock(object user, int bit)
{
    object pet;

    pet = _session_pet(user);
    if (!pet) {
        return _finish_invalid_preview_session(user);
    }

    _show_preview_menu(user, pet);
    return 1;
}

private mixed _apply_session_growth(object user)
{
    return _confirm_preview_growth(user);
}

private mixed _cancel_session(object user)
{
    if (!_ensure_item_in_user_bag(user)) return 1;
    _clear_lock_session(user);
    consume_amount(ITEM_CONSUME_AMOUNT);
    if (user) charge_item_consume_log(user);
    if (user) {
        if (catch(user->destroy_menu())) {
        }
        user->send_dialog_ok("你放弃了本次同源属性，宠物同源丹已经消失。");
    }
    return 1;
}

private mixed _pick_pet_slot(object user, int slot)
{
    object pet;

    if (!_ensure_item_in_user_bag(user)) return 1;
    pet = _query_select_pet_slot(user, slot);
    if (!pet) {
        return _finish_invalid_preview_session(user);
    }

    _start_preview_menu(user, pet);
    return 1;
}

void apply_pet_choose_lock(object user, object pet, object item, int mask, mixed action)
{
    if (!user || !pet || !user->has_pet(pet)) return;

    if (action == ACTION_CANCEL) return;
    _start_preview_menu(user, pet);
}

mixed menu_action_lock_life(object me, mixed npc) { return _toggle_session_lock(me, LOCK_LIFE); }
mixed menu_action_lock_mana(object me, mixed npc) { return _toggle_session_lock(me, LOCK_MANA); }
mixed menu_action_lock_speed(object me, mixed npc) { return _toggle_session_lock(me, LOCK_SPEED); }
mixed menu_action_lock_phy(object me, mixed npc) { return _toggle_session_lock(me, LOCK_PHY); }
mixed menu_action_lock_mag(object me, mixed npc) { return _toggle_session_lock(me, LOCK_MAG); }
mixed menu_action_lock_line_1(object me, mixed npc) { return _toggle_preview_line(me, 1); }
mixed menu_action_lock_line_2(object me, mixed npc) { return _toggle_preview_line(me, 2); }
mixed menu_action_lock_line_3(object me, mixed npc) { return _toggle_preview_line(me, 3); }
mixed menu_action_lock_line_4(object me, mixed npc) { return _toggle_preview_line(me, 4); }
mixed menu_action_lock_line_5(object me, mixed npc) { return _toggle_preview_line(me, 5); }
mixed menu_action_confirm_lock_1(object me, mixed npc) { return _confirm_lock_line(me, 1); }
mixed menu_action_confirm_lock_2(object me, mixed npc) { return _confirm_lock_line(me, 2); }
mixed menu_action_confirm_lock_3(object me, mixed npc) { return _confirm_lock_line(me, 3); }
mixed menu_action_confirm_lock_4(object me, mixed npc) { return _confirm_lock_line(me, 4); }
mixed menu_action_confirm_lock_5(object me, mixed npc) { return _confirm_lock_line(me, 5); }
mixed menu_action_reroll(object me, mixed npc) { return _request_reroll_preview(me); }
mixed menu_action_confirm_reroll(object me, mixed npc) { return _confirm_reroll_preview(me); }
mixed menu_action_auto_reroll(object me, mixed npc) { return _auto_reroll_preview(me); }
mixed menu_action_start_once(object me, mixed npc) { return _start_once_preview(me); }
mixed menu_action_confirm(object me, mixed npc) { return _confirm_preview_growth(me); }
mixed menu_action_apply(object me, mixed npc) { return _apply_session_growth(me); }
mixed menu_action_back_preview(object me, mixed npc) { return _back_preview(me); }
mixed menu_action_cancel(object me, mixed npc) { return _cancel_session(me); }
mixed menu_action_pick_pet_1(object me, mixed npc) { return _pick_pet_slot(me, 1); }
mixed menu_action_pick_pet_2(object me, mixed npc) { return _pick_pet_slot(me, 2); }
mixed menu_action_pick_pet_3(object me, mixed npc) { return _pick_pet_slot(me, 3); }
mixed menu_action_pick_pet_4(object me, mixed npc) { return _pick_pet_slot(me, 4); }
mixed menu_action_pick_pet_5(object me, mixed npc) { return _pick_pet_slot(me, 5); }
mixed menu_action_pick_pet_6(object me, mixed npc) { return _pick_pet_slot(me, 6); }
mixed menu_action_pick_pet_7(object me, mixed npc) { return _pick_pet_slot(me, 7); }
mixed menu_action_pick_pet_8(object me, mixed npc) { return _pick_pet_slot(me, 8); }
mixed menu_action_pick_pet_9(object me, mixed npc) { return _pick_pet_slot(me, 9); }
mixed menu_action_pick_pet_10(object me, mixed npc) { return _pick_pet_slot(me, 10); }
mixed menu_action_pick_pet_11(object me, mixed npc) { return _pick_pet_slot(me, 11); }
mixed menu_action_pick_pet_12(object me, mixed npc) { return _pick_pet_slot(me, 12); }
mixed menu_action_pick_pet_13(object me, mixed npc) { return _pick_pet_slot(me, 13); }
mixed menu_action_pick_pet_14(object me, mixed npc) { return _pick_pet_slot(me, 14); }
mixed menu_action_pick_pet_15(object me, mixed npc) { return _pick_pet_slot(me, 15); }
mixed menu_action_pick_pet_16(object me, mixed npc) { return _pick_pet_slot(me, 16); }
mixed menu_action_pick_pet_17(object me, mixed npc) { return _pick_pet_slot(me, 17); }
mixed menu_action_pick_pet_18(object me, mixed npc) { return _pick_pet_slot(me, 18); }
mixed menu_action_pick_pet_19(object me, mixed npc) { return _pick_pet_slot(me, 19); }
mixed menu_action_pick_pet_20(object me, mixed npc) { return _pick_pet_slot(me, 20); }
mixed menu_action_pick_pet_21(object me, mixed npc) { return _pick_pet_slot(me, 21); }
mixed menu_action_pick_pet_22(object me, mixed npc) { return _pick_pet_slot(me, 22); }
mixed menu_action_pick_pet_23(object me, mixed npc) { return _pick_pet_slot(me, 23); }
mixed menu_action_pick_pet_24(object me, mixed npc) { return _pick_pet_slot(me, 24); }
mixed menu_action_pick_pet_25(object me, mixed npc) { return _pick_pet_slot(me, 25); }
mixed menu_action_pick_pet_26(object me, mixed npc) { return _pick_pet_slot(me, 26); }
mixed menu_action_pick_pet_27(object me, mixed npc) { return _pick_pet_slot(me, 27); }
mixed menu_action_pick_pet_28(object me, mixed npc) { return _pick_pet_slot(me, 28); }
mixed menu_action_pick_pet_29(object me, mixed npc) { return _pick_pet_slot(me, 29); }
mixed menu_action_pick_pet_30(object me, mixed npc) { return _pick_pet_slot(me, 30); }

mixed menu_default_action(object me, mixed npc, mixed para, mixed text)
{
    mixed key;

    key = para;
    if (!key || key == "") key = text;

    if (key == "pgr_lock_life" || key == "血量成长") return menu_action_lock_life(me, npc);
    if (key == "pgr_lock_mana" || key == "法力成长") return menu_action_lock_mana(me, npc);
    if (key == "pgr_lock_speed" || key == "速度成长") return menu_action_lock_speed(me, npc);
    if (key == "pgr_lock_phy" || key == "物攻成长") return menu_action_lock_phy(me, npc);
    if (key == "pgr_lock_mag" || key == "法攻成长") return menu_action_lock_mag(me, npc);
    if (key == "pgr_lock_line_1") return menu_action_lock_line_1(me, npc);
    if (key == "pgr_lock_line_2") return menu_action_lock_line_2(me, npc);
    if (key == "pgr_lock_line_3") return menu_action_lock_line_3(me, npc);
    if (key == "pgr_lock_line_4") return menu_action_lock_line_4(me, npc);
    if (key == "pgr_lock_line_5") return menu_action_lock_line_5(me, npc);
    if (key == "pgr_confirm_lock_1") return menu_action_confirm_lock_1(me, npc);
    if (key == "pgr_confirm_lock_2") return menu_action_confirm_lock_2(me, npc);
    if (key == "pgr_confirm_lock_3") return menu_action_confirm_lock_3(me, npc);
    if (key == "pgr_confirm_lock_4") return menu_action_confirm_lock_4(me, npc);
    if (key == "pgr_confirm_lock_5") return menu_action_confirm_lock_5(me, npc);
    if (key == "pgr_reroll" || key == "进行同源转属") return menu_action_reroll(me, npc);
    if (key == "pgr_start_once" || key == "开始同源") return menu_action_start_once(me, npc);
    if (key == "pgr_confirm_reroll") return menu_action_confirm_reroll(me, npc);
    if (key == "pgr_auto_reroll" || key == "进行自动转属") return menu_action_auto_reroll(me, npc);
    if (key == "pgr_back_preview") return menu_action_back_preview(me, npc);
    if (key == "pgr_confirm" || key == "确定") return menu_action_confirm(me, npc);
    if (key == "pgr_apply" || key == "开始随机") return menu_action_apply(me, npc);
    if (key == "pgr_cancel" || key == "取消" || key == "离开") return menu_action_cancel(me, npc);

    if (key == "pgr_pet_1") return menu_action_pick_pet_1(me, npc);
    if (key == "pgr_pet_2") return menu_action_pick_pet_2(me, npc);
    if (key == "pgr_pet_3") return menu_action_pick_pet_3(me, npc);
    if (key == "pgr_pet_4") return menu_action_pick_pet_4(me, npc);
    if (key == "pgr_pet_5") return menu_action_pick_pet_5(me, npc);
    if (key == "pgr_pet_6") return menu_action_pick_pet_6(me, npc);
    if (key == "pgr_pet_7") return menu_action_pick_pet_7(me, npc);
    if (key == "pgr_pet_8") return menu_action_pick_pet_8(me, npc);
    if (key == "pgr_pet_9") return menu_action_pick_pet_9(me, npc);
    if (key == "pgr_pet_10") return menu_action_pick_pet_10(me, npc);
    if (key == "pgr_pet_11") return menu_action_pick_pet_11(me, npc);
    if (key == "pgr_pet_12") return menu_action_pick_pet_12(me, npc);
    if (key == "pgr_pet_13") return menu_action_pick_pet_13(me, npc);
    if (key == "pgr_pet_14") return menu_action_pick_pet_14(me, npc);
    if (key == "pgr_pet_15") return menu_action_pick_pet_15(me, npc);
    if (key == "pgr_pet_16") return menu_action_pick_pet_16(me, npc);
    if (key == "pgr_pet_17") return menu_action_pick_pet_17(me, npc);
    if (key == "pgr_pet_18") return menu_action_pick_pet_18(me, npc);
    if (key == "pgr_pet_19") return menu_action_pick_pet_19(me, npc);
    if (key == "pgr_pet_20") return menu_action_pick_pet_20(me, npc);
    if (key == "pgr_pet_21") return menu_action_pick_pet_21(me, npc);
    if (key == "pgr_pet_22") return menu_action_pick_pet_22(me, npc);
    if (key == "pgr_pet_23") return menu_action_pick_pet_23(me, npc);
    if (key == "pgr_pet_24") return menu_action_pick_pet_24(me, npc);
    if (key == "pgr_pet_25") return menu_action_pick_pet_25(me, npc);
    if (key == "pgr_pet_26") return menu_action_pick_pet_26(me, npc);
    if (key == "pgr_pet_27") return menu_action_pick_pet_27(me, npc);
    if (key == "pgr_pet_28") return menu_action_pick_pet_28(me, npc);
    if (key == "pgr_pet_29") return menu_action_pick_pet_29(me, npc);
    if (key == "pgr_pet_30") return menu_action_pick_pet_30(me, npc);

    return 1;
}

void create()
{
    object me;

    if (strsrch(object_name(this_object()), "#") >= 0) return;

    me = this_object();
    me->set_name(ITEM_NAME);
    me->import(ITEM_NAME);
    me->set(PROP_COLOR, ITEM_COLOR);
    me->set(PROP_ATTRIB, me->query(PROP_ATTRIB) | ITEM_ATTRIB_MASK);
    me->setup_charge_item();
}

int apply_pet(object pet)
{
    object user;

    user = env_object();
    if (!user) return 0;
    _show_pet_select_menu(user);
    return 0;
}

int apply()
{
    object user;
    mixed error_message;

    user = env_object();
    if (!user) return 0;

    error_message = this_object()->can_be_deal(TRADE_CHECK_LEVEL);
    if (error_message) {
        user->send_dialog_ok(error_message);
        return 0;
    }

    if (user->is_in_combat() ||
        user->is_in_lookon() ||
        user->is_in_stall() ||
        TASKD->query_task(user, TASK_STATE_KEY)) {
        user->send_dialog_ok(STATUS_FORBIDDEN_MESSAGE);
        return 0;
    }

    _show_pet_select_menu(user);
    return 0;
}
