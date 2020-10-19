#!/usr/bin/env python3

""" Compile PSDK binaries.

Usage:
  {this_script} [options]

Options:
  -h  --help     Prints this docstring.
  -l  --launch   Execute the compiled binary if the compilation succeeds.
"""

import sys
import os

def print_blue(*args, **kwargs):
	print("\x1b[36m", end = "")
	print(*args, **kwargs)
	print("\x1b[39m", end = "", flush = True)

# Options
def cmdline_has_option(*options):
	for option in options:
		if option in sys.argv[1:]:
			return True
	return False
option_help = cmdline_has_option("-h", "--help")
option_launch = cmdline_has_option("-l", "--launch")
src_dir_name = "src"
bin_dir_name = "bin"
bin_name = "ports"

# Help message if -h
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

# Bin directory
if not os.path.exists(bin_dir_name):
	os.makedirs(bin_dir_name)

# Build
command_args = ["gcc"]
for src_file_name in src_file_names:
	command_args.append(src_file_name)
command_args.append("-o")
command_args.append(os.path.join(bin_dir_name, bin_name))
command_args.append("-Wall")
command_args.append("-Wextra")
command_args.append("-pedantic") # serious project certified..
command_args.append("-std=c99")
command = " ".join(command_args)
print_blue(command)
status = os.system(command)

# Launch if -l
if option_launch and status == 0:
	os.chdir(bin_dir_name)
	os.system("./" + bin_name)
	os.chdir("..")
