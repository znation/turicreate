# Linux Installation Instructions

The minimum requirements are:
 - python 2.7 
 - glibc >= 2.12 (Centos >= 6, Ubuntu >= 10.10)
 - For Neural Network support, glibc >= 2.17 is needed. Centos >= 7 and Ubuntu >= 13.04
 - libstdc++ >= 6.0.21
 - libblas
 - liblapack

## Ubuntu

### Ubuntu >= 16.04
On recent versions of Ubuntu, we just need a few dependencies

```shell
sudo apt-get install -y libblas3 liblapack3 libstdc++6 python-setuptools
sudo easy_install pip
sudo pip install virtualenv
```

To avoid making system changes, we are going to do everything in a virtualenv

```shell
cd $HOME
virtualenv venv
```

Activate the virtualenv

```shell
cd venv
source bin/activate
```
Install turicreate

```shell
pip install --upgrade pip
pip install turicreate
```


### Ubuntu >= 14.04
On older versions, we need to get a newer installation of libstdc++. This should work up to Ubuntu 14.04

```shell
sudo apt-get -y install libblas3 liblapack3 python-setuptools
wget http://security.ubuntu.com/ubuntu/pool/main/g/gcc-5/gcc-5-base_5.4.0-6ubuntu1~16.04.9_amd64.deb
wget http://security.ubuntu.com/ubuntu/pool/main/g/gcc-5/libstdc++6_5.4.0-6ubuntu1~16.04.9_amd64.deb
sudo dpkg -i gcc-5-base_5.4.0-6ubuntu1~16.04.9_amd64.deb
sudo dpkg -i libstdc++6_5.4.0-6ubuntu1~16.04.9_amd64.deb
sudo easy_install pip
sudo pip install virtualenv
```


To avoid making system changes, we are going to do everything in a virtualenv

```shell
cd $HOME
virtualenv venv
```

Activate the virtualenv

```shell
cd venv
source bin/activate
```

Install turicreate

```shell
pip install --upgrade pip
pip install turicreate
```


### Ubuntu < 14.04
(Tested on Ubuntu 12.04)

Compiling gcc from source is necessary here.
You can download any version of gcc >= 5 from https://gcc.gnu.org

```shell
sudo apt-get install gcc g++

cd ~
wget https://mirrors-usa.go-parts.com/gcc/releases/gcc-7.2.0/gcc-7.2.0.tar.gz
tar -xzvf gcc-7.2.0.tar.gz
cd gcc-7.2.0
contrib/download_prerequisites
./configure --disable-multilib --enable-languages=c,c++ --disable-bootstrap
make
sudo make install
```


Every time you open a new bash session, you should run the following commands.You could put them in ~/.bashrc for convenience.

```shell
export LD_LIBRARY_PATH=/usr/local/lib64:$LD_LIBRARY_PATH
```

To avoid making system changes, we are going to do everything in a virtualenv

```shell
cd $HOME
virtualenv venv
```

Activate the virtualenv

```shell
cd venv
source bin/activate
```

Install turicreate

```shell
pip install --upgrade pip
pip install turicreate
```


## Centos

### Centos 7

First we obtain the basic dependencies

```shell
sudo yum install -y blas lapack
sudo easy_install pip
sudo pip install virtualenv
```

We will need to get a newer libstdc++ from a package in the Nux Dextop repo.

```shell
wget ftp://ftp.pbone.net/mirror/li.nux.ro/download/nux/dextop/el6/x86_64/chrome-deps-stable-3.11-1.x86_64.rpm
sudo rpm -i --badreloc --noscripts --relocate /opt/google/chrome=$HOME chrome-deps-stable-3.11-1.x86_64.rpm
```

This will install libstdc++.so into $HOME/lib. 
Alternatively, you can compile gcc >= 5.4 from source.

Every time you open a new bash session, you should run the following commands.You could put them in ~/.bashrc for convenience.

```shell
export LD_LIBRARY_PATH=$HOME/lib:$LD_LIBRARY_PATH
```

To avoid making system changes, we are going to do everything in a virtualenv

```shell
cd $HOME
virtualenv venv
```

Activate the virtualenv

```shell
cd venv
source bin/activate
```

Install turicreate

```shell
pip install --upgrade pip
pip install turicreate
```

### Centos 6

The situation with Centos 6 is somewhat complicated since Centos 6 ships with
Python 2.6 which is not compatible. We also need to be obtaining a recent version of 
libstdc++.

Basically, what you need is
1. Install Python 2.7
2. Obtain a recent libstdc++

#### Installing Python 2.7

We can get it from the centos-release-SCL repository:

```shell
sudo yum install -y centos-release-SCL
sudo yum install -y python27
```

Alternatively, if you compile Python 2.7 from source, you *must* configure
with `--enable-unicode=ucs4` and `--enable-shared`, and we recommend --with-suffix=2.7 to avoid name
collisions.

To check if your Python has the correct unicode version:
```python
# When built with --enable-unicode=ucs4:
>>> import sys
>>> print sys.maxunicode
1114111
# When built with --enable-unicode=ucs2:
>>> import sys
>>> print sys.maxunicode
65535
```

#### Obtain a recent libstdc++

We can find it in a package in the Nux Dextop repo.

```shell
wget ftp://ftp.pbone.net/mirror/li.nux.ro/download/nux/dextop/el6/x86_64/chrome-deps-stable-3.11-1.x86_64.rpm
sudo rpm -i --badreloc --noscripts --relocate /opt/google/chrome=$HOME chrome-deps-stable-3.11-1.x86_64.rpm
```

This will install libstdc++.so into $HOME/lib. 
Alternatively, you can compile gcc >= 5.4 from source.

#### Installing the other dependencies

```shell
sudo yum install -y blas lapack
```

#### Setting up your environment

Every time you open a new bash session, you should run the following commands.You could put them in ~/.bashrc for convenience.

```shell
export LD_LIBRARY_PATH=$HOME/lib:$LD_LIBRARY_PATH
scl enable python27 bash
```

To avoid making system changes, we are going to do everything in a virtualenv

```shell
cd $HOME
virtualenv-2.7 venv
```

Activate the virtualenv

```shell
cd venv
source bin/activate
```

We now need upgrade pip

```shell
pip2.7 install --upgrade pip
pip install turicreate
```

(The second 'pip' command runs the newer pip inside of the virtualenv.)


