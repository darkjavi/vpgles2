#ifndef VPTESTWINDOW_H
#define VPTESTWINDOW_H

#include <QMainWindow>

namespace Ui {
class vpTestWindow;
}

class vpTestWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit vpTestWindow(QWidget *parent = 0);
    ~vpTestWindow();
    
private:
    Ui::vpTestWindow *ui;
};

#endif // VPTESTWINDOW_H
