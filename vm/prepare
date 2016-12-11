#!/bin/bash
rm assemble.me -f
cat assemble.me.tpl | tr -d '\r' > preparing.tmp
cpp -trigraphs $@ -I. --sysroot . preparing.tmp assemble.me
rm -f preparing.tmp
