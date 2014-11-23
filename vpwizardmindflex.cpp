#include "vpwizardmindflex.h"
#include "ui_vpwizardmindflex.h"

vpWizardMindflex::vpWizardMindflex(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::vpWizardMindflex)
{
    ui->setupUi(this);
    ui->ly_file_explorer->addWidget(&m_file_dialog);
}

vpWizardMindflex::~vpWizardMindflex()
{
    delete ui;
}
