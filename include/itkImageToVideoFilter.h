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
#ifndef itkImageToVideoFilter_h
#define itkImageToVideoFilter_h

#include "itkImage.h"
#include "itkVideoSource.h"
#include "itkVideoStream.h"
#include "itkTemporalRegion.h"
#include "itkMacro.h"

namespace itk
{

/**
 *\class ImageToVideoFilter
 * \brief Base class for filters that use an Image as input and a VideoStream as output
 *
 * ImageToVideoFilter is the base class for all process objects that output
 * VideoStream data and require Image data as input. This class defines
 * the SetInput() method for setting the input to a filter.
 *
 * An implementation of GenerateInputRequestedRegion() is provided here that
 * uses the implementation from TemporalProcessObject to generate input
 * temporal regions and then provides its own mechanism for generating input
 * spatial regions. The default implementation simply takes the requested
 * spatial region from the first frame of output and uses that as the requested
 * region for each of the input frames.
 *
 * \ingroup ITKVideoCore
 */
template <typename TInputImage,
  typename TOutputVideoStream = itk::VideoStream<itk::Image<typename TInputImage::PixelType, TInputImage::ImageDimension - 1>>>
class ITK_TEMPLATE_EXPORT ImageToVideoFilter : public VideoSource<TOutputVideoStream>
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(ImageToVideoFilter);

  /** Standard class type aliases */
  using InputImageType = TInputImage;
  using OutputVideoStreamType = TOutputVideoStream;
  using Self = ImageToVideoFilter<InputImageType, OutputVideoStreamType>;
  using Superclass = VideoSource<OutputVideoStreamType>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;
  using ConstWeakPointer = WeakPointer<const Self>;

  /** Output type alias */
  using OutputFrameType = typename Superclass::OutputFrameType;
  using OutputFrameSpatialRegionType = typename Superclass::OutputFrameSpatialRegionType;
  using OutputTemporalRegionType = typename TOutputVideoStream::TemporalRegionType;

  /** Input type alias */
  using InputImageType = TInputImage;
  using InputImagePointer = typename InputImageType::Pointer;
  using InputImageConstPointer = typename InputImageType::ConstPointer;
  using InputImageRegionType = typename InputImageType::RegionType;
  using InputImagePixelType = typename InputImageType::PixelType;
  using InputImageIndexType = typename InputImageType::IndexType;
  static constexpr unsigned int InputImageDimension = TInputImage::ImageDimension;

  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageToVideoFilter, VideoSource);

  /** Set the input VideoStream for this temporal process object */
  using Superclass::SetInput;
  virtual void
  SetInput(const InputImageType * image);

  virtual void
  SetInput(unsigned int idx, const InputImageType * videoStream);

  /** Get the input Image for this temporal process object */
  const InputImageType *
  GetInput() const;

  const InputImageType *
  GetInput(unsigned int idx) const;

  /** Extend UpdateOutputInformation to propagate largest possible spatial
   * region as well as temporal region. The default implementation here will
   * use the largest spatial region from the first input frame to set the
   * largest spatial region of each of the output frames. This will need to be
   * overwritten for filters that need different behavior (eg: need edge pixels
   * or different spatial regions for different frames) */
  void
  UpdateOutputInformation() override;

  /** Allow the user to set the default axis in the input image that will
   *  correspond to the temporal axis in the output temporal object. */
  itkGetMacro(FrameAxis, InputImageIndexType);
  itkSetMacro(FrameAxis, InputImageIndexType);

  /** TODO: Set requested output region */
  itkGetMacro(RequestedTemporalRegion, TemporalRegion);
  itkSetMacro(RequestedTemporalRegion, TemporalRegion);


protected:
  /** Get a non-const version of the input for internal use when setting
   * input's requested regions. This is the only time input should be modified
   */
  InputImageType *
  GetInput();

  InputImageType *
  GetInput(unsigned int idx);

  /** Override GenerateOutputRequestedRegion to handle the case where no
   * requested spatial region has been set for the frames. By default, we set
   * the requested spatial region of each frame to be its largest possible
   * spatial region. */
  void
  GenerateOutputRequestedRegion(DataObject * output) override;

  /** Extend the default implementation of GenerateInputRequestedRegion from
   * TemporalProcessObject to propagate spatial regions as well as temporal
   * regions. This default implementation takes the requested spatial region
   * from the first requested output frame and applies it to all of the
   * requested input frames. */
  void
  GenerateInputRequestedRegion() override;

  /** Method that gets called before individual temporal requests are
   * dispatched by GenerateData. The default implementation makes sure that
   * the input's buffer can hold enough frames for a single input request. */
  void
  BeforeTemporalStreamingGenerateData() override;

  ImageToVideoFilter();
  ~ImageToVideoFilter() override = default;

  void
  PrintSelf(std::ostream & os, Indent indent) const override;

private:
  typename InputImageIndexType m_FrameDirection;
  
}; // end class ImageToVideoFilter

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkImageToVideoFilter.hxx"
#endif

#endif //itkImageToVideoFilter_h
