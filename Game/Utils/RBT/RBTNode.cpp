#include "RBTNode.h"

RBTNode::RBTNode(int data, NodeColor color)
	: data(data), color(color)  // ��� �̴ϼȶ�����.
{
}

const char* RBTNode::GetColorString() const
{
	return color == NodeColor::Red ? "RED" : "BLACK";
}