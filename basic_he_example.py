import os
import cv2 as cv
from rich.progress import track as prog_bar

import_directory:str = "./exports/dicom-data"
import_file:str = "list.txt"

export_directory:str = "./exports/opencv/histogram-eq"

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
        
except:
    print("File not found...")
    print("Directory or file may not exist")
    


src = cv.imread(import_directory+"/"+file_import_list[0], cv.IMREAD_GRAYSCALE)

if src is None:
    print('Image could not be opened at:\n.' + import_directory+"/"+file_import_list[0])
    exit(0)
    

dst = cv.equalizeHist(src)

cv.imshow('Source Image', src)
cv.imshow('HE Image', dst)

cv.waitKey()