#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <QFrame>
#include <QIntValidator>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class QUInt64Validator : public QValidator {
public:
    QUInt64Validator(QObject *parent = nullptr) : QValidator(parent) {}

    State validate(QString &input, int &pos) const override {
        Q_UNUSED(pos);

        bool ok;
        quint64 value = input.toULongLong(&ok);

        if (ok && value <= Q_UINT64_C(18446744073709551615)) {
            return Acceptable;
        } else {
            return Invalid;
        }
    }
};

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
    QCheckBox *checkBoxes[64];
    QFrame * frames[64];
    QIntValidator  *validator;
    QUInt64Validator  *qvDecvd;
    void setbox(quint64 value);
    void getbox(quint64 &value);
    void changtoLE(void);
    void getlecont(quint64 &value);
    void freamrest(void);
    void addhis(QString cmd,quint64 src,QString cent,quint64 dst);
};
#endif // WIDGET_H
