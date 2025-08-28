#include "RBTNode.h"

RBTNode::RBTNode(int data, NodeColor color)
	: data(data), color(color)  // ∏‚πˆ ¿Ã¥œº»∂Û¿Ã¡Æ.
{
}

const char* RBTNode::GetColorString() const
{
	return color == NodeColor::Red ? "RED" : "BLACK";
}