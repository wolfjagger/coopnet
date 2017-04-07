#include "prop.h"

using namespace coopnet;



std::ostream& coopnet::operator<<(std::ostream& os, const BaseSatProp::Node& base) {
	return os;
}
std::istream& coopnet::operator >> (std::istream& is, BaseSatProp::Node& base) {
	return is;
}

std::ostream& coopnet::operator<<(std::ostream& os, const BaseSatProp::Clause& base) {
	return os;
}
std::istream& coopnet::operator >> (std::istream& is, BaseSatProp::Clause& base) {
	return is;
}

std::ostream& coopnet::operator<<(std::ostream& os, const BaseSatProp::Edge& base) {
	os << "sgn: " << (base.sgn ? "T" : "F");
	return os << std::endl;
}
std::istream& coopnet::operator >> (std::istream& is, BaseSatProp::Edge& base) {
	throw std::exception("Input stream to BaseSatEProp not set up.");
}
