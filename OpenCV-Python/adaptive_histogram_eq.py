import os
import cv2 as cv
import numpy as np

# Application params
display2user:bool = True

import_directory:str = "./exports/dicom-data"
import_file:str = "list.txt"

export_directory:str = "./exports/opencv/adaptive-histogram-eq"

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
    
for image in file_import_list:
    src = cv.imread(import_directory+"/"+image, cv.IMREAD_GRAYSCALE)
    if src is None:
        print("File not found at:\n" + import_directory+"/"+image)
    else:
        kernel_size = (8,8) # Too large of a kernel and all detail or entire image can be lost. Stay well below 32.
        # Adaptive HE - https://docs.opencv.org/4.x/d5/daf/tutorial_py_histogram_equalization.html - 07/11/2023

        clahe4 = cv.createCLAHE(clipLimit=4, tileGridSize=kernel_size)
        clahe5 = cv.createCLAHE(clipLimit=5, tileGridSize=kernel_size)
        clahe7 = cv.createCLAHE(clipLimit=7, tileGridSize=kernel_size)
        clahe8_5 = cv.createCLAHE(clipLimit=8.5, tileGridSize=kernel_size)

        cl4 = clahe4.apply(src)
        cl5 = clahe5.apply(src)
        cl7 = clahe7.apply(src)
        cl8_5 = clahe8_5.apply(src)

        if display2user:
            print(image)
            cv.imshow("Source", src)
            cv.imshow("AHE - 4", cl4)
            cv.imshow("AHE - 5", cl5) # Seems to do better
            cv.imshow("AHE - 7", cl7)
            cv.imshow("AHE - 8.5", cl8_5)
            cv.waitKey()
        
        export_fp = export_directory +"/"
        
        cv.imwrite(export_fp + "cl4_" + image, cl4)
        cv.imwrite(export_fp + "cl5_" + image, cl5)
        cv.imwrite(export_fp + "cl7_" + image, cl7)
        cv.imwrite(export_fp + "cl8_5_" + image, cl8_5)
