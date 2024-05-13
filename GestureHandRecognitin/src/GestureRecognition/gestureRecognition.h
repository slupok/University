#ifndef GESTURERECOGNITION_H
#define GESTURERECOGNITION_H
#include <math.h>
#include <QFile>

//#include "imageDataBuffer.h"
#include "ImageProcessing/imageProcessing.h"
#include "../Camera/camera.h"
#include "../Camera/ImageView.h"
#include "ImageProcessing/ipImage.h"
#include "ImageProcessing/ipContext.h"

#define GESTURE_COUNT 4
#define INVARIANTS_COUNT 7
#define MOMENTS_COUNT 5//колво моментов на жест

#define FIVE_INV 0
#define NICE_INV 1

class GestureRecognition : public QObject
{
    Q_OBJECT
public:
    GestureRecognition();
    ~GestureRecognition();
    void startGR();
    void setCaptureView(ImageView *baseImage);
    void setMaskView(ImageView *baseImage);

public slots:
    void onUpdateFrame(QImage frame);

private:
    float NormalizedCentralMoment(int p, int q, int cx, int cy, int m00);

private:
    ImageView *m_captureView;
    ImageView *m_maskView;
    Camera *m_camera;
    IPContext *m_context;

    bool m_block;
    int cntFrame = 0;//костыль
    IPImage *m_image;
    IPImage *m_backgroundMask;
    IPImage *m_mask;
    IPImage *m_tmpMask;

    //[количество жестов] [инварианты(I1 - I7)] [кол-во моментов]
    float m_DataInv[GESTURE_COUNT][INVARIANTS_COUNT][MOMENTS_COUNT] = {

        {//FIVE
/*I1*/            {0.0615554,    0.0603889,    0.0511095,    0.0534412,    0.0507631  },
/*I2*/            {0.000106103,  0.000101047,  8.80011e-05,  8.68162e-05,  7.29581e-05},
/*I3*/            {0.0397348,    0.0394448,    0.0200887,    0.0227824,    0.0188558  },
/*I4*/            {0.0397348,    0.0394447,    0.0200887,    0.0227824,    0.0188558  },
/*I5*/            {0.00157885,   0.00155589,   0.000403554,  0.000519039,  0.000355539},
/*I6*/            {0.000405856,  0.000388815,  0.000141205,  0.000168483,  0.00013763 },
/*I7*/            {0.00101549,   0.00100511,   0.000257469,  0.000331198,  0.000224246} },


        {//NICE
/*I1*/            {0.0739533,  0.0753686 ,  0.0779389,  0.0904668,   0.0904668 },
/*I2*/            {0.00106087, 0.00110573,  0.00123795, 0.0019031,   0.00188523},
/*I3*/            {0.320625,   0.362642,    0.401574,   0.185462,    0.174823  },
/*I4*/            {0.320625,   0.362642,    0.401574,   0.185462,    0.174823  },
/*I5*/            { 0.1028,    0.13151,     0.161262,   0.0343961,   0.030563  },
/*I6*/            {0.009544,   0.0109316,   0.0128851,  0.00645205,  0.00598996},
/*I7*/            {0.0732019,  0.0938967,   0.115233,   0.0239323,   0.0212735 } },

        {//THREE
/*I1*/            { 0.0732608,     0.0712561,    0.0691404,    0.0696871,     0.0688371  },
/*I2*/            { 0.000604777,   0.000549942,  0.000506437,  0.000480408,   0.000421161},
/*I3*/            { 0.0423553,     0.040507,     0.0378789,    0.0374632,     0.0340527  },
/*I4*/            { 0.0423553,     0.040507,     0.0378789,    0.0374632,     0.0340527  },
/*I5*/            { 0.00179397,    0.00164082,   0.00143481,   0.00140349,    0.00115959 },
/*I6*/            { 0.000521337,   0.000488936,  0.000431634,  0.000388656,   0.000351985},
/*I7*/            { 0.00118531,    0.00108318,   0.000946475,  0.000923461,   0.000756144} },

        {//ROCK
/*I1*/            { 0.0920754,  0.1027,     -0.0407256,   0.0942083,   0.0942083 },
/*I2*/            { 0.0011308,  0.00292099,  0.00995682,  0.00262867,  0.00262867},
/*I3*/            { 0.156403,   0.10073,     0.0378789,   0.0643037,   0.0643037 },
/*I4*/            { 0.156403,   0.10073,     0.0378789,   0.0643037,   0.0643037 },
/*I5*/            { 0.0244618,  0.0101465,   0.00143481,  0.00413497,  0.00413497},
/*I6*/            { 0.00179983, 0.00151208,  0.000902898, 0.00107332,  0.00107332},
/*I7*/            { 0.0170237,  0.00703593,  0.000929703, 0.00283758,  0.00283758} }

    };

    float m_fiveInv[7];
    float m_niceInv[7];

};
#endif // GESTURERECOGNITION_H
