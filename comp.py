#!/usr/bin/env python3

""" Compile PSDK binaries.

Usage:
  {this_script} [options]

Options:
  -h  --help     Prints this docstring.
  -l  --launch   Execute the compiled binary if the compilation succeeds.
"""

print("aaaaaaaa")

import sys
import os

# Options
def cmdline_has_option(*options):
	for option in options:
		if option in sys.argv[1:]:
			return True
	return False
option_help = cmdline_has_option("-h", "--help")
option_launch = cmdline_has_option("-l", "--launch")
src_dir_name = "src"

if option_help:
	this_script = sys.argv[0]
	python = "" if this_script.startswith("./") else "python3 "
	print(__doc__.strip().format(this_script = python + sys.argv[0]))
	sys.exit(0)

# List src files
src_file_names = []
for dir_name, _, file_names in os.walk(src_dir_name):
	for file_name in file_names:
		if file_name.split(".")[-1] == "c":
			src_file_names.append(os.path.join(dir_name, file_name))

# Call gcc
command = ["gcc"]
for src_file_name in src_file_names:
	command_args.append(src_file_name)
command_args.append("-o")
command_args.append(dst_dir_name + "/" + bin_name)
command_args.append("-no-pie") # executable
