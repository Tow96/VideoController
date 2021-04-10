### build flags.py
# Copyright (c) Jose Tow, 2021
# Reads the configuration.h to check if the firmware has the debug flag enabled to
# set the appropriate build-flags

import os
import sys
import re

debug_mode = False;

script_dir = os.path.dirname(__file__)
rel_path = "src/configuration.h"
abs_file_path = os.path.join(script_dir, rel_path)

with open(abs_file_path) as main_file:
  for line in main_file.readlines():
    stripped = line.strip()
    if stripped == "#define DEBUG":
      debug_mode = True
      break

if debug_mode:
  sys.stdout.write('-D USB_SERIAL_HID')
else:
  sys.stdout.write('-D USB_HID')
