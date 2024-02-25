# DirectX9-Screenshot-BOF
![memez](/img/8gku6o.jpg)
## Overview

Capture Screenshots with DirectX 9   
TBH I haven't tested this too much just seemed like a cool idea

## Notes

- Export screenshots to **%TEMP%** or the **current working directory** as `.tmp` files
- Output format: `time_<width>oo<height>.tmp`
- Reconstruct `.tmp` files into PNG format locally
- Libraries Used - MSVCRT, D3D9

### BOF Execution
```beacon> screenshot-bof```

### Example

![Cobalt Strike Example](/img/example.png)

## Image Reconstruction

To reconstruct the image with this script, follow these steps:

1. Install the required packages:
    ```bash
    pip3 install Pillow
    ```

2. Run the script with the desired log file path and output file path:
    ```bash
    python3 assemble.py <tmp file> output.png
    ```

### Contributers
Big thanks to ma boy Kyle started this
@kyle41111
