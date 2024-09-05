#!/bin/bash
cat << EOS
User=pi
Environment=DISPLAY=:0
#ExecStartPost=${TARGET_DIR}/restrictCPU.sh

EOS
