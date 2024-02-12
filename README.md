# iRadioAndroid（インターネットTVとWebSDR/KiwiSDRにも対応）

![sysoverview](https://github.com/BM45/iRadioAndroid/blob/main/pics4www/systemoverview.jpg)

#### 対応システム

現在、APIレベル17（A4.2 Jelly Bean）からAndroid 12までのAndroidベースのシステム（スマートフォンおよびタブレットPC）がテストされ、サポートされています。
新しいデバイスでの使用は基本的に可能ですが、GoogleによるAPIの変更に伴い、将来的に一定の制限を受ける可能性があります。
通常、端末をroot化する必要はありません！

#### システム設計

iRadioAndroidのデザインは、iRadio for Raspberry（）とiRadioMini for ESP32（）のモジュール原理に基づいている。バックグラウンド・プロセスとしてのメディア・プレーヤー（iRadioPlayer）に加えて、可視化（displayd...）と、OTG USBシリアル・ポート経由で接続されたGPIO（gpiod）経由で制御するための様々な「プロセス」があります。iRadioAndroidを外部から制御するためのサンプル・ファームウェアは "firmware "フォルダにあります。

![sysoverview](https://github.com/BM45/iRadioAndroid/blob/main/pics4www/folders.jpg)

drawable "フォルダには、例として追加したラジオスケールシミュレーションで使用したすべての画像リソース（アルファチャンネル付きのPNG）が含まれています。画像ファイルの変更と追加は、ポスト#127のiRadioスケール・シミュレーションの説明（ここhttps://radio-bastler.de/forum/showthread.php?tid=11484&pid=142892#pid142892）と同様に行う必要があります。

iRadioAndroidの様々なサービスの中心的な出発点はiRadioStartup.javaファイルです：

![startup](https://github.com/BM45/iRadioAndroid/blob/main/pics4www/startup.jpg)

ここで、iRadioAndroidの全コンポーネント（バックグラウンド・サービス、無線スケール・シミュレーション、その他のUI）が選択され、有効化され、iRadio for Raspberry用のrc.localファイルも選択される：

iRadioをボタンやロータリー・エンコーダーで制御することで、古い無線機への統合が容易になるとしても、このオプションはあくまでオプションです。他のアプリと同様に、iRadioAndroidはシステムのタッチ・ディスプレイから直接コントロールすることができ、付属の表示コード例はこのタイプの操作をサポートしています。

#### インストール

iRadioAndroidをコンパイルするには、まずGoogleのAndroid Studioが必要です。https://developer.android.com/studio 開発者は現在のバージョン2023.1.1 Hedgehogを推奨しています。Android Studioをダウンロードしてインストールしたら、iRadioAndroidをローカルのprojectsディレクトリにコピーします。

呼び出し


`git clone https://github.com/BM45/iRadioAndroid/`


ターミナル / projects-フォルダから実行することができます。

将来のAndroid無線デバイスを開発者モードに切り替える必要があります。howto: https://developer.android.com/studio/debug/dev-options#enable

USBデバッグを有効にし（必要であればWiFi経由でデバッグ）、開発用PCとAndroid Studioに接続（ペアリング）します。howto: https://developer.android.com/studio/run/device#connect

iRadioアプリケーションをコンパイルした直後に、PCに接続されたAndroidデバイスにもインストールされます。また、Android端末からのデバッグ出力はすべてAndroid Studioに表示されるため（Logcat表示）、問題が発生した場合のモニタリングやトラブルシューティングが容易になります。

![logcat](https://developer.android.com/static/studio/images/debug/logcat_dolphin_2x.png)

パフォーマンス上の理由から、すべての開発作業はAndroidエミュレータ上で行わず、実際のスマートフォン/タブレットで直接行うことをお勧めします！スケールシミュレーションは通常、使用するデバイスの解像度とジオメトリに合わせる必要があります！

![skalensim_cass](https://github.com/BM45/iRadioAndroid/blob/main/pics4www/skalensim.jpg)


重要：iRadioAndroidの初回起動後も、小さな内部放送局リストが使用されます！まず、iRadioAndroidアプリがAndroid設定/アプリ経由でデバイス・メモリーにアクセスできるようにする必要があります。これにより、iRadioMiniと同様にiRadioAndroidが独自のステーションリスト（playlist.m3u）を使用できるようになります。デフォルトでは、このチャンネル・リストはAndroidデバイスのダウンロード・フォルダーに保存されます。既存のplaylist.m3uは、ADBシェル（ラズベリー上のiRadioからのSSHアクセスと同等）を使用してPCからデバイスにコピーすることができますAndroidデバイスがUSBケーブルまたはWiFi経由でPCにペアリングされている場合は、図のように チャンネルリストが保存されているPCフォルダからスマートフォン/タブレットへの転送を開始することができます：

`adb push playlist.m3u /sdcard/Download`

adbシェル・コマンドで携帯電話にログインし、ls、cd、cp、mv、reboot ...などの（Linux）コマンドを使って携帯電話のファイル・システムをナビゲートし、チャンネル・リストが利用可能かどうかをチェックすることができます。この瞬間から、iRadioAndroidはアプリを起動するたびに、自分で作成したステーション・リストを使うようになる。

将来的には、無線機の筐体がシステムのタッチスクリーンに直接アクセスできなくなった場合、iRadioAndroidはADBシェル経由で新しいWiFiアクセスに設定することもできます：
`adb shell cmd -w wifi connect-network "Home" wpa2 "qwerty"`

この例では、パスワード "qwerty "でWiFiネットワーク "Home "のAndroidデバイスを設定します。

iRadioAndroidをロータリーパルスエンコーダーとボタンで制御するために、ArduinoとRP2040（Raspberry Pico）プラットフォーム用のサンプルコードが "firmware "フォルダに含まれています。このタイプの操作を使用するには、ArduinoまたはRP2040マイクロコントローラー本体のほかに、Arduino IDE https://www.arduino.cc/en/software 、USBプログラミングケーブル、USB OTGケーブルが必要です。マイコンのプログラミングが完了し、ソースコードに従ってボタンやロータリーエンコーダを接続したら、OTG USBケーブル/ハブを使用してArduinoまたはRP2040をAndroidデバイスに接続します。AndroidがUSBポートの許可を求めてきます。この許可をiRadioAndroidアプリに与えます。周辺機器を接続した状態で）アプリを再起動すると、iRadioを外部から操作できるようになります。

通信とすべてのコマンドは、物理的なGPIOインターフェースを提供する外部プロセッサーのファームウェアと、gpiodのサンプルコードで指定します。

![gpiodcontrol](https://github.com/BM45/iRadioAndroid/blob/main/pics4www/gpiodcommands.jpg)

OTG-USBケーブルの使用は双方向です！iRadioAndroidにデータを送ってコントロールできるように、iRadioAndroidも他の周辺機器にデータを送ることができます。例えば、OTG-USB経由でWiFiネットワークの電界強度などの値を送信し、歴史的なディスプレイチューブ（マジックアイ）をシミュレートすることができます。

![me1](https://github.com/BM45/iRadioAndroid/blob/main/pics4www/me1.jpg)  
![me2](https://github.com/BM45/iRadioAndroid/blob/main/pics4www/me2.jpg)

gpiodのサンプルコードはgpiodSerialOTG_magiceye_support.javaにあります。両タイプのファームウェアは、RP2040ファームウェア・フォルダにあります。このアプリケーションは決してGC9A01タイプのディスプレイに限定されるものではない。ファームウェアで使用されているライブラリ（https://github.com/moononournation/Arduino_GFX/tree/master/src/display）のおかげで、すでに多数のディスプレイをアドレス指定し、二次画面上の多種多様な出力に使用することができる。

![devkit](https://github.com/BM45/iRadioAndroid/blob/main/pics4www/devkit.jpg)

お知らせ USBデバイスには、メーカーID（ベンダーID）とプロダクトIDからなる識別子があります。いくつかのGPIOインターフェースでは、IDはすでにiRadioAndroidのファイルhttps://github.com/BM45/iRadioAndroid/blob/main/app/src/main/res/xml/device_filter.xml。

![devicefilter](https://github.com/BM45/iRadioAndroid/blob/main/pics4www/devicefilter.jpg)

独自に開発したGPIOインターフェイスの識別子が異なる場合は、プロジェクトをコンパイルする前にdevice_filterファイルにその識別子を入力してください。

#### 同調ノイズのシミュレーション

iRadioAndroidは、実際のラジオの動作をよりよくシミュレートするために、iRadio for Raspberryにすでに存在するノイズの入ったサービス/プロセスを導入した。

これは、2つのインターネットラジオ局を切り替えている間、チューニングサウンドを再生できることを意味します。つのチューニング・サウンドが利用可能です
noise.mp3とtuning.wavです。
 これらはアプリケーションのres/rawフォルダにあります。独自のチューニングサウンドを作成することもできます。その場合は、チューニングサウンドを含むファイルを 
をres/rawフォルダに追加します。その際 
ノイジーなソースコードの以下の位置にファイル名を入力します。
R.raw.my_file_with_noise（拡張子なし）。

```
noisePlayer.setDataSource(getApplicationContext(), Uri.parse("android.resource://" + getPackageName() + "/" + R.raw.tuning));
```

noisedサービスを有効にするには、iRadioStartup.javaファイルのサービス起動をコメントアウトします。

noisedの使用は（！）音階シミュレーションに縛られないが、特にこれに適している！

さらに、周波数ダイヤルが新しい位置に達するまで、インターネットラジオ番組の再生を一時停止することができます。これを行うには、iRadioStartup.javaファイルのWAIT_UNTIL_RADIO_DIAL_STOPS = false;をtrueに設定します。

#### インターネットTVサポート

iRadioAndroidでは、インターネットラジオに加え、多数のテレビチャンネルを受信することができます。

ラジオスケールシミュレーションの中で、ラジオとテレビの画像の切り替えは完全に自動化されています。
ラジオとテレビ局のストリーミングアドレスは1つのプレイリストに保存されています。

![radioandtv](https://github.com/BM45/iRadioAndroid/blob/main/pics4www/tvandradiosupport.jpg)

#### WebSDR / KiwiSDRサポート

![websdr](https://github.com/BM45/iRadioAndroid/blob/main/pics4www/websdrsupport.jpg)

iRadioAndroidはWebSDRとKiwiSDRのコントロールをサポートしています。つまり、iRadioAndroidは世界中の何百ものSDRレシーバーを受信する準備ができているということです。時報トランスミッターからQatar-OSCAR 100まで。付属のデモスケールシミュレーションでは、ウェブベースのSDRレシーバーを自分のアプリケーションに統合することができます。

![websdr2](https://github.com/BM45/iRadioAndroid/blob/main/pics4www/kiwidb.jpg)

#### 古いラジオにインターネットラジオ番組を放送

iRadioAndroid は、古いラジオの変調ソースとして機能し、RF 経由でインターネット ラジオ局を中継できます。 inet2RF サービスを使用すると、局リスト内の無線局が選択可能な RF 範囲にマッピングされます (送信モジュールでサポートされている場合)。送信周波数や放送するインターネットラジオ番組の変更は、スーパーヘテロダイン原理を利用して中間周波数を考慮した供給ラジオの局部発振器の周波数を測定することで行われます。

![inet2rf](https://github.com/BM45/iRadioAndroid/blob/main/pics4www/inet2rf.jpg)

無線機で測定したLo周波数、現在設定されている送信周波数、変調源をリアルタイムに表示します。

[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/wITcIP00m-0/0.jpg)](https://www.youtube.com/watch?v=wITcIP00m-0)

