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

for i in range(0,frame_count.value):
    plt.
    plt.imshow(ds.pixel_array[i], cmap=plt.cm.gray)
    plt.show()