# IMPORTS
import datetime
import subprocess
import sys
import os
# import time
from datetime import datetime
from genDipoles import buildSphere
from numpy import *

# Init formatted output file
file_output = open('outputExcelParsed.txt', 'w')

# Execute command and yield output as it is received
def execute(cmd):
    popen = subprocess.Popen(cmd, stdout=subprocess.PIPE, universal_newlines=True)
    for stdout_line in iter(popen.stdout.readline, ""):
        yield stdout_line
    popen.stdout.close()
    return_code = popen.wait()
    if return_code:
        raise subprocess.CalledProcessError(return_code, cmd)

def gen_sphere(dpl, grid, x, y, z):
    return buildSphere(dpl, grid, x, y, z)

def write_dipoles(dipoles, filename, new_file=True):
    # Open file
    f = open(filename, 'w+')
    for d in dipoles:
        f.write(str(int(d[0])) + ' ' + str(int(d[1])) + ' ' + str(int(d[2])) + '\n')
    f.close()

# Run ADDA and grab output as it runs
def adda_run(monomers, grid='30', dplSize='10', filename='./output/runs/outputDipoleXYZ.txt'):
    dipoles = []
    for m in monomers:
        print('monomer: {}'.format(m))
        dipoles = list(set(dipoles + buildSphere(dplSize, grid, m[0], m[1], m[2])))

    write_dipoles(dipoles, './output/runs/dipole_output_grid' + grid + '_dplsize' + dplSize + '.txt')

    # Run ADDA
    cmdRunADDA = ['adda', '-m', '1.85', '0.71', '-lambda', '0.55', '-shape', 'read', filename, '-dpl', dplSize, '-dir', './output/runs/dipole_output_grid' + grid + '_dplsize' + dplSize]

    s1 = ''
    s2 = ''
    t0 = -1
    for output in execute(cmdRunADDA):
        # if t0 == -1:
        #     t0 = time.time()

        # Push output to console
        print(output, end="")

        # Run #
        if ('all data is saved in' in output):
            value = output[25:28]
            s1 += value + '\t'

        # Date + ADDA memory usage
        if ('Total memory usage' in output):
            value = output.split()[-2:]
            s1 += datetime.now().strftime("%m/%d/%Y") + '\t'
            s1 += value[0] + '\t'

        # Num dipoles
        if ('Total number of occupied dipoles' in output):
            value = output.split()[-1:]
            s1 += value[0] + '\t'

        # ADDA light data
        if ('Cext' in output or 'Qext' in output or 'Cabs' in output or 'Qabs' in output):
            value = output.split()[-1:]
            s2 += value[0] + '\t'

    # Time to execute
    # t1 = time.time()
    s1 += '\t' # str(t1 - t0)[:9] + '\t'

    # Write to file
    file_output.write(s1 + '\t' + s2 + '\n')

def readExcelInput(filename='./input/inputExcelRuns.txt'):
    # Read excel data input
    f = open(filename)
    try:
         text = f.read()
    finally:
         f.close()
    text = text.split()
    dplSize = []
    grid = []
    for number in range(2,len(text),2):
        dplSize.append(text[number])
        grid.append(text[number+1])

    return dplSize, grid

# Parse FracMAP output
def operate_shift(monomers, radius):
    minx=radius
    miny=radius
    minz=radius
    # finding minmum x y z
    for m in monomers:
        minx = min(minx, m[0])
        miny = min(miny, m[1])
        minz = min(minz, m[2])
    # shifting
    for m in monomers:
        m[0] = round(m[0] - minx + radius)
        m[1] = round(m[1] - miny + radius)
        m[2] = round(m[2] - minz + radius)
    return monomers

# Read fracmap output file
def read_fracmap(filename):
    print('Reading fracmap output from file: {}'.format(filename))
    f = open(filename)
    try:
        text = f.read()
    except:
        print('ERROR: Could not open fracmap output file')
        return
    finally:
        f.close()
    text = text.split('\n')
    read_centers = False
    monomers = []
    monomer_radius = 0
    for line in text:
        if not read_centers and 'a:' in line:
            monomer_radius = float(line.split()[1])
        elif read_centers and len(line) == 0:
            break

        if read_centers:
            x,y,z = line.split()
            monomers.append([float(x),float(y),float(z)])

        if line == 'X Y Z':
            read_centers = True

    return monomer_radius, monomers

# program start
def main():
    # Read input parameters
    dplSize, grid = readExcelInput()

    print('dplSize: {}\ngrid: {}'.format(dplSize, grid))

    # Read FracMAP input
    monomers = []
    monomer_radius = 0
    if len(sys.argv) > 1:
        monomer_radius, monomers = read_fracmap(sys.argv[1])

    # Shift monomers to lose any negative values
    monomers = operate_shift(monomers, monomer_radius)

    # Iterate over run parameters
    for k in range(len(dplSize)):
        adda_run(monomers,
                grid[k],
                dplSize[k],
                './output/runs/dipole_output_grid' + str(grid[k]) + '_dplsize' + str(dplSize[k]) + '.txt')


main()
