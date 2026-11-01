# 第三方依赖来源

| 目录 | 版本 | 来源 | 用途 | 授权 |
| --- | --- | --- | --- | --- |
| `lua-5.4.9/` | 5.4.9 | 取自同机项目 survivalv2 的 vendored 副本，未改动 | 内嵌脚本 | MIT |
| `raylib-6.2/` | 6.2 | 头文件与官方 6.2 相同；`lib/libraylib.a` 由 `tools/build_raylib.sh` 在 egg 上用官方源码和 egg 的 mingw 编译（survivalv2 的预编译库与 egg 的 mingw 不兼容，缺 `stat64i32`） | 窗口、输入、呈现 | zlib |
| `fusion-pixel/` | 2026.10.25 | 取自同机项目 survivalv2，BDF 未改动 | 12px 中文像素字体，构建时抽取用到的字形 | OFL 1.1 |
| `monocypher-4.0.2/` | 4.0.2 | monocypher.org 官方包，已核对 SHA-512 | 校验更新清单的 Ed25519 签名 | BSD-2-Clause 或 CC0 |
