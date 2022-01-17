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
#ifndef itkSpectra1DNormalizeImageFilter_hxx
#define itkSpectra1DNormalizeImageFilter_hxx


#include "itkImageScanlineIterator.h"
#include "itkTotalProgressReporter.h"

namespace itk
{
template <typename TInputImage, typename TReferenceImage>
void
Spectra1DNormalizeImageFilter<TInputImage, TReferenceImage>::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  typename ReferenceImageType::Pointer reference = const_cast<ReferenceImageType *>(this->GetInput(1));
  // instead of fiddling with cropping the requested region to reference line,
  // we simply request the entire reference line
  reference->SetRequestedRegionToLargestPossibleRegion();
}


template <typename TScalarA, typename TScalarB, unsigned VDimension>
Vector<TScalarA, VDimension>
operator/(const Vector<TScalarA, VDimension> & a, const VariableLengthVector<TScalarB> & b)
{
  Vector<TScalarA, VDimension> result{ a };
  for (unsigned i = 0; i < VDimension; ++i)
  {
    if (b[i] == 0)
    {
      result[i] = 0;
    }
    else
    {
      result[i] /= b[i];
    }
  }
  return result;
}

template <typename TScalarA, typename TScalarB, unsigned VDimension>
VariableLengthVector<TScalarA>
operator/(const VariableLengthVector<TScalarA> & a, const Vector<TScalarB, VDimension> & b)
{
  VariableLengthVector<TScalarA> result{ a };
  for (unsigned i = 0; i < VDimension; ++i)
  {
    if (b[i] == 0)
    {
      result[i] = 0;
    }
    else
    {
      result[i] /= b[i];
    }
  }
  return result;
}

template <typename TInputImage, typename TReferenceImage>
void
Spectra1DNormalizeImageFilter<TInputImage, TReferenceImage>::DynamicThreadedGenerateData(
  const OutputImageRegionType & outputRegionForThread)
{
  const InputImageType *     input = this->GetInput();
  const ReferenceImageType * reference = this->GetInput(1);
  OutputImageType *          output = this->GetOutput();

  TotalProgressReporter progress(this, output->GetRequestedRegion().GetNumberOfPixels());

  MultiThreaderBase * multiThreader = this->GetMultiThreader();
  multiThreader->SetNumberOfWorkUnits(this->GetNumberOfWorkUnits());

  typename ReferenceImageType::IndexType ind1{ 0 }; // initialize all indices zero

  ImageScanlineConstIterator<InputImageType> iIt(input, outputRegionForThread);
  ImageScanlineIterator<OutputImageType>     oIt(output, outputRegionForThread);

  while (!iIt.IsAtEnd())
  {
    while (!iIt.IsAtEndOfLine())
    {
      ind1[0] = iIt.GetIndex()[0]; // set the index along the depth dimension

      oIt.Set(iIt.Get() / reference->GetPixel(ind1));

      ++iIt;
      ++oIt;
    }

    iIt.NextLine();
    oIt.NextLine();
    progress.Completed(outputRegionForThread.GetSize()[0]);
  }
}

} // end namespace itk

#endif // itkSpectra1DNormalizeImageFilter_hxx
