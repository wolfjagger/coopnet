#pragma once

#include <iostream>
#include <stack>
#include "boost/logic/tribool.hpp"
#include "boost/variant.hpp"
#include "graph.h"



namespace sat {

	enum class PruneStatus {
		Active, Inactive
	};



	using VertStatusMap
		= std::unordered_map<VertDescriptor, PruneStatus>;
	using EdgeStatusMap = EdgeDescUnordMap<PruneStatus>;
	using IncompleteAssignmentMap
		= std::unordered_map<VertDescriptor, boost::tribool>;

	using VertStatusPair = std::pair<VertDescriptor, PruneStatus>;
	using EdgeStatusPair = std::pair<EdgeDescriptor, PruneStatus>;
	using IncompleteAssignmentPair
		= std::pair<VertDescriptor, boost::tribool>;


	struct PruneAction {

		enum class PruneObject { Vertex, Edge, Assignment };

		PruneObject type;
		boost::variant<VertStatusPair, EdgeStatusPair,
			IncompleteAssignmentPair> suppData;

		explicit PruneAction(VertStatusPair pruneData);
		explicit PruneAction(EdgeStatusPair pruneData);
		explicit PruneAction(IncompleteAssignmentPair pruneData);

	};

	using PruneStack = std::stack<PruneAction>;




	class PruneInfo {

	private:

		VertStatusMap vertStatus;
		EdgeStatusMap edgeStatus;
		IncompleteAssignmentMap assignment;
		PruneStack pruneStack;

	public:

		template<typename Graph>
		PruneInfo(const Graph& g) {
		
			auto vPair = boost::vertices(g);
			auto ePair = boost::edges(g);

			std::for_each(vPair.first, vPair.second, [this, &g](VertDescriptor v) {
				vertStatus.emplace(v, PruneStatus::Active);
				if (g[v].kind == VertProp::Node) {
					assignment.emplace(v, boost::indeterminate);
				}
			});

			std::for_each(ePair.first, ePair.second, [this](EdgeDescriptor v) {
				edgeStatus.emplace(v, PruneStatus::Active);
			});

		}



		PruneStatus get_vert_status(VertDescriptor v) const;
		void set_vert_status(VertDescriptor v, PruneStatus newStatus);

		PruneStatus get_edge_status(EdgeDescriptor e) const;
		void set_edge_status(EdgeDescriptor e, PruneStatus newStatus);

		bool is_indeterminate() const;
		boost::tribool get_assignment(VertDescriptor v) const;
		void set_assignment(VertDescriptor v, boost::tribool newAssignment);

		const IncompleteAssignmentMap& get_assignment_map() const;



		void reverse_to_vert(VertDescriptor v);

		void reset_prune();

	};



	inline std::ostream& operator<<(std::ostream& os, PruneStatus status);

	inline std::ostream& operator<<(std::ostream& os, boost::logic::tribool b);

}
