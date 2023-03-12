#include <iostream>
#include <string>

// fungsi konversi bilangan bulat ke hex
std::string intToHex(int num)
{
	static const char *const lut = "0123456789ABCDEF";
	std::string hex;
	hex.reserve(8);
	for (int i = 28; i >= 0; i -= 4)
	{
		int nibble = (num >> i) & 0xF;
		hex += lut[nibble];
	}
	hex.erase(0, hex.find_first_not_of('0'));
	if (hex.empty())
		hex = "0";
	return hex;
}

int main()
{
	int decValue = 0;
	char answer = 'n';

	do
	{
		std::cout << "Masukkan nilai desimal (byte): ";
		if (!(std::cin >> decValue))
		{
			std::cerr << "Input tidak valid!\n";
			return 1;
		}

		std::string hexValue = intToHex(decValue);
		std::cout << "Decimal " << decValue << " = Hex " << hexValue << std::endl;

		std::cout << "\nIngin mencoba lagi? (y/n) ";
		std::cin >> answer;
	}
	while (answer == 'y' || answer == 'Y');

	return 0;
}
