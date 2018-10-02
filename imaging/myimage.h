#ifndef MYIMAGE_H
#define MYIMAGE_H

#include <iostream>
#include <cmath>

#include <QFile>
#include <QImage>
#include <QString>
#include <QVector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class MyImage
{
private:
    QString qTitle;

    // --- Sizing Variables ---
    int sizeM = 0;
    int sizeN = 0;

public:
    // --- Consructor / Destructor ---
    MyImage(QString input);
    ~MyImage();

    // --- RGB Matrices ---
    cv::Mat imgRaw;
    cv::Mat imgResized;
    cv::Mat imgCheckerBoard;
    cv::Mat imgCentered;
    cv::Mat imgRecovered;
    cv::Mat imgHighPassIntensity;
    // --- Complex Grayscale Matrices ---
    cv::Mat complexMask;
    cv::Mat complexBlueSpectrum;
    cv::Mat complexGreenSpectrum;
    cv::Mat complexRedSpectrum;
    cv::Mat complexBlueFilter;
    cv::Mat complexGreenFilter;
    cv::Mat complexRedFilter;

    // --- Real Grayscale Matrices ---
    cv::Mat realMask;
    cv::Mat realBlueSpectrum;
    cv::Mat realGreenSpectrum;
    cv::Mat realRedSpectrum;
    cv::Mat realBlueFilter;
    cv::Mat realGreenFilter;
    cv::Mat realRedFilter;

    // --- ---
    cv::Scalar meanRaw;
    cv::Scalar stdDevRaw;
    cv::Scalar meanFiltered;
    cv::Scalar stdDevFiltered;

    // --- Accessors ---
    QImage getQImage(int selector);
    void saveImageToPNG(QString outputPath);
    double getImageSize();

    // --- Channel Mean Values ---
    double getMean1Blue();
    double getMean1Red();
    double getMean1Green();
    double getMean2Blue();
    double getMean2Red();
    double getMean2Green();

    // --- Channel Variance Values ---
    double getStdDev1Blue();
    double getStdDev1Red();
    double getStdDev1Green();
    double getStdDev2Blue();
    double getStdDev2Red();
    double getStdDev2Green();

    // --- Initial Mutators ---
    void loadData(QString filePath);
    void loadData(QString filePath, int intensityValue);

    // --- Mutators ---
    void preFilter();
    void resizeInputImage();
    void centerInputImage();
    void transform2Spectrum();

    void filterImage(int filterSelector, double D0, int n);
    void buildFilter(int filterSelector, double D0, int n);
    void applyFilter();
    void transform2Spatial(int filterSelector);


};

#endif // MYIMAGE_H
