#!/bin/sh
make -f Makefile1
mv ssmct /usr/local/bin/ssmct
make -f Makefile2
mv smct /usr/local/bin/smct
make -f Makefile3
mv ssmct_util /usr/local/bin/ssmct_util
make -f Makefile4
mv muestra /usr/local/bin/muestra
make -f Makefile5
mv muestra2 /usr/local/bin/muestra2
make -f Makefile6
mv muestra_corr /usr/local/bin/muestra_corr
make -f Makefile7
mv muestra_corr1 /usr/local/bin/muestra_corr1
make -f Makefile8
mv smct_demo /usr/local/bin/smct_demo
make -f Makefile9
mv smct_clase /usr/local/bin/smct_clase
make -f Makefile10
mv smct_bscw /usr/local/bin/smct_bscw
gcc -o funciones main.c callbacks.c support.c interface.c `gtk-config \
--cflags --libs`
mv funciones /usr/local/bin/funciones
gcc -o test main_t.c callbacks_t.c support_t.c interface_t.c `gtk-config \
--cflags --libs`
mv test /usr/local/bin/test
gcc -o notebook notebook.c `gtk-config --cflags --libs`
mv notebook /usr/local/bin/notebook
gcc -o notebook_co notebook_co.c `gtk-config --cflags --libs`
mv notebook_co /usr/local/bin/notebook_co
gcc -o notebook_pr notebook_pr.c `gtk-config --cflags --libs`
mv notebook_pr /usr/local/bin/notebook_pr
cp *.awk /usr/local/bin/
cp Show /usr/local/bin/
cp AYUDA /usr/local/bin/
cp COPYING /usr/local/bin/
