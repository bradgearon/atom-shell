#!/usr/bin/env python

import argparse
import os
import subprocess
import sys


CONFIGURATIONS = ['Release', 'Debug']
SOURCE_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def main():
  print SOURCE_ROOT
  os.chdir(SOURCE_ROOT)

  ninja = os.path.join('vendor', 'depot_tools', 'ninja')
  if sys.platform == 'win32':
    ninja += '.exe'
  print ninja
  args = parse_args()
  for config in args.configuration:
    build_path = os.path.join('out', config)
    subprocess.call([ninja, '-C', build_path, args.target])


def parse_args():
  parser = argparse.ArgumentParser(description='Build atom-shell')
  parser.add_argument('-c', '--configuration',
                      help='Build with Release or Debug configuration',
                      nargs='+',
                      default=CONFIGURATIONS,
                      required=False)
  parser.add_argument('-t', '--target',
                      help='Build specified target',
                      default='atom',
                      required=False)
  return parser.parse_args()


if __name__ == '__main__':
  sys.exit(main())
