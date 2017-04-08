#pragma once

#include "boost/variant/variant.hpp"
#include "boost/variant/get.hpp"
#include "bgl_inc.h"



namespace coopnet {

	using boost::default_color_type;

	enum class VertKind { Node = 0, Clause = 1 };

	inline std::ostream& operator<<(std::ostream& os, VertKind kind) {
		switch (kind) {
		case VertKind::Node:
			os << "Node";
		case VertKind::Clause:
			os << "Clause";
		}
		return os;
	}



	// This collects node property and clause property
	//  into a single structure with a variant.
	// This class will handle access to node vs clause.
	template<typename SatProp>
	struct VProp {

		using NProp = typename SatProp::Node;
		using CProp = typename SatProp::Clause;

		std::string name;
		mutable default_color_type color;
		int kindInt;

	private:
		boost::variant<NProp, CProp> payload;

	public:

		VProp() :
			name(),
			color(default_color_type::black_color) {}
		VProp(NProp nProp) :
			name(),
			color(default_color_type::black_color),
			kindInt(int(VertKind::Node)),
			payload(nProp) {}
		VProp(CProp cProp) :
			name(),
			color(default_color_type::black_color),
			kindInt(int(VertKind::Clause)),
			payload(cProp) {}

		const NProp& node() const { return boost::get<NProp>(payload); }
		NProp& node() { return boost::get<NProp>(payload); }
		const CProp& clause() const { return boost::get<CProp>(payload); }
		CProp& clause() { return boost::get<CProp>(payload); }
		VertKind kind() const { return VertKind(kindInt); }

	};

	template<typename SatProp>
	std::ostream& operator<<(std::ostream& os, const VProp<SatProp>& vProp) {
		
		os << "kind: " << vProp.kind() << " || name: " << vProp.name;
		switch (vProp.kind()) {
		case VertKind::Node:
			os << " " << vProp.node();
			break;
		case VertKind::Clause:
			os << " " << vProp.clause();
			break;
		}
		return os << std::endl;

	}

	template<typename SatProp>
	std::istream& operator >> (std::istream& is, VProp<SatProp>& vProp) {
		throw("Input stream for VProp not set up");
	}


	template<typename SatProp>
	struct EProp : SatProp::Edge {

	};


	struct BaseSatProp {
		struct Node { };
		struct Clause { };
		struct Edge {
			bool sgn;
			Edge() :
				sgn(true) { }
		};
	};

	std::ostream& operator<<(std::ostream& os, const BaseSatProp::Node& base);
	std::istream& operator >> (std::istream& is, BaseSatProp::Node& base);

	std::ostream& operator<<(std::ostream& os, const BaseSatProp::Clause& base);
	std::istream& operator >> (std::istream& is, BaseSatProp::Clause& base);

	std::ostream& operator<<(std::ostream& os, const BaseSatProp::Edge& base);
	std::istream& operator >> (std::istream& is, BaseSatProp::Edge& base);



	template<typename SatProp>
	VProp<SatProp> copy_base(const VProp<BaseSatProp>& other) {
		auto prop = VProp<SatProp>();
		switch(other.kind()) {
		case VertKind::Node:
			prop = SatProp::Node();
			break;
		case VertKind::Clause:
			prop = SatProp::Clause();
			break;
		}
		prop.name = other.name;
		prop.color = other.color;
		return prop;
	}

	template<typename SatProp>
	EProp<SatProp> copy_base(const EProp<BaseSatProp>& other) {
		auto prop = EProp<SatProp>();
		prop.sgn = other.sgn;
		return prop;
	}

}
