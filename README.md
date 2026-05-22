# 2.13 寸 122×250 全反射 SPI 模组（ST7305）资料与示例

**English：** [`README_EN.md`](README_EN.md)

---

> 本仓库提供该模组的 **ESP-IDF 示例工程**，以及数据手册、规格说明等资料。

## 产品概要

| 项目 | 说明 |
|:--|:--|
| 模组规格 | 2.13 英寸 **全反射式 LCD**（黑白），分辨率 **122×250** |
| 接口 | **SPI** |
| 驱动芯片 | **ST7305** |
| 规格标识 | 产品资料中常用 **`2.13-lcd-122x250-spi-st7305`** 表示本规格 |
| 与 AMOLED 版关系 | 同尺寸 **CO5300 QSPI AMOLED** 见 **`2.13-amoled-410x502-qspi-co5300`**，面板类型与接口不同、独立维护 |

---

## 仓库结构

### 顶层目录

| 路径 | 说明 |
|:--|:--|
| `assets/` | 示例工程 Demo 效果图片 |
| `docs/` | 数据手册、规格说明 |
| `examples/` | **示例工程** |

### `examples/` 分类

| 分类 | 说明 |
|:--|:--|
| `examples/` 根目录 | ESP32-S3 bringup：ST7305 SPI 显示与图片缓冲演示 |

### 示例工程路径

| 说明 | 路径 |
|:--|:--|
| ST7305 SPI bringup（含 `tools/png_to_st7305.py` 转图脚本） | `examples/esp32s3-2.13lcd-122x250-spi-st7305-bringup/` |

#### 示例效果示意

<p align="center">
  <img src="assets/image_1.jpg" alt="esp32s3-2.13lcd-122x250-spi-st7305-bringup 示例运行效果" width="480">
</p>
