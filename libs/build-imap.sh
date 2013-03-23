#!/usr/bin/env bash

cd imap-2007f

make slx EXTRACFLAGS="-I /usr/include/openssl/ -fPIC" SSLTYPE=unix.nopwd SHLIBBASE=c-client SHLIBNAME=libc-client.so.1 IP=6

# SSLDIR=/etc/ssl SSLLIB=/usr/lib

