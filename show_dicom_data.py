import os
import pydicom as dicom

dicom_filepath:str = "./data/P_1_SR3"
# If directory does not exist, create it
if not os.path.exists(dicom_filepath):
    print("Filepath not found: " + dicom_filepath)
    exit(0)

# Read dicom file - https://pydicom.github.io/pydicom/stable/tutorials/dataset_basics.html - 3/11/2023
ds = dicom.dcmread(dicom_filepath)
print(ds)

