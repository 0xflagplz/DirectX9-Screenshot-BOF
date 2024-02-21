import struct
from PIL import Image
import sys
import os

def read_log_file(log_file_path):
    with open(log_file_path, 'rb') as log_file:
        # Read the encoded resolution from the log file name
        file_name = os.path.basename(log_file_path)  # Get the file name from the path
        decoded_resolution = file_name.split('_')[-1].split('.')[0].replace('oo', 'x')

        # Read the raw byte stream from the log file
        raw_data = log_file.read()

    return decoded_resolution, raw_data

def assemble_image(log_file_path, output_file_path):
    decoded_resolution, raw_data = read_log_file(log_file_path)

    # Extract width and height from the decoded resolution
    width, height = map(int, decoded_resolution.split('x'))
    pixels = struct.unpack('B' * (width * height * 4), raw_data)

    # create an image from pixel data
    img = Image.frombytes('RGBA', (width, height), bytes(pixels))

    # Save image to output file path
    img.save(output_file_path)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python assemble_image.py <log_file_path> <output_file_path>")
        sys.exit(1)

    log_file_path = sys.argv[1]
    output_file_path = sys.argv[2]

    assemble_image(log_file_path, output_file_path)
