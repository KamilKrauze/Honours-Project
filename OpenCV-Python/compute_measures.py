import cv2 as cv
import numpy as npy
from rich.progress import track as prog_bar

# Python helper modules
from modules import ioUtilties as IO 

# Python modules for computing constrast measures
import ssim
import psnr
import cii

# Import directories
SOURCE_IMG_DIR:str = "../exports/dicom-data/"
ENHANCED_IMG_DIR:str = "../exports/weighted_dwt/cl2_cl10_fusion_db4/"
SUFFIX:str = "cl2_cl10_"

# Export directories
CIIS_DIR: str = "../exports/contrast-measures/dwt/cl2_cl10_fusion_db4/ciis.txt"
PSNR_DIR: str = "../exports/contrast-measures/dwt/cl2_cl10_fusion_db4/psnr.txt"
SSIM_DIR: str = "../exports/contrast-measures/dwt/cl2_cl10_fusion_db4/ssim.txt"

# If either directory does not exist, exit.
if(not (IO.checkIfDirectoryExists(SOURCE_IMG_DIR)) or not (IO.checkIfDirectoryExists(ENHANCED_IMG_DIR)) ):
    raise SystemExit("ERROR: One or both of these directories do not exist or are invalid.")

sourceImgs = IO.getListofImgs(SOURCE_IMG_DIR, "")
enhancedImgs = IO.getListofImgs(ENHANCED_IMG_DIR, SUFFIX)

# Check whether both lists are of the same size
if (len(sourceImgs) != len(enhancedImgs)):
    raise SystemExit("ERROR: Frame count does not match")

src = any
enh = any

ciis = list()
psnrs = list()
ssims = list()

def computeCIIs():
    for i in prog_bar( range(0, len(sourceImgs)), description="Computing CII's..."):
        src = cv.imread(SOURCE_IMG_DIR + sourceImgs[i])
        enh = cv.imread(ENHANCED_IMG_DIR + enhancedImgs[i])
        
        ciis.append( cii.CII(src, enh) )
        
def computePSNRs():
    for i in prog_bar( range(0, len(sourceImgs)), description="Computing PSNR's..."):
        src = cv.imread(SOURCE_IMG_DIR + sourceImgs[i])
        enh = cv.imread(ENHANCED_IMG_DIR + enhancedImgs[i])
        
        psnrs.append( psnr.PSNR(enh, src) )
        
def computeSSIMs():
    for i in prog_bar( range(0, len(sourceImgs)), description="Computing SSIM's..."):
        src = cv.imread(SOURCE_IMG_DIR + sourceImgs[i])
        enh = cv.imread(ENHANCED_IMG_DIR + enhancedImgs[i])
        
        ssims.append( ssim.SSIM(enh, src) )

computeCIIs()
computePSNRs()
computeSSIMs()
  
IO.writeListToFile(ciis, CIIS_DIR)
IO.writeListToFile(psnrs, PSNR_DIR)
IO.writeListToFile(ssims, SSIM_DIR)