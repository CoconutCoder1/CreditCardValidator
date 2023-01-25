#include "card.hpp"

int main()
{
	// Construct a default credit card class
	c_credit_card card;

	// Register callbacks for card classification
	c_credit_card::setup_callbacks();

	// Input buffer
	std::string input;

	// Let the user input credit card data
	printf("Enter a credit card number to validate: ");
	std::cin >> input;

	card.set_card_number(input);
	if (card.valid())
	{
		auto card_name = card.classify_card();

		printf("Valid credit card number entered.\n");
		printf("Credit Card Name: %s\n", card_name.c_str());
	}
	else
	{
		printf("Invalid credit card number entered.\n");
	}

	return 0;
}