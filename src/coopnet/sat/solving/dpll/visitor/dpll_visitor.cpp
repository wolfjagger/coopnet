#include "dpll_visitor.h"
#include <iostream>

using namespace sat;



namespace {
	constexpr bool DEBUG = false;
}



DPLLBeginVertVisitor::DPLLBeginVertVisitor(
	PruneInfo& initPruneInfo,
	bool& initIsContradicting,
	DPLLPropMaps initMaps) :
	PruneSatVertVisitor(initPruneInfo),
	isContradicting(initIsContradicting),
	maps(initMaps) {}



// This visitor will decide if the vert should be removed:
//  either if it only has one edge (clause)
//  or if it has all edges with same sign (node).
// It also needs to color the surrounding edges if they
//  should be (re)visited (i.e. if vert is to be removed).
void DPLLBeginVertVisitor::node_event(
	const SatGraph& g, VertDescriptor node, const VertProp& prop) {

	switch(maps.vertStatusMap[node]) {
	case DPLLVertStatus::SetToTrue:

		select_node(g, node, true);
		break;

	case DPLLVertStatus::SetToFalse:

		select_node(g, node, false);
		break;

	case DPLLVertStatus::Remove:

		throw std::exception("Node inappropriately set to 'Remove'.");

	case DPLLVertStatus::Default:

		// If there are no active edges, select node freely
		if (!any_active_edge(node, g)) {

			//TODO: Select at random?
			select_node(g, node, true);

		} else {

			auto edges_pair = boost::out_edges(node, g);
			auto constrain_node_fcn = [this](EdgeDescriptor e) {
				return maps.edgeStatusMap[e] == DPLLEdgeStatus::ConstrainNode;
			};

			auto constraining_edge = std::find_if(
				edges_pair.first, edges_pair.second, constrain_node_fcn);

			// If any edge is set to constrain the node, constrain it
			//  (select_node will check for multiple conflicting constraints)
			if (constraining_edge != edges_pair.second) {

				prune_info().set_edge_status(*constraining_edge, PruneStatus::Inactive);
				select_node(g, node, g[*constraining_edge].sgn);

			}
			// Otherwise, if all active edges have same sgn, select node = sgn
			else {

				auto first_active_edge = find_active_edge(node, g);
				auto first_sgn = g[*first_active_edge].sgn;
				auto same_sgn_fcn = [first_sgn, &g, this](EdgeDescriptor e) {
					return first_sgn == g[e].sgn;
				};

				if (all_of_active_edges(node, g, same_sgn_fcn))
					select_node(g, node, first_sgn);

			}

		}

		break;

	default:
		throw std::exception("Unknown DPLLVertStatus.");
	}

}

void DPLLBeginVertVisitor::clause_event(
	const SatGraph& g, VertDescriptor clause, const VertProp& prop) {

	auto& clause_status = maps.vertStatusMap[clause];

	switch(clause_status) {
	case DPLLVertStatus::Remove: {

		satisfy_clause(g, clause);
		break;

	}
	case DPLLVertStatus::SetToTrue:
	case DPLLVertStatus::SetToFalse: {

		auto stream = std::stringstream();
		stream << "Clause inappropriately set to '";
		stream << clause_status << "'.";
		auto str = stream.str();
		throw std::exception(str.c_str());

	}
	case DPLLVertStatus::Default: {

		auto edges_pair = boost::out_edges(clause, g);

		auto satisfy_clause_fcn = [this](EdgeDescriptor e) {
			return maps.edgeStatusMap[e] == DPLLEdgeStatus::SatisfyClause;
		};
		auto satisfying_edge = find_if_active_edge(clause, g, satisfy_clause_fcn);

		// If any edge is set to satisfy the clause, satisfy it
		//  (satisfy_clause will check for multiple conflicting constraints)
		if (satisfying_edge != edges_pair.second) {

			deactivate_edge(*satisfying_edge);
			satisfy_clause(g, clause);

		} else {
			
			auto num_active_edges = count_active_edges(clause, g);
			
			// If no edges left, no way to satisfy clause: contradicting
			if (num_active_edges == 0) {
				if(DEBUG) std::cout << "Contradict: no edges to satisfy clause.\n";
				isContradicting = true;
			}
			// If one edge left, it must be used to satisfy clause
			else if (num_active_edges == 1) {
			
				auto edge = find_active_edge(clause, g);

				change_edge_status(*edge, DPLLEdgeStatus::ConstrainNode);
				satisfy_clause(g, clause);

			}
			
		}
		break;

	}
	default:
		throw std::exception("Unknown DPLLVertStatus.");
	}

}



void DPLLBeginVertVisitor::select_node(
	const SatGraph& g, VertDescriptor node, bool sgn) {

	prune_info().set_assignment(node, sgn);

	auto prop_to_edges_fcn = [this, &g, sgn](EdgeDescriptor edge) {

		auto status = maps.edgeStatusMap[edge];
		switch (status) {
		case DPLLEdgeStatus::Default:
			// If active edge, use to satisfy clause (if correct sgn) or remove
			if (g[edge].sgn == sgn) {
				change_edge_status(edge, DPLLEdgeStatus::SatisfyClause);
			} else {
				change_edge_status(edge, DPLLEdgeStatus::Remove);
			}
			break;
		case DPLLEdgeStatus::ConstrainNode:
			// If already set to constrain node, set inactive or contradict
			if (g[edge].sgn == sgn) {
				deactivate_edge(edge);
			} else {
				if(DEBUG) std::cout << "Contradict: opposite constraints on node.\n";
				isContradicting = true;
			}
			break;
		}

	};

	for_each_active_edge(node, g, prop_to_edges_fcn);

	deactivate_vert(node);

}



void DPLLBeginVertVisitor::satisfy_clause(
	const SatGraph& g, VertDescriptor clause) {
	
	auto remove_edges_fcn = [this](EdgeDescriptor edge) {

		auto status = maps.edgeStatusMap[edge];
		switch (status) {
		case DPLLEdgeStatus::Default:
		case DPLLEdgeStatus::SatisfyClause:
			// Even if pushing node info to clause, info is now unnecessary
			change_edge_status(edge, DPLLEdgeStatus::Remove);
			break;
		}

	};

	for_each_active_edge(clause, g, remove_edges_fcn);

	deactivate_vert(clause);

}



void DPLLBeginVertVisitor::deactivate_vert(VertDescriptor vert) {
	change_vert_status(vert, DPLLVertStatus::Default);
	prune_info().set_vert_status(vert, PruneStatus::Inactive);
}

void DPLLBeginVertVisitor::deactivate_edge(EdgeDescriptor edge) {
	change_edge_status(edge, DPLLEdgeStatus::Default);
	prune_info().set_edge_status(edge, PruneStatus::Inactive);
}




void DPLLBeginVertVisitor::change_vert_status(
	VertDescriptor vert, DPLLVertStatus newStatus) {

	auto& status = maps.vertStatusMap[vert];
	if (status != newStatus) {
		if(DEBUG) std::cout << "vert " << vert << " goes " << status << " to " << newStatus << std::endl;
		status = newStatus;
	}

}

void DPLLBeginVertVisitor::change_edge_status(
	EdgeDescriptor edge, DPLLEdgeStatus newStatus) {

	auto& status = maps.edgeStatusMap[edge];
	if (status != newStatus) {
		if(DEBUG) std::cout << "edge " << edge << " goes " << status << " to " << newStatus << std::endl;
		status = newStatus;
	}

}





DPLLEdgeVisitor::DPLLEdgeVisitor(
	PruneInfo& initPruneInfo,
	bool& initIsContradicting,
	DPLLPropMaps initMaps) :
	PruneSatEdgeVisitor(initPruneInfo),
	isContradicting(initIsContradicting),
	maps(initMaps) {}



// This has three responsibilities.
// (1) color unvisited vertex to visit it
// (2) it needs to propagate removals to the next vertices,
//  with cases depending on the circumstances:
//  (a) node = a => remove clause iff sgn(edge) == a
//  (b) clause => set node = a iff clause.num_edges == 1
// (3) remove edge
void DPLLEdgeVisitor::edge_event(
	const SatGraph& g, EdgeDescriptor edge,
	const EdgeProp& prop,
	VertDescriptor node, VertDescriptor clause) {

	auto status = maps.edgeStatusMap[edge];

	// If edge is in default state, there is no action required;
	//  the edge is not meant to be traversed
	if (status == DPLLEdgeStatus::Default) return;

	// If edge should be traversed (i.e. has status SatisfyClause, ConstrainNode, or Remove),
	//  propagate white color to target if black (grey means in queue already)
	auto& color = maps.colorMap[boost::target(edge, g)];
	if (color == default_color_type::black_color)
		color = default_color_type::white_color;


	switch (status) {
	case DPLLEdgeStatus::SatisfyClause: {

		// If node satisfies clause, remove clause

		if (prune_info().get_vert_status(clause) != PruneStatus::Inactive)
			change_vert_status(clause, DPLLVertStatus::Remove);
		break;

	}
	case DPLLEdgeStatus::ConstrainNode: {

		// If clause constrains node, set node to sgn

		if(prune_info().get_vert_status(node) == PruneStatus::Active) {

			switch (maps.vertStatusMap[node]) {
			case DPLLVertStatus::Default:

				change_vert_status(node, prop.sgn ?
					DPLLVertStatus::SetToTrue : DPLLVertStatus::SetToFalse);
				break;

			case DPLLVertStatus::SetToTrue:

				if (!prop.sgn) {
					if (DEBUG) std::cout << "Contradict: Can't constrain node to false when already true.\n";
					isContradicting = true;
				}
				break;

			case DPLLVertStatus::SetToFalse:

				if (prop.sgn) {
					if (DEBUG) std::cout << "Contradict: Can't constrain node to true when already false.\n";
					isContradicting = true;
				}
				break;

			}

		}

		break;

	}}
	
	deactivate_edge(edge);

}



void DPLLEdgeVisitor::deactivate_edge(EdgeDescriptor edge) {
	change_edge_status(edge, DPLLEdgeStatus::Default);
	prune_info().set_edge_status(edge, PruneStatus::Inactive);
}



//TODO: Undo replication here and redundancy btwn the three unique methods
void DPLLEdgeVisitor::change_vert_status(
	VertDescriptor vert, DPLLVertStatus newStatus) {

	auto& status = maps.vertStatusMap[vert];
	if (status != newStatus) {
		if(DEBUG) std::cout << "vert " << vert << " goes " << status << " to " << newStatus << std::endl;
		status = newStatus;
	}

}

void DPLLEdgeVisitor::change_edge_status(
	EdgeDescriptor edge, DPLLEdgeStatus newStatus) {

	auto& status = maps.edgeStatusMap[edge];
	if (status != newStatus) {
		if(DEBUG) std::cout << "edge " << edge << " goes " << status << " to " << newStatus << std::endl;
		status = newStatus;
	}

}





DPLLFinishVertVisitor::DPLLFinishVertVisitor(
	boost::queue<VertDescriptor>& initGreyBuffer,
	bool& initIsContradicting,
	DPLLPropMaps initMaps) :
	greyBuffer(initGreyBuffer),
	isContradicting(initIsContradicting),
	maps(initMaps) {}



namespace {

	DPLLVisitorTuple make_visitor_tuple(
		PruneInfo& pruneInfo,
		boost::queue<VertDescriptor>& grey_queue,
		bool& is_contradicting,
		DPLLPropMaps maps) {

		DPLLBeginVertVisitor begin_vert_vis(
			pruneInfo, is_contradicting, maps);
		DPLLEdgeVisitor edge_vis(
			pruneInfo, is_contradicting, maps);
		DPLLFinishVertVisitor finish_vert_vis(
			grey_queue, is_contradicting, maps);
		return std::make_pair(begin_vert_vis,
			std::make_pair(edge_vis, finish_vert_vis));

	}

}

DPLLVisitor::DPLLVisitor(
	PruneInfo& pruneInfo,
	boost::queue<VertDescriptor>& greyQueue,
	DPLLPropMaps maps) :
	boost::bfs_visitor<DPLLVisitorTuple>(
		make_visitor_tuple(pruneInfo, greyQueue, isContradicting, maps)),
	isContradicting(false) {

}
