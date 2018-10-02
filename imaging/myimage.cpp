#include "myimage.h"

// ===== CONSTRUCTOR / DESTRUCTOR =============================================
MyImage::MyImage(QString input)
{
    qTitle = input; // arbitrary title for the MyImage object
}
MyImage::~MyImage()
{
    // destructor call goes here
}
// ===== ACCESSORS ============================================================
// --- Get QImage conversions from OpenCV image to use in the GUI ---
QImage MyImage::getQImage(int selector)
{
    cv::Mat tmpRGB;

    switch (selector) {
    case 0:
        //cv::imshow("Input Test",imgRaw);
        imgRaw.convertTo(tmpRGB,CV_8UC3,255,0);
        cv::cvtColor(tmpRGB, tmpRGB, CV_BGR2RGB);
        break;
    case 1:
        //cv::imshow("Resized Test",imgResized);
        imgResized.convertTo(tmpRGB,CV_8UC3,255,0);
        cv::cvtColor(tmpRGB, tmpRGB, CV_BGR2RGB);
        break;
    case 3:
        //cv::imshow("Centered Test",imgCentered);
        imgCentered.convertTo(tmpRGB,CV_8UC3,255,0);
        cv::cvtColor(tmpRGB, tmpRGB, CV_BGR2RGB);
        break;
    case 4:
        realMask.convertTo(tmpRGB,CV_8UC1,255,0);
        cv::cvtColor(tmpRGB, tmpRGB, CV_GRAY2RGB);
        break;
    case 5:
        //cv::imshow("Output Test",imgRecovered);
        imgRecovered.convertTo(tmpRGB,CV_8UC3,255,0);
        cv::cvtColor(tmpRGB, tmpRGB, CV_BGR2RGB);
        break;
    case 10:
        realBlueSpectrum.convertTo(tmpRGB,CV_8UC1,255,0);
        cv::cvtColor(tmpRGB, tmpRGB, CV_GRAY2RGB);
        break;
    case 11:
        realGreenSpectrum.convertTo(tmpRGB,CV_8UC1,255,0);
        cv::cvtColor(tmpRGB, tmpRGB, CV_GRAY2RGB);
        break;
    case 12:
        realRedSpectrum.convertTo(tmpRGB,CV_8UC1,255,0);
        cv::cvtColor(tmpRGB, tmpRGB, CV_GRAY2RGB);
        break;
    case 20:
        realBlueFilter.convertTo(tmpRGB,CV_8UC1,255,0);
        cv::cvtColor(tmpRGB, tmpRGB, CV_GRAY2RGB);
        break;
    case 21:
        realGreenFilter.convertTo(tmpRGB,CV_8UC1,255,0);
        cv::cvtColor(tmpRGB, tmpRGB, CV_GRAY2RGB);
        break;
    case 22:
        realRedFilter.convertTo(tmpRGB,CV_8UC1,255,0);
        cv::cvtColor(tmpRGB, tmpRGB, CV_GRAY2RGB);
        break;
    default:
        imgRaw.convertTo(tmpRGB,CV_8UC3,255,0);
        cv::cvtColor(imgRaw, tmpRGB, CV_BGR2RGB);
        break;
    }

    QImage dest((const uchar *)
                tmpRGB.data,
                tmpRGB.cols,
                tmpRGB.rows,
                tmpRGB.step,
                QImage::Format_RGB888);

    tmpRGB.release();
    dest.bits();
    return dest;
}
// --- Save the OpenCV image to a PNG file ---
void MyImage::saveImageToPNG(QString outputPath)
{
    std::vector<int> compression_parameters;
    compression_parameters.push_back(cv::IMWRITE_PNG_COMPRESSION);
    compression_parameters.push_back(3);

    outputPath += "/";
    QString tmpString;
    cv::Mat tmpRGB;

    tmpString = "img_raw.png";
    imgRaw.convertTo(tmpRGB,CV_8UC3,255,0);
    cv::imwrite((outputPath+tmpString).toStdString(), tmpRGB, compression_parameters);

    tmpString = "img_mask.png";
    realMask.convertTo(tmpRGB,CV_8UC1,255,0);
    cv::imwrite((outputPath+tmpString).toStdString(), tmpRGB, compression_parameters);

    tmpString = "img_output.png";
    imgRecovered.convertTo(tmpRGB,CV_8UC3,255,0);
    cv::imwrite((outputPath+tmpString).toStdString(), tmpRGB, compression_parameters);

    tmpRGB.release();
}
double MyImage::getImageSize()
{
    return sizeM;
}
// ===== CHANNEL STATISTIC ACCESSORS ==========================================
double MyImage::getMean1Blue(){ return meanRaw.val[0] * 255.0; }
double MyImage::getMean1Green(){ return meanRaw.val[1] * 255.0; }
double MyImage::getMean1Red(){ return meanRaw.val[2] * 255.0; }

double MyImage::getMean2Blue(){ return meanFiltered.val[0] * 255.0; }
double MyImage::getMean2Green(){ return meanFiltered.val[1] * 255.0; }
double MyImage::getMean2Red(){ return meanFiltered.val[2] * 255.0; }

double MyImage::getStdDev1Blue(){ return stdDevRaw.val[0] * 255.0; }
double MyImage::getStdDev1Green(){ return stdDevRaw.val[1] * 255.0; }
double MyImage::getStdDev1Red(){ return stdDevRaw.val[2] * 255.0; }

double MyImage::getStdDev2Blue(){ return stdDevFiltered.val[0] * 255.0; }
double MyImage::getStdDev2Green(){ return stdDevFiltered.val[1] * 255.0; }
double MyImage::getStdDev2Red(){ return stdDevFiltered.val[2] * 255.0; }

// ===== INITIAL MUTATORS =====================================================
// --- Set image to data read from file ---
void MyImage::loadData(QString filePath)
{
    //imgRaw.release();
    imgRaw = cv::imread(filePath.toStdString(), cv::IMREAD_COLOR);
    preFilter();
}
// --- Set image to data read from default file or to default flat intensity ---
void MyImage::loadData(QString filePath, int intensityValue)
{
    //cv::Mat tmp;
    imgRaw.release();

    // --- Read image from the default file list ---
    if(intensityValue<0) {
        QFile file(filePath);

        if(file.open(QIODevice::ReadOnly)) {
            qint64 imageFileSize = file.size();
            std::vector<uchar> buf(imageFileSize);

            file.read((char*)buf.data(), imageFileSize);

            imgRaw = cv::imdecode(cv::Mat(buf), cv::IMREAD_ANYCOLOR);
        }
    }
    // --- Generate uniform dummy image ---
    else {
        imgRaw = cv::Mat::ones(256, 256, CV_32FC3) * intensityValue;
    }
    //tmp.copyTo(imgRaw);
    preFilter();
}

// ===== MUTATORS =============================================================
void MyImage::preFilter()
{
    resizeInputImage();
    centerInputImage();
    transform2Spectrum();
}
void MyImage::resizeInputImage()
{
    // --- Convert input image to the correct 32-bit floating RGB values ---
    imgRaw.convertTo(imgRaw,CV_32FC3,1.0/255,0);
    cv::meanStdDev(imgRaw, meanRaw, stdDevRaw);

    // --- Get the minimum size dimension from row and column size ---
    int tmpMin = imgRaw.rows;

    if(imgRaw.cols < tmpMin){
        tmpMin = imgRaw.cols;
    }

    // --- Define optimized sizing variables to use with image ---
    for(int i=1; i<12; i++){
        if (pow(2,i)>tmpMin){
            sizeM = pow(2,i-1);
            sizeN = sizeM;
            break;
        }
    }

    // --- Create the crop region of interest ---
    cv::Rect croppedRegion;
    croppedRegion.x = floor((imgRaw.cols - sizeM) / 2.0);
    croppedRegion.y = floor((imgRaw.rows - sizeN) / 2.0);
    croppedRegion.width = sizeM;
    croppedRegion.height = sizeN;

    // --- Crop the image to appropriate size for DFT/FFT ---
    cv::Mat tmpROI = imgRaw(croppedRegion);
    imgResized.release();
    tmpROI.copyTo(imgResized);
}
void MyImage::centerInputImage()
{
    imgCheckerBoard = cv::Mat::zeros(sizeM,sizeN,CV_32FC1);

    for(int x=0; x<sizeM; x++){
        for(int y=0; y<sizeN; y++){
            if((x+y) % 2 == 0){
                imgCheckerBoard.at<float>(x,y) = 1.0;
            }
            else{
                imgCheckerBoard.at<float>(x,y) = -1.0; // TODO : change to -1
            }
        }
    }

    cv::Mat channelsBGR[3];
    cv::split(imgResized, channelsBGR);

    for(int i=0; i<3 ; i++){
        cv::multiply(channelsBGR[i], imgCheckerBoard, channelsBGR[i], 1, CV_32FC1);
    }

    cv::merge(channelsBGR, 3, imgCentered);
}
void MyImage::transform2Spectrum()
{
    cv::Mat channelsBGR[3];
    cv::split(imgCentered, channelsBGR);

    for(int i=0; i<3 ; i++){

        // --- Generate complex data matrix to hold the DFT output ---
        cv::Mat tmpComplex;
        cv::Mat tmpPlanes[] = {cv::Mat_<float>(channelsBGR[i]), cv::Mat::zeros(channelsBGR[i].size(), CV_32FC1)};
        cv::merge(tmpPlanes, 2, tmpComplex);

        // --- Apply FFT via the DFT method ---
        cv::dft(tmpComplex, tmpComplex, cv::DFT_SCALE);

        // --- Extract real spectrum from the complex spectrum ---
        cv::Mat tmpReal = cv::Mat::zeros(sizeM, sizeN, channelsBGR[i].type());
        cv::split(tmpComplex,tmpPlanes);
        cv::magnitude(tmpPlanes[0], tmpPlanes[1], tmpReal);

        // --- Adjust real spectrum scaling for output display ---
        cv::log(tmpReal,tmpReal);
        cv::normalize(tmpReal, tmpReal, 1, 0, cv::NORM_MINMAX, CV_32FC1);

        // --- ---
        switch (i) {
        case 0: // blue channel
            tmpComplex.copyTo(complexBlueSpectrum);
            tmpReal.copyTo(realBlueSpectrum);
        case 1: // green channel
            tmpComplex.copyTo(complexGreenSpectrum);
            tmpReal.copyTo(realGreenSpectrum);
            break;
        case 2: // red channel
            tmpComplex.copyTo(complexRedSpectrum);
            tmpReal.copyTo(realRedSpectrum);
            break;
        default:
            break;
        }
    }
}
void MyImage::filterImage(int filterSelector, double D0, int n)
{
    std::string tmpString;

    switch (filterSelector) {
    case -1: // Full pass filter
        tmpString = "Full Pass";
        break;
    case 1: // Ideal low-pass filter
        tmpString = "Ideal Low Pass";
        break;
    case 2: // Ideal high-pass filter
        tmpString = "Ideal High Pass";
        break;
    case 3: // Butterworth low-pass filter
        tmpString = "Butterworth Low Pass";
        break;
    case 4: // Butterworth high-pass filter
        tmpString = "Butterworth High Pass";
        break;
    case 5: // Gaussian low-pass filter
        tmpString = "Gaussian Low Pass";
        break;
    case 6: // Third high pass filter
        tmpString = "Gaussian High Pass";
        break;
    default: // Full pass filter
        tmpString = "Default";
        break;
    }
    std::cout << "Applying filter: " << tmpString
              << "\t Cutoff frequency: " << D0
              << "\t Sampling: " << n
              << std::endl;

    buildFilter(filterSelector,D0,n);
    applyFilter();
    transform2Spatial(filterSelector);
}
void MyImage::buildFilter(int filterSelector, double D0, int n)
{
    cv::Mat tmp = cv::Mat::zeros(sizeM, sizeN, CV_32FC1);
    double tmpU, tmpV, tmpD;

    for(int u=0; u<sizeM; u++){
        for(int v=0; v<sizeN; v++){
            tmpU = pow(u-sizeM/2,2);
            tmpV = pow(v-sizeN/2,2);
            tmpD = pow((tmpU + tmpV),0.5);
            switch (filterSelector) {
            case -1: // Full pass filter
                tmp.at<float>(u,v) = 1.0;
                break;
            case 1: // Ideal low-pass filter
                if(tmpD <= D0){
                    tmp.at<float>(u,v) = 1.0;
                }
                else{
                    tmp.at<float>(u,v) = 0.0;
                }
                break;
            case 2: // Ideal high-pass filter
                if(tmpD <= D0){
                    tmp.at<float>(u,v) = 0.0;
                }
                else{
                    tmp.at<float>(u,v) = 1.0;
                }
                break;
            case 3: // Butterworth low-pass filter
                tmp.at<float>(u,v) = 1.0 / (1.0 + pow(tmpD/D0, 2.0*n));
                break;
            case 4: // Butterworth high-pass filter
                tmp.at<float>(u,v) = 1.0 / (1.0 + pow(D0/tmpD, 2.0*n));
                break;
            case 5: // Gaussian low-pass filter
                tmp.at<float>(u,v) = 1.0 * exp(-1.0 / 2.0 * std::pow(tmpD,2.0) / std::pow(D0,2.0));
                break;
            case 6: // Third high pass filter
                tmp.at<float>(u,v) = 1.0 - 1.0 * exp(-1.0 / 2.0 * std::pow(tmpD,2.0) / std::pow(D0,2.0));
                break;
            default: // Full pass filter
                tmp.at<float>(u,v) = 1.0;
                break;
            }
        }
    }
    // --- Create the complex mask ---
    complexMask.release();
    cv::Mat tmpPlanes[] = {cv::Mat_<float>(tmp), cv::Mat::zeros(tmp.size(), tmp.type())};
    cv::merge(tmpPlanes, 2, complexMask);

    // --- Extract real mask from the complex mask ---
    realMask = cv::Mat::zeros(sizeM, sizeN, CV_32FC1);
    cv::split(complexMask, tmpPlanes);
    cv::magnitude(tmpPlanes[0], tmpPlanes[1], realMask);

    // --- Adjust real spectrum scaling for output display ---
    realMask += cv::Scalar::all(1);
    cv::log(realMask,realMask);
    cv::normalize(realMask, realMask, 1, 0, cv::NORM_MINMAX, CV_32FC1);
}
void MyImage::applyFilter()
{
    for(int i=0; i<3; i++){
        cv::Mat tmpComplexSpectrum, tmpRealSpectrum, tmpComplexFilter, tmpRealFilter;

        switch (i) {
        case 0:
            tmpComplexSpectrum = complexBlueSpectrum;
            tmpRealSpectrum =realBlueSpectrum;
            break;
        case 1:
            tmpComplexSpectrum = complexGreenSpectrum;
            tmpRealSpectrum = realGreenSpectrum;
            break;
        case 2:
            tmpComplexSpectrum = complexRedSpectrum;
            tmpRealSpectrum = realRedSpectrum;
            break;
        default:
            break;
        }

        cv::mulSpectrums(tmpComplexSpectrum, complexMask, tmpComplexFilter, cv::DFT_ROWS, false);

        cv::multiply(tmpRealSpectrum, realMask, tmpRealFilter, 1, CV_32FC1);
        cv::normalize(tmpRealFilter, tmpRealFilter, 1, 0, cv::NORM_MINMAX, CV_32FC1);

        switch (i) {
        case 0:
            tmpComplexFilter.copyTo(complexBlueFilter);
            tmpRealFilter.copyTo(realBlueFilter);
            break;
        case 1:
            tmpComplexFilter.copyTo(complexGreenFilter);
            tmpRealFilter.copyTo(realGreenFilter);
            break;
        case 2:
            tmpComplexFilter.copyTo(complexRedFilter);
            tmpRealFilter.copyTo(realRedFilter);
            break;
        default:
            break;
        }
    }
}
void MyImage::transform2Spatial(int filterSelector)
{
    cv::Mat channelsBGR[3];

    for(int i=0; i<3; i++){
        cv::Mat tmpComplex;
        cv::Mat tmpReal;

        switch (i) {
        case 0:
            tmpComplex = complexBlueFilter;
            break;
        case 1:
            tmpComplex = complexGreenFilter;
            break;
        case 2:
            tmpComplex = complexRedFilter;
            break;
        default:
            break;
        }

        cv::dft(tmpComplex, tmpReal, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);
        cv::multiply(tmpReal, imgCheckerBoard, tmpReal, 1, CV_32FC1);
        tmpReal.copyTo(channelsBGR[i]);
    }

    imgRecovered.release();
    cv::merge(channelsBGR, 3, imgRecovered);

    if(filterSelector % 2 == 0){

        // --- Convert to the HSV color space ---
        cv::Mat imgRecolor;
        cv::cvtColor(imgRecovered,imgRecolor,CV_BGR2HSV);

        // --- Split to obtain individual channel ---
        cv::Mat tmpChannels[3];
        cv::split(imgRecolor,tmpChannels);

        // --- Extract saturation channel for intensity adjustment ---
        tmpChannels[2].copyTo(imgHighPassIntensity);

        // --- Normalize intensity channel ---
        //cv::exp(imgHighPassIntensity,imgHighPassIntensity);
        cv::normalize(imgHighPassIntensity,imgHighPassIntensity,1,0,cv::NORM_MINMAX);
        imgHighPassIntensity.copyTo(tmpChannels[2]);
        cv::merge(tmpChannels,3,imgRecolor);

        // -- Convert image back to correct colorspace ---
        cv::cvtColor(imgRecolor,imgRecovered,CV_HSV2BGR);
        //cv::normalize(imgRecovered,imgRecovered,1,0,cv::NORM_MINMAX);
    }

    //double tmpMin, tmpMax;
    //cv::minMaxLoc(imgRecovered,&tmpMin,&tmpMax);
    //std::cout << "Range = [" << tmpMin << ", " << tmpMax << " ]" << std::endl;

    cv::meanStdDev(imgRecovered, meanFiltered, stdDevFiltered);
    //std::cout << "Mean: " << meanFiltered << std::endl;
    //std::cout << "StdDev: " << stdDevFiltered << std::endl;

}
