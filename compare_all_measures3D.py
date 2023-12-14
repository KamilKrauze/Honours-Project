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
        "CLAHE: CL=10"
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
        "cl4",
        "cl5",
        "cl7",
        "cl8_5",
        "cl10"
    ]

    # List of filepaths
    # HEQ at the start as theres only one dataset
    ciis = [
        HEQ+CII,
    ]
    psnr = [
        HEQ+PSNR
    ]
    ssim = [
        HEQ+SSIM
    ]

    # Obtain the rest of filepaths
    for CLS in AHE_CLS:
        ciis.append(AHE + CLS + "/" + CII)
        psnr.append(AHE + CLS + "/" + PSNR)
        ssim.append(AHE + CLS + "/" + SSIM)

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
        
        ax.plot(x, y, z, marker="o", label=NAMES[i])
        # ax.text(x,y,z, NAMES[i])

    ax.set_xlabel("CII")
    ax.set_ylabel("PSNR")
    ax.set_zlabel("SSIM")

    print("Greatest Value: ")
    
    max = getMaxOfList(measures)
    
    name = NAMES[max[0]]
    val = max[1]
    print(name)
    print(val)
    
    plt.legend()

    plt.show()