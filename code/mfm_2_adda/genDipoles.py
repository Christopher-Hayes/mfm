import math
import sys

# For find cube root...
# Returns the absolute value of
# n-mid*mid*mid
def diff(n, mid) :
    if (n > (mid * mid * mid)) :
        return (n - (mid * mid * mid))
    else :
        return ((mid * mid * mid) - n)
# Returns cube root of a no n
def cubicRoot(n) :
    start = 0
    end = n
    e = 0.0000001
    while (True) :
        mid = (start + end) / 2
        error = diff(n, mid)
        if (error <= e):
            return mid
        if ((mid * mid * mid) > n) :
            end = mid
        else :
            start = mid

def distFromCenter(x,y,z,center):
    return math.sqrt((x-center[0])**2 + (y-center[1])**2 + (z-center[2])**2)

def generatePoints(dpl, grid, x_offset, y_offset, z_offset): # , new_file, filename='./runs/outputDipoleXYZ.txt'):
    # Get radius from 'volume' (dpl count)
    # r = cubicRoot(dpl * 3 / 4 / math.pi) + offset
    # print('offset: ', offset)
    r = grid * 0.5 # grid / dpl * 0.5
    center = (r + x_offset, r + y_offset, r + z_offset)

    print('grid:', grid)
    print('dpl:', dpl)
    print('r:', r)
    print('center:', center)
    print('x-offset:', x_offset)
    print('y-offset:', y_offset)
    print('z-offset:', z_offset)
    # print('new file:', new_file)

    # Open file
    """
    f = open(filename, 'a')
    if new_file:
        print('NEW FILE')
        f.close()
        f = open(filename, 'w+')
        """

    # Populate sphere with dipoles
    # for range doesn't support float values, hence the strange setup
    count = 0
    x = 0.0 + x_offset
    dipoles = []
    while (x < grid + x_offset):
        y = 0.0 + y_offset
        while (y < grid + y_offset):
            z = 0.0 + z_offset
            while (z < grid + z_offset):
                if (distFromCenter(x,y,z,center) <= r):
                    # print('[',x,',',y,',',z,end='],\n')
                    # f.write(str(int(x)) + ' ' + str(int(y)) + ' ' + str(int(z)) + '\n')
                    # print(x,'\t', y,'\t',z,'\t',distFromCenter(x,y,z,center),'\t1')
                    dipoles.append((x,y,z))
                    count += 1
                # else:
                #     print(x,'\t', y,'\t',z,'\t',distFromCenter(x,y,z,center),'\t0')
                z += 1.0
            y += 1.0
        x += 1.0

    # write dipoles
    # for d in dipoles:
    #     f.write(str(int(d[0])) + ' ' + str(int(d[1])) + ' ' + str(int(d[2])) + '\n')
    # f.close()
    print('Dipole Count: ', count)
    return dipoles

# arguments: { dpl, grid, x-offset, y-offset, z-offset, create-new-file }
def term():
    if (len(sys.argv) < 6):
        # generatePoints(float(input("Generate a sphere with what diameter?")), )
        # TODO: prompt for parameters
        print('TODO...')
    else:
        dummy, dpl, grid, x_offset, y_offset, z_offset, create_file = sys.argv
        print(generatePoints(float(dpl), float(grid), float(x_offset), float(y_offset), float(z_offset)))

def buildSphere(dpl, grid, x_offset, y_offset, z_offset):
        return generatePoints(float(dpl), float(grid), float(x_offset), float(y_offset), float(z_offset))
                # , create_file == 'True', './runs/dipole_output_grid' + grid + '_dplsize' + dpl + '.txt')

term()
