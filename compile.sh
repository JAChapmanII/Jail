#!/bin/bash
make 2>&1 | grep -v '^/usr/include'
