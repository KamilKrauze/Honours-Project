from typing import List

import numpy as npy
import matplotlib.pyplot as plt

from modules import ioUtilties as IO

sub_folder_selection:int = 0
clipping_selection:int = 1
measure_selection: int = 2

sub_folder = ["heq/", "adap-heq/"]
clipping = ["cl2/", "cl3/", "cl10/"]
files = ["ciis.txt", "psnr.txt", "ssim.txt"]

appending_string:str = ""
if sub_folder_selection == 1:
    appending_string += sub_folder[sub_folder_selection] + clipping[clipping_selection]
else:
    appending_string += sub_folder[sub_folder_selection]


IMPORT_DIRECTORY:str = "./exports/contrast-measures/" + appending_string + files[measure_selection]
if not IO.checkIfDirectoryExists(IMPORT_DIRECTORY):
    raise SystemExit("ERROR: Directory does not exist, or is invalid!")

TITLE:str = ""
names = ["Contrast Improvement Index", "Peak Signal to Noise Ratio", "Structural Similarity Index Measure"]
clipping_vals = ["CL-2", "CL-3", "CL-10"]

if measure_selection == 2:
    TITLE = names[measure_selection] + " " + clipping_vals[clipping_selection]
else:
    TITLE = names[measure_selection]

data = ( IO.readFileToList(IMPORT_DIRECTORY) )

# Convert string list to numerical type - https://stackoverflow.com/questions/58573407/pyplot-plotting-straight-line-always - 15/11/2023
data = list(map(npy.float64, data))

frames = list()
# Fill y-axis from 0 - 60
for i in range(len(data)):
    frames.append(i) 
x_axis = npy.array(frames)

y_axis = ( npy.array(data) )



plt.title(TITLE +" per frame")
plt.plot(x_axis, y_axis, label=TITLE, marker="o")

plt.xlabel("Frames")
plt.ylabel(TITLE)

plt.legend()
plt.show()
