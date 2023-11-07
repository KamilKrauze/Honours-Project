import os
import cv2 as cv
import numpy as np
from matplotlib import pyplot as plt

# Application params
display2user:bool = False

import_directory:str = "./exports/dicom-data"
import_file:str = "list.txt"

export_directory:str = "./exports/opencv/adaptive-histogram-eq"

# If directory does not exist, create it
if not os.path.exists(export_directory):
    os.makedirs(export_directory)
    
# If directory does not exist, create it
if not os.path.exists(export_directory):
    os.makedirs(export_directory)

# Get all files
file_import_list = list(str())
filepath=import_directory+"/"+import_file
try:
    file = open(filepath, "r")    
    for elem in file.readlines():
        file_import_list.append( elem.rstrip('\n') ) # Remove '\n' escape character
    
    file.close()    
except:
    print("File not found...")
    print("Directory or file may not exist")
    
src = cv.imread(import_directory+"/"+file_import_list[0], cv.IMREAD_GRAYSCALE)

heq = cv.equalizeHist(src)

# Adaptive HE - https://docs.opencv.org/4.x/d5/daf/tutorial_py_histogram_equalization.html - 07/11/2023
clahe1 = cv.createCLAHE(clipLimit=1.0, tileGridSize=(8,8))
clahe2 = cv.createCLAHE(clipLimit=2.0, tileGridSize=(8,8))
clahe3 = cv.createCLAHE(clipLimit=3.0, tileGridSize=(8,8))
clahe10 = cv.createCLAHE(clipLimit=10.0, tileGridSize=(8,8))

cl1 = clahe1.apply(src)
cl2 = clahe2.apply(src)
cl3 = clahe3.apply(src)
cl10 = clahe10.apply(src)

cv.imshow("Source", src)
cv.imshow("HE", heq)
cv.imshow("AHE - 1", cl1)
cv.imshow("AHE - 2", cl2)
cv.imshow("AHE - 3", cl3)
cv.imshow("AHE - 10", cl10)
cv.waitKey()