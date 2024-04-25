# Installed modules
from typing import List, Tuple

import numpy as npy
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# Custom modules
from modules import ioUtilties as IO

# Returns a tuple of the index and value
def getMaxOfList(values: List[Tuple[npy.float64, npy.float64, npy.float64]]) -> Tuple[int, Tuple[npy.float64, npy.float64, npy.float64]]:
    max: Tuple[npy.float64, npy.float64, npy.float64] = values[0]
    maxIndex: int = 0
    
    print(len(values))
    
    for i in range(1, len(values)):
        if values[i][0] > max[0] and values[i][1] < max[1] and values[i][2] > max[2]:
            max = values[i]
            maxIndex = i
        
    return (maxIndex, max)

if __name__ == "__main__":

    NAMES:str = [
        "HEQ",
        "CLAHE: CL=2",
        "CLAHE: CL=3",
        "CLAHE: CL=4",
        "CLAHE: CL=5",
        "CLAHE: CL=7",
        "CLAHE: CL=8.5",
        "CLAHE: CL=10",
        "PCA: CL2,4",
        "PCA: CL2,10",
        "DWT Image Fusion: CL2,3",
        "DWT Image Fusion: CL2,4",
        "DWT Image Fusion: CL2,10",
        "Weighted PCA-Based DWT Image Fusion: CL2,4",
        "Weighted PCA-Based DWT Image Fusion: CL2,10"
    ]
    
    # File path prefixes
    HEQ = "./exports/contrast-measures/heq/"
    AHE = "./exports/contrast-measures/adap-heq/"

    # File names
    CII:str = "ciis.txt"
    PSNR:str = "psnr.txt"
    SSIM:str = "ssim.txt"

    # CLAHE Clip Limits
    AHE_CLS = [
        "cl2",
        "cl3",
        "cl10"
    ]

    # List of filepaths
    # HEQ at the start as theres only one dataset
    ciis = [
        HEQ+CII,
        AHE+"cl2/"+CII,
        AHE+"cl3/"+CII,
        AHE+"cl4/"+CII,
        AHE+"cl5/"+CII,
        AHE+"cl7/"+CII,
        AHE+"cl8_5/"+CII,
        AHE+"cl10/"+CII,
        "./exports/contrast-measures/pca/cl2_cl4/"+CII,
        "./exports/contrast-measures/pca/cl2_cl10/"+CII,
        "./exports/contrast-measures/dwt/cl2_cl3_fusion_db4/"+CII,
        "./exports/contrast-measures/dwt/cl2_cl4_fusion_db4/"+CII,
        "./exports/contrast-measures/dwt/cl2_cl10_fusion_db4/"+CII,
        "./exports/contrast-measures/weighted_pca_dwt/cl2_cl4/"+CII,
        "./exports/contrast-measures/weighted_pca_dwt/cl2_cl10/"+CII,
    ]
    psnr = [
        HEQ+PSNR,
        AHE+"cl2/"+PSNR,
        AHE+"cl3/"+PSNR,
        AHE+"cl4/"+PSNR,
        AHE+"cl5/"+PSNR,
        AHE+"cl7/"+PSNR,
        AHE+"cl8_5/"+PSNR,
        AHE+"cl10/"+PSNR,
        "./exports/contrast-measures/pca/cl2_cl4/"+PSNR,
        "./exports/contrast-measures/pca/cl2_cl10/"+PSNR,
        "./exports/contrast-measures/dwt/cl2_cl3_fusion_db4/"+PSNR,
        "./exports/contrast-measures/dwt/cl2_cl4_fusion_db4/"+PSNR,
        "./exports/contrast-measures/dwt/cl2_cl10_fusion_db4/"+PSNR,
        "./exports/contrast-measures/weighted_pca_dwt/cl2_cl4/"+PSNR,
        "./exports/contrast-measures/weighted_pca_dwt/cl2_cl10/"+PSNR,
    ]
    ssim = [
        HEQ+SSIM,
        AHE+"cl2/"+SSIM,
        AHE+"cl3/"+SSIM,
        AHE+"cl4/"+SSIM,
        AHE+"cl5/"+SSIM,
        AHE+"cl7/"+SSIM,
        AHE+"cl8_5/"+SSIM,
        AHE+"cl10/"+SSIM,
        "./exports/contrast-measures/pca/cl2_cl4/"+SSIM,
        "./exports/contrast-measures/pca/cl2_cl10/"+SSIM,
        "./exports/contrast-measures/dwt/cl2_cl3_fusion_db4/"+SSIM,
        "./exports/contrast-measures/dwt/cl2_cl4_fusion_db4/"+SSIM,
        "./exports/contrast-measures/dwt/cl2_cl10_fusion_db4/"+SSIM,
        "./exports/contrast-measures/weighted_pca_dwt/cl2_cl4/"+SSIM,
        "./exports/contrast-measures/weighted_pca_dwt/cl2_cl10/"+SSIM,
    ]

    # List of values
    measures = [ (npy.float64(0), npy.float64(0), npy.float64(0)) ]
    measures.remove( (npy.float64(0), npy.float64(0), npy.float64(0)) )
    
    fig = plt.figure()
    ax=fig.add_subplot(111, projection='3d')

    for i in range(0, len(ciis)):
        x = npy.average( npy.float64(IO.readFileToList(ciis[i])))
        y = npy.average( npy.float64(IO.readFileToList(ssim[i])))
        z = npy.average( npy.float64(IO.readFileToList(psnr[i])))
        
        measures.append((x,y,z))
        
        ax.scatter(x,y,z, marker="o", label=NAMES[i])
        ax.plot([x,x], [y,y], [z,0], color='gray')
        ax.plot([x,x], [y,0], [z,z], color='gray')
        ax.plot([x,0], [y,y], [z,z], color='gray')
        # ax.text(x,y,z, NAMES[i])

    ax.set_xlabel("CII")
    ax.set_ylabel("SSIM")
    ax.set_zlabel("PSNR")

    
    # plt.legend()
    plt.show()