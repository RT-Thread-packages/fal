# fal：Flash 抽象层

## 1、介绍

FAL (Flash Abstraction Layer) Flash 抽象层，是对 Flash 及基于 Flash 的分区进行管理、操作的抽象层，对上层统一了 Flash 及 分区操作的 API ，并具有以下特性：

- 支持静态可配置的分区表，并可关联多个 Flash 设备；
- 分区表支持 **自动装载** 。避免在多固件项目，分区表被多次定义的问题；
- 代码精简，对操作系统 **无依赖** ，可运行于裸机平台，比如对资源有一定要求的 Bootloader；
- 统一的操作接口。保证了文件系统、OTA、NVM 等对 Flash 有一定依赖的组件，底层 Flash 驱动的可重用性；
- 自带基于 Finsh/MSH 的测试命令，可以通过 Shell 按字节寻址的方式操作（读写擦） Flash 或分区，方便开发者进行调试、测试；

### 1.1 目录结构

> 说明：参考下面表格，整理出 packages 的目录结构

| 名称 | 说明 |
| ---- | ---- |
| inc  | 头文件目录 |
| src  | 源代码目录 |
| samples | 例程目录 |

### 1.2 许可证

fal package 遵循 LGPLv2.1 许可，详见 `LICENSE` 文件。

### 1.3 依赖

对 RT-Thread 无依赖，也可用于裸机

> 测试命令功能需要依赖 RT-Thread Finsh/MSH

## 2、如何打开 fal

使用 fal package 需要在 RT-Thread 的包管理器中选择它，具体路径如下：

```
RT-Thread online packages
    system packages --->
        [*] fal: Flash Abstraction Layer implement. Manage flash device and partition.
        [*]   Enable debug log output
        [ ]   FAL partition table config has defined on 'fal_cfg.h'
        (onchip) The flash device which saving partition table
        (65536) The patition table end address relative to flash device offset.
              version (latest)  --->
```

每个功能的配置说明如下：

- 开启调试日志输出（默认开启）；
- 分区表是否在 `fal_cfg.h` 中定义（默认开启），；



然后让 RT-Thread 的包管理器自动更新，或者使用 `pkgs --update` 命令更新包到 BSP 中。

## 3、使用 fal

> 说明：在这里介绍 package 的移植步骤、使用方法、初始化流程、准备工作、API 等等，如果移植或 API 文档内容较多，可以将其独立至 `docs` 目录下。

## 4、注意事项

> 说明：列出在使用这个 package 过程中需要注意的事项；列出常见的问题，以及解决办法。

## 5、联系方式 & 感谢

* 维护：armink
* 主页：https://github.com/RT-Thread-packages/fal
