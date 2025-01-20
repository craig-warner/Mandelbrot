# Mandelbrot
- A multi-threaded C program for generating Mandelbort Images and Movies.

# Building
Here are  the steps for building the executables for

 * cd Mandelbrot
 * cmake -S ./ -B apps/
 * cd apps
 * make -j

# Running (From the apps directory)
 * ../scripts/run_mandelbrot.py --num_processes 32 --num_threads 1 
 * ../scripts/run_mandelbrot.py --num_processes 1 --num_threads 256 

# Issues
 * Multi-Threaded Cmake is broken
 * RISC-V tager not added yet
