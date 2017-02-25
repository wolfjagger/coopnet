#include "graph.h"

using namespace coopnet;



inline std::ostream& coopnet::operator<<(std::ostream& os, const BaseSatVProp::Base& base) {

	switch (base.kind) {
	case BaseSatVProp::Node:
		os << "kind: Node";
		break;
	case BaseSatVProp::Clause:
		os << "kind: Clause";
		break;
	}
	os << "name: " << base.name;
	return os << std::endl;

}
inline std::istream& coopnet::operator>>(std::istream& is, BaseSatVProp::Base& base) {

	throw std::exception("Input stream to BaseSatVProp not set up.");

}

inline std::ostream& coopnet::operator<<(std::ostream& os, const BaseSatEProp::Base& base) {
	
	os << "sgn: " << (base.sgn ? "T" : "F");
	return os << std::endl;

}
inline std::istream& coopnet::operator >> (std::istream& is, BaseSatEProp::Base& base) {

	throw std::exception("Input stream to BaseSatVProp not set up.");

}
