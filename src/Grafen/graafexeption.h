#pragma once
#include <stdexcept>
#include <exception>
#include <string>
#include <iostream>

namespace grafen {
	class GraafExceptie : public std::logic_error {
	public:
		GraafExceptie(const std::string &boodschap_) : std::logic_error(boodschap_) {}
	};

	std::ostream &operator<<(std::ostream &os, const GraafExceptie& exc) {
		return os << exc.what();
	}
}