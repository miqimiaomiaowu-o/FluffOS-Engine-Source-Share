///gs/clone/item/hell/bailianjing.c

inherit "/gs/inherit/item/special_item.c";

array equip_type_arr = ({ 4, 5, 6, 8, 9 });

// 参考attrib_show_name.list，统一prop2/前缀
array attr_candidates = ({
    "prop2/str",
    "prop2/con",
    "prop2/wiz",
    "prop2/dex",
    "prop2/resist_metal",
    "prop2/resist_wood",
    "prop2/resist_water",
    "prop2/resist_fire",
    "prop2/resist_earth",
    "prop2/resist_poison",
    "prop2/resist_frozen",
    "prop2/resist_sleep",
    "prop2/resist_forgotten",
    "prop2/resist_confusion",
    "prop2/all_polar",
    "prop2/all_resist_polar",
    "prop2/all_resist_except",
    "prop2/all_skill",
    "prop2/ignore_all_resist_polar",
    "prop2/ignore_all_resist_except",
    "prop2/ignore_resist_forgotten",
    "prop2/ignore_resist_poison",
    "prop2/ignore_resist_frozen",
    "prop2/ignore_resist_sleep",
    "prop2/ignore_resist_confusion",
    "prop2/ignore_resist_metal",
    "prop2/ignore_resist_wood",
    "prop2/ignore_resist_water",
    "prop2/ignore_resist_fire",
    "prop2/ignore_resist_earth",
    "prop2/resist_repress",
    "prop2/resist_melt",
    "prop2/resist_cage",
    "prop2/resist_lock",
    "prop2/resist_lost",
    "prop2/ignore_resist_repress",
    "prop2/ignore_resist_melt",
    "prop2/ignore_resist_cage",
    "prop2/ignore_resist_lock",
    "prop2/ignore_resist_lost",
    });

// 每条属性的强化上限
mapping attr_max = ([
    "prop2/str":26,
    "prop2/con" : 26,
    "prop2/wiz" : 26,
    "prop2/dex" : 26,
    "prop2/resist_metal" : 15,
    "prop2/resist_wood" : 15,
    "prop2/resist_water" : 15,
    "prop2/resist_fire" : 15,
    "prop2/resist_earth" : 15,
    "prop2/resist_poison" : 20,
    "prop2/resist_frozen" : 20,
    "prop2/resist_sleep" : 20,
    "prop2/resist_forgotten" : 20,
    "prop2/resist_confusion" : 20,
    "prop2/all_polar" : 5,
    "prop2/all_resist_polar" : 15,
    "prop2/all_resist_except" : 15,
    "prop2/all_skill" : 10,
    "prop2/ignore_all_resist_polar" : 20,
    "prop2/ignore_all_resist_except" : 20,
    "prop2/ignore_resist_forgotten" : 30,
    "prop2/ignore_resist_poison" : 30,
    "prop2/ignore_resist_frozen" : 30,
    "prop2/ignore_resist_sleep" : 30,
    "prop2/ignore_resist_confusion" : 30,
    "prop2/ignore_resist_metal" : 30,
    "prop2/ignore_resist_wood" : 30,
    "prop2/ignore_resist_water" : 30,
    "prop2/ignore_resist_fire" : 30,
    "prop2/ignore_resist_earth" : 30,
    "prop2/resist_repress" : 20,
    "prop2/resist_melt" : 20,
    "prop2/resist_cage" : 20,
    "prop2/resist_lock" : 20,
    "prop2/resist_lost" : 20,
    "prop2/ignore_resist_repress" : 30,
    "prop2/ignore_resist_melt" : 30,
    "prop2/ignore_resist_cage" : 30,
    "prop2/ignore_resist_lock" : 30,
    "prop2/ignore_resist_lost" : 30,
]);
private int get_bailian_equip_level(object item) {
    int level;

    level = (int)item->query("min_level");
    if (!level) {
        level = (int)item->query("level");
    }
    if (!level) {
        level = (int)item->query("req_level");
    }
    if (!level && member_array(item->get_name(), ({ "金碧莲花", "通灵宝玉", "天星奇光" })) >= 0) {
        level = 100;
    }
    if (!level && member_array(item->get_name(), ({ "流光绝影", "寒玉龙勾", "碎梦涵光" })) >= 0) {
        level = 110;
    }
    if (!level && member_array(item->get_name(), ({ "五蕴悯光", "八宝如意", "九天霜华" })) >= 0) {
        level = 120;
    }
    if (!level && member_array(item->get_name(), ({ "千彩流光", "游火灵焰", "岚金火链" })) >= 0) {
        level = 130;
    }
    if (!level && member_array(item->get_name(), ({ "掠虹宝坠", "炫元玲珑", "龙御七星" })) >= 0) {
        level = 140;
    }
    if (!level && member_array(item->get_name(), ({ "破军困灵", "七杀固元", "贪狼破日" })) >= 0) {
        level = 150;
    }
    if (!level && member_array(item->get_name(), ({ "洛神回血", "菩提镜明", "屠龙封魔" })) >= 0) {
        level = 160;
    }
    if (!level && member_array(item->get_name(), ({ "景云烛日", "和光同尘", "九霞朝真" })) >= 0) {
        level = 170;
    }
    if (!level && member_array(item->get_name(), ({ "玄晖吐兰", "上善若水", "负阴抱阳" })) >= 0) {
        level = 180;
    }
    return level;
}

private int get_bailian_attr_max(object item, string prop_key) {
    int level;

    if (prop_key == "prop2/str" || prop_key == "prop2/con" || prop_key == "prop2/wiz" || prop_key == "prop2/dex") {
        level = get_bailian_equip_level(item);
        if (level == 100) {
            return 25;
        }
        if (level == 110) {
            return 27;
        }
        if (level == 120) {
            return 29;
        }
        if (level == 130) {
            return 32;
        }
        if (level == 140) {
            return 34;
        }
        if (level == 150) {
            return 36;
        }
        if (level == 160) {
            return 39;
        }
        if (level == 170) {
            return 41;
        }
        if (level == 180) {
            return 44;
        }
    }
    return attr_max[prop_key];
}
//战斗中使用
public int apply_item(object me, array obs) {
    return 0;

}

//战斗中使用
public int cant_apply_in_combat(object me) {
    return 1;
}

//用于物品
override public int apply() {

    object me;
    me = env_object();
    if ((!is_object(me))) {
        return 1;
    }
    me->send(64769, pad_buffer(make16(query("position")), make8(1)));  // call (discard result)
    return 0;

}

//使用限制
private mixed check_condition_on_item(object me, object dest_item) {

    if ((me != env_object(dest_item))) {
        return "无法对包裹外的物品使用。";
    }
    if ((!me->is_legal_inv_pos(dest_item->query("position")))) {
        return "无法对过期的包裹内物品使用。";
    }
    if ((member_array(dest_item->query("equip_type"), equip_type_arr) < 0)) {
        return "该道具只能附在首饰上。";
    }
    if (dest_item->is_unidentified_equip()) {
        return "该道具无法对未鉴定使用。";
    }
    if (member_array(dest_item->get_name(), ({ "千彩流光", "游火灵焰", "岚金火链", "金碧莲花", "通灵宝玉", "天星奇光", "流光绝影", "寒玉龙勾", "碎梦涵光", "五蕴悯光", "八宝如意", "九天霜华", "掠虹宝坠", "炫元玲珑", "龙御七星", "破军困灵", "七杀固元", "贪狼破日", "洛神回雪", "菩提镜明", "屠龙封魔", "景云烛日", "和光同尘", "九霞朝真", "玄晖吐兰", "上善若水", "负阴抱阳" })) < 0) {
        return "该道具只能对100级以上的首饰使用。";
    }
    return 1;

}

public int try_to_cost_amount() {
    return 1;

}

private void create() {
    if ((!is_clone())) {
        set_name("百炼精");  // call (discard result)
import(get_name());  // call (discard result)
        set("attrib", 270);  // call (discard result)
        set("amulet", 1);  // call (discard result)
    }
    setup_special_item();  // call (discard result)
}

// 获取属性的中文显示名
private string get_prop_cn_name(string prop_key) {
    if (prop_key == "prop2/str") return "力量";
    if (prop_key == "prop2/con") return "体质";
    if (prop_key == "prop2/wiz") return "灵力";
    if (prop_key == "prop2/dex") return "敏捷";
    if (prop_key == "prop2/resist_metal") return "抗金";
    if (prop_key == "prop2/resist_wood") return "抗木";
    if (prop_key == "prop2/resist_water") return "抗水";
    if (prop_key == "prop2/resist_fire") return "抗火";
    if (prop_key == "prop2/resist_earth") return "抗土";
    if (prop_key == "prop2/resist_poison") return "抗中毒";
    if (prop_key == "prop2/resist_frozen") return "抗冰冻";
    if (prop_key == "prop2/resist_sleep") return "抗昏睡";
    if (prop_key == "prop2/resist_forgotten") return "抗遗忘";
    if (prop_key == "prop2/resist_confusion") return "抗混乱";
    if (prop_key == "prop2/all_polar") return "所有相性";
    if (prop_key == "prop2/all_resist_polar") return "所有抗性";
    if (prop_key == "prop2/all_resist_except") return "抗所有异常";
    if (prop_key == "prop2/all_skill") return "所有技能上升";
    if (prop_key == "prop2/ignore_all_resist_polar") return "忽视所有抗性";
    if (prop_key == "prop2/ignore_all_resist_except") return "忽视所有抗异常";
    if (prop_key == "prop2/ignore_resist_forgotten") return "忽视目标抗遗忘";
    if (prop_key == "prop2/ignore_resist_poison") return "忽视目标抗中毒";
    if (prop_key == "prop2/ignore_resist_frozen") return "忽视目标抗冰冻";
    if (prop_key == "prop2/ignore_resist_sleep") return "忽视目标抗昏睡";
    if (prop_key == "prop2/ignore_resist_confusion") return "忽视目标抗混乱";
    if (prop_key == "prop2/ignore_resist_metal") return "忽视目标抗金";
    if (prop_key == "prop2/ignore_resist_wood") return "忽视目标抗木";
    if (prop_key == "prop2/ignore_resist_water") return "忽视目标抗水";
    if (prop_key == "prop2/ignore_resist_fire") return "忽视目标抗火";
    if (prop_key == "prop2/ignore_resist_earth") return "忽视目标抗土";
    if (prop_key == "prop2/resist_repress") return "抗镇魂";
    if (prop_key == "prop2/resist_melt") return "抗化功";
    if (prop_key == "prop2/resist_cage") return "抗水牢";
    if (prop_key == "prop2/resist_lock") return "抗锁灵";
    if (prop_key == "prop2/resist_lost") return "抗迷心";
    if (prop_key == "prop2/ignore_resist_repress") return "忽视目标抗镇魂";
    if (prop_key == "prop2/ignore_resist_melt") return "忽视目标抗化功";
    if (prop_key == "prop2/ignore_resist_cage") return "忽视目标抗水牢";
    if (prop_key == "prop2/ignore_resist_lock") return "忽视目标抗锁灵";
    if (prop_key == "prop2/ignore_resist_lost") return "忽视目标抗迷心";
    return prop_key;
}

public int apply_on_item(object dest_item) {

    object me;
    string msg;
    string equip_name;
    string chosen_prop;
    string prop_cn_name;
    array available_props;
    int i;
    int current_val;
    int max_val;
    int enhance_val;
    int new_val;
    int success_chance;
    int success_roll;

    me = env_object(this_object());
    msg = check_condition_on_item(me, dest_item);
    if (is_string(msg)) {
        me->send_dialog_ok(msg);  // call (discard result)
        return 0;
    }

    // 遍历候选列表，收集首饰实际拥有且未达上限的prop2/属性
    available_props = ({});
    for (i = 0; i < sizeof(attr_candidates); i++) {
        current_val = (int)dest_item->query(attr_candidates[i]);
        if (current_val > 0) {
            max_val = get_bailian_attr_max(dest_item, attr_candidates[i]);
            if (max_val > 0 && current_val < max_val) {
                available_props += ({ attr_candidates[i] });
            }
        }
    }

    if (sizeof(available_props) == 0) {
        me->send_dialog_ok("该首饰所有属性均已达到强化上限。");  // call (discard result)
        return 0;
    }

    // 随机选一条prop2/属性
    chosen_prop = available_props[random(sizeof(available_props))];

    // 获取中文显示名
    prop_cn_name = get_prop_cn_name(chosen_prop);

    // 随机强化1-5点，但不超过上限
    current_val = (int)dest_item->query(chosen_prop);
    max_val = get_bailian_attr_max(dest_item, chosen_prop);
    enhance_val = random(5) + 1;
    if (current_val + enhance_val > max_val) {
        enhance_val = max_val - current_val;
    }
    new_val = current_val + enhance_val;

    // 消耗道具（无论成功失败都消耗）
    cost_amount(1);  // call (discard result)

    // 当前属性越接近上限，强化成功率越低；范围为10%-90%
    success_chance = 10 + (max_val - current_val) * 80 / max_val;
    if (success_chance < 10) {
        success_chance = 10;
    }
    if (success_chance > 90) {
        success_chance = 90;
    }
    success_roll = random(100);
    if (success_roll >= success_chance) {
        // 失败：不执行强化，只提示
        me->refresh_item(dest_item->query("position"));  // call (discard result)
        me->send_dialog_ok("很遗憾，你的首饰未能吸收百炼精中的精华，首饰的属性没有得到提升。");  // call (discard result)
        return 1;
    }

    // 成功：执行强化
    dest_item->set(chosen_prop, new_val);  // call (discard result)
    me->refresh_item(dest_item->query("position"));  // call (discard result)
    equip_name = dest_item->get_name();
    me->send_dialog_ok(sprintf("你的首饰吸收了百炼精中的精华，#R%s %d 增加#n提升至#R%s %d 增加#n。", prop_cn_name, current_val, prop_cn_name, new_val));  // call (discard result)

    return 1;
}