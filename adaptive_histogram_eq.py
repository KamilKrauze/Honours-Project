import os
import cv2 as cv

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
    
    
