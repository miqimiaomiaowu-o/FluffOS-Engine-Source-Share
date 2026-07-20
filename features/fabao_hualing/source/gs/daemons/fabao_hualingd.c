#define ATTACH_SKILLD "/gs/daemons/attach_skilld.c"
#define UPGRADED "/gs/daemons/upgraded.c"
#define NAMED_D "/gs/daemons/named.c"
#define NPC_NAME "活动大使"
#define NPC_MENU_NAME "法宝化灵"
#define NPC_MENU_KEY "fabao_hualing_npc_menu"
#define FABAO_HUALING_D "/gs/daemons/fabao_hualingd.c"

#define TOKEN_ITEM "fabao_hualing/item"
#define TOKEN_IID "fabao_hualing/item_iid"
#define TOKEN_MARK "fabao_hualing/mark"
#define MAIN_IID "fabao_hualing/main_iid"
#define SUB_IID "fabao_hualing/sub_iid"
#define SELECT_MODE "fabao_hualing/select_mode"
#define SUBMIT_STAGE "fabao_hualing/submit_stage"
#define SLOT_IID_PREFIX "fabao_hualing/slot_iid_"
#define MAX_SELECT_SLOT 30
#define HUALING_EXTRA_DESC_KEY "fabao_hualing/orig_extra_desc"
#define HUALING_EXTRA_DESC_MARK "fabao_hualing/has_extra_desc"
#define HUALING_SUB_SAVE_TEXT "fabao_hualing/sub_save_text"
#define HUALING_SUB_SAVE_NAME "fabao_hualing/sub_save_name"
#define SKILL_ARTIFACT_BASIC "/gs/clone/item/skill_artifact/basic.c"

string query_marker()
{
    return "fabao-hualing-daemon-v17-single-submit-guard-20260630";
}

private void _msg(object me, string text)
{
    if (!me) return;
    if (catch(me->send_dialog_ok(text))) {
    }
}

private mixed _safe_item_name(mixed item)
{
    mixed name;

    if (!item) return "法宝化灵符";
    name = 0;
    if (!catch(name = item->get_name()) && name) return name;
    return "法宝化灵符";
}

private mixed _skill_name(mixed skill)
{
    mixed ob;
    mixed name;

    ob = ATTACH_SKILLD->query_entry(skill);
    if (!ob) return skill;
    if (catch(name = ob->get_name())) return skill;
    if (!name || name == "") return skill;
    return name;
}

private int _is_skill_artifact(mixed item)
{
    mixed ret;

    if (!item) return 0;
    ret = 0;
    if (catch(ret = item->is_skill_artifact())) ret = 0;
    return ret ? 1 : 0;
}

private mixed _query_attach_skill(mixed item)
{
    mixed skill;

    if (!item) return 0;
    skill = 0;
    if (catch(skill = item->query("attach_skill"))) skill = 0;
    if (skill && skill != "") return skill;
    if (catch(skill = item->get_raw_skill())) skill = 0;
    if (skill && skill != "") return skill;
    return 0;
}

private int _has_attach_skill(mixed item)
{
    mixed skill;

    skill = _query_attach_skill(item);
    return (skill && skill != "") ? 1 : 0;
}

private int _has_hualing(mixed item)
{
    if (!item) return 0;
    if (item->query("fabao_hualing_v1")) return 1;
    if (item->query("extra_attach_skill")) return 1;
    if (item->query("extra_attach_from_name")) return 1;
    return 0;
}

private void _set_hualing_desc(mixed item, mixed sub_name)
{
    mixed extra;

    if (!item || !sub_name) return;

    extra = 0;
    if (catch(extra = item->query("extra_desc"))) extra = 0;
    if (!item->query(HUALING_EXTRA_DESC_MARK)) {
        catch(item->set(HUALING_EXTRA_DESC_KEY, extra));
        catch(item->set(HUALING_EXTRA_DESC_MARK, 1));
    } else {
        if (catch(extra = item->query(HUALING_EXTRA_DESC_KEY))) extra = 0;
    }
    if (!extra) extra = "";
    catch(item->set("extra_desc", sprintf("%s#Y该法宝已化灵:%s#n", extra, sub_name)));
}

private mixed _skill_module(mixed skill)
{
    mixed module;

    if (!skill || skill == "") return 0;
    module = ATTACH_SKILLD->query_entry(skill);
    if (module) return module;
    module = "/gs/daemons/attach_skill/" + skill + ".c";
    if (catch(load_object(module))) return 0;
    return module;
}

private mixed _append_inventory_items(mixed out, mixed inv)
{
    mixed ks;
    mixed item;
    int i;

    if (!out) out = ({});
    if (!inv) return out;

    ks = 0;
    if (is_mapping(inv) && !catch(ks = keys(inv)) && ks) {
        for (i = 0; i < sizeof(ks); i++) {
            item = 0;
            if (catch(item = inv[ks[i]])) item = 0;
            if (item) out += ({ item });
        }
        return out;
    }

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

private mixed _safe_iid(mixed item)
{
    mixed iid;

    if (!item) return 0;
    iid = 0;
    if (catch(iid = item->get_iid())) iid = 0;
    return iid;
}

private mixed _get_equip_at(object me, int pos)
{
    mixed item;

    if (!me) return 0;
    item = 0;
    if (catch(item = me->get_equip_item(pos))) item = 0;
    return item;
}

private mixed _equipped_skill_artifact(object me)
{
    mixed positions;
    mixed item;
    int i;

    if (!me) return 0;
    positions = ({ 9, 53 });
    for (i = 0; i < sizeof(positions); i++) {
        item = _get_equip_at(me, positions[i]);
        if (item && _is_skill_artifact(item)) return item;
    }
    return 0;
}

private int _equipped_skill_artifact_iid(object me, mixed iid)
{
    mixed positions;
    mixed item;
    int i;

    if (!me || !iid) return 0;
    positions = ({ 9, 53 });
    for (i = 0; i < sizeof(positions); i++) {
        item = _get_equip_at(me, positions[i]);
        if (item && _safe_iid(item) == iid) return 1;
    }
    return 0;
}

private mixed _set_equipped_main(object me)
{
    mixed item;
    mixed iid;

    if (!me) return 0;
    item = _equipped_skill_artifact(me);
    iid = _safe_iid(item);
    if (item && iid) {
        if (catch(me->set_temp(MAIN_IID, iid))) {
        }
        return item;
    }
    if (catch(me->delete_temp(MAIN_IID))) {
    }
    return 0;
}

private mixed _find_item_by_iid(object me, mixed iid)
{
    mixed inv;
    mixed item;
    mixed cur_iid;
    mixed positions;
    int i;

    if (!me || !iid) return 0;
    positions = ({ 9, 53 });
    for (i = 0; i < sizeof(positions); i++) {
        item = _get_equip_at(me, positions[i]);
        if (item && _safe_iid(item) == iid) return item;
    }

    inv = _inventory(me);
    if (!inv) return 0;

    for (i = 0; i < sizeof(inv); i++) {
        item = inv[i];
        if (!item) continue;
        cur_iid = _safe_iid(item);
        if (cur_iid && cur_iid == iid) return item;
    }
    return 0;
}

private void _consume_item(mixed item)
{
    mixed ret;

    if (!item) return;
    ret = 0;
    if (catch(ret = item->cost_amount(1))) {
        if (catch(item->destruct())) {
        }
        return;
    }
    if (!ret && item) {
        if (catch(item->destruct())) {
        }
    }
}

private mixed _bad(mixed msg)
{
    return ({ 0, msg });
}

mixed query_merge_item(object me)
{
    mixed item;
    mixed mark;
    mixed iid;

    if (!me) return 0;
    item = 0;
    mark = 0;
    if (catch(item = me->query_temp(TOKEN_ITEM))) item = 0;
    if (catch(mark = me->query_temp(TOKEN_MARK))) mark = 0;
    if (mark != query_marker()) return 0;
    if (item) return item;

    iid = 0;
    if (catch(iid = me->query_temp(TOKEN_IID))) iid = 0;
    if (iid) item = _find_item_by_iid(me, iid);
    return item;
}

mixed check_merge_token(object me)
{
    mixed item;

    item = query_merge_item(me);
    if (!item) return ({ 1, 0 });
    return ({ 1, item });
}

mixed consume_merge_token(object me)
{
    mixed item;

    item = query_merge_item(me);

    if (catch(me->delete_temp(TOKEN_ITEM))) {
    }
    if (catch(me->delete_temp(TOKEN_IID))) {
    }
    if (catch(me->delete_temp(TOKEN_MARK))) {
    }
    if (catch(me->delete_temp(MAIN_IID))) {
    }
    if (catch(me->delete_temp(SUB_IID))) {
    }

    if (!item) return 1;

    _consume_item(item);
    return 1;
}

void refresh_merge_token(object me)
{
    mixed item;
    mixed pos;

    item = query_merge_item(me);
    if (!item) return;

    if (catch(item->move_to_bag(me))) {
    }
    if (catch(item->refresh_item())) {
    }
    pos = 0;
    if (catch(pos = item->query("position"))) pos = 0;
    if (pos) {
        if (catch(me->refresh_item(pos))) {
        }
    }
}

void clear_merge_token(object me)
{
    if (!me) return;
    if (catch(me->delete_temp(TOKEN_ITEM))) {
    }
    if (catch(me->delete_temp(TOKEN_IID))) {
    }
    if (catch(me->delete_temp(TOKEN_MARK))) {
    }
    if (catch(me->delete_temp(MAIN_IID))) {
    }
    if (catch(me->delete_temp(SUB_IID))) {
    }
}

private mixed _check_items(object me)
{
    mixed main_iid;
    mixed sub_iid;
    mixed main_artifact;
    mixed sub_artifact;
    mixed main_skill;
    mixed sub_skill;
    mixed token;

    token = check_merge_token(me);
    if (!token || token[0] != 1) {
        if (token && sizeof(token) > 1) return _bad(token[1]);
        return _bad("请先右键使用#R法宝化灵符#n。");
    }

    main_iid = me->query_temp(MAIN_IID);
    sub_iid = me->query_temp(SUB_IID);
    if (!main_iid) return _bad("请先选择主法宝。");
    if (!sub_iid) return _bad("请先选择副法宝。");

    main_artifact = _find_item_by_iid(me, main_iid);
    sub_artifact = _find_item_by_iid(me, sub_iid);
    if (!main_artifact || !sub_artifact || main_artifact == sub_artifact) {
        return _bad("主法宝和副法宝必须是背包里的两个不同法宝。");
    }
    if (_has_hualing(main_artifact)) {
        return _bad(sprintf("你的#R%s#n已经化灵过，不能继续化灵。", main_artifact->get_name()));
    }
    if (_has_hualing(sub_artifact)) {
        return _bad(sprintf("你的#R%s#n已经化灵过，不能作为副法宝。", sub_artifact->get_name()));
    }

    if (!_has_attach_skill(main_artifact) || !_has_attach_skill(sub_artifact)) {
        return _bad("只能选择带被动技能的法宝。");
    }

    if (main_artifact->is_limit_item()) {
        return _bad(sprintf("你的#R%s#n是限时法宝，不能作为主法宝。", main_artifact->get_name()));
    }
    if (sub_artifact->is_limit_item()) {
        return _bad(sprintf("你的#R%s#n是限时法宝，不能作为副法宝。", sub_artifact->get_name()));
    }
    if (main_artifact->query("no_drop")) {
        return _bad(sprintf("你的#R%s#n已经加锁，不能作为主法宝。", main_artifact->get_name()));
    }
    if (main_artifact->query("master_id")) {
        return _bad(sprintf("你的#R%s#n已经认主，不能作为主法宝。", main_artifact->get_name()));
    }
    if (sub_artifact->query("no_drop")) {
        return _bad(sprintf("你的#R%s#n已经加锁，不能作为副法宝。", sub_artifact->get_name()));
    }
    if (sub_artifact->query("master_id")) {
        return _bad(sprintf("你的#R%s#n已经认主，不能作为副法宝。", sub_artifact->get_name()));
    }
    if (sub_artifact->query("bind_owner")) {
        return _bad(sprintf("你的#R%s#n已经绑定，不能作为副法宝。", sub_artifact->get_name()));
    }
    if (sub_artifact->query("seal_pet")) {
        return _bad(sprintf("你的#R%s#n已经封印宠物，不能作为副法宝。", sub_artifact->get_name()));
    }

    main_skill = _query_attach_skill(main_artifact);
    sub_skill = _query_attach_skill(sub_artifact);
    if (!main_skill || main_skill == "" || !sub_skill || sub_skill == "") {
        return _bad("两个法宝都必须带有可触发的被动技能。");
    }
    if (!_skill_module(main_skill)) {
        return _bad(sprintf("主法宝被动#R%s#n没有对应触发模块，暂不能化灵。", main_skill));
    }
    if (!_skill_module(sub_skill)) {
        return _bad(sprintf("副法宝被动#R%s#n没有对应触发模块，暂不能化灵。", sub_skill));
    }
    if (main_skill == sub_skill) {
        return _bad("两个法宝被动技能相同，不需要化灵。");
    }
    if (main_artifact->query("extra_attach_skill")) {
        return _bad(sprintf("你的#R%s#n已经拥有第二被动技能。", main_artifact->get_name()));
    }

    return ({ 1, main_artifact, sub_artifact, main_skill, sub_skill });
}

private mixed _check_pair(object me, mixed main_artifact, mixed sub_artifact)
{
    mixed main_skill;
    mixed sub_skill;
    mixed token;

    token = check_merge_token(me);
    if (!token || token[0] != 1) {
        if (token && sizeof(token) > 1) return _bad(token[1]);
        return _bad("请先右键使用#R法宝化灵符#n。");
    }

    if (!main_artifact || !sub_artifact || main_artifact == sub_artifact) {
        return _bad("请提交两个不同的法宝，第一个为主法宝，第二个为副法宝。");
    }
    if (_has_hualing(main_artifact)) {
        return _bad(sprintf("你的#R%s#n已经化灵过，不能继续化灵。", main_artifact->get_name()));
    }
    if (_has_hualing(sub_artifact)) {
        return _bad(sprintf("你的#R%s#n已经化灵过，不能作为副法宝。", sub_artifact->get_name()));
    }

    if (!_has_attach_skill(main_artifact) || !_has_attach_skill(sub_artifact)) {
        return _bad("只能提交带被动技能的法宝。");
    }

    if (main_artifact->is_limit_item()) {
        return _bad(sprintf("你的#R%s#n是限时法宝，不能作为主法宝。", main_artifact->get_name()));
    }
    if (sub_artifact->is_limit_item()) {
        return _bad(sprintf("你的#R%s#n是限时法宝，不能作为副法宝。", sub_artifact->get_name()));
    }
    if (main_artifact->query("no_drop")) {
        return _bad(sprintf("你的#R%s#n已经加锁，不能作为主法宝。", main_artifact->get_name()));
    }
    if (main_artifact->query("master_id")) {
        return _bad(sprintf("你的#R%s#n已经认主，不能作为主法宝。", main_artifact->get_name()));
    }
    if (sub_artifact->query("no_drop")) {
        return _bad(sprintf("你的#R%s#n已经加锁，不能作为副法宝。", sub_artifact->get_name()));
    }
    if (sub_artifact->query("master_id")) {
        return _bad(sprintf("你的#R%s#n已经认主，不能作为副法宝。", sub_artifact->get_name()));
    }
    if (sub_artifact->query("bind_owner")) {
        return _bad(sprintf("你的#R%s#n已经绑定，不能作为副法宝。", sub_artifact->get_name()));
    }
    if (sub_artifact->query("seal_pet")) {
        return _bad(sprintf("你的#R%s#n已经封印宠物，不能作为副法宝。", sub_artifact->get_name()));
    }

    main_skill = _query_attach_skill(main_artifact);
    sub_skill = _query_attach_skill(sub_artifact);
    if (!main_skill || main_skill == "" || !sub_skill || sub_skill == "") {
        return _bad("两个法宝都必须带有可触发的被动技能。");
    }
    if (!_skill_module(main_skill)) {
        return _bad(sprintf("主法宝被动#R%s#n没有对应触发模块，暂不能化灵。", main_skill));
    }
    if (!_skill_module(sub_skill)) {
        return _bad(sprintf("副法宝被动#R%s#n没有对应触发模块，暂不能化灵。", sub_skill));
    }
    if (main_skill == sub_skill) {
        return _bad("两个法宝被动技能相同，不需要化灵。");
    }
    if (main_artifact->query("extra_attach_skill")) {
        return _bad(sprintf("你的#R%s#n已经拥有第二被动技能。", main_artifact->get_name()));
    }

    return ({ 1, main_artifact, sub_artifact, main_skill, sub_skill });
}

private int _query_item_pos(mixed item)
{
    mixed pos;

    if (!item) return 999999;
    pos = 0;
    if (catch(pos = item->query("position"))) pos = 0;
    if (!pos && catch(pos = item->get_pos())) pos = 0;
    if (!pos) return 999999;
    return pos;
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

private mixed _submit_item_at(mixed items, int idx)
{
    mixed ks;

    if (!items) return 0;
    if (!catch(ks = keys(items))) {
        if (sizeof(ks) <= idx) return 0;
        return ks[idx];
    }
    if (_looks_like_item(items)) {
        if (idx == 0) return items;
        return 0;
    }
    if (!catch(items + "")) return 0;
    if (catch(sizeof(items))) return 0;
    if (sizeof(items) <= idx) return 0;
    return items[idx];
}

private int _submit_item_count(mixed items)
{
    mixed item;
    mixed seen;
    int i;

    if (!items) return 0;
    seen = ({});
    for (i = 0; i < MAX_SELECT_SLOT; i++) {
        item = _submit_item_at(items, i);
        if (!_looks_like_item(item)) continue;
        if (member_array(item, seen) >= 0) continue;
        seen += ({ item });
    }
    return sizeof(seen);
}

private mixed _pick_submit_source(object me, mixed a, mixed b, mixed c, mixed d, mixed e)
{
    mixed items;

    if (_submit_item_count(a) > 0) return a;
    if (_submit_item_count(b) > 0) return b;
    if (_submit_item_count(c) > 0) return c;
    if (_submit_item_count(d) > 0) return d;
    if (_submit_item_count(e) > 0) return e;

    items = _query_submit_items(me);
    if (_submit_item_count(items) > 0) return items;
    return 0;
}

private void _return_to_bag(object me, mixed item)
{
    if (!me || !_looks_like_item(item)) return;
    if (!catch(item->move_to_bag(me))) return;
    if (catch(item->move(me))) {
    }
}

private void _return_submit_items(object me, mixed items)
{
    mixed item;
    mixed seen;
    int i;

    if (!me || !items) return;
    seen = ({});
    for (i = 0; i < MAX_SELECT_SLOT; i++) {
        item = _submit_item_at(items, i);
        if (!_looks_like_item(item)) continue;
        if (member_array(item, seen) >= 0) continue;
        seen += ({ item });
        _return_to_bag(me, item);
    }
}

private void _refresh_artifact(object me, mixed item)
{
    mixed pos;

    if (!me || !item) return;
    if (catch(item->refresh_item())) {
    }
    pos = 0;
    if (catch(pos = item->query("position"))) pos = 0;
    if (pos) {
        if (catch(me->refresh_item(pos))) {
        }
    }
    if (catch(me->update_char())) {
    }
}

private void _cost_submit_item(object me, mixed item)
{
    if (!me || !item) return;

    _consume_item(item);
}

private mixed _pick_submit_items(object me, mixed a, mixed b, mixed c, mixed d, mixed e)
{
    mixed items;

    if (_looks_like_item(_submit_item_at(a, 0)) && _looks_like_item(_submit_item_at(a, 1))) return a;
    if (_looks_like_item(_submit_item_at(b, 0)) && _looks_like_item(_submit_item_at(b, 1))) return b;
    if (_looks_like_item(_submit_item_at(c, 0)) && _looks_like_item(_submit_item_at(c, 1))) return c;
    if (_looks_like_item(_submit_item_at(d, 0)) && _looks_like_item(_submit_item_at(d, 1))) return d;
    if (_looks_like_item(_submit_item_at(e, 0)) && _looks_like_item(_submit_item_at(e, 1))) return e;

    items = _query_submit_items(me);
    if (_looks_like_item(_submit_item_at(items, 0)) && _looks_like_item(_submit_item_at(items, 1))) return items;
    return items;
}

private mixed _pick_submit_item(object me, mixed a, mixed b, mixed c, mixed d, mixed e)
{
    mixed item;
    mixed items;

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

    items = _query_submit_items(me);
    item = _submit_item_at(items, 0);
    if (_looks_like_item(item)) return item;
    return 0;
}

private void _save_sub_artifact_snapshot(mixed main_artifact, mixed sub_artifact, mixed sub_name)
{
    mixed text;

    if (!main_artifact || !sub_artifact) return;
    text = 0;
    if (catch(text = sub_artifact->save_to_text())) text = 0;
    if (text && text != "") {
        if (catch(main_artifact->set(HUALING_SUB_SAVE_TEXT, text))) {
        }
        if (sub_name && sub_name != "") {
            if (catch(main_artifact->set(HUALING_SUB_SAVE_NAME, sub_name))) {
            }
        }
    }
}

private int _restore_sub_artifact_snapshot(object me, mixed main_artifact)
{
    mixed text;
    mixed item;
    mixed sub_name;
    mixed pos;

    if (!me || !main_artifact) return 0;
    text = 0;
    if (catch(text = main_artifact->query(HUALING_SUB_SAVE_TEXT))) text = 0;
    if (!text || text == "") return 0;

    item = 0;
    if (catch(item = clone_object(SKILL_ARTIFACT_BASIC))) item = 0;
    if (!item) return 0;
    if (catch(item->restore_from_text(text))) {
        if (catch(item->set("basic_object", 0))) {
        }
        if (catch(item->destruct())) {
        }
        return 0;
    }
    if (catch(item->set("basic_object", 0))) {
    }
    sub_name = 0;
    if (catch(sub_name = main_artifact->query(HUALING_SUB_SAVE_NAME))) sub_name = 0;
    if (sub_name && sub_name != "") {
        if (catch(item->import(sub_name))) {
        }
        if (catch(item->set_name(sub_name))) {
        }
        if (catch(item->set("name", sub_name))) {
        }
        if (catch(item->set("file_id", "skill_artifact/basic"))) {
        }
    }
    if (catch(item->set("extra_attach_skill", 0))) {
    }
    if (catch(item->set("extra_attach_skill_name", 0))) {
    }
    if (catch(item->set("extra_attach_from_name", 0))) {
    }
    if (catch(item->set("extra_attach_from_iid", 0))) {
    }
    if (catch(item->set("fabao_hualing_owner_iid", 0))) {
    }
    if (catch(item->set("fabao_hualing_v1", 0))) {
    }
    if (catch(item->set(HUALING_SUB_SAVE_TEXT, 0))) {
    }
    if (catch(item->set(HUALING_SUB_SAVE_NAME, 0))) {
    }
    if (catch(item->set(HUALING_EXTRA_DESC_KEY, 0))) {
    }
    if (catch(item->set(HUALING_EXTRA_DESC_MARK, 0))) {
    }
    if (catch(item->move_to_bag(me))) {
        if (catch(item->move(me))) {
        }
    }
    if (catch(item->refresh_item())) {
    }
    pos = 0;
    if (catch(pos = item->query("position"))) pos = 0;
    if (pos) {
        if (catch(me->refresh_item(pos))) {
        }
    }
    return 1;
}

private mixed _finish_merge(object me, mixed main_artifact, mixed sub_artifact, mixed sub_skill)
{
    mixed msg;
    mixed main_name;
    mixed sub_name;
    mixed main_iid;
    mixed sub_iid;

    main_name = main_artifact->get_name();
    sub_name = sub_artifact->get_name();
    main_iid = main_artifact->get_iid();
    sub_iid = sub_artifact->get_iid();

    main_artifact->set("extra_attach_skill", sub_skill);
    main_artifact->set("extra_attach_skill_name", _skill_name(sub_skill));
    main_artifact->set("extra_attach_from_name", sub_name);
    main_artifact->set("extra_attach_from_iid", sub_iid);
    main_artifact->set("fabao_hualing_owner_iid", main_iid);
    main_artifact->set("fabao_hualing_v1", 1);
    _save_sub_artifact_snapshot(main_artifact, sub_artifact, sub_name);
    _set_hualing_desc(main_artifact, sub_name);


    _cost_submit_item(me, sub_artifact);
    consume_merge_token(me);

    _refresh_artifact(me, main_artifact);

    msg = sprintf("#R%s#n化灵成功，已融合副法宝#R%s#n的被动。", main_name, sub_name);
    _msg(me, msg);
    if (catch(me->destroy_menu())) {
    }

    return 1;
}

private int _item_name_has(mixed item, string name_part)
{
    mixed name;

    if (!item || !name_part || name_part == "") return 0;
    name = 0;
    if (catch(name = item->get_name())) name = 0;
    if (!name || name == "") return 0;
    return strsrch(name, name_part) >= 0;
}

private mixed _find_named_skill_artifact(object me, string name_part)
{
    mixed inv;
    mixed item;
    int i;

    inv = _inventory(me);
    if (!inv) return 0;
    for (i = 0; i < sizeof(inv); i++) {
        item = inv[i];
        if (!item) continue;
        if (_item_name_has(item, name_part)) return item;
    }
    return 0;
}

private mixed _find_skill_artifact_by_skill(object me, string skill_key)
{
    mixed inv;
    mixed item;
    mixed skill;
    mixed name;
    int i;

    if (!me || !skill_key || skill_key == "") return 0;
    inv = _inventory(me);
    if (!inv) return 0;
    for (i = 0; i < sizeof(inv); i++) {
        item = inv[i];
        if (!item) continue;
        name = 0;
        if (catch(name = item->get_name())) name = 0;
        skill = 0;
        skill = _query_attach_skill(item);
        if (skill == skill_key) return item;
    }
    return 0;
}

private mixed _find_first_other_skill_artifact(object me, mixed main_artifact)
{
    mixed inv;
    mixed item;
    mixed skill;
    int i;

    inv = _inventory(me);
    if (!inv) return 0;
    for (i = 0; i < sizeof(inv); i++) {
        item = inv[i];
        if (!item || item == main_artifact) continue;
        skill = 0;
        skill = _query_attach_skill(item);
        if (skill && skill != "") return item;
    }
    return 0;
}

private mixed _auto_merge_fantian_hunyuan(object me)
{
    mixed main_artifact;
    mixed sub_artifact;
    mixed ret;

    main_artifact = _find_skill_artifact_by_skill(me, "fantianyin");
    if (!main_artifact) main_artifact = _find_named_skill_artifact(me, "番天印");
    if (!main_artifact) main_artifact = _find_named_skill_artifact(me, "翻天印");

    sub_artifact = _find_skill_artifact_by_skill(me, "hunyuan-jindou");
    if (!sub_artifact) sub_artifact = _find_named_skill_artifact(me, "混元金兜");
    if (!sub_artifact) sub_artifact = _find_named_skill_artifact(me, "混元金斗");

    if (!main_artifact) main_artifact = _find_first_other_skill_artifact(me, sub_artifact);
    if (!sub_artifact) sub_artifact = _find_first_other_skill_artifact(me, main_artifact);


    if (!main_artifact || !sub_artifact) {
        clear_merge_token(me);
        _msg(me, "背包里需要同时放入两个带被动技能的法宝，优先把#R混元金斗#n被动合到#R翻天印#n上。");
        return 1;
    }

    ret = _check_pair(me, main_artifact, sub_artifact);
    if (!ret || ret[0] != 1) {
        clear_merge_token(me);
        if (ret && sizeof(ret) > 1) _msg(me, ret[1]);
        else _msg(me, "法宝化灵失败，请检查两个法宝是否可用。");
        return 1;
    }

    return _finish_merge(me, ret[1], ret[2], ret[4]);
}

mixed merge_by_token_on_main(object me, object token_item, object main_artifact)
{
    mixed sub_artifact;
    mixed ret;

    if (!me || !token_item || !main_artifact) return 0;

    me->set_temp(TOKEN_ITEM, token_item);
    me->set_temp(TOKEN_MARK, query_marker());
    if (catch(me->delete_temp(MAIN_IID))) {
    }
    if (catch(me->delete_temp(SUB_IID))) {
    }


    if (!_is_skill_artifact(main_artifact)) {
        clear_merge_token(me);
        _msg(me, "请把#R法宝化灵符#n与要保留的主法宝进行合成。");
        return 1;
    }

    sub_artifact = _find_first_other_skill_artifact(me, main_artifact);
    if (!sub_artifact) {
        clear_merge_token(me);
        _msg(me, "背包里还需要放入一个带被动技能的副法宝。");
        return 1;
    }

    ret = _check_pair(me, main_artifact, sub_artifact);
    if (!ret || ret[0] != 1) {
        clear_merge_token(me);
        if (ret && sizeof(ret) > 1) _msg(me, ret[1]);
        else _msg(me, "法宝化灵失败，请检查主法宝和副法宝是否可用。");
        return 1;
    }

    return _finish_merge(me, ret[1], ret[2], ret[4]);
}

private string _chosen_text(object me, string which)
{
    mixed iid;
    mixed item;

    if (which == "main") iid = me->query_temp(MAIN_IID);
    else iid = me->query_temp(SUB_IID);

    item = _find_item_by_iid(me, iid);
    if (!item) return "未选择";
    return item->get_name();
}

private mixed _menu_action(string fn)
{
    return (: call_other, "/gs/daemons/fabao_hualingd.c", fn :);
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

private void _bind_menu_in(object ob, string menu_key, string key, string fn)
{
    mixed action;

    if (!ob || !menu_key || menu_key == "" || !key || key == "") return;
    action = _menu_action(fn);
    if (catch(ob->set_temp("menu/" + menu_key + "/" + key, action))) {
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

private void _bind_hualing_main_menu(object me, mixed menu_ob)
{
    _bind_menu_in(me, "法宝化灵", "hualing_select_main", "menu_action_select_main");
    _bind_menu_in(me, "法宝化灵", "hualing_select_sub", "menu_action_select_sub");
    _bind_menu_in(me, "法宝化灵", "hualing_split", "menu_action_split");
    _bind_menu_in(me, "法宝化灵", "hualing_cancel", "menu_action_cancel_merge");
    _bind_menu_in(me, "法宝化灵", "选择主法宝", "menu_action_select_main");
    _bind_menu_in(me, "法宝化灵", "选择副法宝", "menu_action_select_sub");
    _bind_menu_in(me, "法宝化灵", "分离化灵", "menu_action_split");
    _bind_menu_in(me, "法宝化灵", "化灵分离", "menu_action_split");
    _bind_menu_in(me, "法宝化灵", "取消化灵", "menu_action_cancel_merge");
    _bind_menu_in(menu_ob, "法宝化灵", "hualing_select_main", "menu_action_select_main");
    _bind_menu_in(menu_ob, "法宝化灵", "hualing_select_sub", "menu_action_select_sub");
    _bind_menu_in(menu_ob, "法宝化灵", "hualing_split", "menu_action_split");
    _bind_menu_in(menu_ob, "法宝化灵", "hualing_cancel", "menu_action_cancel_merge");
    _bind_menu_in(menu_ob, "法宝化灵", "选择主法宝", "menu_action_select_main");
    _bind_menu_in(menu_ob, "法宝化灵", "选择副法宝", "menu_action_select_sub");
    _bind_menu_in(menu_ob, "法宝化灵", "分离化灵", "menu_action_split");
    _bind_menu_in(menu_ob, "法宝化灵", "化灵分离", "menu_action_split");
    _bind_menu_in(menu_ob, "法宝化灵", "取消化灵", "menu_action_cancel_merge");
}

private void _bind_hualing_select_menu(object me, mixed menu_ob, string title, int count)
{
    string command;
    int i;

    for (i = 1; i <= count; i++) {
        command = sprintf("hualing_pick_%d", i);
        _bind_menu(me, command, sprintf("menu_pick_slot_%d", i));
        _bind_menu_on(menu_ob, command, sprintf("menu_pick_slot_%d", i));
    }
    _bind_menu(me, "hualing_back", "menu_action_back");
    _bind_menu(me, "返回化灵", "menu_action_back");
    _bind_menu_on(menu_ob, "hualing_back", "menu_action_back");
    _bind_menu_on(menu_ob, "返回化灵", "menu_action_back");
}

private void _set_select_slot(object me, int slot, mixed iid)
{
    if (!me || slot <= 0 || slot > MAX_SELECT_SLOT) return;
    if (catch(me->set_temp(sprintf("%s%d", SLOT_IID_PREFIX, slot), iid))) {
    }
}

private mixed _query_select_slot(object me, int slot)
{
    mixed iid;

    if (!me || slot <= 0 || slot > MAX_SELECT_SLOT) return 0;
    iid = 0;
    if (catch(iid = me->query_temp(sprintf("%s%d", SLOT_IID_PREFIX, slot)))) iid = 0;
    return iid;
}

private int _select_slot_has_iid(object me, int count, mixed iid)
{
    int i;

    if (!me || !iid) return 0;
    for (i = 1; i <= count; i++) {
        if (_query_select_slot(me, i) == iid) return 1;
    }
    return 0;
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

private void _show_menu(object me)
{
    mixed main_text;
    mixed sub_text;
    mixed content;
    mixed menu_ob;

    if (!me) return;
    _set_equipped_main(me);
    main_text = _chosen_text(me, "main");
    sub_text = _chosen_text(me, "sub");
    menu_ob = _menu_object(me);
    _prepare_menu_object(me);

    content = sprintf("#R法宝化灵#n\n主法宝：#Y%s#n（自动识别身上穿戴的法宝）\n副法宝：#Y%s#n\n[选择副法宝/hualing_select_sub][化灵分离/hualing_split][取消/hualing_cancel]",
        main_text, sub_text);
    _bind_menu(me, "hualing_select_main", "menu_action_select_main");
    _bind_menu(me, "hualing_select_sub", "menu_action_select_sub");
    _bind_menu(me, "hualing_split", "menu_action_split");
    _bind_menu(me, "hualing_cancel", "menu_action_cancel_merge");
    _bind_menu(me, "选择主法宝", "menu_action_select_main");
    _bind_menu(me, "选择副法宝", "menu_action_select_sub");
    _bind_menu(me, "分离化灵", "menu_action_split");
    _bind_menu(me, "化灵分离", "menu_action_split");
    _bind_menu(me, "取消化灵", "menu_action_cancel_merge");
    me->set_menu(menu_ob, "法宝化灵", content, _menu_action("menu_default_action"));
    _activate_menu_session(me, menu_ob);
    _bind_default_menu(me);
    _bind_default_menu_on(menu_ob);
    _bind_menu(me, "hualing_select_main", "menu_action_select_main");
    _bind_menu(me, "hualing_select_sub", "menu_action_select_sub");
    _bind_menu(me, "hualing_split", "menu_action_split");
    _bind_menu(me, "hualing_cancel", "menu_action_cancel_merge");
    _bind_menu(me, "选择主法宝", "menu_action_select_main");
    _bind_menu(me, "选择副法宝", "menu_action_select_sub");
    _bind_menu(me, "分离化灵", "menu_action_split");
    _bind_menu(me, "化灵分离", "menu_action_split");
    _bind_menu(me, "取消化灵", "menu_action_cancel_merge");
    _bind_menu_on(menu_ob, "hualing_select_main", "menu_action_select_main");
    _bind_menu_on(menu_ob, "hualing_select_sub", "menu_action_select_sub");
    _bind_menu_on(menu_ob, "hualing_split", "menu_action_split");
    _bind_menu_on(menu_ob, "hualing_cancel", "menu_action_cancel_merge");
    _bind_menu_on(menu_ob, "选择主法宝", "menu_action_select_main");
    _bind_menu_on(menu_ob, "选择副法宝", "menu_action_select_sub");
    _bind_menu_on(menu_ob, "分离化灵", "menu_action_split");
    _bind_menu_on(menu_ob, "化灵分离", "menu_action_split");
    _bind_menu_on(menu_ob, "取消化灵", "menu_action_cancel_merge");
    _bind_hualing_main_menu(me, menu_ob);
}

private void _show_confirm_menu(object me)
{
    mixed main_artifact;
    mixed sub_artifact;
    mixed main_name;
    mixed sub_name;
    mixed sub_skill;
    mixed content;
    mixed menu_ob;

    if (!me) return;
    main_artifact = _find_item_by_iid(me, me->query_temp(MAIN_IID));
    sub_artifact = _find_item_by_iid(me, me->query_temp(SUB_IID));
    if (!main_artifact || !sub_artifact) {
        _msg(me, "主法宝或副法宝已经失效，请重新选择。");
        _show_menu(me);
        return;
    }
    main_name = "主法宝";
    sub_name = "副法宝";
    if (catch(main_name = main_artifact->get_name())) main_name = "主法宝";
    if (catch(sub_name = sub_artifact->get_name())) sub_name = "副法宝";
    sub_skill = _query_attach_skill(sub_artifact);
    if (!sub_skill || sub_skill == "") sub_skill = "未知被动";

    menu_ob = _menu_object(me);
    _prepare_menu_object(me);
    content = sprintf("[@确定#DLG:1#prompt:#Y【法宝化灵确认】#n#r主法宝：#G%s#n#r副法宝：#G%s#n#r#Y【化灵内容】#n#r将把副法宝被动#R%s#n融合到主法宝。#r#R【处理结果】#n#r副法宝会消失，主法宝获得该被动效果。#r#R是否确认化灵？#n]",
        main_name, sub_name, _skill_name(sub_skill));
    _bind_menu(me, "hualing_confirm", "menu_action_confirm_merge");
    _bind_menu(me, "确定", "menu_action_confirm_merge");
    _bind_menu(me, "hualing_cancel", "menu_action_cancel_merge");
    _bind_menu(me, "取消", "menu_action_cancel_merge");
    me->set_menu(menu_ob, "法宝化灵确认", content, _menu_action("menu_default_action"));
    _activate_menu_session(me, menu_ob);
    _bind_default_menu(me);
    _bind_default_menu_on(menu_ob);
    _bind_menu(me, "hualing_confirm", "menu_action_confirm_merge");
    _bind_menu(me, "确定", "menu_action_confirm_merge");
    _bind_menu(me, "hualing_cancel", "menu_action_cancel_merge");
    _bind_menu(me, "取消", "menu_action_cancel_merge");
    _bind_menu_on(menu_ob, "hualing_confirm", "menu_action_confirm_merge");
    _bind_menu_on(menu_ob, "确定", "menu_action_confirm_merge");
    _bind_menu_on(menu_ob, "hualing_cancel", "menu_action_cancel_merge");
    _bind_menu_on(menu_ob, "取消", "menu_action_cancel_merge");
}

private void _show_select_menu(object me, int main_flag)
{
    mixed inv;
    mixed item;
    mixed skill;
    mixed title;
    mixed content;
    mixed label;
    mixed command;
    mixed iid;
    mixed main_iid;
    mixed menu_ob;
    int i;
    int count;

    if (!me) return;
    menu_ob = _menu_object(me);
    _prepare_menu_object(me);

    if (main_flag) {
        item = _set_equipped_main(me);
        if (!item) _msg(me, "请先穿戴要保留的主法宝。");
        _show_menu(me);
        return;
    }

    _set_equipped_main(me);
    main_iid = 0;
    if (catch(main_iid = me->query_temp(MAIN_IID))) main_iid = 0;
    inv = _inventory(me);
    if (!inv) {
        _msg(me, "背包数据读取失败，请重新打开法宝化灵。");
        return;
    }

    title = main_flag ? "选择主法宝" : "选择副法宝";
    content = main_flag ? "#R选择主法宝#n" : "#R选择副法宝#n";
    count = 0;
    if (catch(me->set_temp(SELECT_MODE, main_flag ? "main" : "sub"))) {
    }
    _bind_menu(me, "hualing_back", "menu_action_back");

    for (i = 0; i < sizeof(inv); i++) {
        item = inv[i];
        if (!item || !_is_skill_artifact(item)) continue;
        if (_has_hualing(item)) continue;
        skill = item->query("attach_skill");
        if (!skill || skill == "") continue;
        iid = _safe_iid(item);
        if (!iid) continue;
        if (main_iid && iid == main_iid) continue;
        if (_equipped_skill_artifact_iid(me, iid)) continue;
        if (_select_slot_has_iid(me, count, iid)) continue;

        count++;
        if (count > MAX_SELECT_SLOT) break;
        label = item->get_name();
        command = sprintf("hualing_pick_%d", count);
        _set_select_slot(me, count, iid);
        _bind_menu(me, command, sprintf("menu_pick_slot_%d", count));
        _bind_menu(me, label, sprintf("menu_pick_slot_%d", count));
        content += sprintf("[%s/%s]", label, command);
    }

    content += "[返回化灵/hualing_back]";

    if (count <= 0) {
        _msg(me, "背包里没有可化灵的被动法宝。");
        _show_menu(me);
        return;
    }

    me->set_menu(menu_ob, title, content, _menu_action("menu_default_action"));
    _activate_menu_session(me, menu_ob);
    _bind_default_menu(me);
    _bind_default_menu_on(menu_ob);
    for (i = 1; i <= count; i++) {
        command = sprintf("hualing_pick_%d", i);
        _bind_menu(me, command, sprintf("menu_pick_slot_%d", i));
        _bind_menu_on(menu_ob, command, sprintf("menu_pick_slot_%d", i));
    }
    _bind_menu(me, "hualing_back", "menu_action_back");
    _bind_menu(me, "返回化灵", "menu_action_back");
    _bind_menu_on(menu_ob, "hualing_back", "menu_action_back");
    _bind_menu_on(menu_ob, "返回化灵", "menu_action_back");
    _bind_hualing_select_menu(me, menu_ob, title, count);
}

private void _restore_hualing_desc(mixed item)
{
    mixed extra;

    if (!item) return;
    extra = 0;
    if (catch(extra = item->query(HUALING_EXTRA_DESC_KEY))) extra = 0;
    if (extra && extra != "") {
        if (catch(item->set("extra_desc", extra))) {
        }
    } else {
        if (catch(item->set("extra_desc", 0))) {
        }
    }
    if (catch(item->set(HUALING_EXTRA_DESC_KEY, 0))) {
    }
    if (catch(item->set(HUALING_EXTRA_DESC_MARK, 0))) {
    }
}

private mixed _split_hualing(object me, mixed item)
{
    mixed name;
    mixed sub_name;
    mixed snapshot;
    int restored;

    if (!me || !item) return 0;
    if (!_has_hualing(item)) {
        _msg(me, "请选择已经化灵的主法宝。");
        return 1;
    }

    name = item->get_name();
    sub_name = item->query("extra_attach_from_name");
    if (!sub_name || sub_name == "") sub_name = "副法宝";
    snapshot = 0;
    if (catch(snapshot = item->query(HUALING_SUB_SAVE_TEXT))) snapshot = 0;
    restored = 0;
    if (snapshot && snapshot != "") {
        restored = _restore_sub_artifact_snapshot(me, item);
        if (!restored) {
            _msg(me, "副法宝快照还原失败，未执行分离，请联系管理员。");
            return 1;
        }
    }

    if (catch(item->set("extra_attach_skill", 0))) {
    }
    if (catch(item->set("extra_attach_skill_name", 0))) {
    }
    if (catch(item->set("extra_attach_from_name", 0))) {
    }
    if (catch(item->set("extra_attach_from_iid", 0))) {
    }
    if (catch(item->set("fabao_hualing_owner_iid", 0))) {
    }
    if (catch(item->set("fabao_hualing_v1", 0))) {
    }
    if (catch(item->set(HUALING_SUB_SAVE_TEXT, 0))) {
    }
    if (catch(item->set(HUALING_SUB_SAVE_NAME, 0))) {
    }
    _restore_hualing_desc(item);

    _refresh_artifact(me, item);
    if (restored) {
        _msg(me, sprintf("#R%s#n已分离化灵，副法宝#R%s#n已恢复到背包。", name, sub_name));
    } else {
        _msg(me, sprintf("#R%s#n已分离化灵，已清除来自#R%s#n的第二被动。旧版数据没有副法宝快照，无法恢复副法宝实物。", name, sub_name));
    }
    if (catch(me->destroy_menu())) {
    }
    return 1;
}

private void _show_split_menu(object me)
{
    mixed inv;
    mixed item;
    mixed title;
    mixed content;
    mixed label;
    mixed command;
    mixed iid;
    mixed menu_ob;
    int i;
    int count;

    if (!me) return;
    menu_ob = _menu_object(me);
    _prepare_menu_object(me);

    inv = _inventory(me);
    if (!inv) {
        _msg(me, "背包数据读取失败，请重新打开法宝化灵。");
        return;
    }

    title = "分离化灵";
    content = "#R分离化灵#n\n请选择要恢复的已化灵主法宝：";
    count = 0;
    if (catch(me->set_temp(SELECT_MODE, "split"))) {
    }
    _bind_menu(me, "hualing_back", "menu_action_back");

    item = _equipped_skill_artifact(me);
    if (item && _has_hualing(item)) {
        iid = _safe_iid(item);
        if (iid) {
            count++;
            label = item->get_name();
            command = sprintf("hualing_pick_%d", count);
            _set_select_slot(me, count, iid);
            _bind_menu(me, command, sprintf("menu_pick_slot_%d", count));
            _bind_menu(me, label, sprintf("menu_pick_slot_%d", count));
            content += sprintf("[%s/%s]", label, command);
        }
    }

    for (i = 0; i < sizeof(inv); i++) {
        item = inv[i];
        if (!item || !_is_skill_artifact(item)) continue;
        if (!_has_hualing(item)) continue;
        iid = _safe_iid(item);
        if (!iid) continue;
        if (_select_slot_has_iid(me, count, iid)) continue;

        count++;
        if (count > MAX_SELECT_SLOT) break;
        label = item->get_name();
        command = sprintf("hualing_pick_%d", count);
        _set_select_slot(me, count, iid);
        _bind_menu(me, command, sprintf("menu_pick_slot_%d", count));
        _bind_menu(me, label, sprintf("menu_pick_slot_%d", count));
        content += sprintf("[%s/%s]", label, command);
    }

    content += "[返回化灵/hualing_back]";

    if (count <= 0) {
        _msg(me, "背包里没有已经化灵的主法宝。");
        _show_menu(me);
        return;
    }

    me->set_menu(menu_ob, title, content, _menu_action("menu_default_action"));
    _activate_menu_session(me, menu_ob);
    _bind_default_menu(me);
    _bind_default_menu_on(menu_ob);
    _bind_hualing_select_menu(me, menu_ob, title, count);
}

private void _show_submit_menu(object me)
{
    mixed content;
    mixed action;

    if (!me) return;

    if (catch(me->set_temp(SUBMIT_STAGE, "main"))) {
    }
    content = "#R法宝化灵#n\n第一步：请提交要保留的主法宝。\n[提交主法宝/$提交|请提交要保留的主法宝,1,0][取消/离开]";
    _bind_menu(me, "$提交", "submit_items");
    _bind_menu(me, "$提交物品", "submit_items");
    _bind_menu(me, "提交法宝", "submit_items");
    action = _menu_action("submit_items");
    if (catch(me->set_temp("menu/$提交", action))) {
    }
    if (catch(me->set_temp("current_menu/$提交", action))) {
    }
    me->set_menu(me, "法宝化灵", content, _menu_action("menu_default_action"));
    _activate_menu_session(me, me);
    _bind_default_menu(me);
    _bind_menu(me, "$提交", "submit_items");
    _bind_menu(me, "$提交物品", "submit_items");
    _bind_menu(me, "提交法宝", "submit_items");
    if (catch(me->set_temp("menu/$提交", action))) {
    }
    if (catch(me->set_temp("current_menu/$提交", action))) {
    }
}

private void _show_submit_sub_menu(object me)
{
    mixed content;
    mixed action;
    mixed main_name;

    if (!me) return;
    if (catch(me->set_temp(SUBMIT_STAGE, "sub"))) {
    }
    main_name = _chosen_text(me, "main");
    content = sprintf("#R法宝化灵#n\n主法宝：#Y%s#n\n第二步：请提交要消耗的副法宝。副法宝会消失，被动转移到主法宝。\n[提交副法宝/$提交|请提交要消耗的副法宝,1,0][取消/离开]", main_name);
    _bind_menu(me, "$提交", "submit_items");
    _bind_menu(me, "$提交物品", "submit_items");
    _bind_menu(me, "提交副法宝", "submit_items");
    action = _menu_action("submit_items");
    if (catch(me->set_temp("menu/$提交", action))) {
    }
    if (catch(me->set_temp("current_menu/$提交", action))) {
    }
    me->set_menu(me, "法宝化灵", content, _menu_action("menu_default_action"));
    _activate_menu_session(me, me);
    _bind_default_menu(me);
    _bind_menu(me, "$提交", "submit_items");
    _bind_menu(me, "$提交物品", "submit_items");
    _bind_menu(me, "提交副法宝", "submit_items");
}

private mixed _merge_submitted_items(object me, mixed npc, mixed items, mixed tag, int reopen_menu)
{
    mixed main_artifact;
    mixed sub_artifact;
    mixed one_item;
    mixed submit_source;
    mixed stage;
    mixed iid;
    mixed ret;
    int submit_count;


    if (!me) return 0;

    stage = 0;
    if (catch(stage = me->query_temp(SUBMIT_STAGE))) stage = 0;
    submit_source = _pick_submit_source(me, items, npc, tag, 0, 0);
    submit_count = _submit_item_count(submit_source);
    if (submit_count > 1) {
        _return_submit_items(me, submit_source);
        _msg(me, "每次只能提交一个法宝，请按第一步主法宝、第二步副法宝分别提交。");
        if (stage == "sub") _show_submit_sub_menu(me);
        else _show_submit_menu(me);
        return 1;
    }

    one_item = _submit_item_at(submit_source, 0);
    if (!one_item) {
        if (reopen_menu) {
            _msg(me, "请提交一个法宝。");
            if (stage == "sub") _show_submit_sub_menu(me);
            else _show_submit_menu(me);
        }
        return 1;
    }

    if (stage != "sub") {
        if (!_is_skill_artifact(one_item) || !_has_attach_skill(one_item)) {
            _msg(me, "主法宝必须是带被动技能的法宝。");
            _show_submit_menu(me);
            return 1;
        }
        if (_has_hualing(one_item)) {
            _msg(me, sprintf("你的#R%s#n已经化灵过，不能继续化灵。", one_item->get_name()));
            _show_submit_menu(me);
            return 1;
        }
        iid = 0;
        if (catch(iid = one_item->get_iid())) iid = 0;
        if (!iid) {
            _msg(me, "主法宝读取失败，请重新提交。");
            _show_submit_menu(me);
            return 1;
        }
        if (catch(me->set_temp(MAIN_IID, iid))) {
        }
        if (catch(one_item->move_to_bag(me))) {
        }
        if (catch(one_item->refresh_item())) {
        }
        _msg(me, sprintf("已选择主法宝#R%s#n，请继续提交副法宝。", one_item->get_name()));
        _show_submit_sub_menu(me);
        return 1;
    }

    main_artifact = _find_item_by_iid(me, me->query_temp(MAIN_IID));
    sub_artifact = one_item;
    if (!main_artifact || !sub_artifact) {
        if (reopen_menu) {
            _msg(me, "主法宝已失效，请重新提交主法宝。");
            _show_submit_menu(me);
        }
        return 1;
    }

    ret = _check_pair(me, main_artifact, sub_artifact);
    if (!ret || ret[0] != 1) {
        if (ret && sizeof(ret) > 1) _msg(me, ret[1]);
        if (reopen_menu) _show_submit_menu(me);
        return 1;
    }

    return _finish_merge(me, ret[1], ret[2], ret[4]);
}

mixed open_merge_dialog(object me, object item)
{
    if (!me || !item) return 0;
    _msg(me, "法宝化灵请前往#R活动大使#n打开菜单。");
    return 0;
}

mixed use_enhance_item(object me, string unused, object item)
{
    return open_merge_dialog(me, item);
}

mixed submit_item(object me, mixed npc, mixed items, mixed para)
{
    return _merge_submitted_items(me, npc, items, "submit_item", 1);
}

mixed submit_items(object me, mixed npc, mixed items, mixed para)
{
    return _merge_submitted_items(me, npc, items, "submit_items", 1);
}

mixed submit_items_succ(object me, mixed npc, mixed items, mixed para, mixed arg4, mixed arg5)
{
    return _merge_submitted_items(me, npc, items, "submit_items_succ", 1);
}

mixed menu_select_main(object me, mixed npc, mixed para, mixed text)
{
    _show_select_menu(me, 1);
    return 1;
}

mixed menu_select_sub(object me, mixed npc, mixed para, mixed text)
{
    _show_select_menu(me, 0);
    return 1;
}

mixed menu_action_select_main(object me, mixed npc)
{
    _show_select_menu(me, 1);
    return 1;
}

mixed menu_action_select_sub(object me, mixed npc)
{
    _show_select_menu(me, 0);
    return 1;
}

mixed menu_action_split(object me, mixed npc)
{
    _show_split_menu(me);
    return 1;
}

mixed menu_pick_slot(object me, int slot)
{
    mixed iid;
    mixed mode;

    if (!me) return 0;
    iid = _query_select_slot(me, slot);
    if (!iid) {
        _msg(me, "选择的法宝已经失效，请重新打开法宝化灵。");
        _show_menu(me);
        return 1;
    }
    mode = 0;
    if (catch(mode = me->query_temp(SELECT_MODE))) mode = 0;
    if (mode == "split") {
        return _split_hualing(me, _find_item_by_iid(me, iid));
    }
    if (mode == "main") {
        me->set_temp(MAIN_IID, iid);
        _show_menu(me);
    } else {
        me->set_temp(SUB_IID, iid);
        _show_confirm_menu(me);
    }
    return 1;
}

mixed menu_pick_slot_1(object me, mixed npc) { return menu_pick_slot(me, 1); }
mixed menu_pick_slot_2(object me, mixed npc) { return menu_pick_slot(me, 2); }
mixed menu_pick_slot_3(object me, mixed npc) { return menu_pick_slot(me, 3); }
mixed menu_pick_slot_4(object me, mixed npc) { return menu_pick_slot(me, 4); }
mixed menu_pick_slot_5(object me, mixed npc) { return menu_pick_slot(me, 5); }
mixed menu_pick_slot_6(object me, mixed npc) { return menu_pick_slot(me, 6); }
mixed menu_pick_slot_7(object me, mixed npc) { return menu_pick_slot(me, 7); }
mixed menu_pick_slot_8(object me, mixed npc) { return menu_pick_slot(me, 8); }
mixed menu_pick_slot_9(object me, mixed npc) { return menu_pick_slot(me, 9); }
mixed menu_pick_slot_10(object me, mixed npc) { return menu_pick_slot(me, 10); }
mixed menu_pick_slot_11(object me, mixed npc) { return menu_pick_slot(me, 11); }
mixed menu_pick_slot_12(object me, mixed npc) { return menu_pick_slot(me, 12); }
mixed menu_pick_slot_13(object me, mixed npc) { return menu_pick_slot(me, 13); }
mixed menu_pick_slot_14(object me, mixed npc) { return menu_pick_slot(me, 14); }
mixed menu_pick_slot_15(object me, mixed npc) { return menu_pick_slot(me, 15); }
mixed menu_pick_slot_16(object me, mixed npc) { return menu_pick_slot(me, 16); }
mixed menu_pick_slot_17(object me, mixed npc) { return menu_pick_slot(me, 17); }
mixed menu_pick_slot_18(object me, mixed npc) { return menu_pick_slot(me, 18); }
mixed menu_pick_slot_19(object me, mixed npc) { return menu_pick_slot(me, 19); }
mixed menu_pick_slot_20(object me, mixed npc) { return menu_pick_slot(me, 20); }
mixed menu_pick_slot_21(object me, mixed npc) { return menu_pick_slot(me, 21); }
mixed menu_pick_slot_22(object me, mixed npc) { return menu_pick_slot(me, 22); }
mixed menu_pick_slot_23(object me, mixed npc) { return menu_pick_slot(me, 23); }
mixed menu_pick_slot_24(object me, mixed npc) { return menu_pick_slot(me, 24); }
mixed menu_pick_slot_25(object me, mixed npc) { return menu_pick_slot(me, 25); }
mixed menu_pick_slot_26(object me, mixed npc) { return menu_pick_slot(me, 26); }
mixed menu_pick_slot_27(object me, mixed npc) { return menu_pick_slot(me, 27); }
mixed menu_pick_slot_28(object me, mixed npc) { return menu_pick_slot(me, 28); }
mixed menu_pick_slot_29(object me, mixed npc) { return menu_pick_slot(me, 29); }
mixed menu_pick_slot_30(object me, mixed npc) { return menu_pick_slot(me, 30); }

mixed menu_back(object me, mixed npc, mixed para, mixed text)
{
    _show_menu(me);
    return 1;
}

mixed menu_action_back(object me, mixed npc)
{
    _show_menu(me);
    return 1;
}

mixed menu_cancel_merge(object me, mixed npc, mixed para, mixed text)
{
    refresh_merge_token(me);
    clear_merge_token(me);
    if (me) {
        if (catch(me->destroy_menu())) {
        }
    }
    return 1;
}

mixed menu_action_cancel_merge(object me, mixed npc)
{
    return menu_cancel_merge(me, npc, 0, 0);
}

mixed menu_confirm_merge(object me, mixed npc, mixed para, mixed text)
{
    mixed ret;
    mixed main_artifact;
    mixed sub_artifact;
    mixed sub_skill;
    mixed msg;

    if (!me) return 0;

    ret = _check_items(me);
    if (!ret || ret[0] != 1) {
        if (ret && sizeof(ret) > 1) _msg(me, ret[1]);
        _show_menu(me);
        return 1;
    }

    main_artifact = ret[1];
    sub_artifact = ret[2];
    sub_skill = ret[4];

    return _finish_merge(me, main_artifact, sub_artifact, sub_skill);
}

mixed menu_action_confirm_merge(object me, mixed npc)
{
    return menu_confirm_merge(me, npc, 0, 0);
}

mixed open_npc_menu(object me, mixed npc)
{
    if (!me) return 0;
    if (catch(me->delete_temp(TOKEN_ITEM))) {
    }
    if (catch(me->delete_temp(TOKEN_IID))) {
    }
    if (catch(me->delete_temp(TOKEN_MARK))) {
    }
    if (catch(me->delete_temp(MAIN_IID))) {
    }
    if (catch(me->delete_temp(SUB_IID))) {
    }
    _show_menu(me);
    return 1;
}

mixed init_fabao_hualing_content(mixed content, mixed me, mixed npc)
{
    if (!content) content = "";
    content += "[" + NPC_MENU_NAME + "/" + NPC_MENU_KEY + "]";
    return 0;
}

mixed init_fabao_hualing_action(object me, mixed npc)
{
    if (!me) return 0;
    _bind_menu(me, NPC_MENU_KEY, "open_npc_menu");
    _bind_menu(me, NPC_MENU_NAME, "open_npc_menu");
    if (catch(me->set_temp("menu/" + NPC_MENU_NAME, _menu_action("open_npc_menu")))) {
    }
    return 1;
}

mixed init_menu_content(mixed content, mixed me, mixed npc)
{
    return init_fabao_hualing_content(content, me, npc);
}

mixed init_menu_action(object me)
{
    return init_fabao_hualing_action(me, 0);
}

void init_npc_menu()
{
    object npc;

    npc = 0;
    if (catch(npc = NAMED_D->npc_ob(NPC_NAME))) npc = 0;
    if (!npc) {
        if (catch(npc = NAMED_D->npc_ob(NPC_NAME, "tianyongcheng/huodong-dashi"))) npc = 0;
    }
    if (!npc) return;
    catch(npc->set_dynamic_menu(this_object(), NPC_MENU_NAME));
    catch(npc->set_dynamic_menu("init_fabao_hualing_action", "init_fabao_hualing_content", this_object(), NPC_MENU_NAME));
}

void do_init()
{
    return;
}

void create()
{
    return;
}

mixed menu_default_action(object me, mixed npc, mixed para, mixed text)
{
    mixed key;
    mixed item;
    mixed label;
    int i;

    key = para;
    if (!key || key == "") key = text;

    if (key == "hualing_select_main" || key == "选择主法宝") {
        return menu_select_main(me, npc, para, text);
    }
    if (key == "hualing_select_sub" || key == "选择副法宝") {
        return menu_select_sub(me, npc, para, text);
    }
    if (key == "hualing_confirm" || key == "确认化灵") {
        return menu_confirm_merge(me, npc, para, text);
    }
    if (key == "hualing_split" || key == "分离化灵") {
        _show_split_menu(me);
        return 1;
    }
    if (key == "hualing_cancel" || key == "取消化灵" || key == "离开") {
        return menu_cancel_merge(me, npc, para, text);
    }
    if (key == "hualing_back" || key == "返回化灵") {
        return menu_back(me, npc, para, text);
    }

    for (i = 1; i <= MAX_SELECT_SLOT; i++) {
        if (key == sprintf("hualing_pick_%d", i)) {
            return menu_pick_slot(me, i);
        }
        item = _find_item_by_iid(me, _query_select_slot(me, i));
        if (item) {
            label = 0;
            if (catch(label = item->get_name())) label = 0;
            if (label && key == label) {
                return menu_pick_slot(me, i);
            }
        }
    }

    return 1;
}
