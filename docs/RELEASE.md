# 发布与更新

对接用户自己的 release-publish（relhub）服务。服务运行在 ssh AI，egg 是镜像；客户端地址 `https://tony-ai-pc-1.tail9f7f1f.ts.net/release`，只在 tailnet 内可达。

## 现状

| 项目 | 状态 |
| --- | --- |
| 应用注册 | `lofta`（遗失的十一日），平台 `win-x64` |
| 发布令牌 | 应用专用令牌，在 AI 的 `~/ci/lofta/etc/relhub-token`，权限 600，不进仓库 |
| 线上发布 | **尚未发布**，见下面的“待决定” |
| 客户端 | `src/update/`，已在真实 Windows 上走通全流程 |

## 版本号

`VERSION` 文件是唯一来源，`版本码 = 主版本×10000 + 次版本×100 + 修订号`，修订号小于 100。`RELEASE=1` 构建时写入程序；不带它的是开发版，版本码 0，不检查更新。项目还不是 Git 仓库，所以没有用提交数算版本。

版本码在服务端只能升高，撤回的版本也算。发新版前先改 `VERSION`。

## 发布

```bash
tools/release.sh --channel beta --notes notes.md   # 在 egg 测试和构建，再到真实 Windows 验证，最后在 AI 发布
tools/release.sh --dry-run                         # 走完全部检查，不上传
```

`release.sh` 依次执行 `make test`、`RELEASE=1 win`、`crosscheck`、`smoke`，任何一步失败都不发布。发布脚本 `tools/publish-relhub.sh` 在 AI 上运行：同一文件不重复上传，版本码不够高时在上传前拒绝，发布后再用固定公钥以客户端身份核对一次。

渠道默认 `beta`，稳定后用 `relhub channel promote -app lofta -platform win-x64 -channel stable -code N`。

## 客户端行为

- 启动 20 秒后检查一次，之后每 6 小时一次。检查失败只写 `lofta_update.log`，不弹窗。
- 版本判定只看签名过的字段，下载地址由配置的基址和 `downloadPath` 拼成，路径必须在 `v1/apps/` 下。
- 新版先下载到程序旁的 `updates\`，核对大小和 SHA-256 才算就绪，然后在游戏里询问，绝不静默安装。
- 替换方式：运行中的 exe 改名为 `.old`，新文件移入，启动新进程后退出；任何一步失败都把旧程序放回。新进程启动时清掉 `.old` 和 `updates\`。
- 公钥固定在 `src/update/update_win.c`，地址和渠道可用环境变量 `LOFTA_UPDATE_URL`、`LOFTA_UPDATE_CHANNEL`、`LOFTA_UPDATE_KEYS` 覆盖，`LOFTA_UPDATE_URL=off` 关闭更新。
- 程序放在不可写的目录（如 Program Files）时无法自替换，这种情况记录原因后放弃。

## 测试

- `tests/unit_update.c`：协议单元测试，用一次性 relhub 的真实响应作夹具。包含 NIST SHA-256 向量、每个签名字段被改后都验不过、截断到任意长度都拒绝、路径和文件名注入。
- `tools/relhub_e2e.py`：在 WSL 起一次性 relhub，在真实 Windows 上跑 12 项：已是最新、错误公钥、只提示不安装、下载包被篡改、完整自替换。**绝不**对线上服务发测试包。

```bash
make RELEASE=1 VERSION_NAME=0.1.0 VERSION_CODE=100 WIN_DIR=build/e2e/co update-client   # 在 egg 上
make RELEASE=1 VERSION_NAME=0.1.1 VERSION_CODE=101 WIN_DIR=build/e2e/cn update-client
python3 tools/relhub_e2e.py --relhub <relhub> --old build/egg/e2e/co/update_client.exe \
    --new build/egg/e2e/cn/update_client.exe --name update_client.exe
```

`update_client.exe` 和游戏链接同一份 `update_win.c`、`relhub.c`，只是没有窗口。

## 待决定：Defender 误报

`Lofta.exe` 被 Windows Defender 判为 `Program:Win32/Contebrew.A!ml` 并隔离。这是机器学习启发式误报，构建之间结果不稳定。在它解决前 `release.sh` 的 smoke 步骤会失败，这是故意的：不把在 Windows 上启动不了的包发出去。

可选做法，都需要用户决定：

1. 把样本提交微软申诉误报（免费，针对具体构建，后续构建可能还要再提交）。
2. 购买代码签名证书。最稳，但有成本，且新证书也要积累信誉。
3. 只在自己的机器上加排除项。只解决开发机，不解决分发。

游戏内容进入 exe 后体积会明显变大，判定也可能变化（survivalv2 改名重建后恢复过），但不能依赖这一点。
