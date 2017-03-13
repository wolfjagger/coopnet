#pragma once

#include "pruned_sat_visitor.h"



namespace coopnet {

	template<class ImplVisitor>
	struct PruningSatVertVisitor :
		public PrunedSatVertVisitor<PruningSatVertVisitor<ImplVisitor>> {

	private:

		PruneStack& pruneStack;

	public:

		PruningSatVertVisitor(PruneStack& initPruneStack) :
			pruneStack(initPruneStack) { }



		template<class SatGraph, typename VProp>
		void node_event(
			const SatGraph& g, VertDescriptor node,
			const VProp& prop) {

			static_cast<ImplVisitor*>(this)->node_event(g, node, prop);

		}

		template<class SatGraph, typename VProp>
		void clause_event(
			const SatGraph& g, VertDescriptor clause,
			const VProp& prop) {

			static_cast<ImplVisitor*>(this)->clause_event(g, clause, prop);

		}



	protected:

		void set_prune_status(VertDescriptor v, PruneStatus& oldStatus, PruneStatus newStatus) {

			if (oldStatus != newStatus) {

				if (DEBUG) std::cout << "Set status for vert " << v << " to " << newStatus << std::endl;

				pruneStack.emplace(std::make_pair(v, oldStatus));
				oldStatus = newStatus;

			}

		}

		void set_prune_status(EdgeDescriptor e, PruneStatus& oldStatus, PruneStatus newStatus) {

			if (oldStatus != newStatus) {

				if (DEBUG) std::cout << "Set status for edge " << e << " to " << newStatus << std::endl;

				pruneStack.emplace(std::make_pair(e, oldStatus));
				oldStatus = newStatus;

			}

		}

		void set_assignment(VertDescriptor v, boost::tribool& oldAssign, boost::tribool newAssign) {

			if (oldAssign != newAssign ||
				(!boost::indeterminate(oldAssign) && boost::indeterminate(newAssign)) ||
				(boost::indeterminate(oldAssign) && !boost::indeterminate(newAssign))) {

				if (DEBUG) std::cout << "Set assignment for vert " << v << " to " << newAssign << std::endl;

				pruneStack.emplace(std::make_pair(v, oldAssign));
				oldAssign = newAssign;

			}

		}

	};



	template<class ImplVisitor>
	struct PruningSatEdgeVisitor :
		public PruneSatEdgeVisitor<PruningSatEdgeVisitor<ImplVisitor>> {

	private:

		PruneStack& pruneStack;

	public:

		PruningSatEdgeVisitor(PruneStack& initPruneStack) :
			pruneStack(initPruneStack) {}



		template<class SatGraph, typename EProp>
		void edge_event(
			const SatGraph& g, EdgeDescriptor edge,
			const EProp& prop,
			VertDescriptor node, VertDescriptor clause) {

			static_cast<ImplVisitor*>(this)->edge_event(
				g, edge, prop, node, clause);

		}




	protected:

		void set_prune_status(EdgeDescriptor e, PruneStatus& oldStatus, PruneStatus newStatus) {

			if (oldStatus != newStatus) {

				if (DEBUG) std::cout << "Set status for edge " << e << " to " << newStatus << std::endl;

				pruneStack.emplace(std::make_pair(e, oldStatus));
				oldStatus = newStatus;

			}

		}

	};

}
