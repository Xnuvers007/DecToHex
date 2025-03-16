#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QComboBox>
#include <QSpinBox>
#include <QDebug>
#include <QMessageBox>
#include <QSplashScreen>
#include <QPixmap>
#include <QTimer>
#include <QFont>
#include <QIcon>
#include <QToolTip>
#include <QValidator>
#include <QRegularExpression>
#include <QStatusBar>
#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QFile>
#include <QPainter>

// Function to convert integer to hex with specified width
QString intToHex(int num, int width = 0)
{
    static const char* const lut = "0123456789ABCDEF";
    QString hex;
    
    // Use width if specified, otherwise use minimal representation
    if (width > 0) {
        hex.reserve(width);
        for (int i = (width * 4) - 4; i >= 0; i -= 4) {
            int nibble = (num >> i) & 0xF;
            hex += lut[nibble];
        }
    } else {
        hex.reserve(8);
        for (int i = 28; i >= 0; i -= 4) {
            int nibble = (num >> i) & 0xF;
            hex += lut[nibble];
        }
        // Remove leading zeros
        hex.remove(0, hex.indexOf(QRegularExpression("[^0]")));
    }
    
    if (hex.isEmpty()) hex = "0";
    return hex;
}

// Function to convert hex to integer
int hexToInt(QString hex)
{
    bool ok;
    int num = hex.toInt(&ok, 16);
    if (!ok) {
        qDebug() << "Invalid input!";
        return 0;
    }
    return num;
}

// Function to convert between number bases
QString convertBase(QString input, int fromBase, int toBase)
{
    bool ok;
    int decimal = input.toInt(&ok, fromBase);
    if (!ok) return "Error";
    
    if (toBase == 16) return intToHex(decimal);
    if (toBase == 2) {
        QString binary;
        for (int i = 31; i >= 0; i--) {
            int bit = (decimal >> i) & 1;
            binary += QString::number(bit);
        }
        // Remove leading zeros
        binary.remove(0, binary.indexOf('1'));
        if (binary.isEmpty()) binary = "0";
        return binary;
    }
    return QString::number(decimal, toBase);
}

// Main Calculator Class
class HexCalculator : public QMainWindow
{
public:
    HexCalculator(QWidget *parent = nullptr) : QMainWindow(parent)
    {
        setupUI();
        connectSignals();
        applyStyleSheet();
    }

private:
    // UI Elements
    QWidget *centralWidget;
    QStatusBar *statusBar;
    QLineEdit *edtDec, *edtHex, *edtBin, *edtOct;
    QLineEdit *edtOperand1, *edtOperand2, *edtResult;
    QComboBox *cmbOperation;
    QComboBox *cmbDisplayFormat;
    QSpinBox *spnWidth;
    QRadioButton *radDec, *radHex, *radBin, *radOct;
    QPushButton *btnConvert, *btnCalculate, *btnClear;
    
    void setupUI()
    {
        setWindowTitle("Advanced Hexadecimal Calculator");
        setMinimumSize(500, 500);
        
        // Create central widget
        centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);
        
        // Create menu bar
        QMenuBar *menuBar = new QMenuBar(this);
        setMenuBar(menuBar);
        
        QMenu *fileMenu = menuBar->addMenu("File");
        QAction *exitAction = fileMenu->addAction("Exit");
        connect(exitAction, &QAction::triggered, this, &HexCalculator::close);
        
        QMenu *helpMenu = menuBar->addMenu("Help");
        QAction *aboutAction = helpMenu->addAction("About");
        connect(aboutAction, &QAction::triggered, this, &HexCalculator::showAbout);
        
        // Main layout
        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
        
        // Conversion group
        QGroupBox *conversionGroup = new QGroupBox("Number Conversion");
        QGridLayout *conversionLayout = new QGridLayout();
        
        // Create a horizontal layout for the decimal input
        QHBoxLayout *decLayout = new QHBoxLayout();
        QLabel *lblDec = new QLabel("Decimal:");
        edtDec = new QLineEdit();
        edtDec->setPlaceholderText("Enter decimal value");
        radDec = new QRadioButton();
        radDec->setChecked(true);
        decLayout->addWidget(lblDec);
        decLayout->addWidget(edtDec);
        decLayout->addWidget(radDec);
        
        // Create a layout for the hex input
        QHBoxLayout *hexLayout = new QHBoxLayout();
        QLabel *lblHex = new QLabel("Hexadecimal:");
        edtHex = new QLineEdit();
        edtHex->setPlaceholderText("Enter hex value");
        radHex = new QRadioButton();
        hexLayout->addWidget(lblHex);
        hexLayout->addWidget(edtHex);
        hexLayout->addWidget(radHex);
        
        // Add binary input
        QHBoxLayout *binLayout = new QHBoxLayout();
        QLabel *lblBin = new QLabel("Binary:");
        edtBin = new QLineEdit();
        edtBin->setPlaceholderText("Enter binary value");
        radBin = new QRadioButton();
        binLayout->addWidget(lblBin);
        binLayout->addWidget(edtBin);
        binLayout->addWidget(radBin);
        
        // Add octal input
        QHBoxLayout *octLayout = new QHBoxLayout();
        QLabel *lblOct = new QLabel("Octal:");
        edtOct = new QLineEdit();
        edtOct->setPlaceholderText("Enter octal value");
        radOct = new QRadioButton();
        octLayout->addWidget(lblOct);
        octLayout->addWidget(edtOct);
        octLayout->addWidget(radOct);
        
        conversionLayout->addLayout(decLayout, 0, 0);
        conversionLayout->addLayout(hexLayout, 1, 0);
        conversionLayout->addLayout(binLayout, 2, 0);
        conversionLayout->addLayout(octLayout, 3, 0);
        
        QHBoxLayout *convBtnLayout = new QHBoxLayout();
        btnConvert = new QPushButton("Convert");
        btnClear = new QPushButton("Clear");
        btnConvert->setIcon(QIcon::fromTheme("edit-convert"));
        btnClear->setIcon(QIcon::fromTheme("edit-clear"));
        
        QLabel *lblFormat = new QLabel("Display Format:");
        cmbDisplayFormat = new QComboBox();
        cmbDisplayFormat->addItem("Standard");
        cmbDisplayFormat->addItem("0x Prefix");
        cmbDisplayFormat->addItem("Fixed Width");
        
        QLabel *lblWidth = new QLabel("Width:");
        spnWidth = new QSpinBox();
        spnWidth->setRange(1, 8);
        spnWidth->setValue(4);
        spnWidth->setEnabled(false);
        
        convBtnLayout->addWidget(btnConvert);
        convBtnLayout->addWidget(btnClear);
        convBtnLayout->addWidget(lblFormat);
        convBtnLayout->addWidget(cmbDisplayFormat);
        convBtnLayout->addWidget(lblWidth);
        convBtnLayout->addWidget(spnWidth);
        
        conversionLayout->addLayout(convBtnLayout, 4, 0);
        conversionGroup->setLayout(conversionLayout);
        
        // Calculator group
        QGroupBox *calculatorGroup = new QGroupBox("Hexadecimal Calculator");
        QGridLayout *calculatorLayout = new QGridLayout();
        
        QLabel *lblOperand1 = new QLabel("First Operand (Hex):");
        edtOperand1 = new QLineEdit();
        edtOperand1->setPlaceholderText("Enter hex value");
        
        QLabel *lblOperand2 = new QLabel("Second Operand (Hex):");
        edtOperand2 = new QLineEdit();
        edtOperand2->setPlaceholderText("Enter hex value");
        
        QLabel *lblOperation = new QLabel("Operation:");
        cmbOperation = new QComboBox();
        cmbOperation->addItem("Addition (+)");
        cmbOperation->addItem("Subtraction (-)");
        cmbOperation->addItem("Multiplication (*)");
        cmbOperation->addItem("Division (/)");
        cmbOperation->addItem("Bitwise AND (&)");
        cmbOperation->addItem("Bitwise OR (|)");
        cmbOperation->addItem("Bitwise XOR (^)");
        cmbOperation->addItem("Left Shift (<<)");
        cmbOperation->addItem("Right Shift (>>)");
        
        btnCalculate = new QPushButton("Calculate");
        btnCalculate->setIcon(QIcon::fromTheme("accessories-calculator"));
        
        QLabel *lblResult = new QLabel("Result (Hex):");
        edtResult = new QLineEdit();
        edtResult->setReadOnly(true);
        
        calculatorLayout->addWidget(lblOperand1, 0, 0);
        calculatorLayout->addWidget(edtOperand1, 0, 1);
        calculatorLayout->addWidget(lblOperand2, 1, 0);
        calculatorLayout->addWidget(edtOperand2, 1, 1);
        calculatorLayout->addWidget(lblOperation, 2, 0);
        calculatorLayout->addWidget(cmbOperation, 2, 1);
        calculatorLayout->addWidget(btnCalculate, 3, 0, 1, 2);
        calculatorLayout->addWidget(lblResult, 4, 0);
        calculatorLayout->addWidget(edtResult, 4, 1);
        
        calculatorGroup->setLayout(calculatorLayout);
        
        // Add to main layout
        mainLayout->addWidget(conversionGroup);
        mainLayout->addWidget(calculatorGroup);
        
        // Set up validators
        QRegularExpression hexRegex("[0-9A-Fa-f]+");
        QValidator *hexValidator = new QRegularExpressionValidator(hexRegex, this);
        edtHex->setValidator(hexValidator);
        edtOperand1->setValidator(hexValidator);
        edtOperand2->setValidator(hexValidator);
        
        QRegularExpression decRegex("[0-9]+");
        QValidator *decValidator = new QRegularExpressionValidator(decRegex, this);
        edtDec->setValidator(decValidator);
        
        QRegularExpression binRegex("[01]+");
        QValidator *binValidator = new QRegularExpressionValidator(binRegex, this);
        edtBin->setValidator(binValidator);
        
        QRegularExpression octRegex("[0-7]+");
        QValidator *octValidator = new QRegularExpressionValidator(octRegex, this);
        edtOct->setValidator(octValidator);
        
        // Create status bar
        statusBar = new QStatusBar(this);
        setStatusBar(statusBar);
        statusBar->showMessage("Ready");
    }
    
    void connectSignals()
    {
        // Connect conversion button
        connect(btnConvert, &QPushButton::clicked, this, &HexCalculator::convert);
        
        // Connect calculate button
        connect(btnCalculate, &QPushButton::clicked, this, &HexCalculator::calculate);
        
        // Connect clear button
        connect(btnClear, &QPushButton::clicked, this, &HexCalculator::clearConversion);
        
        // Connect display format combobox
        connect(cmbDisplayFormat, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &HexCalculator::updateDisplayFormat);
    }
    
    void convert()
    {
        int fromBase = 10; // Default base is decimal
        QString input;
        
        // Determine which input field to use based on radio button selection
        if (radDec->isChecked()) {
            fromBase = 10;
            input = edtDec->text();
        } else if (radHex->isChecked()) {
            fromBase = 16;
            input = edtHex->text();
        } else if (radBin->isChecked()) {
            fromBase = 2;
            input = edtBin->text();
        } else if (radOct->isChecked()) {
            fromBase = 8;
            input = edtOct->text();
        }
        
        if (input.isEmpty()) {
            statusBar->showMessage("Please enter a value", 3000);
            return;
        }
        
        // Convert to all bases
        bool ok;
        int decimal = input.toInt(&ok, fromBase);
        if (!ok) {
            statusBar->showMessage("Invalid input for selected base", 3000);
            return;
        }
        
        // Format according to selection
        QString hexValue;
        if (cmbDisplayFormat->currentIndex() == 0) {
            // Standard
            hexValue = intToHex(decimal);
        } else if (cmbDisplayFormat->currentIndex() == 1) {
            // 0x Prefix
            hexValue = "0x" + intToHex(decimal);
        } else {
            // Fixed width
            hexValue = intToHex(decimal, spnWidth->value());
        }
        
        edtDec->setText(QString::number(decimal));
        edtHex->setText(hexValue);
        edtBin->setText(convertBase(QString::number(decimal), 10, 2));
        edtOct->setText(convertBase(QString::number(decimal), 10, 8));
        
        statusBar->showMessage("Conversion completed", 3000);
    }
    
    void calculate()
    {
        QString hex1 = edtOperand1->text();
        QString hex2 = edtOperand2->text();
        
        if (hex1.isEmpty() || hex2.isEmpty()) {
            statusBar->showMessage("Please enter both operands", 3000);
            return;
        }
        
        int num1 = hexToInt(hex1);
        int num2 = hexToInt(hex2);
        int result = 0;
        
        switch (cmbOperation->currentIndex()) {
            case 0: // Addition
                result = num1 + num2;
                break;
            case 1: // Subtraction
                result = num1 - num2;
                break;
            case 2: // Multiplication
                result = num1 * num2;
                break;
            case 3: // Division
                if (num2 == 0) {
                    statusBar->showMessage("Error: Division by zero", 3000);
                    edtResult->setText("Error");
                    return;
                }
                result = num1 / num2;
                break;
            case 4: // Bitwise AND
                result = num1 & num2;
                break;
            case 5: // Bitwise OR
                result = num1 | num2;
                break;
            case 6: // Bitwise XOR
                result = num1 ^ num2;
                break;
            case 7: // Left Shift
                result = num1 << num2;
                break;
            case 8: // Right Shift
                result = num1 >> num2;
                break;
        }
        
        // Format result according to display format
        QString hexResult;
        if (cmbDisplayFormat->currentIndex() == 0) {
            hexResult = intToHex(result);
        } else if (cmbDisplayFormat->currentIndex() == 1) {
            hexResult = "0x" + intToHex(result);
        } else {
            hexResult = intToHex(result, spnWidth->value());
        }
        
        edtResult->setText(hexResult);
        statusBar->showMessage("Calculation completed: " + 
                             QString::number(num1) + 
                             " " + cmbOperation->currentText().split(" ").first() + 
                             " " + QString::number(num2) + 
                             " = " + QString::number(result), 5000);
    }
    
    void clearConversion()
    {
        edtDec->clear();
        edtHex->clear();
        edtBin->clear();
        edtOct->clear();
        statusBar->showMessage("Fields cleared", 2000);
    }
    
    void updateDisplayFormat(int index)
    {
        // Enable/disable width spinner based on selected format
        spnWidth->setEnabled(index == 2);
        
        // Re-run conversion to update display format
        if (!edtDec->text().isEmpty() || !edtHex->text().isEmpty() || 
            !edtBin->text().isEmpty() || !edtOct->text().isEmpty()) {
            convert();
        }
    }
    
    void showAbout()
    {
        QMessageBox::about(this, "About Hexadecimal Calculator",
                         "Advanced Hexadecimal Calculator v1.0\n\n"
                         "A professional tool for converting between number bases\n"
                         "and performing calculations with hexadecimal numbers.\n\n"
                         "Based on original code by Xnuvers007\n"
                         "https://github.com/Xnuvers007/Dectohex");
    }
    
    void applyStyleSheet()
    {
        QFile styleFile(":/styles/dark.qss");
        if (styleFile.exists()) {
            styleFile.open(QFile::ReadOnly);
            QString style = QLatin1String(styleFile.readAll());
            setStyleSheet(style);
        } else {
            // Fallback style
            setStyleSheet(
                "QMainWindow, QWidget { background-color: #2D2D30; color: #E0E0E0; }"
                "QGroupBox { background-color: #3E3E42; border: 1px solid #555555; border-radius: 5px; margin-top: 1ex; }"
                "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 3px; }"
                "QLineEdit { background-color: #1E1E1E; border: 1px solid #555555; border-radius: 3px; padding: 2px; }"
                "QComboBox, QSpinBox { background-color: #3E3E42; border: 1px solid #555555; border-radius: 3px; padding: 2px; }"
                "QPushButton { background-color: #0E639C; color: white; border-radius: 3px; padding: 5px; }"
                "QPushButton:hover { background-color: #1177BB; }"
                "QPushButton:pressed { background-color: #0D5789; }"
                "QRadioButton::indicator { width: 13px; height: 13px; }"
                "QRadioButton::indicator::unchecked { background-color: #1E1E1E; border: 1px solid #555555; border-radius: 7px; }"
                "QRadioButton::indicator::checked { background-color: #3399FF; border: 1px solid #555555; border-radius: 7px; }"
                "QStatusBar { background-color: #007ACC; color: white; }"
                "QMenuBar { background-color: #3E3E42; }"
                "QMenuBar::item:selected { background-color: #007ACC; }"
                "QMenu { background-color: #3E3E42; border: 1px solid #555555; }"
                "QMenu::item:selected { background-color: #007ACC; }"
            );
        }
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Create splash screen
    QPixmap splashPixmap(":/images/splash.png");
    if (splashPixmap.isNull()) {
        // If custom splash image is not found, create a simple one
        splashPixmap = QPixmap(400, 300);
        splashPixmap.fill(QColor(45, 45, 48));
        QPainter painter(&splashPixmap);
        painter.setPen(QColor(255, 255, 255));
        painter.setFont(QFont("Arial", 20, QFont::Bold));
        painter.drawText(QRect(0, 100, 400, 50), Qt::AlignCenter, "Advanced Hex Calculator");
        painter.setFont(QFont("Arial", 12));
        painter.drawText(QRect(0, 150, 400, 50), Qt::AlignCenter, "Loading...");
        painter.end();
    }
    
    QSplashScreen splash(splashPixmap);
    splash.show();
    app.processEvents();
    
    // Simulate some loading time
    QTimer::singleShot(1500, &splash, &QSplashScreen::close);
    
    HexCalculator calculator;
    QTimer::singleShot(1500, &calculator, &HexCalculator::show);
    
    return app.exec();
}
