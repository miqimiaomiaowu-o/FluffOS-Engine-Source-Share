//gs/clone/item/hell/zixuan_libao.c

inherit "/gs/inherit/item/special_item.c";

public int apply_item(object me, array obs) { return 0; }
public int cant_apply_in_combat(object me) { return 1; }

private array query_bonus_list() {
    return ({
        ({ "Ëø÷ëÄÒ¡¤½®Á¼", "Ëø÷ëÄÒ¡¤½®Á¼" }),
        ({ "Ëø÷ëÄÒ¡¤ĞşÎä", "Ëø÷ëÄÒ¡¤ĞşÎä" }),
        ({ "Ëø÷ëÄÒ¡¤¶«É½ÉñÁé", "Ëø÷ëÄÒ¡¤¶«É½ÉñÁé" }),
        ({ "Ëø÷ëÄÒ¡¤ÖìÈ¸", "Ëø÷ëÄÒ¡¤ÖìÈ¸" }),
        ({ "Ëø÷ëÄÒ¡¤¾ÅÎ²ºü", "Ëø÷ëÄÒ¡¤¾ÅÎ²ºü" }),
        ({ "Ëø÷ëÄÒ¡¤İêÊÕ", "Ëø÷ëÄÒ¡¤İêÊÕ" }),
        ({ "Ëø÷ëÄÒ¡¤¾äÃ¢", "Ëø÷ëÄÒ¡¤¾äÃ¢" }),
        ({ "Ëø÷ëÄÒ¡¤×£ÈÚ", "Ëø÷ëÄÒ¡¤×£ÈÚ" }),
        ({ "Ëø÷ëÄÒ¡¤ºóÍÁ", "Ëø÷ëÄÒ¡¤ºóÍÁ" }),
        ({ "Ëø÷ëÄÒ¡¤æÏ¶ğ", "Ëø÷ëÄÒ¡¤æÏ¶ğ" }),
        ({ "Ëø÷ëÄÒ¡¤¹´³Â", "Ëø÷ëÄÒ¡¤¹´³Â" }),
        ({ "Ëø÷ëÄÒ¡¤ĞşÚ¤", "Ëø÷ëÄÒ¡¤ĞşÚ¤" }),
        ({ "Ëø÷ëÄÒ¡¤Ê¥µ®ÇòÇò", "Ëø÷ëÄÒ¡¤Ê¥µ®ÇòÇò" }),
        });
}

private void bind_menu_session(object me, object menu_ob) {
    if (!is_object(menu_ob)) menu_ob = this_object();

    set("ignore_distance", 1);
    set("ingore_distance_npc", 1);
    set("talk_distance", 999999);
    set("command_distance", 999999);
    lite_set("ignore_distance", 1);

    me->set_temp("start_talk_to", menu_ob);
    me->set_temp("ingore_distance_npc", 1);
    me->set_temp("current_npc", menu_ob);
    me->set_temp("click_npc", menu_ob);
}

private mixed get_menu_user(mixed arg1, mixed arg2) {
    object me;

    if (is_object(arg1) && arg1 == env_object()) return arg1;
    if (is_object(arg2) && arg2 == env_object()) return arg2;
    if (is_object(arg1) && arg1->is_player()) return arg1;
    if (is_object(arg2) && arg2->is_player()) return arg2;

    me = env_object();
    if (is_object(me)) return me;
    return 0;
}

private int give_bonus(mixed arg1, mixed arg2, int index) {
    object me, item;
    array bonuses, info;
    string show_name, item_name;

    me = get_menu_user(arg1, arg2);
    if (!is_object(me)) return 0;

    if (env_object() != me) {
        me->send_dialog_ok("Õâ¸ö¾ĞÁéÃØ±¦¡¤ÉñÊŞ²»ÔÚÄãµÄ°ü¹üÖĞ¡£");
        return 0;
    }

    bonuses = query_bonus_list();
    if (index < 0 || index >= sizeof(bonuses)) {
        me->send_dialog_ok("½±ÀøÑ¡ÔñÎŞĞ§¡£");
        return 0;
    }

    if ((int)me->get_idlesse_place() < 1) {
        me->send_dialog_ok("ÄãµÄ°ü¹ü¿Õ¸ñ²»×ã£¬ÎŞ·¨´ò¿ª¾ĞÁéÃØ±¦¡¤ÉñÊŞ¡£");
        return 0;
    }

    info = bonuses[index];
    show_name = info[0];
    item_name = info[1];

    item = "/gs/daemons/named.c"->new_item(item_name);
    if (!is_object(item)) {
        me->send_dialog_ok("Éú³É½±ÀøÊ§°Ü£¬Çë¼ì²é½±ÀøÃû³ÆÊÇ·ñ´æÔÚ¡£");
        return 0;
    }

    item->move_to_bag(me);
    me->destroy_menu();
    me->send_dialog_ok(sprintf("Äã´ò¿ªÁË#R¾ĞÁéÃØ±¦¡¤ÉñÊŞ#n£¬ÁìÈ¡ÁË#R%s#n¡£", show_name));
    cost_amount(1);
    return 1;
}

public int menu_bonus_1(mixed me, mixed npc) { return give_bonus(me, npc, 0); }
public int menu_bonus_2(mixed me, mixed npc) { return give_bonus(me, npc, 1); }
public int menu_bonus_3(mixed me, mixed npc) { return give_bonus(me, npc, 2); }
public int menu_bonus_4(mixed me, mixed npc) { return give_bonus(me, npc, 3); }
public int menu_bonus_5(mixed me, mixed npc) { return give_bonus(me, npc, 4); }
public int menu_bonus_6(mixed me, mixed npc) { return give_bonus(me, npc, 5); }
public int menu_bonus_7(mixed me, mixed npc) { return give_bonus(me, npc, 6); }
public int menu_bonus_8(mixed me, mixed npc) { return give_bonus(me, npc, 7); }
public int menu_bonus_9(mixed me, mixed npc) { return give_bonus(me, npc, 8); }
public int menu_bonus_10(mixed me, mixed npc) { return give_bonus(me, npc, 9); }
public int menu_bonus_11(mixed me, mixed npc) { return give_bonus(me, npc, 10); }
public int menu_bonus_12(mixed me, mixed npc) { return give_bonus(me, npc, 11); }
public int menu_bonus_13(mixed me, mixed npc) { return give_bonus(me, npc, 12); }

public int menu_close(mixed me, mixed npc) {
    object user;

    user = get_menu_user(me, npc);
    if (is_object(user)) user->destroy_menu();
    return 1;
}

private void bind_menu_action(object me, string key, mixed action) {
    me->set_temp("menu/" + key, action);
    me->set_temp("current_menu/" + key, action);
}

private void bind_menu_actions(object me) {
    bind_menu_action(me, "ÁìÈ¡Ëø÷ëÄÒ¡¤½®Á¼", (: menu_bonus_1:));
    bind_menu_action(me, "ÁìÈ¡Ëø÷ëÄÒ¡¤ĞşÎä", (: menu_bonus_2:));
    bind_menu_action(me, "ÁìÈ¡Ëø÷ëÄÒ¡¤¶«É½ÉñÁé", (: menu_bonus_3:));
    bind_menu_action(me, "ÁìÈ¡Ëø÷ëÄÒ¡¤ÖìÈ¸", (: menu_bonus_4:));
    bind_menu_action(me, "ÁìÈ¡Ëø÷ëÄÒ¡¤¾ÅÎ²ºü", (: menu_bonus_5:));
    bind_menu_action(me, "ÁìÈ¡Ëø÷ëÄÒ¡¤İêÊÕ", (: menu_bonus_6:));
    bind_menu_action(me, "ÁìÈ¡Ëø÷ëÄÒ¡¤¾äÃ¢", (: menu_bonus_7:));
    bind_menu_action(me, "ÁìÈ¡Ëø÷ëÄÒ¡¤×£ÈÚ", (: menu_bonus_8:));
    bind_menu_action(me, "ÁìÈ¡Ëø÷ëÄÒ¡¤ºóÍÁ", (: menu_bonus_9:));
    bind_menu_action(me, "ÁìÈ¡Ëø÷ëÄÒ¡¤æÏ¶ğ", (: menu_bonus_10:));
    bind_menu_action(me, "ÁìÈ¡Ëø÷ëÄÒ¡¤¹´³Â", (: menu_bonus_11:));
    bind_menu_action(me, "ÁìÈ¡Ëø÷ëÄÒ¡¤ĞşÚ¤", (: menu_bonus_12:));
    bind_menu_action(me, "ÁìÈ¡Ëø÷ëÄÒ¡¤Ê¥µ®ÇòÇò", (: menu_bonus_13:));
    bind_menu_action(me, "Àë¿ª", (: menu_close:));
}

override public int apply() {
    object me;
    mapping menu;
    string content;

    me = env_object();
    if (!is_object(me)) return 1;

    menu = ([
        "ÁìÈ¡Ëø÷ëÄÒ¡¤½®Á¼":(: menu_bonus_1:),
        "ÁìÈ¡Ëø÷ëÄÒ¡¤ĞşÎä" : (: menu_bonus_2:),
        "ÁìÈ¡Ëø÷ëÄÒ¡¤¶«É½ÉñÁé" : (: menu_bonus_3:),
        "ÁìÈ¡Ëø÷ëÄÒ¡¤ÖìÈ¸" : (: menu_bonus_4:),
        "ÁìÈ¡Ëø÷ëÄÒ¡¤¾ÅÎ²ºü" : (: menu_bonus_5:),
        "ÁìÈ¡Ëø÷ëÄÒ¡¤İêÊÕ" : (: menu_bonus_6:),
        "ÁìÈ¡Ëø÷ëÄÒ¡¤¾äÃ¢" : (: menu_bonus_7:),
        "ÁìÈ¡Ëø÷ëÄÒ¡¤×£ÈÚ" : (: menu_bonus_8:),
        "ÁìÈ¡Ëø÷ëÄÒ¡¤ºóÍÁ" : (: menu_bonus_9:),
        "ÁìÈ¡Ëø÷ëÄÒ¡¤æÏ¶ğ" : (: menu_bonus_10:),
        "ÁìÈ¡Ëø÷ëÄÒ¡¤¹´³Â" : (: menu_bonus_11:),
        "ÁìÈ¡Ëø÷ëÄÒ¡¤ĞşÚ¤" : (: menu_bonus_12:),
        "ÁìÈ¡Ëø÷ëÄÒ¡¤Ê¥µ®ÇòÇò" : (: menu_bonus_13:),
        "Àë¿ª" : (: menu_close:),
    ]);

    content = "ÇëÑ¡ÔñÄãÒªÁìÈ¡µÄ½±Àø£º[Ñ¡Ôñ½®Á¼#n/ÁìÈ¡Ëø÷ëÄÒ¡¤½®Á¼][Ñ¡ÔñĞşÎä#n/ÁìÈ¡Ëø÷ëÄÒ¡¤ĞşÎä][Ñ¡Ôñ¶«É½ÉñÁé#n/ÁìÈ¡Ëø÷ëÄÒ¡¤¶«É½ÉñÁé][Ñ¡ÔñÖìÈ¸#n/ÁìÈ¡Ëø÷ëÄÒ¡¤ÖìÈ¸][Ñ¡Ôñ¾ÅÎ²ºü#n/ÁìÈ¡Ëø÷ëÄÒ¡¤¾ÅÎ²ºü][Ñ¡ÔñİêÊÕ#n/ÁìÈ¡Ëø÷ëÄÒ¡¤İêÊÕ][Ñ¡Ôñ¾äÃ¢#n/ÁìÈ¡Ëø÷ëÄÒ¡¤¾äÃ¢][Ñ¡Ôñ×£ÈÚ#n/ÁìÈ¡Ëø÷ëÄÒ¡¤×£ÈÚ][Ñ¡ÔñºóÍÁ#n/ÁìÈ¡Ëø÷ëÄÒ¡¤ºóÍÁ][Ñ¡ÔñæÏ¶ğ#n/ÁìÈ¡Ëø÷ëÄÒ¡¤æÏ¶ğ][Ñ¡Ôñ¹´³Â#n/ÁìÈ¡Ëø÷ëÄÒ¡¤¹´³Â][Ñ¡ÔñĞşÚ¤#n/ÁìÈ¡Ëø÷ëÄÒ¡¤ĞşÚ¤][Ñ¡ÔñÊ¥µ®ÇòÇò#n/ÁìÈ¡Ëø÷ëÄÒ¡¤Ê¥µ®ÇòÇò][ÎÒÔÙÏëÏë/Àë¿ª]";

    bind_menu_session(me, me);
    me->set_temp("not_notify_team_members", 1);
    me->set_temp("show_name", "¾ĞÁéÃØ±¦¡¤ÉñÊŞ");
    me->set_menu(me, menu, content, 0, 0, 4);
    me->delete_temp("show_name");
    me->delete_temp("not_notify_team_members");
    bind_menu_session(me, me);
    bind_menu_actions(me);
    me->set_temp("menu_renew", 1);
    return 0;
}

public int try_to_cost_amount() { return 1; }

private void create() {
    if (!is_clone()) {
        set_name("¾ĞÁéÃØ±¦¡¤ÉñÊŞ");
import(get_name());
        set("attrib", 270);
        set("amulet", 1);
        set("ignore_distance", 1);
        set("ingore_distance_npc", 1);
        set("talk_distance", 999999);
        set("command_distance", 999999);
    }
    setup_special_item();
}