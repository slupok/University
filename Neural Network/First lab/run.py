import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy
import random
import os
import argparse
from PIL import Image


def compressedImage(image):
    compressed_shape = (10,10)
    compressedImage = numpy.zeros(compressed_shape)
    base_shape = image.shape
    pixelsPerBlock = ((base_shape[0] - 1) // compressed_shape[0] + 1,
                      (base_shape[1] - 1) // compressed_shape[1] + 1)
    for x in range(0, base_shape[0]):
        for y in range(0, base_shape[1]):
            _x = x // pixelsPerBlock[0]
            _y = y // pixelsPerBlock[1]
            if image[x][y][0] != 255 or image[x][y][1] != 255 or image[x][y][2] != 255:
                compressedImage[_x][_y] = 1
                
    return compressedImage

def readImage(fileName):
    os.chdir('c:/University/Neural Network/First lab')
    im = Image.open(fileName)
    image = numpy.asarray(im)
    return compressedImage(image)


def saveWeights(W):
    f = open('Weights.txt', 'w')
    shape = W.shape
    for x in range(0, shape[0]):
        for y in range(0, shape[1]):
            f.write(str(W[x][y]) + ' ')
        f.write('\n')
    f.close()

def readWeights():
    f = open('Weights.txt', 'r')
    W = []
    line_str = f.readline().split(' ')[0:10]
    while line_str[0]:
        W.append([float(x) for x in line_str])
        line_str = f.readline().split(' ')[0:10]
    f.close()
    return W

def F(X, W):
    sum = 0
    for x in  range(0, X.shape[0]):
        for y in  range(0, X.shape[1]):
            sum += X[x][y] * W[x][y]
    return 1 if (sum >= 0) else 0

def modifyWeights(X, W, error):
    lerning_rate = 0.5
    next_W = W + lerning_rate * error * X
    return next_W

def run(fileName):
    #Input X
    X = readImage(fileName)
    
    # Init Weights
    if(os.path.exists('Weights.txt')):
        W = readWeights()
    else:
        print("Weights не найден")

    #Compute Y
    Y = F(X, W)
    print("Крестик" if Y == 1 else "Нолик")


parser = argparse.ArgumentParser(description='Image Info')
parser.add_argument('-f', '--filename')
args = parser.parse_args()

# крестик = 1
# нолик = 0
fileName = 'img/X1.png' if (args.filename is None) else args.filename
run(fileName)