#pragma once

#include "coopnet/sat/component/node.h"
#include "base/graph.h"



namespace coopnet {

	class Problem;

	class SatGraphTranslator {

	private:

		NodeVertMap nodeVertMap;
		NodeList nodeList;

	public:

		SatGraphTranslator(const NodeVertMap& initNodeVertMap);
		SatGraphTranslator(NodeVertMap&& initNodeVertMap);
		SatGraphTranslator(const Problem& prob);



		auto node_begin() const {
			return nodeList.cbegin();
		}
		auto node_end() const {
			return nodeList.cend();
		}

		VertDescriptor node_to_vert(Node node) const;
		Node vert_to_node(VertDescriptor vert) const;

		template<typename Pred>
		void apply(Pred&& pred) const {
			std::for_each(nodeVertMap.cbegin(), nodeVertMap.cend(), std::forward<Pred>(pred));
		}

	private:

		void init_node_list();

	};



	template<typename SatProp>
	struct TranslatedSatGraph {
		SatGraph<SatProp> graph;
		SatGraphTranslator translator;
	};

}
