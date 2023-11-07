import numpy as np
import pydicom as dicom
import cv2 as cv
import matplotlib.pyplot as plt
from rich.progress import track as prog_bar

ds = dicom.dcmread("./P_1_SR3")


# Type of image
img_type = ds['ImageType']

# Photometric Interpreation
photometric_interp = ds[0x0028, 0x0004]

# Total amount of frames
frame_count = ds[0x0028,0x0008]

# Image dimensions
rows = ds[0x0028,0x0010]
cols = ds[0x0028,0x0011]
dimensions = (int(rows.value), int(cols.value))

# Image pixel spacing
pixel_spacing = ds[0x0018, 0x1164]

# Turn off the axes and labels
plt.axis('off')
plt.tick_params(axis='both', which='both', bottom=False, top=False, labelbottom=False, right=False, left=False, labelleft=False)
    
# Progress bar with rich package - https://stackoverflow.com/questions/71923704/new-color-terminal-prograss-bar-in-pip - 07/11/2023
for i in prog_bar( range(0,frame_count.value), description="Exporting frames: " ):
    
    # Plot frame by frame in grayscale
    plt.imshow(ds.pixel_array[i], cmap=plt.cm.gray)
    
    if ( i < 10 ):
        plt.savefig("./exports/frame" + "0" + str(i) +".png")
    else:
        plt.savefig("./exports/frame" + str(i) +".png")