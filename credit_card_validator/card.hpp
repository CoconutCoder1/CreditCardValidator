#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <functional>

using card_digits_t = std::vector<int>;

// Visa has the least digits out of the supported cards
//   so it's used as the minimum length
#define MINIMUM_CARD_LEN 13
#define CARD_UNKNOWN "Unknown Card"

class c_credit_card
{
public:
	c_credit_card();
	c_credit_card(const std::string& card_number);

	static void setup_callbacks();

	void set_card_number(const std::string& card_number);
	bool validate_nr();
	const std::string& classify_card();

	inline const bool& valid() { return m_valid; }

private:
	card_digits_t m_card_digits;
	bool m_valid;
};