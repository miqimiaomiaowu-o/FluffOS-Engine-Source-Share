# 修罗怒击降魔斩同时触发

## 已验证内容

这套历史成功包主要是直接替换 `.o`，不是完整 LPC 源码工程。

`verified/README_success_package.md` 记录了已部署成功版本：

- `receive.o` v8
- `receivf.o` 干净原始 base
- `god_boo2d.o` v29
- `god_book.o` v27
- `god_boo1d.o` v15
- `god_boo0_status.o`
- `status/god_boo0.o`

## 补丁脚本

`patch_scripts/` 保留了当时用于构建/修补法术连击链路的 Python 脚本。它们属于二进制补丁脚本，不是普通 LPC 源码。

## 替换路径

成功包对应的对象路径见：

- `verified/README_success_package.md`
- `verified/SHA256SUMS.txt`
