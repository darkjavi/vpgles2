#ifndef VPWIZARDMINDFLEX_H
#define VPWIZARDMINDFLEX_H

#include <QDialog>
#include <QFileDialog>

namespace Ui {
class vpWizardMindflex;
}

class vpWizardMindflex : public QDialog
{
    Q_OBJECT
    
public:
    explicit vpWizardMindflex(QWidget *parent = 0);
    ~vpWizardMindflex();
    
private:
    Ui::vpWizardMindflex *ui;
    QFileDialog m_file_dialog;
};

#endif // VPWIZARDMINDFLEX_H
