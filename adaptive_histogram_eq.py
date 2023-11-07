import os
import cv2 as cv
import numpy as np
from matplotlib import pyplot as plt

# https://docs.opencv.org/4.x/d5/daf/tutorial_py_histogram_equalization.html

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

clahe = cv.createCLAHE(clipLimit=3.0, tileGridSize=(8,8))
cl1 = clahe.apply(src)

cv.imshow("Source", src)
cv.imshow("HE", heq)
cv.imshow("AHE", cl1)
cv.waitKey()