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
        heq = cv.equalizeHist(src)

        kernel_size = (8,8) # Too large of a kernel and all detail or entire image can be lost. Stay well below 32.
        # Adaptive HE - https://docs.opencv.org/4.x/d5/daf/tutorial_py_histogram_equalization.html - 07/11/2023
        clahe1 = cv.createCLAHE(clipLimit=1.0, tileGridSize=kernel_size)
        clahe2 = cv.createCLAHE(clipLimit=2.0, tileGridSize=kernel_size)
        clahe3 = cv.createCLAHE(clipLimit=3.0, tileGridSize=kernel_size)
        clahe10 = cv.createCLAHE(clipLimit=10.0, tileGridSize=kernel_size)

        cl1 = clahe1.apply(src)
        cl2 = clahe2.apply(src)
        cl3 = clahe3.apply(src)
        cl10 = clahe10.apply(src)

        if display2user:
            print(image)
            cv.imshow("Source", src)
            cv.imshow("HE", heq)
            cv.imshow("AHE - 1", cl1)
            cv.imshow("AHE - 2", cl2) # Seems to do better
            cv.imshow("AHE - 3", cl3)
            cv.imshow("AHE - 10", cl10)
            cv.waitKey()

        # results = [cl1, cl2, cl3, cl10]
        
        # export_fp = export_directory +"/"
        # for i in range(0,4):
        #     export_fp = export_directory +"/"
        #     if i < 3:
        #         export_fp += "cl"+str(i+1)+"_" + image
        #     else: 
        #         export_fp += "cl"+str(10)+"_" + image   
            
        #     cv.imwrite(export_fp, results[i])