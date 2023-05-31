#include "ui_configuration.h"

#include <QTextEdit>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QLabel>

#include "configurationModel.h"
#include "configuration.h"

configuration::configuration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::configuration),
    _configuration(new ConfigurationModel(parent))
{
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->horizontalHeader()->hide();
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setFrameStyle(QFrame::NoFrame);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setShowGrid(false);

    setStyleSheet("QTableWidget {background-color: transparent;}"
                  "QHeaderView::section {background-color: transparent;}"
                  "QHeaderView {background-color: transparent;}"
                  "QTableCornerButton::section {background-color: transparent;}");

    connect(_configuration, &ConfigurationModel::valueUpdated, this, &configuration::valueUpdated);
}

configuration::~configuration()
{
    delete ui;
}

void configuration::syncModel()
{
    QMap<QString, Value_t*> m = _configuration->model();

    ui->tableWidget->setRowCount(m.size());

    QString str;
    int row = 0;

    foreach (str, m.keys())
    {
        QWidget * w;

        if (m[str]->type == Type_t::STRING)
        {
            QTextEdit * txt = new QTextEdit(this);
            txt->setText(m[str]->s);

            w = txt;
        }

        if (m[str]->type == Type_t::DOUBLE)
        {
            QDoubleSpinBox * dsb = new QDoubleSpinBox(this);
            dsb->setMaximum(100000);
            dsb->setMinimum(-100000);
            dsb->setValue(m[str]->d) ;

            connect(dsb, &QDoubleSpinBox::valueChanged, this, [this, str](double value) {
                _configuration->setModelValue(str, value);
            });

            w = dsb;
        }

        if (m[str]->type == Type_t::BOOL)
        {
            QCheckBox * chc = new QCheckBox(this);
            chc->setChecked(m[str]->b);

            w = chc;
        }

        QLabel * lbl = new QLabel(this);
        lbl->setText(str);

        ui->tableWidget->setCellWidget(row, 0, lbl);
        ui->tableWidget->setCellWidget(row, 1, w);

        row++;
    }
}

