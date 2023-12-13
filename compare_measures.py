import numpy as npy
import matplotlib.pyplot as plt

# Custom modules
from modules import ioUtilties as IO

SELECTION:int = 2

TITLE = ["Contrast Improvement Index", "Peak Signal to Noise Ratio", "Structural Similarity Index Measure"]

HEQ_SRC = [
    "./exports/contrast-measures/heq/ciis.txt",
    "./exports/contrast-measures/heq/psnr.txt",
    "./exports/contrast-measures/heq/ssim.txt"
]

ADAP_HEQ_CL2_SRC = [
    "./exports/contrast-measures/adap-heq/cl2/ciis.txt",
    "./exports/contrast-measures/adap-heq/cl2/psnr.txt",
    "./exports/contrast-measures/adap-heq/cl2/ssim.txt"
]

ADAP_HEQ_CL3_SRC = [
    "./exports/contrast-measures/adap-heq/cl3/ciis.txt",
    "./exports/contrast-measures/adap-heq/cl3/psnr.txt",
    "./exports/contrast-measures/adap-heq/cl3/ssim.txt"
]


ADAP_HEQ_CL4_SRC = [
    "./exports/contrast-measures/adap-heq/cl4/ciis.txt",
    "./exports/contrast-measures/adap-heq/cl4/psnr.txt",
    "./exports/contrast-measures/adap-heq/cl4/ssim.txt"
]

ADAP_HEQ_CL5_SRC = [
    "./exports/contrast-measures/adap-heq/cl5/ciis.txt",
    "./exports/contrast-measures/adap-heq/cl5/psnr.txt",
    "./exports/contrast-measures/adap-heq/cl5/ssim.txt"
]

ADAP_HEQ_CL7_SRC = [
    "./exports/contrast-measures/adap-heq/cl7/ciis.txt",
    "./exports/contrast-measures/adap-heq/cl7/psnr.txt",
    "./exports/contrast-measures/adap-heq/cl7/ssim.txt"
]

ADAP_HEQ_CL8_5_SRC = [
    "./exports/contrast-measures/adap-heq/cl8_5/ciis.txt",
    "./exports/contrast-measures/adap-heq/cl8_5/psnr.txt",
    "./exports/contrast-measures/adap-heq/cl8_5/ssim.txt"
]

ADAP_HEQ_CL10_SRC = [
    "./exports/contrast-measures/adap-heq/cl10/ciis.txt",
    "./exports/contrast-measures/adap-heq/cl10/psnr.txt",
    "./exports/contrast-measures/adap-heq/cl10/ssim.txt"
]

heq = IO.readFileToList( HEQ_SRC[SELECTION] )
adap_heq_cl2 = IO.readFileToList( ADAP_HEQ_CL2_SRC[SELECTION] )
adap_heq_cl3 = IO.readFileToList( ADAP_HEQ_CL3_SRC[SELECTION] )
adap_heq_cl4 = IO.readFileToList( ADAP_HEQ_CL4_SRC[SELECTION] )
adap_heq_cl5 = IO.readFileToList( ADAP_HEQ_CL5_SRC[SELECTION] )
adap_heq_cl7 = IO.readFileToList( ADAP_HEQ_CL7_SRC[SELECTION] )
adap_heq_cl8_5 = IO.readFileToList( ADAP_HEQ_CL8_5_SRC[SELECTION] )
adap_heq_cl10 = IO.readFileToList( ADAP_HEQ_CL10_SRC[SELECTION] )

heq = list( map(npy.float64, heq) )
adap_heq_cl2 = list( map(npy.float64, adap_heq_cl2) )
adap_heq_cl3 = list( map(npy.float64, adap_heq_cl3) )
adap_heq_cl4 = list( map(npy.float64, adap_heq_cl4) )
adap_heq_cl5 = list( map(npy.float64, adap_heq_cl5) )
adap_heq_cl7 = list( map(npy.float64, adap_heq_cl7) )
adap_heq_cl8_5 = list( map(npy.float64, adap_heq_cl8_5) )
adap_heq_cl10 = list( map(npy.float64, adap_heq_cl10) )

x_axis = range(0, len(heq))

plt.title(TITLE[SELECTION] + " per frame")
plt.xlabel("Frames")
plt.ylabel(TITLE[SELECTION])

plt.plot(heq, label="Histogram Equalization")
plt.plot(x_axis, adap_heq_cl2, label="Adaptive Histogram Equalization - Clipping Limit = 2")
plt.plot(x_axis, adap_heq_cl3, label="Adaptive Histogram Equalization - Clipping Limit = 3")
plt.plot(x_axis, adap_heq_cl4, label="Adaptive Histogram Equalization - Clipping Limit = 4")
plt.plot(x_axis, adap_heq_cl5, label="Adaptive Histogram Equalization - Clipping Limit = 5")
plt.plot(x_axis, adap_heq_cl7, label="Adaptive Histogram Equalization - Clipping Limit = 7")
plt.plot(x_axis, adap_heq_cl8_5, label="Adaptive Histogram Equalization - Clipping Limit = 8.5")
plt.plot(x_axis, adap_heq_cl10, label="Adaptive Histogram Equalization - Clipping Limit = 10")

plt.legend()
plt.show()