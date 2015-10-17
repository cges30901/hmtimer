#include "selectfiledialog.h"
#include <QFileDialog>

SelectFileDialog::SelectFileDialog(ProgramOptions *programOptions,QWidget *parent):QDialog(parent)
{
    setupUi(this);
    this->programOptions=programOptions;
    lneFilename->setText(programOptions->lneFilename_Text);
    lneParameters->setText(programOptions->lneParameters_Text);
    connect(toolButton,SIGNAL(clicked()),
            this,SLOT(setFile()));
}

void SelectFileDialog::setFile()
{
    QString file=QFileDialog::getOpenFileName(this,QString(),lneFilename->text());
    if (file.length()!=0){
        lneFilename->setText(file);
    }
}

void SelectFileDialog::on_btnOk_clicked()
{
    programOptions->lneFilename_Text=lneFilename->text();
    programOptions->lneParameters_Text=lneParameters->text();
    this->close();
}
