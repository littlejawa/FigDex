#include <stdio.h>
#include <fcntl.h>
#include <math.h>

#include <QPainter>

#include "displaywindow.h"
#include "ui_displaywindow.h"

#include "../../calcul.h"


#define PI_CONST		3.141592653589793

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

void DisplayWindow::paintEvent(QPaintEvent *pEv)
{
    QPainter painter(this) ;
    QRect lstRect = ui->lstResults->rect() ;
    QRect wndRect = this->rect() ;

    unsigned int nbPoints = atoi(ui->boxNb->text().toAscii()) ;

    // définition de notre zone de dessin : à droite des listes/boutons et à 10 pixels de marge de chaque côté
    QRect drawingArea(lstRect.right() + 20, 10,
                       wndRect.width() - lstRect.width() - lstRect.x() - 30,
                       wndRect.height() - 20) ;

    // on recadre la zone de dessin pour former un carré
    if (drawingArea.width() > drawingArea.height())
        drawingArea.setWidth(drawingArea.height()) ;
    else
        drawingArea.setHeight(drawingArea.width()) ;

    printf("Drawing circle from %d/%d to %d/%d\n", drawingArea.x(), drawingArea.y(), drawingArea.right(), drawingArea.bottom()) ;
    painter.drawEllipse(drawingArea);


    if (ui->lstResults->selectedItems().count() > 0)
    {
        QString selection ;
        selection = ui->lstResults->selectedItems().at(0)->text().trimmed() ;

        QStringList points = selection.split(" ") ;

        float x = drawingArea.left() + drawingArea.width()/2 ;
        float y = drawingArea.top() ;

        for (unsigned int i = 1 ; i < points.count() ; i++)
        {
            int iPoint = atoi(points[i].toAscii()) ;

            float destX, destY ;
            float angle ;

            // calcul de l'angle par rapport à la verticale
            angle = ((360 / nbPoints) * (iPoint - 1)) ;

            // conversion par rapport à l'horizontale
            angle = (angle - 90) * -1 ;

            // conversion en radians (pour les fonctions cos() et sin() qui ne gèrent pas les degrés
            angle = angle * PI_CONST / 180 ;

            // on va calculer la position du point destination par rapport au centre du cercle
            // avec x et y définit à partir de la position centrale
            // sin(angle) = (y) / rayon => y = rayon * sin(angle)
            // cos(angle) = (x) / rayon => x = rayon * cos(angle)

            destX = drawingArea.x() + drawingArea.width()/2 + (cosf(angle) * (drawingArea.width()/2)) ;
            destY = drawingArea.y() + drawingArea.height()/2 - (sinf(angle) * drawingArea.height()/2) ;

            printf("Angle = %f, cos=%f, sin=%f\n", angle, cosf(angle), sinf(angle)) ;
            printf("Drawing line from %f/%f to %f/%f\n", x, y, destX, destY) ;
            painter.drawLine(x, y, destX, destY) ;

            x = destX ;
            y = destY ;
        }
        printf("Drawing (final) line from %f/%f to %u/%u\n", x, y, drawingArea.x() + drawingArea.width()/2, drawingArea.y() + 10) ;
        painter.drawLine(x, y, drawingArea.x() + drawingArea.width()/2, drawingArea.y()) ;
    }
}

void DisplayWindow::on_lstResults_itemSelectionChanged()
{
    this->repaint();
}
