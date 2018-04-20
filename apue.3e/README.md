## 安装
### Ubuntu

```shell
sudo apt-get install libbsd-dev
cd apue.3e
make
......

sudo cp ./include/apue.h /usr/include/
sudo cp ./lib/libapue.a /usr/local/lib/
```

### MacOS

```shell
cd apue.3e
make
......

sudo cp ./include/apue.h /usr/local/include/
sudo cp ./lib/libapue.a /usr/local/lib/
```

## 编译

```shell
gcc filename.c -lapue
```