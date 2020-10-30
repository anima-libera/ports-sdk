#!/usr/bin/env python3

""" Compile PSDK binaries.

Usage:
  {this_script} [options]

Options:
  -h  --help     Prints this docstring.
  -l  --launch   Execute the bin if compiled, with what follows as args.
"""

import sys
import os

def print_blue(*args, **kwargs):
	print("\x1b[36m", end = "")
	print(*args, **kwargs)
	print("\x1b[39m", end = "", flush = True)

# Launch option -l
if "-l" in sys.argv[1:]:
	option_launch = True
	i = sys.argv[1:].index("-l")
elif "--launch" in sys.argv[1:]:
	option_launch = True
	i = sys.argv[1:].index("--launch")
else:
	option_launch = False
if option_launch:
	options = sys.argv[1:i+1]
	launch_args = sys.argv[i+2:]
else:
	options = sys.argv[1:]

# Options
def cmdline_has_option(*option_names):
	for option_name in option_names:
		if option_name in options:
			return True
	return False
option_help = cmdline_has_option("-h", "--help")
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
#command_args.append("-fno-stack-protector")
command_args.append("-O2")
command = " ".join(command_args)
print_blue(command)
status = os.system(command)

# Launch if -l
if option_launch and status == 0:
	launch_command_args = ["./" + bin_name]
	for launch_arg in launch_args:
		launch_command_args.append(launch_arg)
	launch_command = " ".join(launch_command_args)
	os.chdir(bin_dir_name)
	print_blue(launch_command)
	launch_status_raw = os.system(launch_command)
	launch_status = launch_status_raw >> 8
	os.chdir("..")
	if launch_status != 0:
		print_blue("exit status {}".format(launch_status))
