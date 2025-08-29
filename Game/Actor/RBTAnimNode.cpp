#include "RBTAnimNode.h"

RBTAnimNode::RBTAnimNode()
	: nodeColor(NodeColor::Red), data(0), parentNode(nullptr), leftNode(nullptr), rightNode(nullptr), referenceNode(nullptr)
{

}

RBTAnimNode::RBTAnimNode(RBTNode* reference)
	: nodeColor(reference->GetColor()), data(reference->GetData()),
	parentNode(nullptr), leftNode(nullptr), rightNode(nullptr),
	referenceNode(reference)
{
}



RBTAnimNode::~RBTAnimNode()
{

}
