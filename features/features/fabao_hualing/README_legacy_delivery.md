# 法宝化灵符交付说明

生成时间：2026-06-19

## 目标

新增右键道具 `法宝化灵符`。使用后打开独立的提交法宝窗口，提交主法宝和副法宝后，把副法宝的 `attach_skill` 转移到主法宝的第二被动字段上。

## 新增道具规则

本次已按 `wufa_qianghua_fu` 的方式添加道具：

- 道具对象：`gs/clone/item/special/fabao_hualing_fu.o`
- 源码继承：`/gs/inherit/item/special_item.c`
- 右键入口：`int apply()`
- 列表：只追加 `special_item.list`
- 描述：追加 `item_temp_desc.list`
- 不写 `charge_item.list`

这个规则已写入：

```text
source_reference/docs/新增道具模板说明.md
```

## 替换文件

需要放入 `lib_gs32.pak`：

```text
replace/lib_gs32/gs/clone/item/special/fabao_hualing_fu.o
replace/lib_gs32/gs/daemons/fabao_hualingd.o
replace/lib_gs32/gs/daemons/attach_skilld.o
replace/lib_gs32/gs/daemons/attach_skilld_base.o
```

需要重包进 `etc.pak`：

```text
replace/etc/special_item.list
replace/etc/item_temp_desc.list
```

## 字段说明

主法宝原被动：

```text
attach_skill
```

副法宝被动转移后写入主法宝：

```text
extra_attach_skill
extra_attach_skill_name
extra_attach_from_name
extra_attach_from_iid
fabao_hualing_v1
```

注意：没有写 `extra_skill`。`extra_skill` 是原法宝额外技能链，不是翻天印、混元金斗这种被动触发链。

## 编译验证

已在 `192.168.163.129` 的临时目录远程编译，并拉回本地对象。所有替换对象头均为：

```text
01000100000000000000000000000000
```

对象 SHA256 见：

```text
SHA256SUMS.txt
```

## 2026-06-19 Live 测试服部署记录

已直接替换 `/home/gs/gs/pack_data/lib_gs32.pak` 与 `/home/gs/gs/pack_data/etc.pak`，并用 `/home/gs/rungs2` 重启 GS2。

当前状态：

```text
GS2 8161 listening
pid: 113497
lib_gs32.pak: 4f83e1e3d2c5dfc82aa89146f0255ac2e713aeb09cb95db03a1d4a0da3177830
etc.pak:      5a49ef04c4be28e4fbc6daee684032033068bafce5f373c1ddf82efa4f7a2872
log:          /home/gs/gs2_fabao_hualingd_submit_restart_20260619_124556.log
```

启动日志已确认：

```text
Load: /gs/daemons/fabao_hualingd.c... OK
Load: /gs/daemons/attach_skilld.c... OK
Load: /gs/clone/item/special/fabao_hualing_fu.c... OK
```

注意：

- 这台服务器 GS2 启动入口是 `/home/gs/rungs2`，不是旧文档里的 `./2`。
- `fabao_hualing_fu.o` 使用已验证的 `wufa_qianghua_fu.o` 作为模板原位改名和改 daemon 路径，避免 `special_item` 父类 ABI 函数槽位不一致。
- `fabao_hualingd.o` 提供 `use_enhance_item(me, unused, item)` 兼容入口，供模板道具对象右键调用。
- `fabao_hualingd.o` 当前不再打开原法宝三合一界面，也不再依赖普通菜单选择回调；右键后使用 PAK 现有 `$提交物品` 提交道具机制。
- 提交窗口按钮为 `[提交法宝/$提交物品|/gs/daemons/fabao_hualingd.c,2,0]`。实测 `$*submit_item` 会走 easy_task，不适合作独立道具合成入口；普通 `$提交物品` 可回调 `fabao_hualingd.c` 的 `submit_items`。
- 当前版本同时保留 `submit_items` 直接处理和 `submit_items` 暂存兜底。提交框回传对象可能是 mapping，不能直接信 `keys(mapping)` 顺序；必须按物品 `position/get_pos` 从小到大还原提交格顺序：第一个为主法宝，第二个为副法宝。
- 按当前线上处理要求，不主动调用清暂存函数；只读取 `submit_items` 并在成功合成时消耗副法宝和化灵符、把主法宝放回背包。
- `attach_skilld.o` 不能用可变参数通用 relay 透传中文字符串，已改为显式 `call_other(BASE_ATTACH_SKILLD, ...)`。

## 2026-06-19 17:09 更新

本次更新已直接替换 `/home/gs/gs/pack_data/lib_gs32.pak` 并重启 GS2。

当前状态：

```text
GS2 8161 listening
pid: 64753
lib_gs32.pak: 8322a86b882599f9b2ca5e8a647d4cabc603833c7f08c2bbb3f546fb36fe5006
log:          /home/gs/gs2_fabao_hualing_more_skills_restart2_20260619_170945.log
```

本次对象：

```text
gs/daemons/fabao_hualingd.o  4a15699787937af596d6d0ff6895861aea7a9c43fc04315ae80e93fa9fa244e3
gs/daemons/attach_skilld.o   f8b5aad3c83ad524401033bccf332e4e2b17832a4806b0ae4f9e556e78d7fec8
```

修正点：

- 右键 `法宝化灵符` 后直接打开提交框，不再自动优先合成翻天印和混元金斗。
- 主副法宝完全按提交顺序判断：第一个提交的是主法宝，第二个提交的是副法宝。
- `attach_skilld.o` 保留 `FABAO_ATTACH_*` 调试输出，方便继续测试更多法宝。
- `has_dinghaizhu_effect` 已补充读取主法宝和第二被动里的 `dinghaizhu`。

## 2026-06-19 19:18 更新

本次更新已直接替换 `/home/gs/gs/pack_data/lib_gs32.pak` 并用 `/home/gs/rungs2` 重启 GS2。

当前状态：

```text
GS2 8161 listening
pid: 35901
lib_gs32.pak: d86b7cb70ac340cdcca0a39cb077c1d29ee982635694ff791993d8487d2858d3
log:          /home/gs/gs2_fabao_hualing_v10_position_cost_manual_20260619_191740.log
```

本次对象：

```text
gs/daemons/fabao_hualingd.o  686e6a0009ea143f2fadcf7c9275582530e62243954f3d568d80330e68f7ac15
```

修正点：

- 提交框返回的物品 mapping 键顺序不稳定，不能直接按 `keys(items)` 判断主副。当前按物品 `position/get_pos` 排序，提交框靠前位置固定为主法宝，靠后位置固定为副法宝。
- 右键使用时额外记录 `法宝化灵符` 的 iid；合成成功时如果临时对象失效，会重新从背包按 iid 找到化灵符再扣除，避免本次看似消失、下线重登又回来的问题。
- 合成成功后先尝试走玩家 `cost_submit_items` 扣副法宝，失败再直接 `cost_amount/destruct` 兜底。
- 调试输出继续保留 `FABAO_HUALING_*` 和 `FABAO_ATTACH_*`，后续扩展其他法宝时继续用。

当前 PAK 内同一套 `attach_skill` 被动模块可作为测试范围：

```text
dinghaizhu
fantianyin
hundun-zhong
hunyuan-jindou
jinjiaojian
jiulong-shenhuozhao
pangu-fan
qibao-miaoshu
shierpin-liantai
xiejia-jinhu
yinyangjing
```

## 2026-06-19 19:45 更新

本次更新已直接替换 `/home/gs/gs/pack_data/lib_gs32.pak` 并从 `/home/gs` 执行 `./rungs2` 重启 GS2。

当前状态：

```text
GS2 8161 listening
pid: 108847
lib_gs32.pak: aa7ee0fe9adb5711c98fb8281ff01351594bd7579f951ba6f52403c246a44c17
log:          /home/gs/gs2_fabao_hualing_v11_all_native_manual_20260619_194354.log
```

本次对象：

```text
gs/daemons/attach_skilld.o   580d3e153f9aad3f8a786eb70ab14598eca620939361299d4df667000b195c55
gs/daemons/fabao_hualingd.o  c536bcaed05ad6e591ee17be74deea9642cd5bcad61210c4ca7bc82726575f47
```

修正点：

- 合成不再限定番天印和混元金斗；只要主副法宝都有 `attach_skill`，并且对应 `/gs/daemons/attach_skill/<skill>.c` 原生模块存在，就允许化灵。
- 旧的自动兜底 `hunyuan-jindou` 已禁用，避免特殊路径把任意失败合成强行写成混元金斗。
- 主法宝原生被动继续交给原 `attach_skilld_base` 判定；包装器只对 `extra_attach_skill` 做独立触发率判定。
- 同一回合主被动和化灵附加被动可以各自判定、各自触发，互不清掉对方的 `attach_skills` 临时状态。
- 化灵附加技能仍调用对应原生模块的 `check_combat_condition`，所以不会跨场景触发：番天印只在法术五法/秒法上下文判定，混元金斗只在物攻/力破上下文判定，定海珠和九龙神火罩只在自身原生受击/障碍相关上下文判定。

## 2026-06-19 20:21 更新

本次更新只替换 `/gs/daemons/fabao_hualingd.o`，保留上一版已验证可加载的 `/gs/clone/item/special/fabao_hualing_fu.o`，避免 special_item 父类函数表 ABI 不匹配。

当前状态：

```text
GS2 8161 listening
pid: 73153
lib_gs32.pak: ead629d3f2a82e034ca71769b6765af1a023629969b2b90895c7b46035004cd6
log:          /home/gs/gs2_fabao_hualing_v13_daemon_only_20260619_202136.log
```

本次对象：

```text
gs/daemons/fabao_hualingd.o  7940076544f4779ea63a31692f6d936aa8df9d8b7030f27f10f124399cdfe721
```

修正点：

- 实机日志确认提交框回调 `items` 是 mapping，两个 value 都是 `1`，`keys(items)` 当前就是玩家提交顺序。上一版按 `position/get_pos` 排序会把“第一个番天印、第二个定海珠”反成定海珠主法宝，本版已撤销 position 排序。
- 主副规则改回：提交回调第一个 key 为主法宝，第二个 key 为副法宝。
- `open_merge_dialog` 打开提交框后返回 `0`，让右键打开菜单不被 special 道具系统视为立即使用成功；化灵符仍只在 `_finish_merge()` 合成成功后扣除。
- 曾尝试重编 `fabao_hualing_fu.o` 让 `apply()` 返回 0，但 live 加载报 `Bad program to inherit, functions (223) mismatched to (419)`，因此放弃替换符对象，改由 daemon 返回值处理。

## 上线前验证

建议先在 GS2 测：

1. 发放 `法宝化灵符`。
2. 右键使用，确认弹出提交法宝窗口。
3. 提交两个被动法宝：第一个主法宝，第二个副法宝。
4. 合成成功后确认副法宝和化灵符都被消耗。
5. 检查主法宝保存了 `extra_attach_skill`。
6. 下线重登后确认 `extra_attach_skill` 仍在。
7. 进入战斗确认 `me->query_temp("attach_skills")` 同时有主被动和第二被动。

## 编码注意

源码和文档按 UTF-8 保存。

`replace/etc/special_item.list` 与 `replace/etc/item_temp_desc.list` 是 GBK/CP936 候选文件，已按五法强化符原表编码追加，不能用 UTF-8 整表重写。
