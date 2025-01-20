#!/usr/bin/python3
# -*- coding: utf-8 -*-

"""
Run Mandelbrot
author: Craig Warner
    This program invokes one or multiple copies of the mandelbrot draw program
"""

# External Imports
import sys 
import os 
import argparse
#from jsonformatter import JsonFormatter
import time
import subprocess
from multiprocessing import Process

# Run mandelbrot  
from version import __version__


def CalcLog2TilesPerSide(num_threads):
    """
    Calculate the number of tiles per side for a given number of threads
    """
    if(num_threads < 1):
        log2_tiles_per_side = 0
    elif (num_threads < 4):  
        log2_tiles_per_side = 1
    elif (num_threads < 16):  
        log2_tiles_per_side = 2
    elif (num_threads < 64):  
        log2_tiles_per_side = 3
    elif (num_threads < 256):  
        log2_tiles_per_side = 4
    else:
        sys.exit("Too many threads")
    return log2_tiles_per_side

def CalcPps(user_pps):
    """
    Calculate the number of pixels per side for a given number of pixels per side
    """
    if(user_pps < 16):
        sys.exit("Image too small")
    if(user_pps < 32):
        pps = 16
    elif(user_pps < 64):
        pps = 32 
    elif(user_pps < 128):
        pps = 64 
    elif(user_pps < 256):
        pps = 128 
    elif(user_pps < 512):
        pps = 256 
    elif(user_pps < 1024):
        pps = 512 
    elif(user_pps < 2048):
        pps = 1024 
    elif(user_pps < 4096):
        pps = 2048 
    elif(user_pps < 8192):
        pps = 4096 
    elif(user_pps < 16384):
        pps = 8192 
    elif(user_pps < 32768):
        pps = 16384 
    elif(user_pps < 65536):
        pps = 32768
    elif(user_pps < 131072):
        pps = 65536
    else:
        sys.exit("Image too large")

    return pps

def Worker(image_num, log2_tiles_per_side, pixels_per_side):
    """
    Worker function for multiprocessing
    """
    cmd = "./Mandelbrot "+str(image_num)+" "+str(log2_tiles_per_side)+" "+str(pixels_per_side)
    print(cmd)
    os.system(cmd)
    #subprocess.run(cmd)

# CLI Parser
parser = argparse.ArgumentParser(description='Run Mandelbrot')
parser.add_argument("--num_processes", help="Number of Processes",default="1")
parser.add_argument("--num_threads", help="Number of Threads (1 to 256)",default="1")
parser.add_argument("--pixels_per_side", help="Number of pixels on image side", default="256")
#parser.add_argument("--ifile", help="Template file (.hjson)", default="../scripts/")
#parser.add_argument("--ofile", help="Output file (.bmp)", default="mbg8.bmp")
parser.add_argument("-v", "--verbose", help="Increase output verbosity",action ="store_true") 
parser.add_argument('-V', '--version', action='version', version="%(prog)s ("+__version__+")")
args = parser.parse_args()

log2tiles_per_side = CalcLog2TilesPerSide(int(args.num_threads))
pps= CalcPps(int(args.pixels_per_side))

if __name__ == '__main__':
    jobs = []
    for i in range(int(args.num_processes)):
            p = Process(target=Worker, args=(i,log2tiles_per_side,args.pixels_per_side))
            jobs.append(p)
            p.start()
