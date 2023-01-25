#include "card.hpp"

using classify_function_t = std::function<bool(card_digits_t&)>;
std::map<std::string, classify_function_t> classify_callbacks;

void set_callback(const std::string& name, classify_function_t callback)
{
	classify_callbacks[name] = callback;
}

void c_credit_card::setup_callbacks()
{
	// Visa prefix: 4
	set_callback("Visa", [](const card_digits_t& digits) -> bool {
		return digits[0] == 4;
	});

	// Mastercard prefix: 51 through 55
	set_callback("MasterCard", [](const card_digits_t& digits) -> bool {
		int a = digits[0] * 10 + digits[1];
		return a >= 51 && a <= 55;
	});

	// American Express prefix: 34 or 37
	set_callback("American Express", [](const card_digits_t& digits) -> bool {
		int a = digits[0] * 10 + digits[1];
		return a == 34 || a == 37;
	});
}

c_credit_card::c_credit_card()
{
	m_card_digits = {};
	m_valid = false;
}

c_credit_card::c_credit_card(const std::string& card_number)
{
	set_card_number(card_number);
}

void c_credit_card::set_card_number(const std::string& card_number)
{
	m_card_digits.clear();
	m_valid = false;

	if (card_number.length() <= MINIMUM_CARD_LEN)
		return;

	for (const auto& c : card_number)
	{
		if (c == ' ') continue;
		// The string should only contain digits
		if (!isdigit(c)) return;

		// char - '0' = digit as an integer
		m_card_digits.push_back(c - '0');
	}

	if (m_card_digits.size() <= MINIMUM_CARD_LEN)
		m_valid = false;
	else
		m_valid = validate_nr();
}

// Validate card number using the luhn algorithm
bool c_credit_card::validate_nr()
{
	int counter = 0;
	std::vector<int> product;
	for (auto itr = m_card_digits.rbegin(); itr != m_card_digits.rend(); itr++)
	{
		int weight = (counter++ % 2 == 0) ? 1 : 2;
		product.emplace_back(*itr * weight);
	}

	int sum = 0;
	for (const auto& n : product)
	{
		// According to the algorithm:
		//   if the number is two-digit then add the two digits as if they were separate
		if (n >= 10)
		{
			// There's most likely a better way to do this

			// Convert 16(example) to separate digits:
			// 16 / 10 = 1.6
			// floor(a) = 1
			// (1.6 - 1.0) * 10 = 6
			float a = (float)n / 10.f;
			float b = floor(a);
			float c = (a - b) * 10.f;

			// Round c because of floating point precision
			sum += (int)b + (int)round(c);
		}
		else
		{
			sum += n;
		}
	}

	return (sum % 10) == 0;
}

// Determine what type of credit card it is
//    returns the name of the recognized card
const std::string& c_credit_card::classify_card()
{
	// Make sure the card length is enough
	if (m_card_digits.size() <= MINIMUM_CARD_LEN)
		return CARD_UNKNOWN;

	// Run the classify callbacks to check if the card number was recognized
	//	 by checking the card number prefix
	for (auto& cb : classify_callbacks)
	{
		if (cb.second(m_card_digits))
			return cb.first;
	}

	return CARD_UNKNOWN;
}