#! /bin/bash

sudo mount output/images/rootfs.ext2 /home/dk/linuxdistro/buildroot/traceOut
wait
sudo cp traceOut/root/trace.dat .
sudo umount /home/dk/linuxdistro/buildroot/traceOut
