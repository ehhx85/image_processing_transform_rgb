#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// --- Header Definitions ---
#include <cstdlib>
#include <iostream>
#include <math.h>

#include <QMainWindow>

#include <QAction>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QLCDNumber>
#include <QList>
#include <QSlider>
#include <QString>
#include <QTimer>
#include <QThread>

#include "myimage.h"
#include "qcustomplot.h"

// --- Main Window Class ---
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    // --- File Menu Slots ---
    void openDefault();
    void open();
    void reset();
    void save();
    void saveAs();
    void close();
    void quit();

    // --- Help Menu Slots ---
    void about();
    void aboutQt();
    void aboutAuthor();

    // --- Status Bar Slots ---
    void menuStatus(QString menuString,
                    QString subMenuString);
    void appendStatus(QString newString);
    void filterStatus();

    // --- Graphics Slots ---
    void clearGraphicsScenes();
    void resizeGraphicsScenes();
    void passGraphicsScenes();
    void applySelectedFilter();
    void applyDefaultFilter();

    // --- Indicator Slots ---
    void updateSizeLCDs();
    void updateStatisticsRawLCDs();
    void updateStatisticsFilteredLCDs();

    // --- Slider Slots ---
    void updateSliderA();
    void updateSliderB();

    // --- Filter Sweep Slots ---
    void sweepFilter();

private:
    // --- Main Window ---
    Ui::MainWindow *ui;

    // --- Directories ---
    QDir defaultDirectory;

    // --- Menus ---
    QMenu *fileMenu;
    QMenu *helpMenu;

    // --- Actions ---
    QAction *openDefaultAction;
    QAction *openAction;
    QAction *resetAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *closeAction;
    QAction *exitAction;
    QAction *aboutAction;
    QAction *aboutQtAction;
    QAction *aboutAuthorAction;

    // --- Graphics ---
    QGraphicsPixmapItem *item;

    QGraphicsScene *rawImageScene;
    QGraphicsScene *resizedImageScene;
    QGraphicsScene *paddedImageScene;
    QGraphicsScene *centeredImageScene;
    QGraphicsScene *spectrumRedScene;
    QGraphicsScene *spectrumGreenScene;
    QGraphicsScene *spectrumBlueScene;
    QGraphicsScene *filterMaskScene;
    QGraphicsScene *filteredRedScene;
    QGraphicsScene *filteredBlueScene;
    QGraphicsScene *filteredGreenScene;
    QGraphicsScene *recoveredScene;
    QGraphicsScene *croppedScene;

    // --- Images ---
    QList<QString> imageFileList;
    QList<QString> imageFilterList;

    MyImage imageData;

    // --- Build Methods ---
    void buildButtons();
    void buildComboBoxes();
    void buildDirectories();
    void buildGraphics();
    void buildLCDs();
    void buildMenu();
    void buildPlots();
    void buildResources();
    void buildSliderBars();

    // --- General Methods ---
};

#endif // MAINWINDOW_H
