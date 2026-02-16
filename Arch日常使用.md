[极客技术博客](https://geek-blogs.com/)

- [主页](https://geek-blogs.com/)
- [在线C语言编译器](https://geek-blogs.com/online-c-compiler/)

[](https://geek-blogs.com/rss.xml)

2025年10月11日

# Arch Linux 命令详解：从入门到精通的技术指南

Arch Linux 作为一款以简洁、轻量和高度可定制著称的 Linux 发行版，其设计哲学遵循 **KISS（Keep It Simple, Stupid）原则**，即“保持简单”。与其他预配置发行版（如 Ubuntu、Fedora）不同，Arch 要求用户主动参与系统的构建与维护，这也意味着**熟练掌握命令行工具**是使用 Arch Linux 的核心技能。无论是系统安装、软件管理，还是日常维护、故障排查，命令行都是最直接、高效的交互方式。

本文将系统梳理 Arch Linux 中最常用的命令，涵盖**包管理、系统维护、用户权限、文件操作、进程管理、网络配置**等核心场景，并结合最佳实践和示例，帮助读者从“会用”到“用好”，真正发挥 Arch Linux 的灵活性与强大功能。

## 目录[#](https://geek-blogs.com/blog/arch-linux-commands/#mu4-lu4)

1. [包管理：Pacman 核心操作](https://geek-blogs.com/blog/arch-linux-commands/#1-%E5%8C%85%E7%AE%A1%E7%90%86pacman-%E6%A0%B8%E5%BF%83%E6%93%8D%E4%BD%9C)
    
    - 1.1 安装、更新与卸载软件
    - 1.2 搜索与查询软件信息
    - 1.3 缓存清理与依赖管理
    - 1.4 最佳实践：避免部分升级与配置备份
2. [系统维护：保障稳定性与性能](https://geek-blogs.com/blog/arch-linux-commands/#2-%E7%B3%BB%E7%BB%9F%E7%BB%B4%E6%8A%A4%E4%BF%9D%E9%9A%9C%E7%A8%B3%E5%AE%9A%E6%80%A7%E4%B8%8E%E6%80%A7%E8%83%BD)
    
    - 2.1 服务管理（systemd）
    - 2.2 日志查看与分析（journalctl）
    - 2.3 内核与 initramfs 管理
    - 2.4 磁盘维护（fstrim、文件系统检查）
3. [用户与权限管理](https://geek-blogs.com/blog/arch-linux-commands/#3-%E7%94%A8%E6%88%B7%E4%B8%8E%E6%9D%83%E9%99%90%E7%AE%A1%E7%90%86)
    
    - 3.1 用户创建与修改（useradd、usermod）
    - 3.2 权限控制（chmod、chown）
    - 3.3 sudo 配置与使用
4. [文件系统操作：高效管理文件与目录](https://geek-blogs.com/blog/arch-linux-commands/#4-%E6%96%87%E4%BB%B6%E7%B3%BB%E7%BB%9F%E6%93%8D%E4%BD%9C%E9%AB%98%E6%95%88%E7%AE%A1%E7%90%86%E6%96%87%E4%BB%B6%E4%B8%8E%E7%9B%AE%E5%BD%95)
    
    - 4.1 基础导航与查看（ls、cd、pwd）
    - 4.2 文件操作（cp、mv、rm、mkdir）
    - 4.3 磁盘空间与挂载管理（df、du、mount）
5. [进程管理：监控与控制系统资源](https://geek-blogs.com/blog/arch-linux-commands/#5-%E8%BF%9B%E7%A8%8B%E7%AE%A1%E7%90%86%E7%9B%91%E6%8E%A7%E4%B8%8E%E6%8E%A7%E5%88%B6%E7%B3%BB%E7%BB%9F%E8%B5%84%E6%BA%90)
    
    - 5.1 进程查看（ps、top、htop）
    - 5.2 进程终止与优先级调整（kill、nice）
6. [网络管理：配置与故障排查](https://geek-blogs.com/blog/arch-linux-commands/#6-%E7%BD%91%E7%BB%9C%E7%AE%A1%E7%90%86%E9%85%8D%E7%BD%AE%E4%B8%8E%E6%95%85%E9%9A%9C%E6%8E%92%E6%9F%A5)
    
    - 6.1 网络接口与连接状态（ip 命令）
    - 6.2 网络诊断工具（ping、ss、curl）
    - 6.3 防火墙配置（nftables）
7. [AUR 与第三方软件管理](https://geek-blogs.com/blog/arch-linux-commands/#7-aur-%E4%B8%8E%E7%AC%AC%E4%B8%89%E6%96%B9%E8%BD%AF%E4%BB%B6%E7%AE%A1%E7%90%86)
    
    - 7.1 AUR 助手：yay/paru 基础操作
    - 7.2 最佳实践：验证 PKGBUILD 与安全性
8. [参考资料](https://geek-blogs.com/blog/arch-linux-commands/#%E5%8F%82%E8%80%83%E8%B5%84%E6%96%99)
    

## 1. 包管理：Pacman 核心操作[#](https://geek-blogs.com/blog/arch-linux-commands/#1-bao1-guan3-li3-pacman-he2-xin1-cao1-zuo4)

Pacman 是 Arch Linux 的官方包管理器，负责软件的安装、更新、卸载及依赖处理。其设计简洁高效，支持增量更新与多仓库配置，是 Arch 生态的基石。

### 1.1 安装、更新与卸载软件[#](https://geek-blogs.com/blog/arch-linux-commands/#11-an1-zhuang1-geng1-xin1-yu3-xie4-zai4-ruan3-jian4)

#### 核心命令速查表[#](https://geek-blogs.com/blog/arch-linux-commands/#he2-xin1-ming4-ling4-su4-cha2-biao3)

|命令|功能描述|
|---|---|
|`sudo pacman -S <包名>`|安装指定软件（从官方仓库）|
|`sudo pacman -Syu`|同步仓库并更新系统（**必学！**）|
|`sudo pacman -R <包名>`|卸载软件（保留依赖）|
|`sudo pacman -Rs <包名>`|卸载软件并移除其未被其他包依赖的依赖|
|`sudo pacman -Rns <包名>`|彻底卸载（移除软件、依赖及配置文件）|

#### 示例与场景[#](https://geek-blogs.com/blog/arch-linux-commands/#shi4-li4-yu3-chang3-jing3)

- **更新系统**（最常用操作，建议每周执行）：
    
    ```
    sudo pacman -Syu  # -S：同步仓库，-y：刷新包数据库，-u：升级已安装包
    ```
    
    > **注意**：Arch 是滚动发行版，`-Syu` 必须完整执行，不可拆分（如单独 `pacman -Sy` 后安装软件可能导致**部分升级**，引发依赖冲突）。
    
- **安装软件**（如安装文本编辑器 `neovim`）：
    
    ```
    sudo pacman -S neovim  # 若有多个版本，会提示选择（如不同架构或分支）
    ```
    
- **卸载软件**（如彻底清理不再使用的 `firefox`）：
    
    ```
    sudo pacman -Rns firefox  # -n：删除配置文件，-s：删除未被依赖的依赖
    ```
    

### 1.2 搜索与查询软件信息[#](https://geek-blogs.com/blog/arch-linux-commands/#12-sou1-suo3-yu3-cha2-xun2-ruan3-jian4-xin4-xi1)

当需要查找某个功能对应的软件，或了解已安装包的详情时，可使用以下命令：

|命令|功能描述|
|---|---|
|`pacman -Ss <关键词>`|搜索仓库中的软件（含描述）|
|`pacman -Qi <包名>`|查看已安装包的详细信息（版本、依赖、大小）|
|`pacman -Ql <包名>`|列出已安装包的所有文件路径|
|`pacman -Qo <文件路径>`|查看某个文件属于哪个已安装包|

#### 示例[#](https://geek-blogs.com/blog/arch-linux-commands/#shi4-li4)

- **搜索“截图工具”**：
    
    ```
    pacman -Ss screenshot  # 输出包含关键词的包，如 flameshot、gnome-screenshot
    ```
    
- **查看已安装的 `linux` 内核信息**：
    
    ```
    pacman -Qi linux  # 显示版本（如 6.5.5-arch1-1）、安装时间、依赖内核模块等
    ```
    
- **定位 `/usr/bin/ls` 属于哪个包**：
    
    ```
    pacman -Qo /usr/bin/ls  # 输出：/usr/bin/ls 属于 coreutils 9.4-1
    ```
    

### 1.3 缓存清理与依赖管理[#](https://geek-blogs.com/blog/arch-linux-commands/#13-huan3-cun2-qing1-li3-yu3-yi1-lai4-guan3-li3)

Pacman 会将下载的包文件缓存至 `/var/cache/pacman/pkg/`，长期不清理会占用磁盘空间。同时，系统中可能存在“孤儿依赖”（不再被任何包需要的依赖），需定期清理。

#### 常用命令[#](https://geek-blogs.com/blog/arch-linux-commands/#chang2-yong4-ming4-ling4)

|命令|功能描述|
|---|---|
|`sudo pacman -Sc`|清理旧版本缓存（保留当前版本）|
|`sudo pacman -Scc`|彻底清理所有缓存（谨慎使用！）|
|`sudo pacman -Qdt`|列出孤儿依赖|
|`sudo pacman -Rns $(pacman -Qdtq)`|自动移除孤儿依赖|

#### 最佳实践[#](https://geek-blogs.com/blog/arch-linux-commands/#zui4-jia1-shi2-jian4)

- **缓存清理**：建议每月执行 `sudo pacman -Sc`，若磁盘空间紧张可配合 `paccache` 工具（需安装 `pacman-contrib`）：
    
    ```
    sudo paccache -rk1  # 保留每个包的最近1个版本缓存
    ```
    
- **依赖清理**：卸载软件时优先使用 `-Rs` 或 `-Rns`，避免孤儿依赖堆积。

### 1.4 最佳实践：避免部分升级与配置备份[#](https://geek-blogs.com/blog/arch-linux-commands/#14-zui4-jia1-shi2-jian4-bi4-mian3-bu4-fen4-sheng1-ji2-yu3-pei4-zhi4-bei4-fen4)

- **严禁“部分升级”**：永远不要单独执行 `pacman -Sy` 后直接安装软件（如 `pacman -Sy <包名>`）。这会导致新安装的包与系统中旧版本库文件不兼容，引发运行时错误（如 `GLIBC` 版本冲突）。**正确流程**：先 `pacman -Syu` 更新系统，再安装新软件。
    
- **配置文件备份**：Pacman 升级时，若配置文件（如 `/etc/pacman.conf`、`/etc/fstab`）被修改，会提示 `(.pacnew)` 文件冲突。此时需手动对比合并（推荐工具 `meld` 或 `vimdiff`），避免直接覆盖自定义配置。
    

## 2. 系统维护：保障稳定性与性能[#](https://geek-blogs.com/blog/arch-linux-commands/#2-xi4-tong3-wei2-hu4-bao3-zhang4-wen3-ding4-xing4-yu3-xing4-neng2)

Arch Linux 作为滚动发行版，系统维护的核心是**监控状态、处理更新残留、保障启动与运行稳定**。以下是关键操作。

### 2.1 服务管理（systemd）[#](https://geek-blogs.com/blog/arch-linux-commands/#21-fu2-wu4-guan3-li3-systemd)

Arch 默认使用 `systemd` 作为 init 系统，负责服务启动、进程管理与系统状态监控。`systemctl` 是其核心命令行工具。

#### 常用命令[#](https://geek-blogs.com/blog/arch-linux-commands/#chang2-yong4-ming4-ling4)

|命令|功能描述|
|---|---|
|`systemctl status <服务名>`|查看服务状态（运行中/停止/失败）|
|`sudo systemctl start <服务名>`|启动服务（临时生效）|
|`sudo systemctl enable <服务名>`|设置服务开机自启（永久生效）|
|`sudo systemctl disable <服务名>`|禁用开机自启|
|`sudo systemctl restart <服务名>`|重启服务（配置修改后常用）|
|`systemctl list-unit-files --type=service`|列出所有服务及自启状态|

#### 示例[#](https://geek-blogs.com/blog/arch-linux-commands/#shi4-li4)

- **查看 SSH 服务状态**（假设使用 `sshd`）：
    
    ```
    systemctl status sshd
    # 输出包含：Active: active (running) （运行中）或 inactive (dead) （已停止）
    ```
    
- **设置 Docker 开机自启并启动**：
    
    ```
    sudo systemctl enable --now docker  # --now：同时生效“启动”和“开机自启”
    ```
    

### 2.2 日志查看与分析（journalctl）[#](https://geek-blogs.com/blog/arch-linux-commands/#22-ri4-zhi4-cha2-kan4-yu3-fen1-xi1-journalctl)

`journalctl` 是 `systemd` 的日志管理工具，集中存储系统、服务及内核日志，支持按时间、服务、优先级筛选，是故障排查的利器。

#### 常用命令[#](https://geek-blogs.com/blog/arch-linux-commands/#chang2-yong4-ming4-ling4)

|命令|功能描述|
|---|---|
|`journalctl`|查看所有日志（按时间倒序，需分页）|
|`journalctl -u <服务名>`|查看指定服务的日志（如 `journalctl -u sshd`）|
|`journalctl -p err`|只显示错误级别（priority=err）日志|
|`journalctl --since "10min ago"`|查看最近10分钟的日志|
|`journalctl -b`|查看当前启动周期的日志（`-b -1` 为上一次启动）|

#### 示例：排查服务启动失败[#](https://geek-blogs.com/blog/arch-linux-commands/#shi4-li4-pai2-cha2-fu2-wu4-qi3-dong4-shi1-bai4)

若 `nginx` 启动失败，可通过以下命令定位原因：

```
journalctl -u nginx -p err --no-pager  # 查看 nginx 的错误日志，不分页
# 可能输出：bind() to 0.0.0.0:80 failed (98: Address already in use) （端口被占用）
```

### 2.3 内核与 initramfs 管理[#](https://geek-blogs.com/blog/arch-linux-commands/#23-nei4-he2-yu3-initramfs-guan3-li3)

内核更新是 Arch 系统更新的常规部分，但有时需手动处理 initramfs（初始化内存文件系统）或内核版本切换。

#### 关键命令[#](https://geek-blogs.com/blog/arch-linux-commands/#guan1-jian4-ming4-ling4)

|命令|功能描述|
|---|---|
|`mkinitcpio -P`|重新生成所有内核的 initramfs（内核升级后必做）|
|`uname -r`|查看当前运行的内核版本|
|`pacman -Q|grep linux`|

#### 最佳实践[#](https://geek-blogs.com/blog/arch-linux-commands/#zui4-jia1-shi2-jian4)

- **内核升级后**：若修改了 `/etc/mkinitcpio.conf`（如添加加密模块、驱动），需执行 `sudo mkinitcpio -P` 重新生成 initramfs，否则可能导致启动失败。
    
- **多内核备份**：建议同时安装 LTS（长期支持）内核（`sudo pacman -S linux-lts`），当主线内核因更新出现兼容性问题时，可从 GRUB 菜单选择 LTS 内核启动。
    

### 2.4 磁盘维护（fstrim、文件系统检查）[#](https://geek-blogs.com/blog/arch-linux-commands/#24-ci2-pan2-wei2-hu4-fstrim-wen2-jian4-xi4-tong3-jian3-cha2)

对于 SSD 用户，`fstrim` 可释放未使用的块，提升性能并延长寿命；文件系统检查（`fsck`）则用于修复磁盘错误。

#### 常用命令[#](https://geek-blogs.com/blog/arch-linux-commands/#chang2-yong4-ming4-ling4)

|命令|功能描述|
|---|---|
|`sudo fstrim -av`|对所有支持 TRIM 的挂载点执行清理（SSD 推荐）|
|`sudo fsck -f /dev/sda1`|强制检查 `/dev/sda1` 文件系统（需卸载后执行）|
|`df -h`|查看磁盘空间使用情况（-h：人类可读单位）|

#### 最佳实践[#](https://geek-blogs.com/blog/arch-linux-commands/#zui4-jia1-shi2-jian4)

- **SSD 维护**：启用 `fstrim.timer` 服务，实现自动定期清理：
    
    ```
    sudo systemctl enable --now fstrim.timer  # 默认每周日执行一次
    ```
    
- **文件系统检查**：若系统异常关机，`systemd` 会自动在下次启动时触发 `fsck`；手动检查需确保分区未挂载（如通过 Live CD 启动检查根分区）。
    

## 3. 用户与权限管理[#](https://geek-blogs.com/blog/arch-linux-commands/#3-yong4-hu4-yu3-quan2-xian4-guan3-li3)

Linux 是多用户系统，严格的权限控制是安全性的基础。Arch 中默认禁用 root 直接登录，需通过 `sudo` 提权，或创建普通用户并分配权限。

### 3.1 用户创建与修改[#](https://geek-blogs.com/blog/arch-linux-commands/#31-yong4-hu4-chuang4-jian4-yu3-xiu1-gai3)

#### 核心命令[#](https://geek-blogs.com/blog/arch-linux-commands/#he2-xin1-ming4-ling4)

|命令|功能描述|
|---|---|
|`sudo useradd -m -G wheel <用户名>`|创建用户并添加到 wheel 组（sudo 权限前提）|
|`sudo passwd <用户名>`|设置用户密码|
|`sudo usermod -aG <组名> <用户名>`|将用户添加到附加组（如 docker、video）|
|`id <用户名>`|查看用户所属组信息|

#### 示例：创建可使用 sudo 的用户[#](https://geek-blogs.com/blog/arch-linux-commands/#shi4-li4-chuang4-jian4-ke3-shi3-yong4-sudo-de-yong4-hu4)

```
sudo useradd -m -G wheel archuser  # -m：自动创建家目录，-G：附加组 wheel
sudo passwd archuser  # 按提示输入密码
```

> **关键**：`wheel` 组是默认允许使用 `sudo` 的用户组，需确保 `/etc/sudoers` 中启用（通过 `visudo` 编辑，取消 `%wheel ALL=(ALL:ALL) ALL` 行的注释）。

### 3.2 权限控制（chmod、chown）[#](https://geek-blogs.com/blog/arch-linux-commands/#32-quan2-xian4-kong4-zhi4-chmodchown)

Linux 文件权限分为 **所有者（user）、所属组（group）、其他用户（other）**，每组包含读（r=4）、写（w=2）、执行（x=1）权限。`chmod` 用于修改权限，`chown` 用于修改所有者/所属组。

#### `chmod`：修改权限[#](https://geek-blogs.com/blog/arch-linux-commands/#chmod-xiu1-gai3-quan2-xian4)

- **符号表示法**：`chmod [u/g/o/a][+/-/=][r/w/x] <文件>`  
    示例：
    
    ```
    chmod u+x script.sh  # 给所有者添加执行权限
    chmod g-w,o-r file.txt  # 移除所属组的写权限，移除其他用户的读权限
    chmod a=rwx app  # 给所有用户（a=all）添加读、写、执行权限
    ```
    
- **八进制表示法**（更简洁）：`chmod <权限值> <文件>`  
    权限值 = 所有者权限 + 所属组权限 + 其他用户权限（如 `755` = `rwxr-xr-x`）。  
    示例：
    
    ```
    chmod 755 script.sh  # 所有者：rwx (7)，组：r-x (5)，其他：r-x (5)
    chmod 600 secret.txt  # 仅所有者可读写（rw-------）
    ```
    

#### `chown`：修改所有者/所属组[#](https://geek-blogs.com/blog/arch-linux-commands/#chown-xiu1-gai3-suo3-you3-zhe3-suo3-shu3-zu3)

```
sudo chown archuser:archuser file.txt  # 所有者改为 archuser，所属组改为 archuser 组
sudo chown root:wheel /opt/app  # 所有者 root，所属组 wheel
```

### 3.3 sudo 配置与使用[#](https://geek-blogs.com/blog/arch-linux-commands/#33-sudo-pei4-zhi4-yu3-shi3-yong4)

`sudo` 允许普通用户以 root 权限执行命令，避免直接使用 root 登录带来的风险。配置文件为 `/etc/sudoers`，**必须通过 `visudo` 命令编辑**（自动语法检查，防止配置错误导致无法提权）。

#### 常用配置项（通过 `sudo visudo` 编辑）[#](https://geek-blogs.com/blog/arch-linux-commands/#chang2-yong4-pei4-zhi4-xiang4-tong1-guo4-sudo-visudo-bian1-ji2)

- 允许 wheel 组用户无密码执行所有命令（需谨慎）：
    
    ```
    %wheel ALL=(ALL:ALL) NOPASSWD: ALL
    ```
    
- 限制用户仅能执行特定命令（如允许 `archuser` 重启网络）：
    
    ```
    archuser ALL=(ALL:ALL) /usr/bin/systemctl restart NetworkManager
    ```
    

## 4. 文件系统操作：高效管理文件与目录[#](https://geek-blogs.com/blog/arch-linux-commands/#4-wen2-jian4-xi4-tong3-cao1-zuo4-gao1-xiao4-guan3-li3-wen2-jian4-yu3-mu4-lu4)

文件系统操作是命令行的基础，以下是日常使用频率最高的命令及最佳实践。

### 4.1 基础导航与查看[#](https://geek-blogs.com/blog/arch-linux-commands/#41-ji1-chu3-dao3-hang2-yu3-cha2-kan4)

|命令|功能描述|
|---|---|
|`pwd`|显示当前工作目录路径|
|`cd <目录>`|切换目录（`cd ~` 回 home，`cd ..` 回上级）|
|`ls`|列出目录内容（默认当前目录）|
|`ls -l`|详细列表（权限、所有者、大小、修改时间）|
|`ls -a`|显示隐藏文件（以 `.` 开头的文件）|
|`ls -lh`|人类可读大小（如 1K、2M、3G）|

### 4.2 文件操作：复制、移动、删除与创建[#](https://geek-blogs.com/blog/arch-linux-commands/#42-wen2-jian4-cao1-zuo4-fu4-zhi4-yi2-dong4-shan1-chu2-yu3-chuang4-jian4)

#### 核心命令[#](https://geek-blogs.com/blog/arch-linux-commands/#he2-xin1-ming4-ling4)

|命令|功能描述|
|---|---|
|`cp <源文件> <目标路径>`|复制文件（`cp -r` 复制目录，`cp -i` 覆盖前提示）|
|`mv <源文件> <目标路径>`|移动/重命名文件（`mv old.txt new.txt` 重命名）|
|`rm <文件>`|删除文件（`rm -r` 删除目录，`rm -f` 强制删除）|
|`mkdir <目录名>`|创建目录（`mkdir -p a/b/c` 创建多级目录）|

#### 最佳实践[#](https://geek-blogs.com/blog/arch-linux-commands/#zui4-jia1-shi2-jian4)

- **复制/移动时避免覆盖**：添加 `-i` 参数（交互模式），防止误操作：
    
    ```
    cp -i file.txt /backup/  # 若目标已存在，会提示是否覆盖
    ```
    
- **删除时谨慎使用 `-rf`**：`rm -rf` 会递归强制删除目录及内容，且无法恢复。建议先通过 `ls` 确认目录内容，或使用 `-i` 逐步确认：
    
    ```
    rm -ri old_dir/  # 递归删除，每个文件都提示确认
    ```
    

### 4.3 磁盘空间与挂载管理[#](https://geek-blogs.com/blog/arch-linux-commands/#43-ci2-pan2-kong1-jian1-yu3-gua4-zai3-guan3-li3)

#### 查看磁盘使用情况[#](https://geek-blogs.com/blog/arch-linux-commands/#cha2-kan4-ci2-pan2-shi3-yong4-qing2-kuang4)

```
df -h  # 查看所有挂载点的空间使用（-h：人类可读单位）
du -sh <目录>  # 查看目录总大小（-s：汇总，-h：可读）
# 示例：查看 home 目录占用
du -sh ~/
```

#### 挂载与卸载设备[#](https://geek-blogs.com/blog/arch-linux-commands/#gua4-zai3-yu3-xie4-zai4-she4-bei4)

挂载（`mount`）用于将外部存储（如 U 盘、移动硬盘）或分区接入系统目录树；卸载（`umount`）则是安全移除前的必要操作。

- **挂载 U 盘**（假设设备为 `/dev/sdb1`，挂载点 `/mnt/usb`）：
    
    ```
    sudo mkdir -p /mnt/usb
    sudo mount /dev/sdb1 /mnt/usb  # 默认按文件系统类型自动挂载（如 ext4、vfat）
    ```
    
- **卸载设备**（确保无人访问该目录）：
    
    ```
    sudo umount /mnt/usb  # 或直接卸载设备：sudo umount /dev/sdb1
    ```
    

> **注意**：若提示“目标忙”（target is busy），需先退出挂载点目录（如 `cd ~`）或关闭访问该目录的程序。

## 5. 进程管理：监控与控制系统资源[#](https://geek-blogs.com/blog/arch-linux-commands/#5-jin4-cheng2-guan3-li3-jian1-kong4-yu3-kong4-zhi4-xi4-tong3-zi1-yuan2)

进程是系统运行的基本单位，管理进程可优化资源占用、排查卡顿或崩溃问题。

### 5.1 进程查看：ps、top、htop[#](https://geek-blogs.com/blog/arch-linux-commands/#51-jin4-cheng2-cha2-kan4-pstophtop)

#### `ps`：静态查看进程快照[#](https://geek-blogs.com/blog/arch-linux-commands/#ps-jing4-tai4-cha2-kan4-jin4-cheng2-kuai4-zhao4)

```
ps aux  # 查看所有用户的所有进程（a：所有终端，u：用户信息，x：无终端进程）
ps aux | grep firefox  # 过滤包含“firefox”的进程
```

#### `top`/`htop`：动态监控进程（推荐 `htop`，需安装）[#](https://geek-blogs.com/blog/arch-linux-commands/#top-htop-dong4-tai4-jian1-kong4-jin4-cheng2-tui1-jian4-htop-xu1-an1-zhuang1)

`top` 是基础动态监控工具，`htop` 则提供彩色界面、鼠标交互和更直观的资源占用显示（安装：`sudo pacman -S htop`）。

- 启动 `htop`：
    
    ```
    htop
    ```
    
    功能键：
    - F5：树形显示进程关系
    - F6：按 CPU/内存/用户等排序
    - F9：发送信号（如终止进程）

### 5.2 进程终止与优先级调整[#](https://geek-blogs.com/blog/arch-linux-commands/#52-jin4-cheng2-zhong1-zhi3-yu3-you1-xian1-ji2-tiao2-zheng3)

#### 终止进程：`kill`/`pkill`[#](https://geek-blogs.com/blog/arch-linux-commands/#zhong1-zhi3-jin4-cheng2-kill-pkill)

每个进程有唯一 PID（进程 ID），可通过 `ps` 或 `htop` 获取。`kill` 向进程发送信号，常用信号：

- `SIGTERM (15)`：请求进程正常退出（默认信号，推荐优先使用）
- `SIGKILL (9)`：强制终止进程（无法被忽略，万不得已时使用）

示例：

```
kill 1234  # 向 PID=1234 的进程发送 SIGTERM
kill -9 1234  # 强制终止 PID=1234 的进程
pkill firefox  # 终止所有名为“firefox”的进程（无需查 PID）
```

#### 调整进程优先级：`nice`/`renice`[#](https://geek-blogs.com/blog/arch-linux-commands/#tiao2-zheng3-jin4-cheng2-you1-xian1-ji2-nice-renice)

Linux 进程优先级通过“nice 值”表示（范围：-20 ~ 19，值越小优先级越高）。默认进程 nice 值为 0。

- **启动时设置优先级**（`nice`）：
    
    ```
    nice -n 5 ./app  # 以 nice 值 5 启动 app（低优先级）
    sudo nice -n -10 ./server  # 以高优先级启动 server（需 root 权限）
    ```
    
- **调整运行中进程优先级**（`renice`）：
    
    ```
    renice 10 -p 1234  # 将 PID=1234 的进程 nice 值改为 10（降低优先级）
    ```
    

## 6. 网络管理：配置与故障排查[#](https://geek-blogs.com/blog/arch-linux-commands/#6-wang3-luo4-guan3-li3-pei4-zhi4-yu3-gu4-zhang4-pai2-cha2)

Arch Linux 网络配置默认依赖 `systemd-networkd` 或 `NetworkManager`（桌面环境常用），命令行工具则以 `ip` 为核心（替代老旧的 `ifconfig`）。

### 6.1 网络接口与连接状态（`ip` 命令）[#](https://geek-blogs.com/blog/arch-linux-commands/#61-wang3-luo4-jie1-kou3-yu3-lian2-jie1-zhuang4-tai4-ip-ming4-ling4)

`ip` 命令整合了网络接口、路由、ARP 等功能，语法清晰，功能强大。

#### 常用操作[#](https://geek-blogs.com/blog/arch-linux-commands/#chang2-yong4-cao1-zuo4)

```
ip addr show  # 查看所有网络接口的 IP 地址（简写：ip a）
ip link set <接口名> up/down  # 启用/禁用接口（如 ip link set eth0 up）
ip route show  # 查看路由表（默认网关：default via ...）
ip neigh  # 查看 ARP 缓存（局域网设备 IP 与 MAC 映射）
```

### 6.2 网络诊断工具[#](https://geek-blogs.com/blog/arch-linux-commands/#62-wang3-luo4-zhen3-duan4-gong1-ju4)

#### 测试网络连通性[#](https://geek-blogs.com/blog/arch-linux-commands/#ce4-shi4-wang3-luo4-lian2-tong1-xing4)

```
ping -c 4 8.8.8.8  # 向 Google DNS 发送 4 个 ICMP 包（测试连通性）
curl https://archlinux.org  # 测试 HTTP 访问（或 wget https://archlinux.org）
```

#### 查看端口占用（`ss` 替代 `netstat`）[#](https://geek-blogs.com/blog/arch-linux-commands/#cha2-kan4-duan1-kou3-zhan4-yong4-ss-ti4-dai4-netstat)

```
ss -tuln  # 查看所有监听的 TCP/UDP 端口（t：TCP，u：UDP，l：监听，n：数字端口）
# 示例：查找 8080 端口被哪个进程占用
ss -tuln | grep 8080  # 先确认端口状态，再用 lsof 查进程
sudo lsof -i :8080  # 显示占用 8080 端口的进程（需安装 lsof）
```

### 6.3 防火墙配置（nftables）[#](https://geek-blogs.com/blog/arch-linux-commands/#63-fang2-huo3-qiang2-pei4-zhi4-nftables)

`nftables` 是 Linux 新一代防火墙框架，替代老旧的 `iptables`，语法更简洁，支持动态规则。Arch 默认未启用防火墙，建议安装并配置基础规则。

#### 基础操作（需安装 `nftables` 包）[#](https://geek-blogs.com/blog/arch-linux-commands/#ji1-chu3-cao1-zuo4-xu1-an1-zhuang1-nftables-bao1)

```
sudo systemctl enable --now nftables  # 启用并启动服务
sudo nft list ruleset  # 查看当前规则集（默认空）
```

#### 添加基础规则（允许 SSH、HTTP/HTTPS 入站）[#](https://geek-blogs.com/blog/arch-linux-commands/#tian1-jia1-ji1-chu3-gui1-ze2-yun3-xu3-sshhttphttps-ru4-zhan4)

```
sudo nft add table inet filter  # 创建名为 filter 的表（inet：IPv4+IPv6）
sudo nft add chain inet filter input '{ type filter hook input priority 0; policy drop; }'  # 入站链默认丢弃
# 允许已建立的连接和回环接口
sudo nft add rule inet filter input ct state related,established accept
sudo nft add rule inet filter input iif lo accept
# 允许 SSH（22 端口）、HTTP（80）、HTTPS（443）
sudo nft add rule inet filter input tcp dport {22, 80, 443} accept
```

> **注意**：规则配置后需保存，否则重启后丢失：
> 
> ```
> sudo nft list ruleset > /etc/nftables.conf  # 保存到配置文件
> ```

## 7. AUR 与第三方软件管理[#](https://geek-blogs.com/blog/arch-linux-commands/#7-aur-yu3-di4-san1-fang1-ruan3-jian4-guan3-li3)

AUR（Arch User Repository）是 Arch 社区驱动的非官方软件仓库，包含大量官方仓库未收录的软件（如闭源工具、最新版本应用）。由于 AUR 包由用户提交，需通过 **AUR 助手**（如 `yay`、`paru`）简化安装流程。

### 7.1 AUR 助手：yay 基础操作[#](https://geek-blogs.com/blog/arch-linux-commands/#71-aur-zhu4-shou3-yay-ji1-chu3-cao1-zuo4)

`yay` 是最流行的 AUR 助手之一，兼容 Pacman 命令格式，支持自动解决依赖与构建软件包。

#### 安装 yay（需先安装基础构建工具）[#](https://geek-blogs.com/blog/arch-linux-commands/#an1-zhuang1-yay-xu1-xian1-an1-zhuang1-ji1-chu3-gou4-jian4-gong1-ju4)

```
sudo pacman -S --needed base-devel git  # 安装基础依赖
git clone https://aur.archlinux.org/yay.git  # 克隆 yay 源码
cd yay && makepkg -si  # 构建并安装（-s：自动安装依赖，-i：安装生成的包）
```

#### 核心命令（与 Pacman 类似）[#](https://geek-blogs.com/blog/arch-linux-commands/#he2-xin1-ming4-ling4-yu3-pacman-lei4-si4)

|命令|功能描述|
|---|---|
|`yay -S <包名>`|安装 AUR 或官方仓库软件（自动区分）|
|`yay -Syu`|更新系统（包含官方仓库与 AUR 包）|
|`yay -Rns <包名>`|卸载软件（同 Pacman）|
|`yay -Ss <关键词>`|搜索官方仓库与 AUR 包|

### 7.2 最佳实践：安全使用 AUR[#](https://geek-blogs.com/blog/arch-linux-commands/#72-zui4-jia1-shi2-jian4-an1-quan2-shi3-yong4-aur)

AUR 包由用户提交，存在潜在风险（如恶意代码、构建错误）。需遵循以下原则：

1. **验证 PKGBUILD**：安装前查看包的构建脚本（`PKGBUILD`），确保无异常命令（如 `rm -rf /`）。
    
    ```
    yay -G <包名>  # 仅下载源码与 PKGBUILD，不安装
    cd <包名> && cat PKGBUILD  # 查看构建脚本
    ```
    
2. **使用 `--cleanbuild` 构建**：避免缓存污染，确保每次构建基于最新源码：
    
    ```
    yay -S --cleanbuild <包名>
    ```
    
3. **定期更新 AUR 助手**：`yay` 自身也通过 AUR 更新，需优先升级：
    
    ```
    yay -S yay
    ```
    

## 参考资料[#](https://geek-blogs.com/blog/arch-linux-commands/#can1-kao3-zi1-liao4)

1. [Arch Linux 官方文档](https://wiki.archlinux.org/)
2. [Pacman 手册](https://man.archlinux.org/man/pacman.8)
3. [Systemd 服务管理](https://wiki.archlinux.org/title/Systemd)
4. [AUR 使用指南](https://wiki.archlinux.org/title/Arch_User_Repository)
5. [nftables 入门](https://wiki.archlinux.org/title/Nftables)
6. [Arch Linux 安全最佳实践](https://wiki.archlinux.org/title/Security)

通过本文的命令梳理与实践指南，相信你已能熟练驾驭 Arch Linux 的日常管理。记住，Arch 的魅力在于“自己动手”——多尝试、多查阅 Wiki，你会逐渐体会到定制化系统的乐趣！

[2025-10](https://geek-blogs.com/tag/2025-10/)

[极客技术博客](https://geek-blogs.com/)

[Terms](https://geek-blogs.com/terms/) · [Privacy Policy](https://geek-blogs.com/privacy/)

Company

- [About](https://geek-blogs.com/about/)

在线运行代码

- [在线C语言编译器](https://geek-blogs.com/online-c-compiler/)
- [在线C++编译器](https://geek-blogs.com/online-cpp-compiler/)
- [在线Java编译器](https://geek-blogs.com/online-java-compiler/)
- [在线Python编辑器](https://geek-blogs.com/online-python-compiler/)
- [在线Fortran编译器](https://geek-blogs.com/online-fortran-compiler/)
- [在线运行bash](https://geek-blogs.com/online-bash-compiler/)
- [在线Clojure编译器](https://geek-blogs.com/online-clojure-compiler/)
- [在线Go语言编译器](https://geek-blogs.com/online-go-compiler/)
- [在线运行haskell](https://geek-blogs.com/online-haskell-compiler/)
- [在线编译kotlin](https://geek-blogs.com/online-kotlin-compiler/)
- [在线运行C#](https://geek-blogs.com/online-csharp-compiler/)
- [在线运行lisp](https://geek-blogs.com/online-lisp-compiler/)
- [在线运行Swift](https://geek-blogs.com/online-swift-compiler/)
- [在线运行Rust](https://geek-blogs.com/online-rust-compiler/)
- [在线PHP编译器](https://geek-blogs.com/online-php-compiler/)
- [sql 在线编译器](https://geek-blogs.com/online-sql-compiler/)
- [lua在线运行](https://geek-blogs.com/online-lua-compiler/)
- [在线运行Javascript](https://geek-blogs.com/online-javascript-compiler/)
- [极客工具箱](https://geek-blogs.com/tools/)

- [](https://geek-blogs.com/blog/arch-linux-commands/#)
- [](https://geek-blogs.com/blog/arch-linux-commands/#)
- [](https://geek-blogs.com/blog/arch-linux-commands/#)
- [](https://geek-blogs.com/rss.xml)

© 2025 [Geek-Blogs.com](https://Geek-Blogs.com) · All rights reserved.