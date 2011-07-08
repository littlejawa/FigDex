#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include <QMainWindow>

namespace Ui {
    class DisplayWindow;
}

class DisplayWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DisplayWindow(QWidget *parent = 0);
    ~DisplayWindow();

private slots:
    void on_btnStart_clicked();

    void on_lstResults_itemSelectionChanged();

private:
    Ui::DisplayWindow *ui;
};

#endif // DISPLAYWINDOW_H
