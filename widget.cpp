#include "widget.h"
#include "ui_widget.h"
#include <QIntValidator>
#include <QDebug>
#include <cmath>

#define MYMAXUINT (0xFFFFFFFFFFFFFFFFull)


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/images/icon.png"));
    validator = new QIntValidator(1, 64, this);
    ui->lel->setValidator(validator);
    ui->ler->setValidator(validator);
    qvDecvd = new QUInt64Validator(this);
    ui->ledec->setValidator(qvDecvd);
    QRegExp hexRegExp("^[0-9A-Fa-f]+$");

    QRegExpValidator *hexValidator = new QRegExpValidator(hexRegExp, this);
    ui->lehex->setValidator(hexValidator);

    QRegExp lecontExp("^0x[0-9A-Fa-f]+$|^[0-9]+$");
    QRegExpValidator *leconValidator;
    leconValidator = new QRegExpValidator(lecontExp, this);
    ui->lecont->setValidator(leconValidator);

    QSize windowSize = this->size();
    int windowWidth = windowSize.width();
    int windowHeight = windowSize.height();
    this->setMaximumHeight(windowHeight-95);
    this->resize(windowWidth, windowHeight-95);

    //qDebug() << "Window Width: " << windowWidth;
   // qDebug() << "Window Height: " << windowHeight;

    ui->ledec->setAttribute(Qt::WA_InputMethodEnabled, false);
    ui->lehex->setAttribute(Qt::WA_InputMethodEnabled, false);
    ui->lecont->setAttribute(Qt::WA_InputMethodEnabled, false);
    ui->tecont->setReadOnly(true);
    ui->tecont->setVisible(false);
    ui->tecont->document()->setMaximumBlockCount(10);

    QCheckBox *tmp[64] = {
        ui->cb0, ui->cb1, ui->cb2, ui->cb3,
        ui->cb4, ui->cb5, ui->cb6, ui->cb7,
        ui->cb8, ui->cb9, ui->cb10, ui->cb11,
        ui->cb12, ui->cb13, ui->cb14, ui->cb15,
        ui->cb16, ui->cb17, ui->cb18, ui->cb19,
        ui->cb20, ui->cb21, ui->cb22, ui->cb23,
        ui->cb24, ui->cb25, ui->cb26, ui->cb27,
        ui->cb28, ui->cb29, ui->cb30, ui->cb31,
        ui->cb32, ui->cb33, ui->cb34, ui->cb35,
        ui->cb36, ui->cb37, ui->cb38, ui->cb39,
        ui->cb40, ui->cb41, ui->cb42, ui->cb43,
        ui->cb44, ui->cb45, ui->cb46, ui->cb47,
        ui->cb48, ui->cb49, ui->cb50, ui->cb51,
        ui->cb52, ui->cb53, ui->cb54, ui->cb55,
        ui->cb56, ui->cb57, ui->cb58, ui->cb59,
        ui->cb60, ui->cb61, ui->cb62, ui->cb63
    };
    QFrame *frametmp[64] = {
        ui->frame_0, ui->frame_1, ui->frame_2, ui->frame_3, ui->frame_4,
        ui->frame_5, ui->frame_6, ui->frame_7, ui->frame_8, ui->frame_9,
        ui->frame_10, ui->frame_11, ui->frame_12, ui->frame_13, ui->frame_14,
        ui->frame_15, ui->frame_16, ui->frame_17, ui->frame_18, ui->frame_19,
        ui->frame_20, ui->frame_21, ui->frame_22, ui->frame_23, ui->frame_24,
        ui->frame_25, ui->frame_26, ui->frame_27, ui->frame_28, ui->frame_29,
        ui->frame_30, ui->frame_31,ui->frame_64_1, ui->frame_64_2, ui->frame_64_3, ui->frame_64_4,
        ui->frame_64_5, ui->frame_64_6, ui->frame_64_7, ui->frame_64_8,
        ui->frame_64_9, ui->frame_64_10, ui->frame_64_11, ui->frame_64_12,
        ui->frame_64_13, ui->frame_64_14, ui->frame_64_15, ui->frame_64_16,
        ui->frame_64_17, ui->frame_64_18, ui->frame_64_19, ui->frame_64_20,
        ui->frame_64_21, ui->frame_64_22, ui->frame_64_23, ui->frame_64_24,
        ui->frame_64_25, ui->frame_64_26, ui->frame_64_27, ui->frame_64_28,
        ui->frame_64_29, ui->frame_64_30, ui->frame_64_31, ui->frame_64_32
    };
    QString styleSheet = QString("QFrame { border: 0px ; }");
    for (quint64 i = 0; i < 64; ++i) {
        checkBoxes[i] = tmp[i];
        frames[i] = frametmp[i];
        frames[i]->setStyleSheet(styleSheet);
        connect(checkBoxes[i], &QCheckBox::clicked, this, &Widget::changtoLE);
    }

    delete hexValidator;
    delete leconValidator;
}


Widget::~Widget()
{
    delete validator;
    delete qvDecvd;
    delete ui;
}


void Widget::setbox(quint64 value)
{
    bool isChecked = false;
    for (quint64 i = 0; i < 64; ++i) {
        isChecked = ((value >> i) & 0x1ull) ? true : false;
        checkBoxes[i]->setChecked(isChecked);
    }
}


void Widget::getbox(quint64 &value)
{
    for (quint64 i = 0; i < 64; ++i) {
        if (checkBoxes[i]->isChecked()) {
            value |= (quint64(1) << i);  // 将对应位设置为1
        }
    }
}


void Widget::changtoLE(void)
{
    quint64 value = 0;
    getbox(value);
    //qDebug()<<value<<QString::number(value,16).toUpper()<< endl;
    ui->lehex->setText(QString::number(value,16).toUpper());
    ui->ledec->setText(QString::number(value,10).toUpper());
}


void Widget::on_lehex_textChanged(const QString &arg1)
{

    QString text = arg1.toUpper();  // 转换为大写
    bool ok;
    quint64  value = 0;
    value = text.toULongLong(&ok, 16);  // 尝试将输入文本解析为十六进制整数

    if (!ok || value > MYMAXUINT) {
        ui->lehex->setText("FFFFFFFFFFFFFFFF");
    }

    ui->lehex->setText(QString::number(value,16).toUpper());
    ui->ledec->setText(QString::number(value,10).toUpper());
    setbox(value);
}


void Widget::on_ledec_textChanged(const QString &arg1)
{

    QString text = arg1;
    quint64  value = 0;
    bool ok;

    value = text.toULongLong(&ok, 10);
    if (!ok || value > MYMAXUINT) {
        ui->lehex->setText("18446744073709551615");
    }

    ui->lehex->setText(QString::number(value,16).toUpper());
    ui->ledec->setText(QString::number(value,10).toUpper());
    setbox(value);

}


void Widget::on_btrst_clicked()
{
    ui->lehex->setText("0");
    ui->ledec->setText("0");
    ui->lel->setText("0");
    ui->ler->setText("0");
    ui->lecont->setText("0");
    QString styleSheet = QString("QLabel#lbtips { font-size: %11px; }").arg(11);
    ui->lbtips->setStyleSheet(styleSheet);
    ui->lbtips->setText("Hex||Dec");
    freamrest();
    //ui->tecont->clear();
    //ui->tecont->setVisible(false);
    setbox(0);
}


void Widget::addhis(QString cmd,quint64 src,QString cent,quint64 dst)
{
    static bool flag = true;
    if(flag)
    {
        QSize windowSize = this->size();
        int windowWidth = windowSize.width();
        int windowHeight = windowSize.height();
        this->setMaximumHeight(windowHeight+95);
        this->setMinimumHeight(windowHeight+95);
        this->resize(windowWidth, windowHeight+95);
        flag = false;
    }
    ui->tecont->setVisible(true);
    QString srcdec ="("+ QString::number(src,10).toUpper()+")";
    QString dstdec ="("+  QString::number(dst,10).toUpper()+")";
    ui->tecont->append("0x:"+QString::number(src,16).toUpper()+srcdec +" "+cmd+" "+cent +" = "+"0x:"+QString::number(dst,16).toUpper()+dstdec);
}


void Widget::on_btlf_clicked()
{
    bool ok;
    quint64 uiShiftLeft = ui->lel->text().toULongLong(&ok, 10);
    quint64 uiCurrentVal = ui->ledec->text().toULongLong(&ok, 10);
    quint64 tmpval = 0;
    tmpval = (uiCurrentVal << uiShiftLeft) & MYMAXUINT;
    //qDebug()<<tmpval<<endl;
    freamrest();
    ui->lehex->setText(QString::number(tmpval,16).toUpper());
    ui->ledec->setText(QString::number(tmpval,10).toUpper());
    setbox(tmpval);
    addhis(" << ",uiCurrentVal,ui->lel->text(),tmpval);
}


void Widget::on_btrf_clicked()
{
    bool ok;
    quint64 tmpval = 0;
    quint64 uiShiftRight = ui->ler->text().toULongLong(&ok, 10);
    quint64 uiCurrentVal = ui->ledec->text().toULongLong(&ok, 10);

    tmpval = (uiCurrentVal >> uiShiftRight) & MYMAXUINT;
    freamrest();
    ui->lehex->setText(QString::number(tmpval,16).toUpper());
    ui->ledec->setText(QString::number(tmpval,10).toUpper());
    setbox(tmpval);
    addhis(" >> ",uiCurrentVal,ui->ler->text(),tmpval);

}


void Widget::on_buand_clicked()
{
    bool ok;
    quint64 Val = 0;
    getlecont(Val);
    quint64 uiCurrentVal = ui->ledec->text().toULongLong(&ok, 10);
    quint64 uiAnd = Val & uiCurrentVal;
    freamrest();
    ui->lehex->setText(QString::number(uiAnd,16).toUpper());
    ui->ledec->setText(QString::number(uiAnd,10).toUpper());
    setbox(uiAnd);
    addhis(" & ",uiCurrentVal,ui->lecont->text(),uiAnd);
}


void Widget::on_lecont_textChanged(const QString &arg1)
{

    QString text = arg1;
    bool ok;
    quint64 Value = 0;
    QString styleSheet = QString("QLabel#lbtips { font-size: %15px; }").arg(15);
    ui->lbtips->setStyleSheet(styleSheet);
    if(! text.contains("0x", Qt::CaseInsensitive))
    {
        ui->lbtips->setText("Dec:");
        Value = text.toULongLong(&ok, 10);

        if(ok)
            ui->lecont->setText(QString::number(Value,10).toUpper());
        else
            ui->lecont->setText(QString::number(MYMAXUINT,10).toUpper());


    }
    if((text.size() >= 3) && (text.contains("0x", Qt::CaseInsensitive)))
    {
        ui->lbtips->setText("Hex:");
        Value = text.toULongLong(&ok, 16);
                //qDebug()<<Value<<endl;

        if(ok)
            ui->lecont->setText("0x"+QString::number(Value,16).toUpper());
        else
            ui->lecont->setText("0x"+QString::number(MYMAXUINT,16).toUpper());
    }

}


void Widget::getlecont(quint64 &value)
{
    QString text =  ui->lecont->text();
    bool ok;
    //QRegularExpression regex("[a-fA-F]");
    //QRegularExpressionMatch match = regex.match(text);
    if(! text.contains("0x", Qt::CaseInsensitive))
    {
        value = text.toULongLong(&ok, 10);
    }
    if((text.size() >= 3) && (text.contains("0x", Qt::CaseInsensitive)))
    {
        value = text.toULongLong(&ok, 16);
    }
}


void Widget::on_btor_clicked()
{
    bool ok;
    quint64 Val = 0;
    getlecont(Val);
    quint64 uiCurrentVal = ui->ledec->text().toULongLong(&ok, 10);
    quint64 uiOr = Val | uiCurrentVal;
    freamrest();
    ui->lehex->setText(QString::number(uiOr,16).toUpper());
    ui->ledec->setText(QString::number(uiOr,10).toUpper());
    setbox(uiOr);
    addhis(" | ",uiCurrentVal,ui->lecont->text(),uiOr);
}


void Widget::on_btao_clicked()
{
    bool ok;
    quint64 Val = 0;
    getlecont(Val);
    quint64 uiCurrentVal = ui->ledec->text().toULongLong(&ok, 10);
    quint64 uiXOR = Val ^ uiCurrentVal;
    freamrest();
    ui->lehex->setText(QString::number(uiXOR,16).toUpper());
    ui->ledec->setText(QString::number(uiXOR,10).toUpper());
    setbox(uiXOR);
    addhis(" ^ ",uiCurrentVal,ui->lecont->text(),uiXOR);
}


void Widget::on_btcont_clicked()
{
    bool ok;
    quint64 Val = 0;
    //getlecont(Val);
    quint64 uiCurrentVal = ui->ledec->text().toULongLong(&ok, 10);
    Val = uiCurrentVal;
    uiCurrentVal = (~uiCurrentVal);
    freamrest();
    ui->lehex->setText(QString::number(uiCurrentVal,16).toUpper());
    ui->ledec->setText(QString::number(uiCurrentVal,10).toUpper());
    setbox(uiCurrentVal);
    addhis(" 取反 ",Val,"",uiCurrentVal);
}

quint32 getBitCount(quint64 number) {
    if (number == 0) {
        return 0;  // 对于 0，最高位是 1
    }

    return static_cast<quint32>(std::numeric_limits<quint64>::digits - __builtin_clzll(number));
}

quint32 gethestDigit(quint64 first,quint64 last)
{
    quint32 tmp1 = getBitCount(first);
    quint32 tmp2 = getBitCount(last);

    return tmp1 > tmp2 ? tmp1 : tmp2;
}


void Widget::on_btdiff_clicked()
{
    bool ok;
    quint64 uiCurrentVal = ui->ledec->text().toULongLong(&ok, 10);
    quint64 Val = 0;
    getlecont(Val);
    quint32 hestDigit =  gethestDigit(uiCurrentVal,Val);
    QString styleSheet = QString("QFrame { border: 1px solid red; }");  // 设置边框为红色实线
    freamrest();
    //int highestDigit = static_cast<int>(log10(Val)) + 1;
    //qDebug()<<highestDigit<<endl;
    for (quint64 i = 0; i < hestDigit; i++) {
        if(((Val >> i) & 0x1ull ) != ((uiCurrentVal >> i) & 0x1ull ))
        {
            //qDebug()<<i<<endl;
            frames[i]->setStyleSheet(styleSheet);
        }
    }
}


void Widget::on_btsame_clicked()
{
    bool ok;
    quint64 uiCurrentVal = ui->ledec->text().toULongLong(&ok, 10);
    quint64 Val = 0;
    quint32 hestDigit =  gethestDigit(uiCurrentVal,Val);
    getlecont(Val);
    QString styleSheet = QString("QFrame { border: 1px solid green; }");  // 设置边框为红色实线
    freamrest();
    for (quint64 i = 0; i < hestDigit; i++) {
        if(((Val >> i) & 0x1 ) == ((uiCurrentVal >> i) & 0x1 ))
        {
            frames[i]->setStyleSheet(styleSheet);
        }
    }

}


void Widget::freamrest(void)
{
    QString styleSheet = QString("QFrame { border: 0px ; }");
    for(auto &i : frames)
    {
        i->setStyleSheet(styleSheet);
    }
}


void Widget::on_btlock_clicked()
{
    if(ui->btlock->isChecked())
    {
       Qt::WindowFlags m_flags = windowFlags();
       setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);
       show();
    }
    else
    {
        setWindowFlags(NULL);
        show();
    }
}
