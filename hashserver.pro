include(global.pri)

TEMPLATE = subdirs

SUBDIRS += src \
    unit_tests

OTHER_FILES += $$files(extras/*, true) \
    README.md

CONFIG += nostrip

service.path = /lib/systemd/system
service.files = extras/hashserver.service

tester.path = $$PROJECT_PREFIX/bin
tester.files = extras/tester.sh

INSTALLS += service tester

shadow_install.target = shadow_install
shadow_install.commands = INSTALL_ROOT=$$OUT_PWD/shadow make install
QMAKE_EXTRA_TARGETS += shadow_install

pkg.target = package
pkg.depends = shadow_install

pkg.commands += cp -r $$PWD/extras/deb/* $$OUT_PWD/shadow
pkg.commands += && sed -i \'2iVersion: $$PROJECT_VERSION\' \
                        $$OUT_PWD/shadow/DEBIAN/control
pkg.commands += && fakeroot dpkg-deb -b $$OUT_PWD/shadow .

QMAKE_EXTRA_TARGETS += pkg
