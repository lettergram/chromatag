'''
  @Description: 
      Given an input matrix, duplicates and rotates image 180 degrees.
      Thereby enabling the encoding of an image twice for a more robust,
      AprilTag dubed ChromaTag.

  @Author: Austin Walters
  @Creation Date: 9/21/2014
  @Last Modified: 5/08/2015
  @Written in Python 2.7
'''

import sys
import os
from PIL import Image


# TODO: ENSURE COLORS ARE OPTIMAL
# AprilTag - (a, b) - colors

# (1, 1) - (a+, b+) - orange = (241, 125, 42)
# (1, 0) - (a+, b-) - magenta = (248, 140, 149)
# (0, 1) - (a-, b+) - lime = (90, 205, 86)
# (0, 0) - (a-, b-) - teal = (23, 249, 255)


orange = (255,  125, 42)
magenta = (200, 114, 239)
lime = (82, 255, 0)
teal = (25, 255, 255)

black = (0, 0, 0)
white = (255, 255, 255)

colors = [orange, magenta, lime, teal]

'''
Flips and encodes!
Could use http://stackoverflow.com/questions/16265673/rotate-image-by-90-180-or-270-degrees
for OpenCV implementation
'''
def encode(matrix):

    encodedMatrix = []
    
    try:
        height = len(matrix) 
        width = len(matrix[0])
    except:
        return [[]]

    for i in range(height):
        newRow = []
        for j in range(width):
            tup = (matrix[i][j], matrix[height - i - 1][width - j - 1])
            newRow.append(tup)
        encodedMatrix.append(newRow)
    return encodedMatrix

'''
Decodes encoded matrix from image
'''
def image2Matrix(imageMatrix):
    
    matrix = []

    try:
        height = len(imageMatrix)
        width = len(imageMatrix[0])
    except:
        return [[]]

    for i in range(height):
        row = []
        for j in range(width):
            if imageMatrix[i][j] is colors[0]:
                row.append((1, 1))
            elif imageMatrix[i][j] is colors[1]:
                row.append((1, 0))
            elif imageMatrix[i][j] is colors[2]:
                row.append((0, 1))
            elif imageMatrix[i][j] is colors[3]:
                row.append((0, 0))
        matrix.append(row)
    return matrix


'''
Creates an image from a matrix
'''
def generateImage(name, matrix):

    scale = 100
    size = len(matrix) * scale
    img = Image.new("RGB", (size, size), (255, 255, 255))
    for i in range(len(matrix)):
        for j in range(len(matrix)):
            for k in range(scale):
                for t in range(scale):
                    img.putpixel((i*scale + t, j*scale + k), matrix[i][j])
    img.save(name, "PNG")


'''
Creates a black and white image
'''
def generateBWMatrix(matrix):

    imageMatrix = []
    i = 0
    
    for row in matrix:
        imageMatrix.append([])
        for entry in row:
            if entry is 1:
                imageMatrix[i].append(black)
            else:
                imageMatrix[i].append(white)
        i += 1
    return imageMatrix

'''
Converts the tuples into pixel matrix
'''
def generateColorMatrix(matrix):    

    imageMatrix = []
    i = 0

    for row in matrix:
        imageMatrix.append([])
        for entry in row:
            if entry[0] is 0:
                if entry[1] is 0:
                    imageMatrix[i].append(colors[0])
                else:
                    imageMatrix[i].append(colors[1])
            else:
                if entry[1] is 0:
                    imageMatrix[i].append(colors[2])
                else:
                    imageMatrix[i].append(colors[3])
        i += 1
    return imageMatrix


'''
Prints a matrix in a decent format
'''
def printMatrix(matrix):
    for i in range(len(matrix)):
        print '\t[',
        for j in range(len(matrix[i])):
            print matrix[i][j], 
        print ']'

'''
  Simple test, checks to see if input == output
'''
def runTest(name, test):

    if not os.path.exists('output/'):
        os.makedirs('output')
    if not os.path.exists('input/'):
        os.makedirs('input')

    print '\n-- Beginning Test %s! --\n' % (name)
    print 'input matrix:'
    printMatrix(test)
    print 'Genearting Input Image...'
    inputName = 'input/' + name + '-april.png'
    generateImage(inputName, generateBWMatrix(test))
#    print 'tuple matrix:'
#    printMatrix(encode(test))
#    print 'coded matrix:'
#    printMatrix(generateColorMatrix(encode(test)))
#    print 'tuple matrix:'
#    printMatrix(image2Matrix(generateColorMatrix(encode(test))))
#    print 'Generating Output Image...'
    outputName = os.getcwd() + '/output/' + name + '-chroma.png'
    generateImage(outputName, generateColorMatrix(encode(test)))

#    if encode(test) == image2Matrix(generateColorMatrix(encode(test))):
#        print '\nOutput == Input!'
#        print 'Successfully, Completed Test %s!\n\n' % (name)
#    else:
#        print '\nOutput != Input!'
#        print 'Failed Test %s!\n\n' % (name)


print '\n\n---- TESTING CODE -----\n\n'
test = [[1, 0],[0, 1]]
runTest('#1', test) 

test2 = [[0, 1, 1], [1, 1, 1], [0, 1, 0]]
runTest('#2', test2)

apriltag36_11_12 = [[0,0,0,0,0,0,0,0,0,0],\
                    [0,1,1,1,1,1,1,1,1,0],\
                    [0,1,1,1,1,1,0,1,1,0],\
                    [0,1,1,0,1,0,0,0,1,0],\
                    [0,1,0,1,1,0,1,1,1,0],\
                    [0,1,0,0,1,1,0,1,1,0],\
                    [0,1,1,0,1,0,1,1,1,0],\
                    [0,1,1,0,1,0,0,1,1,0],\
                    [0,1,1,1,1,1,1,1,1,0],\
                    [0,0,0,0,0,0,0,0,0,0]]

runTest('tag36_11_12', apriltag36_11_12)

apriltag36_11_13 = [[0,0,0,0,0,0,0,0,0,0],\
                    [0,1,1,1,1,1,1,1,1,0],\
                    [0,1,0,1,1,1,0,1,1,0],\
                    [0,1,1,1,0,1,0,0,1,0],\
                    [0,1,0,1,0,0,1,0,1,0],\
                    [0,1,0,0,1,1,1,1,1,0],\
                    [0,1,0,1,1,1,1,0,1,0],\
                    [0,1,0,0,0,0,0,1,1,0],\
                    [0,1,1,1,1,1,1,1,1,0],\
                    [0,0,0,0,0,0,0,0,0,0]]

runTest('tag36_11_13', apriltag36_11_13)

apriltag36_11_14 = [[0,0,0,0,0,0,0,0,0,0],\
                    [0,1,1,1,1,1,1,1,1,0],\
                    [0,1,0,0,1,1,0,1,1,0],\
                    [0,1,0,1,1,0,1,0,1,0],\
                    [0,1,1,1,0,1,0,0,1,0],\
                    [0,1,1,1,0,1,1,0,1,0],\
                    [0,1,0,0,0,0,1,1,1,0],\
                    [0,1,0,1,1,0,1,0,1,0],\
                    [0,1,1,1,1,1,1,1,1,0],\
                    [0,0,0,0,0,0,0,0,0,0]]

runTest('tag36_11_14', apriltag36_11_14)

apriltag36_11_80 = [[0,0,0,0,0,0,0,0,0,0],\
                    [0,1,1,1,1,1,1,1,1,0],\
                    [0,1,1,0,0,1,0,1,1,0],\
                    [0,1,0,0,0,0,1,0,1,0],\
                    [0,1,1,1,1,1,0,1,1,0],\
                    [0,1,1,0,0,1,1,1,1,0],\
                    [0,1,0,1,1,1,1,1,1,0],\
                    [0,1,1,0,1,0,1,1,1,0],\
                    [0,1,1,1,1,1,1,1,1,0],\
                    [0,0,0,0,0,0,0,0,0,0]]

runTest('tag36_11_80', apriltag36_11_80)


apriltag36_11_544= [[0,0,0,0,0,0,0,0,0,0],\
                    [0,1,1,1,1,1,1,1,1,0],\
                    [0,1,0,0,0,0,1,1,1,0],\
                    [0,1,1,0,1,1,1,0,1,0],\
                    [0,1,0,0,1,0,0,1,1,0],\
                    [0,1,0,1,1,0,0,1,1,0],\
                    [0,1,1,0,0,0,1,1,1,0],\
                    [0,1,1,1,0,0,1,1,1,0],\
                    [0,1,1,1,1,1,1,1,1,0],\
                    [0,0,0,0,0,0,0,0,0,0]]

runTest('tag36_11_544', apriltag36_11_544)


test4 = []
print 'Enter 6 rows, each row MUST contain 6 entries of 1s and 0s ONLY!'
print 'EXAMPLE: 1 0 1 0 1 0'
'''
for i in range(6):
    row = input('Row #%s: ' % (i))
    row = row.split()
    test4.append(row)
runTest('#4', test4)
'''
