# zmk-keyboard-torabo-tsuki-lp

[torabo-tsuki LP](https://github.com/sekigon-gonnoc/torabo-tsuki-lp)用のZMKファームウェアです。

このリポジトリは、torabo-tsuki LP M を自分の環境で安定して使うためのフォークです。主な目的は、手元のキーボードで発生している不具合等を調査・修正することです。

## 使い方

* _centralがついているuf2をトラックボールがついている方に、_peripheralを反対側に書き込んでください
* キーマップはkeymap-editorおよびzmk-studioで編集できます

現在の主なビルド対象は以下です。自分はトラックボールを右手側につけて使用しています。

* `torabo_tsuki_lp_right_central`
* `torabo_tsuki_lp_left_peripheral`

## 使用環境

主に以下の環境で動作させることを目的としています。

* Keyboard: torabo-tsuki LP M
* Trackball side: 右手側
* Trackball sensor: PAW3222LU-TJDU
* Firmware: このリポジトリのGitHub ActionsでビルドしたUF2
* Host
  * MacBook Air M2
  * Windows desktop
* Connection
  * Bluetooth
  * USB

## 関連リポジトリとローカルディレクトリ

このリポジトリだけではなく、以下のリポジトリやローカルディレクトリも調査対象に含めます。

* このZMK設定リポジトリ
  * GitHub: `https://github.com/SoMinesawa/zmk-keyboard-torabo-tsuki-lp`
  * Local: `/Users/somine/workspace/zmk-keyboard-torabo-tsuki-lp`
* torabo-tsuki LP本体のハードウェア/回路図/PCB
  * Upstream: `https://github.com/sekigon-gonnoc/torabo-tsuki-lp`
  * Local: `/Users/somine/workspace/torabo-tsuki-lp`
* PAW3222トラックボールドライバ
  * GitHub: `https://github.com/SoMinesawa/zmk-driver-paw3222`
  * Local: `/Users/somine/workspace/zmk-driver-paw3222`
