# IMPORTS
import datetime
import subprocess
import sys
import os
import time

# Init formatted output file
file_output = open('./output/outputExcelParsed.txt', 'w')

# Execute command and yield output as it is received
def execute(cmd):
    popen = subprocess.Popen(cmd, stdout=subprocess.PIPE, universal_newlines=True)
    for stdout_line in iter(popen.stdout.readline, ""):
        yield stdout_line
    popen.stdout.close()
    return_code = popen.wait()
    if return_code:
        raise subprocess.CalledProcessError(return_code, cmd)

# Run ADDA and grab output as it runs
def adda_run(grid='30', dplSize='10', filename='./output/outputDipoleXYZ.txt', overlap=1.0):
    # Build dipoles
    cmdRunADDA = ['adda', '-m', '1.85', '0.71', '-lambda', '0.55', '-bishape', overlap, 'sphere', '-dpl', dplSize, '-grid', grid, '-dir', './output/runs/dipole_output_grid' + grid + '_dplsize' + dplSize];
    s1 = ''
    s2 = ''
    t0 = -1
    for output in execute(cmdRunADDA):
        if t0 == -1:
            t0 = time.time()

        # Push output to console
        print(output, end="")

        # Run #
        if ('all data is saved in' in output):
            value = output[25:28]
            s1 += value + '\t'
            # file_output.write(value + '\t')

        # Date + ADDA memory usage
        if ('Total memory usage' in output):
            value = output.split()[-2:]
            # file_output.write(value[0] + '\t')
            # file_output.write(datetime.date.today().strftime("%m/%d/%Y") + '\t\t')
            s1 += datetime.date.today().strftime("%m/%d/%Y") + '\t'
            s1 += value[0] + '\t'

        # Num dipoles
        # Total number of occupied dipoles: 14144
        if ('Total number of occupied dipoles' in output):
            value = output.split()[-1:]
            # file_output.write(value[0] + '\t')
            # file_output.write(datetime.date.today().strftime("%m/%d/%Y") + '\t\t')
            s1 += value[0] + '\t'
         

        # ADDA light data
        if ('Cext' in output or 'Qext' in output or 'Cabs' in output or 'Qabs' in output):
            value = output.split()[-1:]
            # file_output.write(value[0] + '\t')
            s2 += value[0] + '\t'

    # Time to execute
    t1 = time.time()
    s1 += str(t1 - t0)[:9] + '\t'

    # Write to file
    file_output.write(s1 + '\t' + s2 + '\n')

# Read excel data input
f = open('./input/inputExcelRuns.txt')
try:
     text = f.read()
finally:
     f.close()
text = text.split()
for number in range(3,len(text),3):
    dplSize=text[number]
    grid=text[number+1]
    overlap=text[number+2]
    print('GRID:', grid)
    print('DPL SIZE:', dplSize)
    print('OVERLAP:', overlap)
    adda_run(grid, dplSize, './output/runs/dipole_output_grid' + str(grid) + '_dplsize' + str(dplSize) + '.txt', overlap)

# File output clean up
file_output.close()

