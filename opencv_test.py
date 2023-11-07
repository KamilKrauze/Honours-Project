import numpy as np
import pydicom as dicom
import cv2 as cv
import matplotlib.pyplot as plt

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

for i in range(0,int(frame_count.value)):
    plt.imshow(ds.pixel_array[i], cmap=plt.cm.gray)
    plt.savefig("./exports/slice" + str(i) +".png")

# Read the saved image with OpenCV
image = cv.imread("./exports/slice45.png")

# Display the image using OpenCV (optional)
cv.imshow('Matplotlib to OpenCV', image)
cv.waitKey(0)