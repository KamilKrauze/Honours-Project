import os
import pydicom as dicom
import matplotlib.pyplot as plt
from rich.progress import track as prog_bar

dicom_filepath:str = "./data/P_1_SR3"

export_directory:str = './exports/dicom-data'
export_img_name:str = 'frame'
export_img_type:str = ".png"
export_img_list = list()

# If directory does not exist, create it
if not os.path.exists(export_directory):
    os.makedirs(export_directory)

# Read dicom file - https://pydicom.github.io/pydicom/stable/tutorials/dataset_basics.html - 3/11/2023
ds = dicom.dcmread(dicom_filepath)


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


# Progress bar with rich package - https://stackoverflow.com/questions/71923704/new-color-terminal-prograss-bar-in-pip - 07/11/2023
for i in prog_bar( range(0,frame_count.value), description="Exporting frames: " ):
    
    # Plot frame by frame in grayscale
    plt.imshow(ds.pixel_array[i], cmap=plt.cm.gray)
    
    number = str()
    if ( i < 10 ):
        number = ( "0" + str(i) )
    else:
        number = str(i)
    
    # Export image without figure elements - https://matplotlib.org/stable/api/_as_gen/matplotlib.pyplot.imsave.html - 07/11/2023
    export_path = export_directory +"/"+ export_img_name + number + export_img_type
    plt.imsave(export_path, ds.pixel_array[i], cmap=plt.cm.gray)
    
    export_img_list.append(export_img_name+number+export_img_type) # Add file to list


# Writing the list of images to be read by other programs
list_dir = export_directory+"/list.txt"
export_img_list_description:str = "Writing to "+list_dir+": "
file = open(list_dir, "w")
for j in prog_bar(range(frame_count.value), description=export_img_list_description ):
    if (j != 60):
        file.write( export_img_list[j] + "\n" )
    else:
        file.write( export_img_list[j])
        
print("Success!")

exit(0)