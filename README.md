lua-quirc
=========

二维码(QRCode) 图像扫描库 quirc 的 lua 接口。

quirc 只支持 8bit 的无压缩灰度图像(下文简称 raw 格式)，像 jpg/png 等带压缩的格式需自行转换。

编译
----

下载 quirc 源码到本文件夹，并编译

```bash
git clone https://github.com/dlbeer/quirc.git
make
```

默认编译 3 个 target

* quirc.so 就是引入在应用项目的 lua 模块
* readraw.so 从 raw 格式的 lightuserdata 模块，测试演示用。
* qrdump 可执行命令，直接分析 jpg/png 中的二维码及信息，依赖 libjpeg/libpng 库，测试演示用。

RAW 格式
--------

表示一张 8bit 的灰度图像，原点在左上角，一字节一像素，所以长度需要等于高宽的乘积。

这里的图像是指从类似相机/视频截取出来的图像，二维码出现在任意位置，可能旋转的、模糊的、残缺的，能否识别出数据取决于图片的清晰度。

只包含像素数据，不带高宽数据，所以被处理时高宽得另外传递。

API 接口
--------

### `scan(bytes, width, height)`

扫描 raw 图像，识别出里面的二维码，并返回二维码的内容。

#### 参数

* `width` 图像的宽度，number 类型
* `height` 图像的高度，number 类型
* `bytes` 图像的数据，string 类型

#### 返回值

如果识别不出任何二维码，则返回 nil，否则返回一个 table。

table 包含以下字段：

| 名称      | 类型   | 描述          |
| --------- | ------ | ------------- |
| payload   | string | 数据内容      |
| length    | number | 数据内容长度  |
| mask      | number | 掩码图案      |
| version   | number | 版本号        |
| data_type | number | 数据类型      |
| ecc_level | number | ECC Level     |

### `scan_lud(lud)`

#### 参数

`lud` 为一个 lightuserdata 对象，基本上同 `scan()` 的参数结构如下：

```c
typedef struct {
    char *bytes;
    unsigned long length;
    unsigned long width;
    unsigned long height;
} qrimageraw;
```

#### 返回值

同 `scan()`

使用示例
--------

假设有二维码图像 hello.png，内容为 `hello`，高宽均为 46 像素。

```bash
wget 'http://chart.googleapis.com/chart?cht=qr&chs=46x46&chld=H|0&chl=hello' -O hello.png
```

### test-raw.lua

使用方法

```bash
lua test-raw.lua image.raw width:height
```

`image.raw` 图像文件，`width:height`，冒号分隔高宽。

可以用上面的 qrdump 来创建 hello.raw 文件

```bash
$ ./qrdump hello.png hello.raw

... 忽略其余输出 ...

Success to write image raw file:
file: hello.raw
width: 46 pixels
height: 46 pixels
length: 2116 bytes

$ lua test-raw.lua hello.raw 46:46
hello.raw
mask: 6
version: 1
length: 5
payload: hello
ecc_level: 2
data_type: 4
```

### test-hex.lua

这个只是为了方便调试 raw 图像构造而已，转换成十六进制文本查看：

```bash
xxd -c 46 hello.raw | sed 's/^.\+: //; s/ //g; s/\.\+//;' > hello.hex
```

xxd 的参数 `-c` 表示多少字节换行，然后 sed 去除掉地址和 ASCII 内容，打开 hello.hex，二维码肉眼能识别，在 lua 里可以转换回去。

```bash
$ lua test-hex.lua hello.hex
hello.raw
mask: 6
version: 1
length: 5
payload: hello
ecc_level: 2
data_type: 4
```

### test-lud.lua

和 test-raw.lua 大同小异，只不过用 readraw.so 来读取 raw 图像文件，使用 `scan_lud()` 接口
