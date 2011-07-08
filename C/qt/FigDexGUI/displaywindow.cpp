#include <stdio.h>
#include <fcntl.h>

#include "displaywindow.h"
#include "ui_displaywindow.h"

#include "../../calcul.h"

DisplayWindow::DisplayWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DisplayWindow)
{
    ui->setupUi(this);
}

DisplayWindow::~DisplayWindow()
{
    delete ui;
}

void DisplayWindow::on_btnStart_clicked()
{
    int fd ;
    int ret ;
    unsigned int nbPoints = 3 ;
    unsigned int *tmpBuf ;

    ui->lstResults->clear();

    nbPoints = atoi(ui->boxNb->text().toAscii()) ;
    printf("Doing job with %u points\n", nbPoints) ;

    ret = initCalcul(nbPoints) ;
    if (ret != 0)
    {
        printf("Error in initialization\n") ;
        return ;
    }

    printf("Opening file\n") ;
    unlink("tmpFigDex") ;
    fd = open("tmpFigDex", O_CREAT | O_RDWR, 0666) ;
    if (fd == -1)
    {
        printf("Error opening file\n") ;
        return ;
    }

    printf("Initializing file\n") ;
    ret = write(fd, &nbPoints, sizeof(unsigned int)) ;
    if (ret != sizeof(unsigned int))
    {
        printf("Error initializing file\n") ;
        ::close(fd) ;
        return ;
    }

    printf("Creating list\n") ;
    createList(nbPoints, fd) ;

    printf("Seek into file\n") ;
    lseek(fd, 0, SEEK_SET) ;

    printf("Allocating memory\n") ;
    tmpBuf = (unsigned int *)malloc(sizeof(unsigned int) * nbPoints) ;
    if (!tmpBuf)
    {
        printf("Memory allocation error\n") ;
        return ;
    }

    printf("Reading size from file\n") ;
    ret = read(fd, tmpBuf, sizeof(unsigned int)) ;
    if (ret == sizeof(int) && *tmpBuf == nbPoints)
    {
        printf("Starting read loop...\n") ;
        while (read(fd, tmpBuf, nbPoints * sizeof(int)) == nbPoints * sizeof(int))
        {
            QString tmpStr = "" ;
            QString resultString = "" ;

            for (unsigned int i = 0 ; i < nbPoints ; i++)
            {
                tmpStr.sprintf("%u ", tmpBuf[i]) ;
                resultString += tmpStr ;
            }
            printf("Adding elemenet %s\n", resultString.toAscii().data()) ;
            ui->lstResults->addItem(resultString) ;
        }
    }

    printf("Done -> repaint\n") ;
    ui->lstResults->repaint();

    printf("Closing file\n") ;
    ::close(fd) ;
}

void DisplayWindow::on_lstResults_itemSelectionChanged()
{
    QString selection ;

    selection = ui->lstResults->selectedItems().at(0)->text() ;

    QStringList points = selection.split(" ") ;
    for (int i = 0 ; i < points.count() - 1 ; i++)
    {
        int iPoint = atoi(points[i].toAscii()) ;
        printf("%d\n", iPoint) ;
    }
}
