#include "reverse_info.h"

using namespace coopnet;



ReversableAction::ReversableAction(VertStatusPair reverseData) :
	type(ReverseCategory::Vertex),
	suppData(reverseData) {

}

ReversableAction::ReversableAction(EdgeStatusPair reverseData) :
	type(ReverseCategory::Edge),
	suppData(reverseData) {

}

ReversableAction::ReversableAction(IncompleteAssignmentPair reverseData) :
	type(ReverseCategory::Assignment),
	suppData(reverseData) {

}
