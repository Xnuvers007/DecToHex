#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QDebug>
/*#include <QProgressBar>
#include <QTimer>*/

// fungsi konversi bilangan bulat ke hex
QString intToHex(int num)
{
    static const char* const lut = "0123456789ABCDEF";
    QString hex;
    hex.reserve(8);
    for (int i = 28; i >= 0; i -= 4) {
        int nibble = (num >> i) & 0xF;
        hex += lut[nibble];
    }
    hex.remove(0, hex.indexOf(QRegExp("[^0]"))); // menghapus leading 0
    if (hex.isEmpty()) hex = "0";
    return hex;
}

// fungsi konversi bilangan hex ke desimal
int hexToInt(QString hex)
{
    bool ok;
    int num = hex.toInt(&ok, 16);
    if (!ok) {
        qDebug() << "Input tidak valid!";
        return 0;
    }
    return num;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QWidget window;
    window.setWindowTitle("Kalkulator Heksadesimal");
    window.setStyleSheet("background-color: black; color: white;");
    window.setMinimumSize(300, 200);
    
        // Tambahkan progress bar
/*    QProgressBar *loadingBar = new QProgressBar(&window);
    loadingBar->setMinimum(0);
    loadingBar->setMaximum(0);
    loadingBar->setValue(0);

    // Animasi loading
    QTimer timer;
    timer.setInterval(50);
    QObject::connect(&timer, &QTimer::timeout, [&loadingBar]() {
        int value = loadingBar->value() + 1;
        if (value > loadingBar->maximum()) {
            value = loadingBar->minimum();
        }
        loadingBar->setValue(value);
    });
    timer.start(); */

    QLabel *lblDec = new QLabel("Bilangan Desimal:");
    QLineEdit *edtDec = new QLineEdit();
    QLabel *lblHex = new QLabel("Bilangan Heksadesimal:");
    QLineEdit *edtHex = new QLineEdit();
    QLabel *lblHex1 = new QLabel("Bilangan Heksadesimal 1:");
    QLineEdit *edtHex1 = new QLineEdit();
    QLabel *lblHex2 = new QLabel("Bilangan Heksadesimal 2:");
    QLineEdit *edtHex2 = new QLineEdit();
    QPushButton *btnConv = new QPushButton("Konversi");
    QPushButton *btnAdd = new QPushButton("Tambah");
    QLabel *lblResult = new QLabel("Hasil:");
    QLineEdit *edtResult = new QLineEdit();
    edtResult->setReadOnly(true);

    QGridLayout *layout = new QGridLayout();
    layout->addWidget(lblDec, 0, 0);
    layout->addWidget(edtDec, 0, 1);
    layout->addWidget(lblHex, 1, 0);
    layout->addWidget(edtHex, 1, 1);
    layout->addWidget(btnConv, 2, 0, 1, 2);
    layout->addWidget(lblHex1, 3, 0);
    layout->addWidget(edtHex1, 3, 1);
    layout->addWidget(lblHex2, 4, 0);
    layout->addWidget(edtHex2, 4, 1);
    layout->addWidget(btnAdd, 5, 0, 1, 2);
    layout->addWidget(lblResult, 6, 0);
    layout->addWidget(edtResult, 6, 1);
    
    window.setLayout(layout);
    QObject::connect(btnConv, &QPushButton::clicked, [&]() {
    bool ok;
    int dec = edtDec->text().toInt(&ok);
    if (ok) {
        QString hex = intToHex(dec);
        edtHex->setText(hex);
    } else {
        QString hex = edtHex->text();
        int dec = hexToInt(hex);
        edtDec->setText(QString::number(dec));
    }
});

QObject::connect(btnAdd, &QPushButton::clicked, [&]() {
    QString hex1 = edtHex1->text();
    QString hex2 = edtHex2->text();
    int num1 = hexToInt(hex1);
    int num2 = hexToInt(hex2);
    int result = num1 + num2;
    QString hexResult = intToHex(result);
    edtResult->setText(hexResult);
});

// Tampilkan loading bar saat aplikasi dibuka
/*loadingBar->show();
timer.start();
*/
// window.setLayout(layout);
window.show();

// Hentikan animasi loading setelah 2 detik
/*    QTimer::singleShot(2000, [&timer]() {
        timer.stop();
    });
*/
return app.exec();
}
