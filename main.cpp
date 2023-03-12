#include <iostream>
#include <string>

// fungsi konversi bilangan bulat ke hex
std::string intToHex(int num)
{
    static const char* const lut = "0123456789ABCDEF";
    std::string hex;
    hex.reserve(8);
    for (int i = 28; i >= 0; i -= 4) {
        int nibble = (num >> i) & 0xF;
        hex += lut[nibble];
    }
    hex.erase(0, hex.find_first_not_of('0'));
    if (hex.empty()) hex = "0";
    return hex;
}

// fungsi konversi bilangan hex ke desimal
int hexToInt(std::string hex)
{
    int num = 0;
    for (int i = 0; i < hex.length(); i++) {
        char c = hex[i];
        int digit = 0;
        if (c >= '0' && c <= '9') {
            digit = c - '0';
        } else if (c >= 'A' && c <= 'F') {
            digit = c - 'A' + 10;
        } else if (c >= 'a' && c <= 'f') {
            digit = c - 'a' + 10;
        } else {
            std::cerr << "Input tidak valid!\n";
            return 0;
        }
        num = (num << 4) | digit;
    }
    return num;
}

int main()
{
    int choice;
    char answer = 'n';

    do {
        std::cout << "Pilih operasi:\n";
        std::cout << "1. Konversi desimal ke heksadesimal\n";
        std::cout << "2. Operasi penjumlahan heksadesimal\n";
        std::cout << "Pilihan Anda: ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                int decValue;
                std::cout << "Masukkan nilai desimal: ";
                if (!(std::cin >> decValue)) {
                    std::cerr << "Input tidak valid!\n";
                    return 1;
                }
                std::string hexValue = intToHex(decValue);
                std::cout << "Decimal " << decValue << " = Hex " << hexValue << std::endl;
                break;
            }
            case 2: {
                std::string hexValue1, hexValue2;
                std::cout << "Masukkan dua bilangan heksadesimal: ";
                if (!(std::cin >> hexValue1 >> hexValue2)) {
                    std::cerr << "Input tidak valid!\n";
                    return 1;
                }
                int intValue1 = hexToInt(hexValue1);
                int intValue2 = hexToInt(hexValue2);
                int sumValue = intValue1 + intValue2;
                std::string sumHexValue = intToHex(sumValue);
                std::cout << "Hex " << hexValue1 << " + " << hexValue2 << " = " << sumHexValue << std::endl;
                break;
            }
            default: {
                std::cerr << "Pilihan tidak valid!\n";
                return 1;
            }
        }

        std::cout << "Ingin mencoba lagi? (y/n) ";
        std::cin >> answer;
    } while (answer=='y' || answer == 'Y');
    return 0;
}
