# fal：Flash 抽象层

## 1、介绍

> 说明：你需要在这里对项目进行简单的介绍，描述项目背景，当前现状、功能特点等等……

### 1.1 目录结构

> 说明：参考下面表格，整理出 packages 的目录结构

| 名称 | 说明 |
| ---- | ---- |
| inc  | 头文件目录 |
| src  | 源代码目录 |

### 1.2 许可证

fal package 遵循 LGPLv2.1 许可，详见 `LICENSE` 文件。

### 1.3 依赖

对 RT-Thread 无依赖，也可用于裸机

## 2、如何打开 fal

> 说明：描述该 package 位于 menuconfig 的位置，并对与其相关的配置进行介绍

使用 fal package 需要在 RT-Thread 的包管理器中选择它，具体路径如下：

```
RT-Thread online packages
    miscellaneous packages --->
        [*] A hello package
```

然后让 RT-Thread 的包管理器自动更新，或者使用 `pkgs --update` 命令更新包到 BSP 中。

## 3、使用 fal

> 说明：在这里介绍 package 的移植步骤、使用方法、初始化流程、准备工作、API 等等，如果移植或 API 文档内容较多，可以将其独立至 `docs` 目录下。

## 4、注意事项

> 说明：列出在使用这个 package 过程中需要注意的事项；列出常见的问题，以及解决办法。

## 5、联系方式 & 感谢

* 维护：armink
* 主页：https://github.com/RT-Thread-packages/fal
