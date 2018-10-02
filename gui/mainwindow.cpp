// ----- Headers --------------------------------------------------------------
#include "mainwindow.h"
#include "ui_mainwindow.h"

// ----- Constructor / Destructor ---------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    imageData(MyImage("Image Data"))
{
    ui->setupUi(this);

    // --- Build the application components ---
    buildDirectories();
    buildMenu();
    buildButtons();
    buildComboBoxes();
    buildLCDs();
    buildSliderBars();
    buildGraphics();
    buildPlots();

    // TESTING
    openDefault();
}
MainWindow::~MainWindow()
{
    delete ui;
}

// ----- Build Methods --------------------------------------------------------
void MainWindow::buildResources()
{
    imageFileList.clear();
//    imageFileList.append("Lenna (large)");
    imageFileList.append("Lenna (small)");
    imageFileList.append("Lenna (medium)");
    imageFileList.append("Lenna (large)");
    imageFileList.append("Autobot");
    imageFileList.append("Decepticon");
    imageFileList.append("Halo");
    imageFileList.append("University of Tennessee");
    imageFileList.append("Astronaut");
    imageFileList.append("Astronaut (resized)");
    imageFileList.append("Test");
    imageFileList.append("Sample 1");
    imageFileList.append("Sample 2");
    imageFileList.append("Sample 3");
    imageFileList.append("Sample 4");
    imageFileList.append("Sample 5");

    imageFilterList.clear();
    imageFilterList.append("No Filter (default)");
    imageFilterList.append("Low  Pass: Ideal");
    imageFilterList.append("Low  Pass: Butterworth");
    imageFilterList.append("Low  Pass: Gaussian");
    imageFilterList.append("High Pass: Ideal");
    imageFilterList.append("High Pass: Butterworth");
    imageFilterList.append("High Pass: Gaussian");
}
void MainWindow::buildDirectories()
{
    QString desktopPath = QDir::homePath() + QString("/Desktop");
    defaultDirectory = QDir(desktopPath);
}
void MainWindow::buildMenu()
{
    // --- New Menu Bars ---
    fileMenu = menuBar()->addMenu(tr("&File"));
    helpMenu = menuBar()->addMenu(tr("&Help"));

    // --- New Menu Actions ---
    openDefaultAction = new QAction(tr("Open &Default Image"), this);
    openAction = new QAction(tr("&Open from File"), this);
    resetAction = new QAction(tr("&Reset Image"), this);
    saveAction = new QAction(tr("&Save"), this);
    saveAsAction = new QAction(tr("Save &As..."), this);
    closeAction = new QAction(tr("&Close"), this);
    exitAction = new QAction(tr("&Exit"), this);
    aboutAction = new QAction(tr("&About This Application"), this);
    aboutQtAction = new QAction(tr("&About Qt"), this);
    aboutAuthorAction = new QAction(tr("&About Author"), this);

    // --- Set File Menu Shortcuts ---
    openDefaultAction->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_O));
    openAction->setShortcut(QKeySequence::Open);
    resetAction->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_R));
    saveAction->setShortcut(QKeySequence::Save);
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    closeAction->setShortcut(QKeySequence::Close);
    exitAction->setShortcut(QKeySequence::Quit);

    // --- Connect menu actions to slots ---
    connect(openDefaultAction, SIGNAL(triggered()), this, SLOT(openDefault()));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
    connect(resetAction, SIGNAL(triggered()), this, SLOT(reset()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(quit()));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(aboutAuthorAction, SIGNAL(triggered()), this, SLOT(aboutAuthor()));

    // --- Add the menu actions to the menu bar---
    fileMenu->addAction(openDefaultAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(resetAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addAction(closeAction);
    fileMenu->addAction(exitAction);
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
    helpMenu->addAction(aboutAuthorAction);
}
void MainWindow::buildButtons()
{
    connect(ui->pushButtonFilterSweep, SIGNAL(clicked(bool)), this, SLOT(sweepFilter()));
}
void MainWindow::buildComboBoxes()
{
    buildResources();

    ui->comboBoxImageSelector->addItems(imageFileList);
    connect(ui->comboBoxImageSelector, SIGNAL(activated(int)), this, SLOT(openDefault()));

    ui->comboBoxFilterSelector->addItems(imageFilterList);
    connect(ui->comboBoxFilterSelector, SIGNAL(activated(int)), this, SLOT(applySelectedFilter()));
}
void MainWindow::buildSliderBars()
{
    // --- Connect the color sliders to their LCD value slot ---
    connect(ui->sliderA, SIGNAL(valueChanged(int)), this, SLOT(updateSliderA()));
    connect(ui->sliderB, SIGNAL(valueChanged(int)), this, SLOT(updateSliderB()));

    // --- Connect the color sliders to the color remap slots ---
    connect(ui->sliderA, SIGNAL(sliderReleased()), this, SLOT(applySelectedFilter()));
    connect(ui->sliderB, SIGNAL(sliderReleased()), this, SLOT(applySelectedFilter()));

    // --- Set the range of the color sliders ---
    int tmpMin = 1;
    int tmpMax = 100;

    ui->sliderA->setRange(tmpMin, tmpMax);
    ui->sliderB->setRange(tmpMin, tmpMax);

    // --- Set the default value for the sliders ---
    ui->sliderA->setValue(tmpMax);
    ui->sliderB->setValue(tmpMax);
}
void MainWindow::buildLCDs()
{
    ui->lcdA->setDigitCount(4);
    ui->lcdB->setDigitCount(3);
    ui->lcdSize1->setDigitCount(4);
    ui->lcdSize2->setDigitCount(4);

    ui->lcdMean1Red->setDigitCount(6);
    ui->lcdMean1Green->setDigitCount(6);
    ui->lcdMean1Blue->setDigitCount(6);
    ui->lcdMean2Red->setDigitCount(6);
    ui->lcdMean2Green->setDigitCount(6);
    ui->lcdMean2Blue->setDigitCount(6);

    ui->lcdVar1Red->setDigitCount(6);
    ui->lcdVar1Green->setDigitCount(6);
    ui->lcdVar1Blue->setDigitCount(6);
    ui->lcdVar2Red->setDigitCount(6);
    ui->lcdVar2Green->setDigitCount(6);
    ui->lcdVar2Blue->setDigitCount(6);
}
void MainWindow::buildGraphics()
{
    item = new QGraphicsPixmapItem;

    rawImageScene = new QGraphicsScene(this);
    resizedImageScene = new QGraphicsScene(this);
    centeredImageScene = new QGraphicsScene(this);
    spectrumRedScene = new QGraphicsScene(this);
    spectrumGreenScene = new QGraphicsScene(this);
    spectrumBlueScene = new QGraphicsScene(this);
    filterMaskScene = new QGraphicsScene(this);
    filteredRedScene = new QGraphicsScene(this);
    filteredGreenScene = new QGraphicsScene(this);
    filteredBlueScene = new QGraphicsScene(this);
    recoveredScene = new QGraphicsScene(this);

    ui->graphicsViewRawImage->setScene(rawImageScene);
    ui->graphicsViewResizedImage->setScene(resizedImageScene);
    ui->graphicsViewCenteredImage->setScene(centeredImageScene);
    ui->graphicsViewSpectrumRed->setScene(spectrumRedScene);
    ui->graphicsViewSpectrumGreen->setScene(spectrumGreenScene);
    ui->graphicsViewSpectrumBlue->setScene(spectrumBlueScene);
    ui->graphicsViewFilterMask->setScene(filterMaskScene);
    ui->graphicsViewFilterRed->setScene(filteredRedScene);
    ui->graphicsViewFilterGreen->setScene(filteredGreenScene);
    ui->graphicsViewFilterBlue->setScene(filteredBlueScene);
    ui->graphicsViewRecoveredImage->setScene(recoveredScene);

    ui->graphicsViewRawImage->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    ui->graphicsViewResizedImage->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    ui->graphicsViewCenteredImage->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    ui->graphicsViewSpectrumRed->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    ui->graphicsViewSpectrumGreen->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    ui->graphicsViewSpectrumBlue->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    ui->graphicsViewFilterMask->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    ui->graphicsViewFilterRed->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    ui->graphicsViewFilterGreen->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    ui->graphicsViewFilterBlue->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    ui->graphicsViewRecoveredImage->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);

    ui->graphicsViewRawImage->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsViewResizedImage->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsViewCenteredImage->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsViewSpectrumRed->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsViewSpectrumGreen->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsViewSpectrumBlue->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsViewFilterMask->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsViewFilterRed->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsViewFilterGreen->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsViewFilterBlue->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsViewRecoveredImage->setDragMode(QGraphicsView::ScrollHandDrag);
}
void MainWindow::buildPlots()
{
    // -- Initialize plot range ---
    ui->customPlotMean->xAxis->setRange(0,1);
    ui->customPlotMean->yAxis->setRange(0,255);

    ui->customPlotVariance->xAxis->setRange(0,1);
    ui->customPlotVariance->yAxis->setRange(0,25);

    // --- Add plot titles ---
    ui->customPlotMean->plotLayout()->insertRow(0);
    ui->customPlotMean->plotLayout()->addElement(0, 0, new QCPTextElement(ui->customPlotMean, "Channel Mean Plots", QFont("sans", 10, QFont::Bold)));

    ui->customPlotVariance->plotLayout()->insertRow(0);
    ui->customPlotVariance->plotLayout()->addElement(0, 0, new QCPTextElement(ui->customPlotVariance, "Channel Standard Deviation Plots", QFont("sans", 10, QFont::Bold)));

    // --- Set axes labeling ---
    ui->customPlotMean->xAxis->setLabel("Cuttoff Frequency");
    ui->customPlotMean->yAxis->setLabel("Channel Mean");

    ui->customPlotVariance->xAxis->setLabel("Cuttoff Frequency");
    ui->customPlotVariance->yAxis->setLabel("Channel Standard Deviation");

    // --- Set legends ---
    ui->customPlotMean->legend->setVisible(true);
    ui->customPlotVariance->legend->setVisible(true);

    // --- Add RGB curves ---
    ui->customPlotMean->addGraph();
    ui->customPlotMean->addGraph();
    ui->customPlotMean->addGraph();
    ui->customPlotMean->addGraph();

    ui->customPlotVariance->addGraph();
    ui->customPlotVariance->addGraph();
    ui->customPlotVariance->addGraph();
    ui->customPlotVariance->addGraph();

    // --- Set curve names ---
    ui->customPlotMean->graph(0)->setName("Red");
    ui->customPlotMean->graph(1)->setName("Green");
    ui->customPlotMean->graph(2)->setName("Blue");
    ui->customPlotMean->graph(3)->setName("Grayscale");

    ui->customPlotVariance->graph(0)->setName("Red");
    ui->customPlotVariance->graph(1)->setName("Green");
    ui->customPlotVariance->graph(2)->setName("Blue");
    ui->customPlotVariance->graph(3)->setName("Grayscale");

    // --- Set curve line color ---
    ui->customPlotMean->graph(0)->setPen(QPen(Qt::red));
    ui->customPlotMean->graph(1)->setPen(QPen(Qt::green));
    ui->customPlotMean->graph(2)->setPen(QPen(Qt::blue));
    ui->customPlotMean->graph(3)->setPen(QPen(Qt::black));

    ui->customPlotVariance->graph(0)->setPen(QPen(Qt::red));
    ui->customPlotVariance->graph(1)->setPen(QPen(Qt::green));
    ui->customPlotVariance->graph(2)->setPen(QPen(Qt::blue));
    ui->customPlotVariance->graph(3)->setPen(QPen(Qt::black));

    // --- Define user interaction settings ---
    ui->customPlotMean->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->customPlotVariance->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    ui->customPlotMean->replot();
}
// ----- File Menu Action Slots -----------------------------------------------
void MainWindow::openDefault()
{
    menuStatus("File","Open from Image List");

    QString tmp = ui->comboBoxImageSelector->currentText();
    tmp = QString(":/") +  tmp; // used to define path to a resource file
    appendStatus(QString("Loading from default list ... ") + tmp);

    imageData.loadData(tmp, -1);
    applyDefaultFilter();
}
void MainWindow::open()
{
    menuStatus("File","Open");

    QFileDialog dialog(this);

    // --- Create a dialog window to generate a user path ---
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setViewMode(QFileDialog::Detail);

    dialog.setDirectory(defaultDirectory);
    dialog.setNameFilter(tr("Image (*.png)"));
    dialog.setDefaultSuffix("png");

    if (dialog.exec())
    {
        QStringList fileList = dialog.selectedFiles();
        QString filePath = fileList.at(0);

        appendStatus(QString("Loading from file ... ") + filePath);

        imageData.loadData(filePath);
        applyDefaultFilter();
    }
    else {
        appendStatus(" ... open canceled");
    }
}
void MainWindow::reset()
{
    menuStatus("File","Reset");

    // --- Check if image data exists ---
    if(false)
    {
        appendStatus(QString("No data loaded."));
        return;
    }

    clearGraphicsScenes();
    appendStatus(QString("Images cleared"));
}
void MainWindow::save()
{
    menuStatus("File","Save");

    // --- Check if image data exists ---
    if(false)
    {
        appendStatus(QString("No data loaded, save canceled."));
        return;
    }

    QString filePath = defaultDirectory.path() + "/";

    imageData.saveImageToPNG(filePath);
    appendStatus(QString("Saving to file ... ") + filePath);
}
void MainWindow::saveAs()
{
    menuStatus("File","Save As...");

    if(false)
    {
        appendStatus(QString("No data loaded, save canceled."));
        return;
    }

    // --- Create a dialog window to generate a user path ---
    QFileDialog dialog(this);

    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setViewMode(QFileDialog::Detail);

    dialog.setDirectory(QDir::homePath() + QString("/Desktop"));
    dialog.setNameFilter(tr("Image (*.png)"));
    dialog.setDefaultSuffix("png");

    if (dialog.exec())
    {
        QStringList fileList = dialog.selectedFiles();
        QString filePath = fileList.at(0);
        appendStatus(QString("Saving to file ... ") + filePath);

        imageData.saveImageToPNG(filePath);
    }
    else {
        appendStatus(" ... save canceled");
    }
}
void MainWindow::close()
{
    menuStatus("File","Close");

    // TODO: add an screen clear function here to close current image processing

}
void MainWindow::quit()
{
    menuStatus("File","Quit");
    QApplication::quit();
}

// ----- Help Menu Action Slots -----------------------------------------------
void MainWindow::about()
{
    menuStatus("Help","About Application");
}
void MainWindow::aboutQt()
{

}
void MainWindow::aboutAuthor()
{
    menuStatus("Help","About Author");
}

// ----- Status Bar Slots -----------------------------------------------------
void MainWindow::menuStatus(QString menuString, QString subMenuString)
{
    QString S = " >> ";
    QString tmp = menuString + S + subMenuString + S;
    statusBar()->showMessage(tmp);
}
void MainWindow::appendStatus(QString newString)
{
    statusBar()->showMessage(statusBar()->currentMessage()+newString);
}
void MainWindow::filterStatus()
{
    QString tmp = "Filter adjusted >>\tSettings: [ ";
            tmp += QString::number(ui->sliderA->value()) + ", ";
            tmp += QString::number(ui->sliderB->value());
            tmp += " ]";
    statusBar()->showMessage(tmp);
}

// ---- Graphics Slots -----------------------------------------------
void MainWindow::clearGraphicsScenes()
{
    rawImageScene->clear();
    resizedImageScene->clear();
    centeredImageScene->clear();
    spectrumRedScene->clear();
    spectrumGreenScene->clear();
    spectrumBlueScene->clear();
    filterMaskScene->clear();
    filteredRedScene->clear();
    filteredGreenScene->clear();
    filteredBlueScene->clear();
    recoveredScene->clear();
}
void MainWindow::resizeGraphicsScenes()
{
    double tmp = imageData.getImageSize();

    rawImageScene->setSceneRect(0,0,tmp,tmp);
    resizedImageScene->setSceneRect(0,0,tmp,tmp);
    centeredImageScene->setSceneRect(0,0,tmp,tmp);
    spectrumRedScene->setSceneRect(0,0,tmp,tmp);
    spectrumGreenScene->setSceneRect(0,0,tmp,tmp);
    spectrumBlueScene->setSceneRect(0,0,tmp,tmp);
    filterMaskScene->setSceneRect(0,0,tmp,tmp);
    filteredRedScene->setSceneRect(0,0,tmp,tmp);
    filteredGreenScene->setSceneRect(0,0,tmp,tmp);
    filteredBlueScene->setSceneRect(0,0,tmp,tmp);
    recoveredScene->setSceneRect(0,0,tmp,tmp);
}
void MainWindow::passGraphicsScenes()
{
    rawImageScene->addPixmap(QPixmap::fromImage(imageData.getQImage(0)));
    resizedImageScene->addPixmap(QPixmap::fromImage(imageData.getQImage(1)));
    centeredImageScene->addPixmap(QPixmap::fromImage(imageData.getQImage(3)));
    filterMaskScene->addPixmap(QPixmap::fromImage(imageData.getQImage(4)));
    recoveredScene->addPixmap(QPixmap::fromImage(imageData.getQImage(5)));

    spectrumRedScene->addPixmap(QPixmap::fromImage(imageData.getQImage(12)));
    spectrumGreenScene->addPixmap(QPixmap::fromImage(imageData.getQImage(11)));
    spectrumBlueScene->addPixmap(QPixmap::fromImage(imageData.getQImage(10)));

    filteredRedScene->addPixmap(QPixmap::fromImage(imageData.getQImage(22)));
    filteredGreenScene->addPixmap(QPixmap::fromImage(imageData.getQImage(21)));
    filteredBlueScene->addPixmap(QPixmap::fromImage(imageData.getQImage(20)));
}
void MainWindow::updateSizeLCDs()
{
    ui->lcdSize1->display(imageData.imgResized.rows);
    ui->lcdSize2->display(imageData.imgResized.cols);
}
void MainWindow::updateStatisticsRawLCDs()
{
    ui->lcdMean1Red->display(imageData.getMean1Red());
    ui->lcdMean1Green->display(imageData.getMean1Green());
    ui->lcdMean1Blue->display(imageData.getMean1Blue());

    ui->lcdVar1Red->display(imageData.getStdDev1Red());
    ui->lcdVar1Green->display(imageData.getStdDev1Green());
    ui->lcdVar1Blue->display(imageData.getStdDev1Blue());
}
void MainWindow::updateStatisticsFilteredLCDs()
{
    ui->lcdMean2Red->display(imageData.getMean2Red());
    ui->lcdMean2Green->display(imageData.getMean2Green());
    ui->lcdMean2Blue->display(imageData.getMean2Blue());

    ui->lcdVar2Red->display(imageData.getStdDev2Red());
    ui->lcdVar2Green->display(imageData.getStdDev2Green());
    ui->lcdVar2Blue->display(imageData.getStdDev2Blue());
}
void MainWindow::applyDefaultFilter()
{
    updateSizeLCDs();
    updateStatisticsRawLCDs();

    // --- Reset sliders to defaults ---
    ui->comboBoxFilterSelector->setCurrentIndex(0);
    ui->sliderA->setMaximum(imageData.getImageSize() * 0.750);
    ui->sliderA->setValue(ui->sliderA->maximum());
    ui->sliderB->setValue(ui->sliderB->maximum());

    // --- Apply the default pass-through filter ---
    imageData.filterImage(-1, 0, 0);
    clearGraphicsScenes();
    resizeGraphicsScenes();
    passGraphicsScenes();
}
void MainWindow::applySelectedFilter()
{   
    int filterSelection = 0;
    switch (ui->comboBoxFilterSelector->currentIndex()) {
    case 0:
        filterSelection = -1;
        break;
    case 1:
        filterSelection = 1;
        break;
    case 2:
        filterSelection = 3;
        break;
    case 3:
        filterSelection = 5;
        break;
    case 4:
        filterSelection = 2;
        break;
    case 5:
        filterSelection = 4;
        break;
    case 6:
        filterSelection = 6;
        break;
    default:
        filterSelection = -1;
        break;
    }

    imageData.filterImage(filterSelection,ui->sliderA->value(),ui->sliderB->value());

    updateSizeLCDs();
    updateStatisticsRawLCDs();
    updateStatisticsFilteredLCDs();

    clearGraphicsScenes();
    passGraphicsScenes();
}
// ---- Slider Live Slots -----------------------------------------------------
void MainWindow::updateSliderA()
{
    ui->lcdA->display(ui->sliderA->value());
    filterStatus();
}
void MainWindow::updateSliderB()
{
    ui->lcdB->display(ui->sliderB->value());
    filterStatus();
}
// --- Filter Sweep -----------------------------------------------------------
void MainWindow::sweepFilter()
{
    std::cout << "Running filter sweep" << std::endl;
    int filterSelection = 0;

    switch (ui->comboBoxFilterSelector->currentIndex()) {
    case 0:
        filterSelection = 0; // set back to 0 after testing
        break;
    case 1:
        filterSelection = 1;
        break;
    case 2:
        filterSelection = 3;
        break;
    case 3:
        filterSelection = 5;
        break;
    case 4:
        filterSelection = 2;
        break;
    case 5:
        filterSelection = 4;
        break;
    case 6:
        filterSelection = 6;
        break;
    default:
        break;
    }

    int tmpSetPoint = 1;
    int tmpLimit = ui->sliderA->maximum();
    double tmpNSteps = 250.0;

    if(tmpNSteps > tmpLimit){
        tmpNSteps = tmpLimit;
    }

    double tmpStep = 1.0 * tmpLimit / tmpNSteps;

    QVector<double> tmpX(tmpNSteps),
            tmpRedMean(tmpNSteps), tmpGreenMean(tmpNSteps), tmpBlueMean(tmpNSteps), tmpRGBMean(tmpNSteps),
            tmpRedStdDev(tmpNSteps), tmpGreenStdDev(tmpNSteps), tmpBlueStdDev(tmpNSteps), tmpRGBStdDev(tmpNSteps);

    std::string testWindow = "Filtering...";
    std::string testHighPass = "High-Pass Intensity";
    //cv::namedWindow(testWindow);
    //cv::namedWindow(testHighPass);

    for(int i=0; i<tmpNSteps; i++){

        tmpSetPoint = (i+1) * tmpStep;
        tmpX[i] = tmpSetPoint;

        ui->sliderA->setSliderPosition(tmpSetPoint);
        imageData.filterImage(filterSelection, tmpSetPoint, ui->sliderB->value());

        // --- Plot active filtered image using OpenCV to increase speed ---
        if(ui->comboBoxFilterSelector->currentIndex() > 3){ //select the high-pass filters at end of list
            cv::imshow(testHighPass,imageData.imgHighPassIntensity);
        }
        cv::imshow(testWindow,imageData.imgRecovered);
        cv::waitKey(10);

        tmpRedMean[i] = imageData.getMean2Red();
        tmpGreenMean[i] = imageData.getMean2Green();
        tmpBlueMean[i] = imageData.getMean2Blue();
        tmpRGBMean[i] = (tmpRedMean[i] + tmpGreenMean[i] + tmpBlueMean[i]) / 3.0;

        tmpRedStdDev[i] = imageData.getStdDev2Red();
        tmpGreenStdDev[i] = imageData.getStdDev2Green();
        tmpBlueStdDev[i] = imageData.getStdDev2Blue();
        tmpRGBStdDev[i] = (tmpRedStdDev[i] + tmpGreenStdDev[i] + tmpBlueStdDev[i]) / 3.0;

        ui->lcdMean2Red->display(imageData.getMean2Red());
        ui->lcdMean2Green->display(imageData.getMean2Green());
        ui->lcdMean2Blue->display(imageData.getMean2Blue());

        ui->lcdVar2Red->display(imageData.getStdDev2Red());
        ui->lcdVar2Green->display(imageData.getStdDev2Green());
        ui->lcdVar2Blue->display(imageData.getStdDev2Blue());

        ui->customPlotMean->graph(0)->setData(tmpX,tmpRedMean);
        ui->customPlotMean->graph(1)->setData(tmpX,tmpGreenMean);
        ui->customPlotMean->graph(2)->setData(tmpX,tmpBlueMean);
        ui->customPlotMean->graph(3)->setData(tmpX,tmpRGBMean);

        ui->customPlotVariance->graph(0)->setData(tmpX,tmpRedStdDev);
        ui->customPlotVariance->graph(1)->setData(tmpX,tmpGreenStdDev);
        ui->customPlotVariance->graph(2)->setData(tmpX,tmpBlueStdDev);
        ui->customPlotVariance->graph(3)->setData(tmpX,tmpRGBStdDev);

        ui->customPlotMean->rescaleAxes();
        ui->customPlotVariance->rescaleAxes();

        ui->customPlotMean->replot();
        ui->customPlotVariance->replot();
    }

    ui->customPlotMean->xAxis->setRange(tmpX.first(),tmpX.last());
    ui->customPlotVariance->xAxis->setRange(tmpX.first(),tmpX.last());

    ui->customPlotMean->replot();
    ui->customPlotVariance->replot();

    cv::destroyWindow(testWindow);
    cv::destroyWindow(testHighPass);
}

