#include "widget.h"
#include "ui_widget.h"
#include <QIntValidator>
#include <QDebug>


#define MYMAXUINT 0xFFFFFFFF


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/images/icon.png"));
    validator = new QIntValidator(1, 32, this);
    ui->lel->setValidator(validator);
    ui->ler->setValidator(validator);
    qvDecvd = new QIntValidator(0, INT_MAX, this);
    ui->ledec->setValidator(qvDecvd);
    QRegExp hexRegExp("^[0-9A-Fa-f]+$");

    QRegExpValidator *hexValidator = new QRegExpValidator(hexRegExp, this);
    ui->lehex->setValidator(hexValidator);

    QRegExp lecontExp("^0x[0-9A-Fa-f]+$|^[0-9]+$");
    QRegExpValidator *leconValidator;
    leconValidator = new QRegExpValidator(lecontExp, this);
    ui->lecont->setValidator(leconValidator);


    ui->ledec->setAttribute(Qt::WA_InputMethodEnabled, false);
    ui->lehex->setAttribute(Qt::WA_InputMethodEnabled, false);
    ui->lecont->setAttribute(Qt::WA_InputMethodEnabled, false);
    ui->tecont->setReadOnly(true);
    ui->tecont->setVisible(false);
    ui->tecont->document()->setMaximumBlockCount(5);
    QCheckBox *tmp[32] = {
        ui->cb0, ui->cb1, ui->cb2, ui->cb3, ui->cb4, ui->cb5, ui->cb6, ui->cb7,
        ui->cb8, ui->cb9, ui->cb10, ui->cb11, ui->cb12, ui->cb13, ui->cb14, ui->cb15,
        ui->cb16, ui->cb17, ui->cb18, ui->cb19, ui->cb20, ui->cb21, ui->cb22, ui->cb23,
        ui->cb24, ui->cb25, ui->cb26, ui->cb27, ui->cb28, ui->cb29, ui->cb30, ui->cb31
    };
    QFrame *frametmp[32] = {
        ui->frame_0, ui->frame_1, ui->frame_2, ui->frame_3, ui->frame_4,
        ui->frame_5, ui->frame_6, ui->frame_7, ui->frame_8, ui->frame_9,
        ui->frame_10, ui->frame_11, ui->frame_12, ui->frame_13, ui->frame_14,
        ui->frame_15, ui->frame_16, ui->frame_17, ui->frame_18, ui->frame_19,
        ui->frame_20, ui->frame_21, ui->frame_22, ui->frame_23, ui->frame_24,
        ui->frame_25, ui->frame_26, ui->frame_27, ui->frame_28, ui->frame_29,
        ui->frame_30, ui->frame_31
    };
    QString styleSheet = QString("QFrame { border: 0px ; }");
    for (int i = 0; i < 32; ++i) {
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


void Widget::setbox(quint32 value)
{
    bool isChecked = false;
    for (int i = 0; i < 32; ++i) {
        isChecked = ((value >> i) & 0x1) ? true : false;
        checkBoxes[i]->setChecked(isChecked);
    }
}


void Widget::getbox(quint32 &value)
{
    for (int i = 0; i < 32; ++i) {
        if (checkBoxes[i]->isChecked()) {
            value |= (1 << i);  // 将对应位设置为1
        }
    }
}


void Widget::changtoLE(void)
{
    quint32 value = 0;
    getbox(value);
    ui->lehex->setText(QString::number(value,16).toUpper());
    ui->ledec->setText(QString::number(value,10).toUpper());
}


void Widget::on_lehex_textChanged(const QString &arg1)
{
    QString text = arg1.toUpper();  // 转换为大写
    bool ok;
    quint32  value = 0;
    value = text.toUInt(&ok, 16);  // 尝试将输入文本解析为十六进制整数

    if (!ok || value > static_cast<quint32>(UINT_MAX)) {
        ui->lehex->setText("FFFFFFFF");
    }

    ui->lehex->setText(QString::number(value,16).toUpper());
    ui->ledec->setText(QString::number(value,10).toUpper());
    setbox(value);
}


void Widget::on_ledec_textChanged(const QString &arg1)
{
    QString text = arg1;
    quint32  value = 0;
    bool ok;

    value = text.toUInt(&ok, 10);
    if (!ok || value > static_cast<quint32>(UINT_MAX)) {
        ui->lehex->setText("4294967295");
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


void Widget::addhis(QString cmd,quint32 src,QString cent,quint32 dst)
{
    ui->tecont->setVisible(true);
    QString srcdec ="("+ QString::number(src,10).toUpper()+")";
    QString dstdec ="("+  QString::number(dst,10).toUpper()+")";
    ui->tecont->append("0x:"+QString::number(src,16).toUpper()+srcdec +" "+cmd+" "+cent +" = "+"0x:"+QString::number(dst,16).toUpper()+dstdec);
}


void Widget::on_btlf_clicked()
{
    bool ok;
    quint32 uiShiftLeft = ui->lel->text().toUInt(&ok, 10);
    quint32 uiCurrentVal = ui->ledec->text().toUInt(&ok, 10);
    quint32 tmpval = 0;
    tmpval = (uiCurrentVal << uiShiftLeft) & MYMAXUINT;
    qDebug()<<tmpval<<endl;
    freamrest();
    ui->lehex->setText(QString::number(tmpval,16).toUpper());
    ui->ledec->setText(QString::number(tmpval,10).toUpper());
    setbox(tmpval);
    addhis(" << ",uiCurrentVal,ui->lel->text(),tmpval);
}


void Widget::on_btrf_clicked()
{
    bool ok;
    quint32 tmpval = 0;
    quint32 uiShiftRight = ui->ler->text().toUInt(&ok, 10);
    quint32 uiCurrentVal = ui->ledec->text().toUInt(&ok, 10);

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
    quint32 Val = 0;
    getlecont(Val);
    quint32 uiCurrentVal = ui->ledec->text().toUInt(&ok, 10);
    quint32 uiAnd = Val & uiCurrentVal;
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
    unsigned int Value = 0;
    QString styleSheet = QString("QLabel#lbtips { font-size: %15px; }").arg(15);
    ui->lbtips->setStyleSheet(styleSheet);
    if(! text.contains("0x", Qt::CaseInsensitive))
    {
        ui->lbtips->setText("Dec:");
        Value = text.toUInt(&ok, 10);
        if(ok)
            ui->lecont->setText(QString::number(Value,10).toUpper());
        else
            ui->lecont->setText(QString::number(MYMAXUINT,10).toUpper());

    }
    if((text.size() >= 3) && (text.contains("0x", Qt::CaseInsensitive)))
    {
        ui->lbtips->setText("Hex:");
        Value = text.toUInt(&ok, 16);
        if(ok)
            ui->lecont->setText("0x"+QString::number(Value,16).toUpper());
        else
            ui->lecont->setText("0x"+QString::number(MYMAXUINT,16).toUpper());
    }

}


void Widget::getlecont(quint32 &value)
{
    QString text =  ui->lecont->text();
    bool ok;
    //QRegularExpression regex("[a-fA-F]");
    //QRegularExpressionMatch match = regex.match(text);
    if(! text.contains("0x", Qt::CaseInsensitive))
    {
        value = text.toUInt(&ok, 10);
    }
    if((text.size() >= 3) && (text.contains("0x", Qt::CaseInsensitive)))
    {
        value = text.toUInt(&ok, 16);
    }
}


void Widget::on_btor_clicked()
{
    bool ok;
    quint32 Val = 0;
    getlecont(Val);
    quint32 uiCurrentVal = ui->ledec->text().toUInt(&ok, 10);
    quint32 uiOr = Val | uiCurrentVal;
    freamrest();
    ui->lehex->setText(QString::number(uiOr,16).toUpper());
    ui->ledec->setText(QString::number(uiOr,10).toUpper());
    setbox(uiOr);
    addhis(" | ",uiCurrentVal,ui->lecont->text(),uiOr);
}


void Widget::on_btao_clicked()
{
    bool ok;
    quint32 Val = 0;
    getlecont(Val);
    quint32 uiCurrentVal = ui->ledec->text().toUInt(&ok, 10);
    quint32 uiXOR = Val ^ uiCurrentVal;
    freamrest();
    ui->lehex->setText(QString::number(uiXOR,16).toUpper());
    ui->ledec->setText(QString::number(uiXOR,10).toUpper());
    setbox(uiXOR);
    addhis(" ^ ",uiCurrentVal,ui->lecont->text(),uiXOR);
}


void Widget::on_btcont_clicked()
{
    bool ok;
    quint32 Val = 0;
    //getlecont(Val);
    quint32 uiCurrentVal = ui->ledec->text().toUInt(&ok, 10);
    Val = uiCurrentVal;
    uiCurrentVal = (~uiCurrentVal);
    freamrest();
    ui->lehex->setText(QString::number(uiCurrentVal,16).toUpper());
    ui->ledec->setText(QString::number(uiCurrentVal,10).toUpper());
    setbox(uiCurrentVal);
    addhis(" 取反 ",Val,"",uiCurrentVal);
}


void Widget::on_btdiff_clicked()
{
    bool ok;
    quint32 uiCurrentVal = ui->ledec->text().toUInt(&ok, 10);
    quint32 Val = 0;
    getlecont(Val);

    QString styleSheet = QString("QFrame { border: 1px solid red; }");  // 设置边框为红色实线
    freamrest();
    for (int i = 0; i < 32; i++) {
        if(((Val >> i) & 0x1 ) != ((uiCurrentVal >> i) & 0x1 ))
        {
            frames[i]->setStyleSheet(styleSheet);
        }
    }
}


void Widget::on_btsame_clicked()
{
    bool ok;
    quint32 uiCurrentVal = ui->ledec->text().toUInt(&ok, 10);
    quint32 Val = 0;
    getlecont(Val);

    QString styleSheet = QString("QFrame { border: 1px solid green; }");  // 设置边框为红色实线
    freamrest();
    for (int i = 0; i < 32; i++) {
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
