#!/bin/sh

cp $BASE_DIR/../custom-scripts/S41network-config $BASE_DIR/target/etc/init.d
chmod +x $BASE_DIR/target/etc/init.d/S41network-config

$BASE_DIR/host/bin/i686-buildroot-linux-uclibc-gcc $BASE_DIR/../../programs/simple_http_server_multiclient.c -o $BASE_DIR/target/usr/bin/simple_http_server_multiclient
chmod +x $BASE_DIR/target/usr/bin/simple_http_server_multiclient

cp $BASE_DIR/../../programs/page.html $BASE_DIR/target/usr/bin
chmod +xr $BASE_DIR/target/usr/bin/page.html

make -C $BASE_DIR/../modules/simple_driver/
