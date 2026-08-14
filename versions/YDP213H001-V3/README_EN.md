<p align="left"><img alt="OSPTEK" src="./images/logo.png" width="200" /></p>

<h1 align="center">OSPTEK 2.13″ LCD 122×250 (ST7305 · SPI)</h1>

<p align="center"><b>Reflective LCD · SPI · ST7305</b></p>

<p align="center"><a href="./README.md">简体中文</a> | English · <a href="../../README_EN.md">Family index</a></p>

<p align="center">
  <img alt="Size: 2.13 inch" src="https://img.shields.io/badge/Size-2.13%22-3498DB?style=flat-square" />
  <img alt="Resolution: 122x250" src="https://img.shields.io/badge/Resolution-122%C3%97250-8E44AD?style=flat-square" />
  <img alt="Interface: SPI" src="https://img.shields.io/badge/Interface-SPI-27AE60?style=flat-square" />
  <img alt="Driver: ST7305" src="https://img.shields.io/badge/Driver-ST7305-E7352C?style=flat-square" />
</p>

<p align="center"><img alt="OSPTEK 2.13″ 122×250 reflective LCD SPI module (ST7305) product image" src="./images/product.png" width="640" /></p>

## Contents

- [Overview](#overview)
- [Specifications](#specifications)
- [Sample projects](#sample-projects)
- [Repository layout](#repository-layout)
- [Resources](#resources)
- [Buy](#buy)
- [Support](#support)

---

## Overview

OSPTEK **2.13″ 122×250 reflective LCD** is a **SPI** monochrome display module driven by **ST7305**. Suited to low-power instruments, labels, and outdoor-readable UIs.

Spec ID (repository name): `2.13-lcd-122x250-spi-st7305`

Current module version: **YDP213H001-V3**. Electrical and mechanical details follow [`docs/YDP_213_H001_V3_056ab92f80.pdf`](./docs/YDP_213_H001_V3_056ab92f80.pdf).

## Specifications

| Item | Spec |
| ---- | ---- |
| Size | 2.13 inch |
| Type | Reflective LCD (monochrome) |
| Resolution | 122×250 |
| Interface | SPI |
| Driver IC | ST7305 |

> Full outline, FPC definition, power, and timing follow the product datasheet / driver IC datasheet.

## Sample projects

| Description | Path |
| ---- | ---- |
| ESP32-S3 · ST7305 SPI bring-up (includes `tools/png_to_st7305.py`) | [`examples/esp32s3-2.13lcd-122x250-spi-st7305-bringup/`](./examples/esp32s3-2.13lcd-122x250-spi-st7305-bringup/) |

Bring-up demo photo: [`assets/image_1.jpg`](./assets/image_1.jpg).

## Repository layout

```text
2.13-lcd-122x250-spi-st7305/                                # repo root (nav: ../../README_EN.md)
└── versions/
    └── YDP213H001-V3/                                # full materials for this part number
        ├── README.md
        ├── README_EN.md
        ├── images/
        ├── docs/
        └── examples/
```

## Resources

### Product files

| Resource | Link |
| ---- | ---- |
| Product datasheet (YDP213H001-V3) | [`docs/YDP_213_H001_V3_056ab92f80.pdf`](./docs/YDP_213_H001_V3_056ab92f80.pdf) |
| Driver IC datasheet (ST7305) | [`docs/ST_7305_V0_2_d0b99d9cdb.pdf`](./docs/ST_7305_V0_2_d0b99d9cdb.pdf) |
| Reflective LCD adapter board | [`docs/PCB-高反屏转接板.pdf`](./docs/PCB-%E9%AB%98%E5%8F%8D%E5%B1%8F%E8%BD%AC%E6%8E%A5%E6%9D%BF.pdf) |

### Samples

- [ESP32-S3 ST7305 SPI bring-up](./examples/esp32s3-2.13lcd-122x250-spi-st7305-bringup/)

## Buy

<p align="center">
  <a href="https://www.aliexpress.com/store/1105701619"><img alt="AliExpress store" src="https://img.shields.io/badge/AliExpress-Official_Store-FF6A00?style=for-the-badge" /></a>
  &nbsp;&nbsp;
  <a href="https://shop110742373.taobao.com/"><img alt="Taobao store" src="https://img.shields.io/badge/Taobao-Official_Store-FF6A00?style=for-the-badge" /></a>
</p>

**Overseas (AliExpress)**

- Store: [OSPTEK Official Store](https://www.aliexpress.com/store/1105701619)

**China (Taobao)**

- Store: [鱼鹰光电工厂店](https://shop110742373.taobao.com/)

## Support

- Technical support / product inquiry: <luyu@osptek.com>
- QQ group: **985881096**
- Website: <https://osptek.com/>
- Feel free to open an Issue in this repository with any questions

---

<p align="center"><sub>© 2026 OSPTEK · Materials in this repository are licensed under CC BY 4.0</sub></p>
