## $5 Tech Unlocked 2021!
[Buy and download this product for only $5 on PacktPub.com](https://www.packtpub.com/)
-----
*The $5 campaign         runs from __December 15th 2020__ to __January 13th 2021.__*

# Qt5 Cross Platform Applications

Course being developed with Packt Publishing showcasing the portability of Qt5 to do Cross Platform development by building out and distributing an Agar.io clone!

## Installation

Unfortunately, Qt [has a bug](https://bugreports.qt.io/browse/QTBUG-70078) that causes some stale cache issues.

To clone the patched version of QtWebChannel in.

```bash
$ git clone https://github.com/PacktPublishing/Qt5-Cross-Platform-Application-Development
$ git submodule init
$ git submodule update
```

Now to compile the patched QtWebChannel

```bash
$ cd qtwebchannel
$ qmake
$ make
```

Build `eatem-server` and then use the LD_PRELOAD to point directly to your patched WebChannel library rather than the system version.

```bash
$ LD_PRELOAD=/absolute/path/here/qtwebchannel/lib/libQt5WebChannel.so.5 ./server
```
