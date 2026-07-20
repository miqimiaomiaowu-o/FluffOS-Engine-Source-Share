# direct_replace_o_current_v29_v8_20260617

这套是 2026-06-17 当前服务器已部署的直接替换 `.o` 文件。

核心状态：
- `gs/daemons/combat/receive.o`: v8，保留首段和修罗后续伤害接管。
- `gs/daemons/combat/receivf.o`: 干净原始 base，用来避免 `receive` 代理递归卡回合。
- `gs/daemons/god_boo2d.o`: v29，只加修罗术优先保护；修罗先命中后，后续怒击/降魔斩不再冲掉原生修罗链。
- `gs/daemons/status/god_book.o`: v27，保留修罗/怒击/降魔斩 UI 显示。
- `gs/daemons/god_boo1d.o`: v15，同伤害真实书识别配套。
- `gs/daemons/god_boo0_status.o` 和 `gs/daemons/status/god_boo0.o`: 当前配套对象。

直接替换时按目录结构覆盖到 pak 对应路径。
