# 死亡缠绵

## 内容

- 普通物理攻击和普通法术攻击触发 2 次攻击。

## 源码

- `source/gs/daemons/status/deadly_kiss.c`

## 服务端说明

用户提供的说明保存在：

- `notes/deadly_kiss_magic_physical_double.txt`

核心路径：删除原 `gs/daemons/status/deadly_kiss`，服务端新增 `gs/daemons/status/deadly_kiss.c` 后编译/替换。

## 编码

源文件为 GBK，可读中文按原始字节保留。
