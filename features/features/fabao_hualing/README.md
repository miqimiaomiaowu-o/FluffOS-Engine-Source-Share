# 法宝化灵

## 已验证效果

- 活动大使 NPC 菜单进入法宝化灵。
- 选择副法宝后弹 `#DLG:1#prompt` 确认框。
- 主菜单不再保留多余的“确认化灵”项。
- 第二被动走 `attach_skilld` 转发，保留原生动画路径。
- 分离恢复副法宝流程已按实测修过。

## 源码

- `source/gs/daemons/fabao_hualingd.c`
- `source/gs/daemons/attach_skilld.c`

## 替换路径

- `gs/daemons/fabao_hualingd.o`
- `gs/daemons/attach_skilld.o`

旧交付说明保存在 `README_legacy_delivery.md`。
