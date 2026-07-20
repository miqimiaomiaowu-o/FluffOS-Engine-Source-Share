// /gs/clone/item/special/chongwu_jichengdan.c
inherit "/gs/inherit/item/special_item.c";

public int apply_item(object me, array obs) { return 0; }
public int cant_apply_in_combat(object me) { return 1; }

mixed get_user(mixed me, mixed npc) {
    if (is_object(me) && me->is_player()) return me;
    if (is_object(npc) && npc->is_player()) return npc;
    return env_object();
}

mixed find_pet(mixed me, mixed pet_no) {
    mixed pets;

    if (!is_object(me)) return 0;
    pets = me->query_all_pets();
    if (!is_mapping(pets)) return 0;
    if (is_object(pets[pet_no])) return pets[pet_no];
    return 0;
}

mixed bind_menu(mixed me) {
    if (!is_object(me)) return 0;
    me->set_temp("menu/src1", (: call_other, this_object(), "src1" :)); me->set_temp("current_menu/src1", (: call_other, this_object(), "src1" :));
    me->set_temp("menu/src2", (: call_other, this_object(), "src2" :)); me->set_temp("current_menu/src2", (: call_other, this_object(), "src2" :));
    me->set_temp("menu/src3", (: call_other, this_object(), "src3" :)); me->set_temp("current_menu/src3", (: call_other, this_object(), "src3" :));
    me->set_temp("menu/src4", (: call_other, this_object(), "src4" :)); me->set_temp("current_menu/src4", (: call_other, this_object(), "src4" :));
    me->set_temp("menu/src5", (: call_other, this_object(), "src5" :)); me->set_temp("current_menu/src5", (: call_other, this_object(), "src5" :));
    me->set_temp("menu/src6", (: call_other, this_object(), "src6" :)); me->set_temp("current_menu/src6", (: call_other, this_object(), "src6" :));
    me->set_temp("menu/src7", (: call_other, this_object(), "src7" :)); me->set_temp("current_menu/src7", (: call_other, this_object(), "src7" :));
    me->set_temp("menu/src8", (: call_other, this_object(), "src8" :)); me->set_temp("current_menu/src8", (: call_other, this_object(), "src8" :));
    me->set_temp("menu/dst1", (: call_other, this_object(), "dst1" :)); me->set_temp("current_menu/dst1", (: call_other, this_object(), "dst1" :));
    me->set_temp("menu/dst2", (: call_other, this_object(), "dst2" :)); me->set_temp("current_menu/dst2", (: call_other, this_object(), "dst2" :));
    me->set_temp("menu/dst3", (: call_other, this_object(), "dst3" :)); me->set_temp("current_menu/dst3", (: call_other, this_object(), "dst3" :));
    me->set_temp("menu/dst4", (: call_other, this_object(), "dst4" :)); me->set_temp("current_menu/dst4", (: call_other, this_object(), "dst4" :));
    me->set_temp("menu/dst5", (: call_other, this_object(), "dst5" :)); me->set_temp("current_menu/dst5", (: call_other, this_object(), "dst5" :));
    me->set_temp("menu/dst6", (: call_other, this_object(), "dst6" :)); me->set_temp("current_menu/dst6", (: call_other, this_object(), "dst6" :));
    me->set_temp("menu/dst7", (: call_other, this_object(), "dst7" :)); me->set_temp("current_menu/dst7", (: call_other, this_object(), "dst7" :));
    me->set_temp("menu/dst8", (: call_other, this_object(), "dst8" :)); me->set_temp("current_menu/dst8", (: call_other, this_object(), "dst8" :));
    me->set_temp("menu_renew", 1);
    return 1;
}

mixed open_menu(mixed me, mixed menu, mixed content, mixed title) {
    me->set_temp("start_talk_to", me);
    me->set_temp("current_npc", me);
    me->set_temp("click_npc", me);
    me->set_temp("not_notify_team_members", 1);
    me->set_temp("show_name", title);
    me->set_menu(me, menu, content, 0, 0, 4);
    me->delete_temp("show_name");
    me->delete_temp("not_notify_team_members");
    bind_menu(me);
    return 0;
}

mixed get_pet_no(mixed me, mixed pet) {
    mixed pets, pet_nos, pet_no, i;

    if (!is_object(me) || !is_object(pet)) return 0;
    pets = me->query_all_pets();
    if (!is_mapping(pets)) return 0;
    pet_nos = keys(pets);
    for (i = 0; i < sizeof(pet_nos); i++) {
        pet_no = pet_nos[i];
        if (pets[pet_no] == pet) return pet_no;
    }
    return 0;
}

mixed move_pet_value(mixed source, mixed target, mixed path) {
    mixed value;

    value = source->query(path);
    if (value) target->set(path, value);
    else target->delete(path);
    source->delete(path);
    return 1;
}

mixed clear_pet_points(mixed pet) {
    if (!is_object(pet)) return 0;
    pet->delete("attrib_already");
    pet->delete("resist_already");
    pet->set("str", 0);
    pet->set("con", 0);
    pet->set("wiz", 0);
    pet->set("dex", 0);
    pet->set("resist_point", 0);
    return 1;
}

mixed refresh_inherit_pet(mixed me, mixed pet) {
    mixed pet_no, update_data, show_type;

    if (!is_object(me) || !is_object(pet)) return 0;
    pet_no = get_pet_no(me, pet);
    if (!pet_no) return 0;
    "/gs/daemons/formulad.c"->calculate(pet, 55);
    "/gs/daemons/formulad.c"->calculate(pet);
    "/gs/daemons/petd.c"->update_pet_ex(pet);
    pet->set("life", pet->query_attrib("max_life"));
    pet->set("mana", pet->query_attrib("max_mana"));
    show_type = "/gs/daemons/petd.c"->get_show_type(pet);
    update_data = pet->query_entire_dbase() + ([]);
    update_data["show_type"] = show_type;
    me->send_update_pet(pet_no, pet->get_id(), update_data);
    return 1;
}

mixed refresh_inherit_pet_skills(mixed source, mixed target) {
    if (!is_object(source) || !is_object(target)) return 0;
    "/gs/daemons/petd.c"->unset_pet_skills(source);
    "/gs/daemons/petd.c"->pet_learn_skill(target);
    "/gs/daemons/petd.c"->update_pet_skills(target);
    return 1;
}

mixed move_pet_growth(mixed source, mixed target, mixed group) {
    mixed effects, effect, value, i;

    effects = "/gs/daemons/petd.c"->get_shape_list();
    if (!effects) effects = ({ "life_effect", "mana_effect", "speed_effect", "phy_effect", "mag_effect" });
    for (i = 0; i < sizeof(effects); i++) {
        effect = effects[i];
        value = source->get_pet_effect(group, effect);
        if (!value) value = 0;
        target->set_pet_effect(group, effect, (int)value);
        source->set_pet_effect(group, effect, 0);
        source->lite_delete(group + "/" + effect);
        source->lite_delete(effect + "/" + group);
    }
    return 1;
}
mixed clear_eclosion_appearance(mixed pet) {
    if (!is_object(pet)) return 0;
    pet->delete_temp("temp_show_icon");
    pet->delete_temp("temp_combat_show_icon");
    pet->delete("temp_show_icon");
    pet->delete("temp_combat_show_icon");
    pet->delete("portrait");
    pet->delete("icon");
    pet->delete("eclosion_rank");
    pet->delete("eclosion_name");
    pet->set("imp_item", 0);
    return 1;
}
mixed clear_pet_growth_group(mixed pet, mixed group) {
    mixed effects, effect, i;

    effects = "/gs/daemons/petd.c"->get_shape_list();
    if (!effects) effects = ({ "life_effect", "mana_effect", "speed_effect", "phy_effect", "mag_effect" });
    for (i = 0; i < sizeof(effects); i++) {
        effect = effects[i];
        pet->set_pet_effect(group, effect, 0);
        pet->lite_delete(group + "/" + effect);
        pet->lite_delete(effect + "/" + group);
    }
    return 1;
}

mixed second_clear_source_pet(mixed me, mixed pet) {
    if (!is_object(me) || !is_object(pet)) return 0;
    pet->set_enchant_state(0);
    pet->set_eclosion_state(0);
    if ((int)pet->query("rank") == 9) pet->set("rank", 2);
    pet->delete("eclosion_rank");
    pet->delete("eclosion_name");
    clear_eclosion_appearance(pet);
    pet->unset_has_upgraded();
    pet->lite_delete("has_upgraded");
    pet->lite_delete("enchant_nimbus");
    pet->lite_delete("max_enchant_nimbus");
    pet->lite_delete("eclosion_stage");
    pet->lite_delete("eclosion_nimbus");
    pet->lite_delete("max_eclosion_nimbus");
    clear_pet_growth_group(pet, "enchant_effect");
    clear_pet_growth_group(pet, "eclosion_effect");
    return refresh_inherit_pet(me, pet);
}
mixed apply_completed_growth(mixed target) {
    mixed effects, effect, i;

    if (!is_object(target)) return 0;
    clear_pet_growth_group(target, "enchant_effect");
    clear_pet_growth_group(target, "eclosion_effect");
    effects = "/gs/daemons/petd.c"->get_shape_list();
    if (!effects) effects = ({ "life_effect", "mana_effect", "speed_effect", "phy_effect", "mag_effect" });
    for (i = 0; i < sizeof(effects); i++) {
        effect = effects[i];
        target->set_pet_effect("enchant_effect", effect, 30);
    }
    "/gs/daemons/pet_eclosiond.c"->calc_eclosion_effect(target, 1.0f);
    return 1;
}
mixed do_pet_inherit(mixed me, mixed source, mixed target) {
    mixed fields, field, i, eclosion_state, enchant_state, upgraded;
    mixed level, exp, martial, intimacy, origin_intimacy;
    mixed enchant_nimbus, eclosion_stage, eclosion_nimbus;

    if (!is_object(me) || !is_object(source) || !is_object(target)) return 0;
    if (source == target) return 0;

    level = source->query("level");
    exp = source->query("exp");
    martial = source->query("martial");
    intimacy = source->query("intimacy");
    origin_intimacy = source->query("origin_intimacy");
    enchant_state = source->get_enchant_state();
    eclosion_state = source->get_eclosion_state();
    upgraded = source->is_upgraded();
    enchant_nimbus = source->query("enchant_nimbus");
    eclosion_stage = source->query("eclosion_stage");
    eclosion_nimbus = source->query("eclosion_nimbus");

    target->set("level", level);
    target->set("exp", exp);
    target->set("martial", martial);
    target->set("intimacy", intimacy);
    target->set("origin_intimacy", origin_intimacy);
    clear_pet_points(target);

    fields = ({
        "upgrade",
        "medicine/attrib_point",
        "medicine_used/jiuzhuan-xianlinglu",
        "morph_life_stat", "morph_life_times",
        "morph_mana_stat", "morph_mana_times",
        "morph_speed_stat", "morph_speed_times",
        "morph_phy_stat", "morph_phy_times",
        "morph_mag_stat", "morph_mag_times"
    });
    for (i = 0; i < sizeof(fields); i++) {
        field = fields[i];
        move_pet_value(source, target, field);
    }
    if (upgraded) target->set_has_upgraded();
    else target->unset_has_upgraded();
    source->unset_has_upgraded();
    source->delete("has_upgraded");

    source->set("level", 1);
    source->set("exp", 0);
    source->set("martial", 0);
    source->set("intimacy", 0);
    source->set("origin_intimacy", 0);
    clear_pet_points(source);

    refresh_inherit_pet(me, source);
    refresh_inherit_pet(me, target);

    apply_completed_growth(target);
    clear_pet_growth_group(source, "enchant_effect");
    clear_pet_growth_group(source, "eclosion_effect");
    target->set_enchant_state(enchant_state);
    target->set_eclosion_state(eclosion_state);
    if ((int)target->query("rank") <= 2) target->set("rank", 9);
    if (enchant_state == 1) target->lite_set("enchant_nimbus", (int)enchant_nimbus);
    else target->lite_delete("enchant_nimbus");
    if (eclosion_state == 1) {
        target->lite_set("eclosion_stage", (int)eclosion_stage);
        target->lite_set("eclosion_nimbus", (int)eclosion_nimbus);
    } else {
        target->lite_delete("eclosion_stage");
        target->lite_delete("eclosion_nimbus");
    }
    source->set_enchant_state(0);
    source->set_eclosion_state(0);
    if ((int)source->query("rank") == 9) source->set("rank", 2);
    source->delete("eclosion_rank");
    source->delete("eclosion_name");
    clear_eclosion_appearance(source);
    source->lite_delete("enchant_nimbus");
    source->lite_delete("max_enchant_nimbus");
    source->lite_delete("eclosion_stage");
    source->lite_delete("eclosion_nimbus");
    source->lite_delete("max_eclosion_nimbus");
    refresh_inherit_pet_skills(source, target);
    "/gs/daemons/formulad.c"->calculate(0, target, 86);

    "/gs/daemons/petd.c"->update_attrib_point(source);
    "/gs/daemons/petd.c"->update_attrib_point(target);
    refresh_inherit_pet(me, source);
    second_clear_source_pet(me, source);
    refresh_inherit_pet(me, target);
    me->refresh_current_pets();
    me->send("pet_inherit", 1, 41191);
    return 1;
}
mixed clear_inherit_confirm(mixed me) {
    if (!is_object(me)) return 0;
    me->delete_temp("chongwu_jichengdan/ready");
    me->delete_temp("chongwu_jichengdan/source_no");
    me->delete_temp("chongwu_jichengdan/target_no");
    me->delete_temp("chongwu_jichengdan/source_id");
    me->delete_temp("chongwu_jichengdan/target_id");
    me->delete_temp("chongwu_jichengdan/item");
    return 1;
}

public int consume_confirm_item(object me) {
    if (!is_object(me) || env_object() != me) return 0;
    if ((int)get_amount() <= 0) return 0;
    cost_amount(1);
    return 1;
}

public int confirm_inherit(object me, mixed type, mixed para) {
    mixed source, target, source_no, target_no, source_id, target_id, pending_item;

    if (!is_object(me)) me = env_object();
    if (!is_object(me)) return 0;
    if ((int)me->query_temp("chongwu_jichengdan/ready") != 1) {
        me->send_dialog_ok("本次宠物继承已经处理，不能重复操作。");
        return 0;
    }
    source_no = me->query_temp("chongwu_jichengdan/source_no");
    target_no = me->query_temp("chongwu_jichengdan/target_no");
    source_id = me->query_temp("chongwu_jichengdan/source_id");
    target_id = me->query_temp("chongwu_jichengdan/target_id");
    pending_item = me->query_temp("chongwu_jichengdan/item");
    clear_inherit_confirm(me);
    if ((int)type != 1) {
        me->send_dialog_ok("你取消了本次宠物继承，道具未消耗。");
        return 0;
    }
    source = find_pet(me, source_no);
    target = find_pet(me, target_no);
    if (!is_object(source) || !is_object(target)) {
        me->send_dialog_ok("宠物列表已经改变，道具未消耗，继承未生效。");
        return 0;
    }
    if (source->get_id() != source_id || target->get_id() != target_id || source == target) {
        me->send_dialog_ok("宠物资料已经改变，道具未消耗，继承未生效。");
        return 0;
    }
    if (source->get_enchant_state() != 2 || source->get_eclosion_state() != 2) {
        me->send_dialog_ok("继承宠状态已经改变，道具未消耗，继承未生效。");
        return 0;
    }
    if (!is_object(pending_item) || env_object(pending_item) != me) {
        me->send_dialog_ok("你的包裹中缺少本次使用的宠物继承卡，继承未生效。");
        return 0;
    }
    if (!pending_item->consume_confirm_item(me)) {
        me->send_dialog_ok("宠物继承卡扣除失败，继承未生效。");
        return 0;
    }
    if (!do_pet_inherit(me, source, target)) {
        me->send_dialog_ok("宠物继承失败，道具已经消耗。");
        return 1;
    }
    delete("inherit/source_no");
    delete("inherit/target_no");
    delete("inherit/source_slots");
    delete("inherit/target_slots");
    me->send_dialog_ok(sprintf("继承成功：#Y%s#n获得了#Y%s#n的等级、点化、羽化、幻化、飞升、九转仙灵露、武学和亲密度；继承宠已清空。", target->get_name(), source->get_name()));
    return 1;
}

public mixed get_confirm_callback() {
    return (: confirm_inherit :);
}
mixed choose_target(mixed me, mixed npc, mixed index) {
    mixed user, slots, source, target, pet_no;

    user = get_user(me, npc);
    if (!is_object(user)) return 0;
    slots = query("inherit/target_slots");
    if (!slots || index >= sizeof(slots)) return 0;
    pet_no = slots[index];
    source = find_pet(user, query("inherit/source_no"));
    target = find_pet(user, pet_no);
    if (!is_object(source) || !is_object(target) || source == target) return 0;
    set("inherit/target_no", pet_no);
    clear_inherit_confirm(user);
    user->set_temp("chongwu_jichengdan/ready", 1);
    user->set_temp("chongwu_jichengdan/source_no", get_pet_no(user, source));
    user->set_temp("chongwu_jichengdan/target_no", get_pet_no(user, target));
    user->set_temp("chongwu_jichengdan/source_id", source->get_id());
    user->set_temp("chongwu_jichengdan/target_id", target->get_id());
    user->set_temp("chongwu_jichengdan/item", this_object());
    user->destroy_menu();
    user->send_confirm_dlg(2, 1, sprintf("是否把#Y%s#n的培养继承给#Y%s#n？#r可继承：#R等级、武学、亲密、点化、羽化、飞升、幻化、九转。#r#n继承宠保留但培养清空。确认时会检查宠物继承卡仍在包裹中。", source->get_name(), target->get_name()), "/gs/clone/item/special/chongwu_jichengdan.c"->get_confirm_callback(), 0, 0, 0);
    return 1;
}

mixed show_target_menu(mixed me, mixed npc) {
    mixed user, pets, pet_nos, slots, menu, content, pet, pet_no, key, i, source_no;

    user = get_user(me, npc);
    if (!is_object(user)) return 0;
    source_no = query("inherit/source_no");
    pets = user->query_all_pets();
    if (!is_mapping(pets)) return 0;
    pet_nos = keys(pets); slots = ({}); menu = ([]);
    content = "请选择主宠物：";
    for (i = 0; i < sizeof(pet_nos) && sizeof(slots) < 8; i++) {
        pet_no = pet_nos[i];
        if (pet_no == source_no) continue;
        pet = pets[pet_no];
        if (!is_object(pet)) continue;
        slots += ({ pet_no }); key = sprintf("dst%d", sizeof(slots));
        menu[key] = user->query_temp("menu/" + key);
        content += sprintf("[#Y%s#n/%s]", pet->get_name(), key);
    }
    if (!sizeof(slots)) { user->send_dialog_ok("没有其他宠物可作为目标。"); return 0; }
    set("inherit/target_slots", slots);
    bind_menu(user);
    return open_menu(user, menu, content + "[离开]", "选择目标宠物");
}

mixed choose_source(mixed me, mixed npc, mixed index) {
    mixed user, slots, pet_no, pet;

    user = get_user(me, npc);
    if (!is_object(user)) return 0;
    slots = query("inherit/source_slots");
    if (!slots || index >= sizeof(slots)) return 0;
    pet_no = slots[index]; pet = find_pet(user, pet_no);
    if (!is_object(pet)) return 0;
    if (pet->get_enchant_state() != 2 || pet->get_eclosion_state() != 2) {
        user->send_dialog_ok("继承宠物必须已经完成点化和全部羽化，当前宠物不能使用继承。");
        return 0;
    }
    set("inherit/source_no", pet_no);
    return show_target_menu(user, user);
}

mixed src1(mixed a, mixed b) { return choose_source(a,b,0); }
mixed src2(mixed a, mixed b) { return choose_source(a,b,1); }
mixed src3(mixed a, mixed b) { return choose_source(a,b,2); }
mixed src4(mixed a, mixed b) { return choose_source(a,b,3); }
mixed src5(mixed a, mixed b) { return choose_source(a,b,4); }
mixed src6(mixed a, mixed b) { return choose_source(a,b,5); }
mixed src7(mixed a, mixed b) { return choose_source(a,b,6); }
mixed src8(mixed a, mixed b) { return choose_source(a,b,7); }
mixed dst1(mixed a, mixed b) { return choose_target(a,b,0); }
mixed dst2(mixed a, mixed b) { return choose_target(a,b,1); }
mixed dst3(mixed a, mixed b) { return choose_target(a,b,2); }
mixed dst4(mixed a, mixed b) { return choose_target(a,b,3); }
mixed dst5(mixed a, mixed b) { return choose_target(a,b,4); }
mixed dst6(mixed a, mixed b) { return choose_target(a,b,5); }
mixed dst7(mixed a, mixed b) { return choose_target(a,b,6); }
mixed dst8(mixed a, mixed b) { return choose_target(a,b,7); }

override public int apply() {
    object me;
    mixed pets, pet_nos, slots, menu, content, pet, pet_no, key, i;

    me = env_object();
    if (!is_object(me)) return 1;
    pets = me->query_all_pets();
    if (!is_mapping(pets) || !sizeof(pets)) { me->send_dialog_ok("当前没有宠物。"); return 0; }
    bind_menu(me);
    pet_nos = keys(pets); slots = ({}); menu = ([]); content = "请选择继承宠物：";
    for (i = 0; i < sizeof(pet_nos) && sizeof(slots) < 8; i++) {
        pet_no = pet_nos[i]; pet = pets[pet_no];
        if (!is_object(pet)) continue;
        slots += ({ pet_no }); key = sprintf("src%d", sizeof(slots));
        menu[key] = me->query_temp("menu/" + key);
        content += sprintf("[#Y%s#n/%s]", pet->get_name(), key);
    }
    set("inherit/source_slots", slots);
    return open_menu(me, menu, content + "[离开]", "选择继承宠物");
}

public int try_to_cost_amount() { return 1; }
private void create() {
    if (!is_clone()) { set_name("冥河之水"); import(get_name()); set("attrib",270); set("amulet",1); }
    setup_special_item();
}
