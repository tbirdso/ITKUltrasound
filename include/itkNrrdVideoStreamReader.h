/*=========================================================================
 *
 *  Copyright NumFOCUS
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkNrrdVideoStreamIO_h
#define itkNrrdVideoStreamIO_h
#include "ITKIONRRDExport.h"


#include "itkImageIOBase.h"
#include <fstream>

struct NrrdEncoding_t;

namespace itk
{
/**
 *\class NrrdVideoStreamIO
 *
 * \brief Read and write the "Nearly Raw Raster Data" (Nrrd) image format.
 * The Nrrd format was developed as part of the Teem package
 * (teem.sourceforge.net).
 *
 * The compressor supported may include "gzip" (default) and
 * "bzip2".  Only the "gzip" compressor support the compression level
 * in the range 0-9.
 *
 *  \ingroup IOFilters
 * \ingroup ITKIONRRD
 */
class ITKIONRRD_EXPORT NrrdVideoReader : public VideoSource
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(NrrdVideoReader);

  /** Standard class type aliases. */
  using Self = NrrdVideoReader;
  using Superclass = VideoSource;
  using Pointer = SmartPointer<Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(NrrdVideoReader, VideoSource);

  /** The different types of ImageIO's can support data of varying
   * dimensionality. For example, some file formats are strictly 2D
   * while others can support 2D, 3D, or even n-D. This method returns
   * true/false as to whether the ImageIO can support the dimension
   * indicated. */
  bool
  SupportsDimension(unsigned long) override;

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  bool
  CanReadFile(const char *) override;

  /** Set the spacing and dimension information for the set filename. */
  void
  ReadImageInformation() override;

  /** Reads the data from disk into the memory buffer provided. */
  void
  Read(void * buffer) override;

  /** Determine the file type. Returns true if this ImageIO can write the
   * file specified. */
  bool
  CanWriteFile(const char *) override;

  /** Set the spacing and dimension information for the set filename. */
  void
  WriteImageInformation() override;

  /** Writes the data to disk from the memory buffer provided. Make sure
   * that the IORegions has been set properly. */
  void
  Write(const void * buffer) override;

protected:
  NrrdVideoStreamIO();
  ~NrrdVideoStreamIO() override;
  void
  PrintSelf(std::ostream & os, Indent indent) const override;

  void
  InternalSetCompressor(const std::string & _compressor) override;

  /** Utility functions for converting between enumerated data type
      representations */
  int
  ITKToNrrdComponentType(const IOComponentEnum) const;

  IOComponentEnum
  NrrdToITKComponentType(const int) const;

  const NrrdEncoding_t * m_NrrdCompressionEncoding{ nullptr };
};
} // end namespace itk

#endif // itkNrrdVideoStreamIO_h
