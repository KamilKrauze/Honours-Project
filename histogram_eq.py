import os
import cv2 as cv

# Application params
display2user:bool = False

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
    
    file.close()    
except:
    print("File not found...")
    print("Directory or file may not exist")
    

for image in file_import_list:
    src = cv.imread(import_directory+"/"+image, cv.IMREAD_GRAYSCALE)

    if src is None:
        print('Image could not be opened at:\n.' + import_directory+"/"+image)
    else:
        # Histogram equalization
        dst = cv.equalizeHist(src)

        if(display2user):
            cv.imshow("Source Image", src)
            cv.imshow("HE Image", dst)
            cv.waitKey()
        
        # Export file
        export_fp = export_directory +"/"+ image
        cv.imwrite( export_fp , dst)