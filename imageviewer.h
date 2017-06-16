#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include <QImage>
#ifndef QT_NO_PRINTER
#include <QPrinter>
#endif

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "medianfilter.h"

class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;

using namespace  cv;

class ImageViewer : public QMainWindow
{
    Q_OBJECT

public:
    ImageViewer();
    bool loadFile(const QString &);
    VideoCapture capture;
    Mat mat_image;

private slots:
    void open();
    void saveAs();
    void print();
    void copy();
    void paste();
    void zoomIn();
    void zoomOut();
    void normalSize();
    void fitToWindow();
    void about();
    void updateVideo();

    void OnFilterWatershed();
    void OnFilterMedian();
    void OnFilterFillHoles();
    void OnFilterKuwahara();
private:
    void createActions();
    void createMenus();
    void updateActions();
    bool saveFile(const QString &fileName);
    void setImage(const QImage &newImage);
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);
    void showMatImage(const Mat& image);

    QImage image;
    QLabel *imageLabel;
    QScrollArea *scrollArea;
    double scaleFactor;

#ifndef QT_NO_PRINTER
    QPrinter printer;
#endif

    QAction *saveAsAct;
    QAction *printAct;
    QAction *copyAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;
    QAction *watershedAct;
    QAction *fillholesAct;
    QAction *medianAct;
    QAction *kuwaharaAct;

    QTimer *timer;
};

#endif
