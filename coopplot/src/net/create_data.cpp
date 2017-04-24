#include "create_data.h"
#include <numeric>
#include "alphali/util/timer.h"
#include "coopnet/sat/problem/problem.h"
#include "coopnet/sat/problem/creation/problem_factory.h"
#include "coopnet/sat/solving/dpll/dpll_solver.h"
#include "coopnet/sat/solving/walk/walk_solver.h"

using namespace coopnet;



namespace {
	constexpr bool PRINTSubTiming = false;
}



namespace coopplot {

	namespace {

		std::array<double, 2> create_x_domain(
			double startRatio, double endRatio, int numRatios) {

			auto diff_ratio
				= (endRatio - startRatio) / (numRatios - 1);

			return std::array<double, 2>{
				startRatio, diff_ratio };

		}

		
		
		std::vector<std::array<double, 3>> frac_satisfiable(
			std::vector<std::unique_ptr<Solver>>& solvers,
			int numNodes, int numClauses, int numAvg) {
			
			auto size = solvers.size();

			auto numSat = std::vector<int>(size);
			auto setTimes = std::vector<std::vector<double>>(size);
			for(auto& times : setTimes) times.reserve(numAvg);

			auto timer = alphali::timer();

			for (auto j = 0; j < numAvg; ++j) {

				auto problem
					= problem_factory::random_3sat_problem(numNodes, numClauses);

				for(unsigned solverIdx = 0; solverIdx < size; ++solverIdx) {

					timer.start();

					auto& solver = *solvers[solverIdx];

					solver.set_problem(problem);
					auto solutionPair = solver.solve();
					if (solutionPair.status == SolutionStatus::Satisfied)
						++numSat[solverIdx];

					timer.stop();
					setTimes[solverIdx].push_back(timer.secs_elapsed());
					if (PRINTSubTiming) {
						timer.output("Generate/solve");
					}

				}

			}

			auto retVec = std::vector<std::array<double, 3>>();
			retVec.reserve(size);
			for(unsigned solverIdx = 0; solverIdx < size; ++solverIdx) {

				auto fracSat = double(numSat[solverIdx]) / numAvg;

				auto timeAvg = std::accumulate(
					setTimes[solverIdx].begin(), setTimes[solverIdx].end(), 0.0);
				timeAvg /= numAvg;

				auto timeErr = 0.0;
				for (auto time : setTimes[solverIdx]) {
					timeErr += (time - timeAvg) * (time - timeAvg);
				}
				timeErr = std::sqrt(timeErr / numAvg);

				retVec.push_back({ fracSat, timeAvg, timeErr });

			}

			return retVec;

		}

		std::vector<std::array<double, 3>> frac_satisfiable(
			std::unique_ptr<Solver>& solver,
			int numNodes, int numClauses, int numAvg) {

			auto solvers = std::vector<std::unique_ptr<Solver>>();
			solvers.push_back(std::move(solver));
			auto retVec = frac_satisfiable(solvers, numNodes, numClauses, numAvg);
			solver = std::move(solvers[0]);
			solvers.pop_back();
			return retVec;

		}

	}



	SatReturn create_sat_data(
		int numNodes,
		double startRatioClauseNode, double endRatioClauseNode,
		int numRatios, int numAvg,
		DPLLNodeChoiceMode nodeChoiceMode) {

		auto xDomain = create_x_domain(
			startRatioClauseNode, endRatioClauseNode, numRatios);

		auto solver = std::make_unique<DPLLSolver>();
		solver->set_chooser(
			DPLLNodeChooser::create(nodeChoiceMode));
		std::unique_ptr<Solver> castSolver = std::move(solver);

		auto vecY = std::vector<double>();
		vecY.reserve(numRatios);
		auto vecT = std::vector<std::array<double, 2>>();
		vecT.reserve(numRatios);
		for (auto i = 0; i < numRatios; ++i) {

			auto ratio = startRatioClauseNode + i*xDomain[1];
			auto numClauses = int(std::ceil(numNodes * ratio));

			std::cout << "Ratio: " << ratio << std::endl;

			auto satData = frac_satisfiable(
				castSolver, numNodes, numClauses, numAvg);

			auto& dpllData = satData[0];

			std::cout << "Set of num_node/num_clause ";
			std::cout << dpllData[1] << std::endl;

			vecY.push_back(dpllData[0]);
			vecT.push_back({ dpllData[1], dpllData[2] });

		}

		return {
			XYData<double, double>(std::make_pair(xDomain, vecY)),
			XYData<double, double>(std::make_pair(xDomain, vecT))
		};

	}


	SatReturn create_sat_data_comparison(
		int numNodes,
		double startRatioClauseNode, double endRatioClauseNode,
		int numRatios, int numAvg,
		DPLLNodeChoiceMode dpllNodeChoiceMode, WalkNodeChoiceMode walkNodeChoiceMode) {

		auto xDomain = create_x_domain(
			startRatioClauseNode, endRatioClauseNode, numRatios);

		auto solvers = std::vector<std::unique_ptr<Solver>>();

		auto dpllSolver = std::make_unique<DPLLSolver>();
		dpllSolver->set_chooser(
			DPLLNodeChooser::create(dpllNodeChoiceMode));
		solvers.push_back(std::move(dpllSolver));

		auto walkSolver = std::make_unique<WalkSolver>(5, 1000);
		walkSolver->set_chooser(
			WalkNodeChooser::create(walkNodeChoiceMode));
		solvers.push_back(std::move(walkSolver));

		auto vecYs = std::vector<std::vector<double>>();
		vecYs.reserve(numRatios);
		auto vecTs = std::vector<std::vector<std::array<double, 2>>>();
		vecTs.reserve(numRatios);
		for (auto i = 0; i < numRatios; ++i) {

			auto ratio = startRatioClauseNode + i*xDomain[1];
			auto numClauses = unsigned int(std::ceil(numNodes * ratio));

			std::cout << "Ratio: " << ratio << std::endl;

			auto satData = frac_satisfiable(
				solvers, numNodes, numClauses, numAvg);

			auto& dpllData = satData[0];
			auto& walkData = satData[1];

			std::cout << "Set of num_node/num_clause ";
			std::cout << dpllData[1] << " dpll avg and ";
			std::cout << walkData[1] << " walk avg" << std::endl;

			auto vecY = std::vector<double>();
			vecY.push_back(dpllData[0]);
			vecY.push_back(walkData[0]);
			vecYs.push_back(std::move(vecY));
			auto vecT = std::vector<std::array<double, 2>>();
			vecT.push_back({ dpllData[1], dpllData[2] });
			vecT.push_back({ walkData[1], walkData[2] });
			vecTs.push_back(std::move(vecT));

		}

		return{
			XYData<double, double>(std::make_pair(xDomain, vecYs)),
			XYData<double, double>(std::make_pair(xDomain, vecTs))
		};

	}


	SatReturn create_multiple_sat_data(
		int startNumNodes, int endNumNodes, int numPlots,
		double startRatioClauseNode, double endRatioClauseNode,
		int numRatios, int numAvg,
		DPLLNodeChoiceMode nodeChoiceMode) {

		auto xDomain = create_x_domain(
			startRatioClauseNode, endRatioClauseNode, numRatios);

		auto diff_numNodes = (endNumNodes - startNumNodes) / (numPlots-1);

		auto solver = std::make_unique<DPLLSolver>();
		solver->set_chooser(
			DPLLNodeChooser::create(nodeChoiceMode));
		std::unique_ptr<Solver> castSolver = std::move(solver);

		auto vecYs = std::vector<std::vector<double>>();
		vecYs.reserve(numRatios);
		auto vecTs = std::vector<std::vector<std::array<double, 2>>>();
		vecTs.reserve(numRatios);
		for (auto i = 0; i < numRatios; ++i) {

			auto ratio = startRatioClauseNode + i*xDomain[1];

			std::cout << "Ratio: " << ratio << std::endl;

			auto vecY = std::vector<double>();
			vecY.reserve(numPlots);
			auto vecT = std::vector<std::array<double, 2>>();
			vecT.reserve(numRatios);
			for(auto j=0; j < numPlots; ++j) {

				auto numNodes = startNumNodes + j*diff_numNodes;
				auto numClauses = int(std::ceil(numNodes * ratio));

				std::cout << "Num nodes: " << numNodes
					<< "  Num clauses: " << numClauses << std::endl;

				auto satData = frac_satisfiable(
					castSolver, numNodes, numClauses, numAvg);

				auto& dpllData = satData[0];

				std::cout << "Set of num_node/num_clause ";
				std::cout << dpllData[1] << std::endl;

				vecY.push_back(dpllData[0]);
				vecT.push_back({ dpllData[1], dpllData[2] });

			}

			vecYs.push_back(std::move(vecY));
			vecTs.push_back(std::move(vecT));

		}

		return {
			XYData<double, double>(std::make_pair(xDomain, vecYs)),
			XYData<double, double>(std::make_pair(xDomain, vecTs))
		};

	}


	SatReturn create_multiple_sat_data_comparison(
		int startNumNodes, int endNumNodes, int numPlots,
		double startRatioClauseNode, double endRatioClauseNode,
		int numRatios, int numAvg,
		DPLLNodeChoiceMode dpllNodeChoiceMode, WalkNodeChoiceMode walkNodeChoiceMode) {

		auto xDomain = create_x_domain(
			startRatioClauseNode, endRatioClauseNode, numRatios);

		auto diff_numNodes = (endNumNodes - startNumNodes) / (numPlots - 1);

		auto solvers = std::vector<std::unique_ptr<Solver>>();
		auto dpllSolver = std::make_unique<DPLLSolver>();
		dpllSolver->set_chooser(
			DPLLNodeChooser::create(dpllNodeChoiceMode));
		solvers.push_back(std::move(dpllSolver));
		auto walkSolver = std::make_unique<WalkSolver>(5, 1000);
		walkSolver->set_chooser(
			WalkNodeChooser::create(walkNodeChoiceMode));
		solvers.push_back(std::move(walkSolver));

		auto vecYs = std::vector<std::vector<double>>();
		vecYs.reserve(numRatios);
		auto vecTs = std::vector<std::vector<std::array<double, 2>>>();
		vecTs.reserve(numRatios);
		for (auto i = 0; i < numRatios; ++i) {

			auto ratio = startRatioClauseNode + i*xDomain[1];

			std::cout << "Ratio: " << ratio << std::endl;

			auto vecY = std::vector<double>();
			vecY.reserve(numPlots);
			auto vecT = std::vector<std::array<double, 2>>();
			vecT.reserve(numRatios);
			for (auto j = 0; j < numPlots; ++j) {

				auto numNodes = startNumNodes + j*diff_numNodes;
				auto numClauses = unsigned int(std::ceil(numNodes * ratio));

				std::cout << "Num nodes: " << numNodes
					<< "  Num clauses: " << numClauses << std::endl;

				auto satData = frac_satisfiable(
					solvers, numNodes, numClauses, numAvg);

				auto& dpllData = satData[0];
				auto& walkData = satData[1];

				std::cout << "Set of num_node/num_clause ";
				std::cout << dpllData[1] << " dpll avg and ";
				std::cout << walkData[1] << " walk avg" << std::endl;

				vecY.push_back(dpllData[0]);
				vecY.push_back(walkData[0]);
				vecT.push_back({ dpllData[1], dpllData[2] });
				vecT.push_back({ walkData[1], walkData[2] });

			}

			vecYs.push_back(std::move(vecY));
			vecTs.push_back(std::move(vecT));

		}

		return{
			XYData<double, double>(std::make_pair(xDomain, vecYs)),
			XYData<double, double>(std::make_pair(xDomain, vecTs))
		};

	}



	SatReturn create_walk_prob_data(
		int numNodes, int numClauses,
		int numAvg, int numProbs) {

		auto deltaX = 1.f / (numProbs-1);

		auto xDomain = create_x_domain(0, 1, numProbs);


		std::cout << "Get frac satisfiable using DPLL\n";

		auto dpllSolver = std::make_unique<DPLLSolver>();
		std::unique_ptr<DPLLNodeChooser> dpllChooser
			= std::make_unique<MaxTotClauseNodeChooser>();
		dpllSolver->set_chooser(std::move(dpllChooser));
		std::unique_ptr<Solver> castDPLLSolver = std::move(dpllSolver);

		auto dpllData = frac_satisfiable(
			castDPLLSolver, numNodes, numClauses, numAvg);


		std::unique_ptr<Solver> solver = std::make_unique<WalkSolver>(5, 10000);


		auto vecY = std::vector<std::vector<double>>();
		vecY.reserve(numProbs);
		auto vecT = std::vector<std::array<double, 2>>();
		vecT.reserve(numProbs);
		for (auto i = 0; i < numProbs; ++i) {

			auto prob = i*xDomain[1];

			std::cout << "Greedy probability: " << prob << std::endl;

			auto& walkSolver = static_cast<WalkSolver&>(*solver);
			walkSolver.create_chooser<UnsatClauseMCNodeChooser>(prob);

			auto satData = frac_satisfiable(
				solver, numNodes, numClauses, numAvg);

			auto& walkData = satData[0];

			std::cout << "Average of " << walkData[1] << " secs." << std::endl;

			vecY.push_back({ dpllData[0][0], walkData[0] });
			vecT.push_back({ walkData[1], walkData[2] });

		}

		return{
			XYData<double, double>(std::make_pair(xDomain, vecY)),
			XYData<double, double>(std::make_pair(xDomain, vecT))
		};


	}
	
}
