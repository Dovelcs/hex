#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <QFrame>
#include <QIntValidator>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_lehex_textChanged(const QString &arg1);

    void on_ledec_textChanged(const QString &arg1);

    void on_btrst_clicked();

    void on_btlf_clicked();

    void on_btrf_clicked();

    void on_buand_clicked();

    void on_lecont_textChanged(const QString &arg1);

    void on_btor_clicked();

    void on_btao_clicked();

    void on_btcont_clicked();

    void on_btdiff_clicked();

    void on_btsame_clicked();

    void on_btlock_clicked();

private:
    Ui::Widget *ui;
    QCheckBox *checkBoxes[32];
    QFrame * frames[32];
    QIntValidator *validator;
    QIntValidator *qvDecvd;
    void setbox(quint32 value);
    void getbox(quint32 &value);
    void changtoLE(void);
    void getlecont(quint32 &value);
    void freamrest(void);
    void addhis(QString cmd,quint32 src,QString cent,quint32 dst);
};
#endif // WIDGET_H
