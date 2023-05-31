#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QWidget>
#include <QObject>
#include <QSettings>
#include <QMap>
#include <QStringList>

class ConfigurationModel;

namespace Ui {
class configuration;
}

class configuration : public QWidget
{
    Q_OBJECT

public:
    explicit configuration(QWidget *parent = nullptr);
    ~configuration();

    ConfigurationModel * getModel() const { return _configuration; }

    void syncModel();

signals:
    void valueUpdated();


private:
    Ui::configuration *ui;

    ConfigurationModel * _configuration;
};

#endif // CONFIGURATION_H
