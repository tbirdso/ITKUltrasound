#==========================================================================
#
#   Copyright NumFOCUS
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#          http://www.apache.org/licenses/LICENSE-2.0.txt
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#
#==========================================================================*/


import itk
import argparse

parser = argparse.ArgumentParser(description="Make an average of spectra along transducer lines.")
parser.add_argument("-i", "--input-image", nargs="+", action="append", help="Input image(s)", required=True)
parser.add_argument("-o", "--output-image", required=True)
args = parser.parse_args()

itk.auto_progress(2)

PixelType = itk.VariableLengthVector[itk.F]
ImageDimension = 2
ImageType = itk.Image[PixelType, ImageDimension]

filter = itk.Spectra1DAveragingImageFilter[ImageType].New()

for i, input_filename in enumerate(args.input_image):
    print(f"Reading {input_filename}")
    image = itk.imread(input_filename, PixelType)
    filter.SetInput(i, image)

print("Executing the filter")
filter.Update()

print(f"Writing resulting image into file: {args.output_image}")
itk.imwrite(filter.GetOutput(), args.output_image, compression=True)

print("Test finished")
