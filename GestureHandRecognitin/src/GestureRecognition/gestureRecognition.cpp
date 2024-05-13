#include <math.h>

#include "gestureRecognition.h"
#include "ImageProcessing/OpenCL/opencl_context.h"

void setBinarizedImage(uint8_t *mask, QImage *img)
{
    int i = 0;
    for(int y = 0; y < img->height(); y++)
        for(int x = 0; x < img->width(); x++)
        {
            i = y*img->width() + x;
            if(mask[i] == 0)
                img->setPixel(x,y,0x000000);
            if(mask[i] == 1)
                img->setPixel(x,y,0x444444);
            if(mask[i] == 2)
                img->setPixel(x,y,0x888888);
            if(mask[i] == 3)
                img->setPixel(x,y,0xCCCCCC);
            if(mask[i] == 4)
                img->setPixel(x,y,0xFFFFFF);
        }
}

GestureRecognition::GestureRecognition() : QObject()
{
    m_context = new OpenclContext();
    m_camera = nullptr;
    m_image = nullptr;
    m_backgroundMask = nullptr;
    m_mask = nullptr;
    m_block = false;
}
GestureRecognition::~GestureRecognition()
{
    if(m_context)
        delete m_context;
    m_context = nullptr;

    if(m_camera)
        delete m_camera;
    m_camera = nullptr;
}
void GestureRecognition::startGR()
{
    QCameraInfo camerainfo = QCameraInfo::defaultCamera();
    m_camera = new Camera();
    m_camera->setCamera(camerainfo);
    m_camera->start();
    connect(m_camera, &CameraHandler::callbackFrame, this, &GestureRecognition::onUpdateFrame);

    //init IPImage with width, height and PixelData
}
static bool m_block_ = false;
void GestureRecognition::onUpdateFrame(QImage frame)
{

    if(m_block_)
        return;
    m_block_ = true;

    IPError error;
    uint width = (uint)frame.width();
    uint height = (uint)frame.height();

    if(cntFrame <= 40 )
    {
        cntFrame++;
        m_block_ = false;
        return;
    }
    if(!m_image)
    {
        // first frame
        m_image = m_context->CreateImage(width, height, PixelType::RGB24, (unsigned char*)frame.bits());
        m_backgroundMask = m_context->CreateImage(width, height, PixelType::RGB24, (unsigned char*)frame.bits());
        error = m_context->GaussianBlur(m_backgroundMask, 3, 4.0f);
        if(error != IPNoError)
            return;
        m_mask = m_context->CreateImage(width, height, PixelType::Grayscale8, 0x0);
        m_tmpMask = m_context->CreateImage(width, height, PixelType::Grayscale8, 0x0);
    }
    else
    {
        m_image->WriteImage((unsigned char*)frame.bits(), width * height * (uint)GetBytesPerPixel(PixelType::RGB24));
    }

    //uint sum = 0;

    error = m_context->GaussianBlur(m_image, 3, 4.0f);
    if(error != IPNoError)
        return;
    error = m_context->FrameDifference(m_image, m_backgroundMask, m_mask, 6);
    if(error != IPNoError)
        return;
    error = m_context->MorphologicalErosion(m_mask, 3);
    if(error != IPNoError)
        return;
    error = m_context->MorphologicalDilation(m_mask, 2);
    if(error != IPNoError)
        return;

    error = m_context->ColorThresholdConversion(m_image, m_mask);
    if(error != IPNoError)
        return;
    error = m_context->copyImage(m_mask, m_tmpMask);
    if(error != IPNoError)
        return;
    error = m_context->MorphologicalErosion(m_mask, 7);
    if(error != IPNoError)
        return;
    error = m_context->MorphologicalDilation(m_mask, 6);
    if(error != IPNoError)
        return;
    error = m_context->BitmapIntersection(m_mask, m_tmpMask);
    if(error != IPNoError)
        return;

   error = m_context->copyImage(m_mask, m_tmpMask);
   if(error != IPNoError)
       return;
   error = m_context->MorphologicalDilation(m_mask, 1);
   if(error != IPNoError)
       return;
   error = m_context->BitmapSubtraction(m_mask, m_tmpMask);
   if(error != IPNoError)
       return;

#if 0
   int centerX = 0;
   int centerY = 0;
   error = m_context->CenterOfBitmap(m_mask, centerX, centerY);
   int m00 = 0;
   error = m_context->CentralMoment(m_image, m_mask, 0, 0, 0, 0, m00);


   //invariants

   float v20 = NormalizedCentralMoment(2, 0, centerX, centerY, m00);
   float v02 = NormalizedCentralMoment(0, 2, centerX, centerY, m00);
   float v11 = NormalizedCentralMoment(1, 1, centerX, centerY, m00);
   float v30 = NormalizedCentralMoment(3, 0, centerX, centerY, m00);
   float v12 = NormalizedCentralMoment(1, 2, centerX, centerY, m00);
   float v21 = NormalizedCentralMoment(2, 1, centerX, centerY, m00);
   float v03 = NormalizedCentralMoment(0, 3, centerX, centerY, m00);

   float I[7];
   I[0] = v20 + v02;
   I[1] = (v20 - v02) * (v20 - v02) + 4 * v11 * v11;
   I[2] = (v30 - 3 * v12) * (v30 - 3 * v12) + (3 * v21 - v03) * (3 * v21 - v03);
   I[3] = (v30 + v12) * (v30 + v12) + (v21 + v03) * (v21 + v03);
   I[4] = (v30 - 3 * v12) * (v30 + v12) * ((v30 + v12)*(v30 + v12) - 3*(v21 + v03)*(v21 + v03)) + (3*v21 - v03)*(v21 + v03) *
           (3*(v30 + v12)*(v30 + v12) - (v21 + v03)*(v21 + v03));
   I[5] = (v20 - v02)*((v30 + v12)*(v30 + v12) - (v21+v03)*(v21+v03)) + 4 * v11 * (v30 + v12) * (v21 + v03);
   I[6] = (3*v21 - v03)  *(v21 + v03) * (3*(v30 + v12)*(v30 + v12)-(v21 + v03)*(v21 + v03)) - (v30 - 3*v12) *(v21 + v02) *
           (3*(v30 + v12)*(v30 + v12) - (v21 + v03)*(v21 + v03));
   qDebug() << I[0] << " " << I[1] << " " << I[2] << " " << I[3] << " " << I[4] << " " << I[5] << " " << I[6];

   int resultInv[GESTURE_COUNT];
   memset(resultInv, 0, GESTURE_COUNT * sizeof(int));

   float minValue;
   int minGesture;
   for(int invariant = 0; invariant < INVARIANTS_COUNT; invariant++)
   {
        minValue = abs(I[invariant] - m_DataInv[0][invariant][0]);
        minGesture = 0;
        for(int gesture = 0; gesture < GESTURE_COUNT; gesture++)
        {
            for(int j = 0; j < MOMENTS_COUNT; j++)
            {
                float val = abs(I[invariant] - m_DataInv[gesture][invariant][j]);
                if(val <= minValue)
                {
                    minValue = val;
                    minGesture = gesture;
                }
            }
        }
        resultInv[minGesture]++;
   }

   float maxValue = resultInv[0];
   int maxGesture = 0;
   for(int i = 0; i < GESTURE_COUNT; i++)
   {
        if(resultInv[i] >= maxValue)
        {
            maxGesture = i;
            maxValue = resultInv[i];
        }
   }
#endif

finish:
    if(m_mask->GetPixelType() == PixelType::Grayscale8)
    {
        QImage maskImage(width, height, QImage::Format_Grayscale8);
        m_mask->ReadImage(maskImage.bits(), width * height * GetBytesPerPixel(PixelType::Grayscale8));
        m_maskView->setImage(maskImage);
    }
    m_captureView->setImage(frame);

    m_block_ = false;
}

void GestureRecognition::setCaptureView(ImageView *baseImage)
{
    m_captureView = baseImage;
}

void GestureRecognition::setMaskView(ImageView *baseImage)
{
    m_maskView = baseImage;
}

float GestureRecognition::NormalizedCentralMoment(int p, int q, int cx, int cy, int m00)
{
    IPError error;

    float denominator = (pow(m00, (p+q)/2 + 1));

    int m = 0;
    error = m_context->CentralMoment(m_image, m_mask, p, q, cx, cy, m);
    float v = (float)m / denominator;

    return v;
}
