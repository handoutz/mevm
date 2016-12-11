#!/bin/bash
rm $@ -f
cat "$@.tpl" | tr -d '\r' > preparing.tmp
cpp -trigraphs -I. --sysroot . preparing.tmp $@
rm -f preparing.tmp
