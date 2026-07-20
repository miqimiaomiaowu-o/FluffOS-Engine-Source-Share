#define BASE_ATTACH_SKILLD "/gs/daemons/attach_skilld_base.c"
#define ARTIFACTD "/gs/daemons/artifactd.c"
#define HUALING_ATTACH_MARKER "fabao-hualing-attach-skilld-wrapper-v4-owner-iid-20260630"

mapping entries;

mixed register_entry(mixed key, mixed module, mixed priority)
{
    if (!key || key == "") return 0;
    if (!entries) entries = ([]);
    entries[key] = module;
    catch(call_other(BASE_ATTACH_SKILLD, "register_entry", key, module));
    return 1;
}

mixed query_entry(mixed key)
{
    mixed module;

    if (!key || key == "") return 0;
    if (!entries) entries = ([]);
    module = entries[key];
    if (!module) {
        if (catch(module = BASE_ATTACH_SKILLD->query_entry(key))) module = 0;
    }
    return module;
}

mixed get_all_entries()
{
    mixed base_entries;

    if (!entries) entries = ([]);
    base_entries = 0;
    if (catch(base_entries = BASE_ATTACH_SKILLD->get_all_entries())) base_entries = 0;
    if (base_entries && is_mapping(base_entries)) {
        return base_entries + entries;
    }
    return entries;
}

private mixed _base_call(string fn, mixed args)
{
    int n;

    n = sizeof(args);
    if (n == 0) return call_other(BASE_ATTACH_SKILLD, fn);
    if (n == 1) return call_other(BASE_ATTACH_SKILLD, fn, args[0]);
    if (n == 2) return call_other(BASE_ATTACH_SKILLD, fn, args[0], args[1]);
    if (n == 3) return call_other(BASE_ATTACH_SKILLD, fn, args[0], args[1], args[2]);
    if (n == 4) return call_other(BASE_ATTACH_SKILLD, fn, args[0], args[1], args[2], args[3]);
    if (n == 5) return call_other(BASE_ATTACH_SKILLD, fn, args[0], args[1], args[2], args[3], args[4]);
    if (n == 6) return call_other(BASE_ATTACH_SKILLD, fn, args[0], args[1], args[2], args[3], args[4], args[5]);
    return call_other(BASE_ATTACH_SKILLD, fn, args[0], args[1], args[2], args[3], args[4], args[5], args[6]);
}

private mixed _relay(string fn, mixed args...)
{
    return _base_call(fn, args);
}

private mixed _safe_relay(string fn, mixed args...)
{
    mixed ret;

    ret = 0;
    if (catch(ret = _base_call(fn, args))) return 0;
    return ret;
}

private mixed _equipped_hualing_artifact(mixed me)
{
    object artifact;
    mixed skill;
    mixed positions;
    int i;

    if (!me) return 0;
    if (catch(me->query("id"))) return 0;

    artifact = 0;
    if (catch(artifact = ARTIFACTD->get_equipped_artifact(me))) artifact = 0;
    if (artifact) {
        skill = 0;
        if (!catch(skill = artifact->is_skill_artifact()) && skill) return artifact;
    }

    positions = ({ 9, 53 });
    for (i = 0; i < sizeof(positions); i++) {
        artifact = 0;
        if (catch(artifact = me->get_equip_item(positions[i]))) artifact = 0;
        if (!artifact) continue;
        skill = 0;
        if (!catch(skill = artifact->is_skill_artifact()) && skill) return artifact;
    }
    return 0;
}

private mixed _artifact_extra_attach_skill(mixed me)
{
    object artifact;
    mixed skill;
    mixed iid;
    mixed owner_iid;

    if (!me) return 0;
    if (catch(me->query("id"))) return 0;
    artifact = _equipped_hualing_artifact(me);
    if (!artifact) return 0;
    if (catch(skill = artifact->is_skill_artifact()) || !skill) return 0;
    if (!artifact->query("fabao_hualing_v1")) return 0;

    iid = 0;
    owner_iid = 0;
    if (catch(iid = artifact->get_iid())) iid = 0;
    if (catch(owner_iid = artifact->query("fabao_hualing_owner_iid"))) owner_iid = 0;
    if (!iid || !owner_iid || iid != owner_iid) {
        return 0;
    }

    skill = artifact->query("extra_attach_skill");
    if (!skill || skill == "") return 0;
    if (skill == artifact->query("attach_skill")) return 0;
    return skill;
}

private mixed _artifact_attach_skill(mixed me)
{
    object artifact;
    mixed skill;

    if (!me) return 0;
    if (catch(me->query("id"))) return 0;
    artifact = _equipped_hualing_artifact(me);
    if (!artifact) return 0;
    skill = 0;
    if (catch(skill = artifact->query("attach_skill"))) skill = 0;
    return skill;
}

private mixed _temp_attach_skills(mixed me)
{
    mixed skills;

    if (!me) return 0;
    if (catch(me->query("id"))) return 0;
    skills = 0;
    if (catch(skills = me->query_temp("attach_skills"))) skills = 0;
    return skills;
}

private mixed _equipped_artifact(mixed me)
{
    object artifact;

    if (!me) return 0;
    if (catch(me->query("id"))) return 0;
    artifact = _equipped_hualing_artifact(me);
    if (!artifact) return 0;
    return artifact;
}

private int _is_current_artifact_skill(mixed me, mixed type)
{
    mixed skill;
    mixed extra;

    if (!me || !type || type == "") return 0;
    skill = _artifact_attach_skill(me);
    if (skill && skill == type) return 1;
    extra = _artifact_extra_attach_skill(me);
    if (extra && extra == type) return 1;
    return 0;
}

private int _is_current_extra_skill(mixed me, mixed type)
{
    mixed extra;

    if (!me || !type || type == "") return 0;
    extra = _artifact_extra_attach_skill(me);
    return (extra && extra == type) ? 1 : 0;
}

private void _allow_extra_attach_skill(mixed me)
{
    mixed skill;

    skill = _artifact_extra_attach_skill(me);
    if (!skill) return;
    call_other(this_object(), "allow_attach_skill_used", me, skill);
}

private mixed _artifact_attach_skill_list(mixed me)
{
    mixed skill;
    mixed extra;
    mixed skills;

    skills = ({});
    skill = _artifact_attach_skill(me);
    if (skill) skills += ({ skill });
    extra = _artifact_extra_attach_skill(me);
    if (extra && member_array(extra, skills) < 0) skills += ({ extra });
    return skills;
}

private void _clear_artifact_attach_skills(mixed me)
{
    mixed attach_skills;
    mixed kept;
    mixed artifact_skills;
    int i;

    attach_skills = me->query_temp("attach_skills");
    if (!attach_skills) return;
    artifact_skills = _artifact_attach_skill_list(me);
    if (!artifact_skills || !sizeof(artifact_skills)) return;

    kept = ({});
    for (i = 0; i < sizeof(attach_skills); i++) {
        if (member_array(attach_skills[i], artifact_skills) < 0) {
            kept += ({ attach_skills[i] });
        }
    }
    me->set_temp("attach_skills", kept);
}

private mixed _skill_module(mixed type)
{
    mixed module;

    module = query_entry(type);
    if (module) return module;
    if (!type || type == "") return 0;

    module = "/gs/daemons/attach_skill/" + type + ".c";
    if (catch(load_object(module))) return 0;
    return module;
}

private void create()
{
    if (!entries) entries = ([]);
}

void do_init()
{
    call_other(BASE_ATTACH_SKILLD, "do_init");
}

mixed register_attach_skill_ok(mixed me)
{
    mixed ret;
    mixed before;
    mixed extra;
    mixed module;

    before = _temp_attach_skills(me);
    ret = 0;
    if (catch(ret = call_other(BASE_ATTACH_SKILLD, "register_attach_skill_ok", me))) ret = 0;

    extra = _artifact_extra_attach_skill(me);
    if (extra && call_other(this_object(), "check_trigger_rate", me, extra)) {
        module = _skill_module(extra);
        if (module) {
            if (catch(call_other(module, "register_attach_skill_ok", me))) {
                call_other(this_object(), "allow_attach_skill_used", me, extra);
            }
        } else {
            call_other(this_object(), "allow_attach_skill_used", me, extra);
        }
    }
    return ret;
}

mixed new_attach_skills(mixed me)
{
    mixed base_news;
    mixed extra;
    mixed news;

    news = ({});
    if (!catch(base_news = call_other(BASE_ATTACH_SKILLD, "new_attach_skills", me)) && base_news) {
        news += base_news;
    }

    extra = _artifact_extra_attach_skill(me);
    if (extra && member_array(extra, news) < 0 &&
        call_other(this_object(), "check_trigger_rate", me, extra)) {
        news += ({ extra });
    }
    return news;
}

mixed check_trigger_rate(mixed me, mixed type)
{
    mixed ret;
    mixed skill;
    mixed module;
    mixed artifact;

    ret = 0;
    if (!catch(ret = call_other(BASE_ATTACH_SKILLD, "check_trigger_rate", me, type)) && ret) {
        if (_artifact_attach_skill(me) == type) return ret;
        if (!_is_current_extra_skill(me, type)) return 0;
    }

    skill = _artifact_extra_attach_skill(me);
    if (!skill || skill != type) return ret;
    module = _skill_module(skill);
    if (!module) return ret;
    artifact = _equipped_artifact(me);
    if (!artifact) return 0;
    if (catch(ret = artifact->check_trigger_rate())) return 0;
    if (ret) {
        call_other(this_object(), "allow_attach_skill_used", me, type);
    }
    return ret;
}

mixed all_attach_skills(mixed args...)
{
    return _safe_relay("all_attach_skills", args);
}

mixed all_attach_skills_ex(mixed args...)
{
    return _safe_relay("all_attach_skills_ex", args);
}

mixed num_attach_skills(mixed args...)
{
    return _safe_relay("num_attach_skills", args);
}

void remove_all_attach_skills(mixed me)
{
    if (catch(call_other(BASE_ATTACH_SKILLD, "remove_all_attach_skills", me))) {
    }
    _clear_artifact_attach_skills(me);
}

void allow_attach_skill_used(mixed me, mixed type)
{
    mixed attach_skill;

    if (!_is_current_artifact_skill(me, type)) return;
    attach_skill = me->query_temp("attach_skills");
    if (!attach_skill) {
        attach_skill = ({});
    }
    if (member_array(type, attach_skill) < 0) {
        attach_skill += ({ type });
    }
    me->set_temp("attach_skills", attach_skill);
}

mixed can_attach_skill_be_used(mixed me, mixed atk, mixed type)
{
    mixed attach_skills;
    mixed ret;
    mixed skill;
    mixed module;

    ret = 0;
    if (!catch(ret = call_other(BASE_ATTACH_SKILLD, "can_attach_skill_be_used", me, atk, type)) && ret) {
        if (_artifact_attach_skill(me) == type) return ret;
        if (!_is_current_extra_skill(me, type)) return 0;
    }

    skill = _artifact_extra_attach_skill(me);
    if (!skill || skill != type) {
        return ret;
    }

    attach_skills = me->query_temp("attach_skills");
    if (!attach_skills) {
        return 0;
    }
    if (member_array(type, attach_skills) < 0) {
        return 0;
    }
    module = _skill_module(type);
    if (!module) {
        return 0;
    }
    if (!module->check_combat_condition(me, atk)) {
        return 0;
    }
    ret = module->can_attach_skill_be_used(me);
    return ret;
}

mixed stat_attach_skill_used(mixed cast_ob, mixed atk, mixed type)
{
    if (!_is_current_artifact_skill(cast_ob, type)) return 0;
    return call_other(BASE_ATTACH_SKILLD, "stat_attach_skill_used", cast_ob, atk, type);
}

mixed adjust_victim(mixed me, mixed victim, mixed type)
{
    mixed module;
    mixed ret;
    mixed skill;
    mixed n;
    mixed obs;
    mixed in_combat;

    ret = 0;
    if (!catch(ret = call_other(BASE_ATTACH_SKILLD, "adjust_victim", me, victim, type)) && ret) {
        if (_artifact_attach_skill(me) == type) return ret;
        if (!_is_current_extra_skill(me, type)) return 0;
    }

    skill = _artifact_extra_attach_skill(me);
    if (skill == type) {
        module = _skill_module(type);
        obs = ({ victim });
        in_combat = 0;
        if (!catch(in_combat = victim->is_in_combat()) && in_combat) {
            if (catch(obs = me->get_living_opponents()) || !obs) obs = ({ victim });
        } else {
            if (catch(obs = victim->get_living_allies()) || !obs) obs = ({});
            obs += ({ victim });
        }
        if (module && !catch(ret = call_other(module, "adjust_victim", me, victim, obs))) {
            n = 0;
            if (!catch(n = sizeof(ret)) && n > 0) {
                if (!catch(ret = ret[0])) {
                    return ret;
                }
            }
            return ret;
        }
    }
    return ret;
}

private mixed _module_call(mixed module, string fn, mixed args)
{
    int n;

    n = sizeof(args);
    if (n == 0) return call_other(module, fn);
    if (n == 1) return call_other(module, fn, args[0]);
    if (n == 2) return call_other(module, fn, args[0], args[1]);
    if (n == 3) return call_other(module, fn, args[0], args[1], args[2]);
    return call_other(module, fn, args[0], args[1], args[2], args[3]);
}

private mixed _extra_sf_call(mixed me, string skill, string fn, mixed args)
{
    mixed module;
    mixed ret;

    ret = 0;
    if (!_is_current_extra_skill(me, skill)) return 0;
    module = _skill_module(skill);
    if (!module) return 0;
    if (catch(ret = _module_call(module, fn, args))) return 0;
    return ret;
}

mixed sf_receive_damage(mixed me, mixed arg)
{
    mixed ret;

    ret = 0;
    if (!catch(ret = call_other(BASE_ATTACH_SKILLD, "sf_receive_damage", me, arg)) && ret) return ret;
    ret = _extra_sf_call(me, "dinghaizhu", "sf_receive_damage", ({ me, arg }));
    return ret;
}

mixed sf_do_attack(mixed me, mixed arg)
{
    mixed ret;

    ret = 0;
    if (!catch(ret = call_other(BASE_ATTACH_SKILLD, "sf_do_attack", me, arg)) && ret) return ret;
    ret = _extra_sf_call(me, "hundun-zhong", "sf_do_attack", ({ me, arg }));
    return ret;
}

mixed sf_change_phy_damage(mixed me, mixed arg)
{
    mixed ret;

    ret = 0;
    if (!catch(ret = call_other(BASE_ATTACH_SKILLD, "sf_change_phy_damage", me, arg)) && ret) return ret;
    ret = _extra_sf_call(me, "hunyuan-jindou", "sf_change_phy_damage", ({ me, arg }));
    if (ret) {
        return ret;
    }
    ret = _extra_sf_call(me, "pangu-fan", "sf_change_phy_damage", ({ me, arg }));
    return ret;
}

mixed sf_change_to_double_magic_hit(mixed me, mixed arg)
{
    mixed ret;

    ret = 0;
    if (!catch(ret = call_other(BASE_ATTACH_SKILLD, "sf_change_to_double_magic_hit", me, arg)) && ret) return ret;
    ret = _extra_sf_call(me, "fantianyin", "sf_change_to_double_magic_hit", ({ me, arg }));
    return ret;
}

mixed sf_be_hit(mixed me, mixed arg)
{
    mixed ret;

    ret = 0;
    if (!catch(ret = call_other(BASE_ATTACH_SKILLD, "sf_be_hit", me, arg)) && ret) return ret;
    ret = _extra_sf_call(me, "qibao-miaoshu", "sf_be_hit", ({ me, arg }));
    return ret;
}

mixed sf_first_hit(mixed me, mixed arg)
{
    mixed ret;

    ret = 0;
    if (!catch(ret = call_other(BASE_ATTACH_SKILLD, "sf_first_hit", me, arg)) && ret) return ret;
    ret = _extra_sf_call(me, "shierpin-liantai", "sf_first_hit", ({ me, arg }));
    return ret;
}

mixed sf_magic_chase(mixed me)
{
    mixed ret;

    ret = 0;
    if (!catch(ret = call_other(BASE_ATTACH_SKILLD, "sf_magic_chase", me)) && ret) return ret;
    ret = _extra_sf_call(me, "jinjiaojian", "sf_magic_chase", ({ me }));
    return ret;
}

mixed get_bma_skill(mixed me)
{
    mixed ret;

    ret = 0;
    if (!catch(ret = call_other(BASE_ATTACH_SKILLD, "get_bma_skill", me)) && ret) return ret;
    ret = _extra_sf_call(me, "jinjiaojian", "get_bma_skill", ({ me }));
    return ret;
}

mixed sf_exclude_victim(mixed me, mixed arg)
{
    mixed ret;

    ret = 0;
    if (!catch(ret = call_other(BASE_ATTACH_SKILLD, "sf_exclude_victim", me, arg)) && ret) return ret;
    ret = _extra_sf_call(me, "yinyangjing", "sf_exclude_victim", ({ me, arg }));
    return ret;
}

mixed has_dinghaizhu_effect(mixed me)
{
    mixed ret;
    mixed attach_skills;

    ret = 0;
    if (!catch(ret = call_other(BASE_ATTACH_SKILLD, "has_dinghaizhu_effect", me)) && ret) {
        if (_artifact_attach_skill(me) == "dinghaizhu") return ret;
        if (_artifact_extra_attach_skill(me) != "dinghaizhu") return 0;
    }
    if (_artifact_extra_attach_skill(me) == "dinghaizhu") {
        attach_skills = _temp_attach_skills(me);
        if (attach_skills && member_array("dinghaizhu", attach_skills) >= 0) return 1;
    }
    return ret;
}

void set_attached_para(mixed cast_ob, mixed type, mixed para)
{
    if (!_is_current_artifact_skill(cast_ob, type)) return;
    call_other(BASE_ATTACH_SKILLD, "set_attached_para", cast_ob, type, para);
}

mixed query_attached_para(mixed me, mixed type)
{
    if (!_is_current_artifact_skill(me, type)) return 0;
    return call_other(BASE_ATTACH_SKILLD, "query_attached_para", me, type);
}

mixed get_all_attach_skills()
{
    return call_other(BASE_ATTACH_SKILLD, "all_attach_skills");
}

mixed get_all_attach_skills_ex()
{
    return call_other(BASE_ATTACH_SKILLD, "all_attach_skills_ex");
}

void play_light_effect(mixed me, mixed type)
{
    mixed combat;
    mixed module;
    mixed effect;
    mixed effect_type;
    mixed id;
    mixed ret;

    if (!catch(ret = call_other(BASE_ATTACH_SKILLD, "play_light_effect", me, type))) {
        return;
    }
    if (!_is_current_extra_skill(me, type)) return;

    combat = 0;
    if (!catch(combat = me->lite_query_temp("combat")) && combat) {
        module = _skill_module(type);
        if (module) {
            effect = 0;
            effect_type = 0;
            id = 0;
            if (catch(effect = module->light_effect())) {
                if (catch(effect = module->get_light_effect())) effect = 0;
            }
            if (catch(effect_type = module->light_effect_type())) {
                if (catch(effect_type = module->get_light_effect_type())) effect_type = 0;
            }
            if (effect && effect_type && !catch(id = me->get_id())) {
                combat->broadcast_msg(9732, ({ effect_type, effect, id }));
                return;
            }
        }
    }

}

mixed is_new_attach_skills(mixed name)
{
    return call_other(BASE_ATTACH_SKILLD, "is_new_attach_skills", name);
}

string marker()
{
    return "fabao-hualing-attach-skilld-wrapper-v3-guard-equipped-20260630";
}
