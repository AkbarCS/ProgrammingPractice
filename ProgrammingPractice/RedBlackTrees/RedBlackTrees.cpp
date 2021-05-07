#include <iostream>
#include <vector>

using namespace std;

enum class nodeColour
{
	red,
	black
};

struct red_black_node
{
	nodeColour colourOfNode;
	void* item;
	red_black_node* left;
	red_black_node* right;
	red_black_node* parent;
};

void insertNode(red_black_node* nodeToInsert);
void appendNode(red_black_node* nodeToInsert);
void left_rotate(red_black_node* node);
void right_rotate(red_black_node* node);
vector<red_black_node> findNode(red_black_node rootNode, void* nodeToFind);
void printPathToANode(vector<red_black_node> pathToNode);
bool doesNodeExist(red_black_node rootNode, void* nodeToFind);
void printTree(red_black_node treeToPrint);
void deleteTree(red_black_node* rootNode);

red_black_node* _rootNode;

bool _isTreeEmpty = true;

void insertNode(red_black_node* nodeToInsert)
{
	/* Insert in the tree in the usual way */
	appendNode(nodeToInsert);

	red_black_node* currentNode = nodeToInsert;
	
	/* Now restore the red-black property */	
	currentNode->colourOfNode = nodeColour::red;

	if (currentNode->parent->parent)
	{
		while (currentNode->parent != NULL && currentNode->parent->colourOfNode == nodeColour::red)
		{
			if (currentNode->parent->parent->left && *(int*)currentNode->parent->item == *(int*)currentNode->parent->parent->left->item)
			{
				/* If x's parent is a left, y is x's right 'uncle' */
				red_black_node* y;

				if (!currentNode->parent->parent->right)
				{
					y = new red_black_node;
					y->left = NULL;
					y->right = NULL;
					y->parent = currentNode->parent->parent;
					y->colourOfNode = nodeColour::black;

					int* tempNumber = new int;
					*tempNumber = -1;
					y->item = tempNumber;

					currentNode->parent->parent->right = y;
				}
				else
				{
					y = currentNode->parent->parent->right;
				}

				if (y->colourOfNode == nodeColour::red)
				{
					/* case 1 - change the colours */
					currentNode->parent->colourOfNode = nodeColour::black;
					y->colourOfNode = nodeColour::black;
					currentNode->parent->parent->colourOfNode = nodeColour::red;

					/* Move x up the tree */
					currentNode = currentNode->parent->parent;
				}
				else
				{
					/* y is a black node */
					if (currentNode->parent->right && *(int*)currentNode->item == *(int*)currentNode->parent->right->item)
					{
						/* and x is to the right */
						/* case 2 - move x up and rotate */
						currentNode = currentNode->parent;
						left_rotate(currentNode);
					}

					/* case 3 */
					currentNode->parent->colourOfNode = nodeColour::black;
					currentNode->parent->parent->colourOfNode = nodeColour::red;
					right_rotate(currentNode->parent->parent);
				}
			}
			else if (*(int*)currentNode->parent->item == *(int*)currentNode->parent->parent->right->item)
			{
				/* repeat the "if" part with right and left exchanged */
				/* If x's parent is a right, y is x's left 'uncle' */
				red_black_node* y;

				if (!currentNode->parent->parent->left)
				{
					y = new red_black_node;
					y->left = NULL;
					y->right = NULL;
					y->parent = currentNode->parent->parent;
					y->colourOfNode = nodeColour::black;

					int* tempNumber = new int;
					*tempNumber = -1;
					y->item = tempNumber;

					currentNode->parent->parent->left = y;
				}
				else
				{
					y = currentNode->parent->parent->left;
				}

				if (y->colourOfNode == nodeColour::red)
				{
					/* case 1 - change the colours */
					currentNode->parent->colourOfNode = nodeColour::black;
					y->colourOfNode = nodeColour::black;
					currentNode->parent->parent->colourOfNode = nodeColour::red;

					/* Move x up the tree */
					currentNode = currentNode->parent->parent;
				}
				else
				{
					/* y is a black node */
					if (currentNode->parent->left && *(int*)currentNode->item == *(int*)currentNode->parent->left->item)
					{
						/* and x is to the left */
						/* case 2 - move x up and rotate */
						currentNode = (currentNode->parent);
						right_rotate(currentNode);
					}

					/* case 3 */
					currentNode->parent->colourOfNode = nodeColour::black;
					currentNode->parent->parent->colourOfNode = nodeColour::red;
					left_rotate(currentNode->parent->parent);
				}
			}
		}
	}

	/* Colour the root black */
	_rootNode->colourOfNode = nodeColour::black;
}

void appendNode(red_black_node* nodeToInsert)
{
	red_black_node* currentNode = _rootNode;
	red_black_node* parentNode = _rootNode;

	while (currentNode && *(int*)currentNode->item > 0)
	{
		parentNode = currentNode;

		if (*(int*)nodeToInsert->item < *(int*)currentNode->item)
		{
			currentNode = currentNode->left;
		}
		else
		{
			currentNode = currentNode->right;
		}
	}

	if (parentNode)
	{
		nodeToInsert->colourOfNode = nodeColour::red;
		nodeToInsert->parent = parentNode;

		if (*(int*)nodeToInsert->item < *(int*)parentNode->item)
		{
			parentNode->left = nodeToInsert;
		}
		else
		{
			parentNode->right = nodeToInsert;
		}
	}
}

void left_rotate(red_black_node* node)
{
	red_black_node* y = (*node).right;

	/* Turn y's left sub-tree into x's right sub-tree */
	(*node).right = y->left;

	if (y->left != NULL)
	{
		(y->left)->parent = node;
	}

	/* y's new parent was x's parent */
	y->parent = (*node).parent;

	/* Set the parent to point to y instead of x */
	/* First see whether we're at the root */
	if ((*node).parent == NULL)
	{
		_rootNode = y;
	}
	else
	{
		if (*(int*)(*node).item == *(int*)(*(*(*node).parent).left).item)
		{
			/* x was on the left of its parent */
			(*(*node).parent).left = y;
		}
		else
		{
			/* x must have been on the right */
			(*(*node).parent).right = y;
		}
	}

	/* Finally, put x on y's left */
	y->left = node;
	(*node).parent = y;
}

void right_rotate(red_black_node* node)
{
	red_black_node* y = (*node).left;

	/* Turn y's right sub-tree into x's left sub-tree */
	(*node).left = y->right;

	if (y->right != NULL)
	{
		(y->right)->parent = node;
	}

	/* y's new parent was x's parent */
	y->parent = (*node).parent;

	/* Set the parent to point to y instead of x */
	/* First see whether we're at the root */
	if ((*node).parent == NULL)
	{
		_rootNode = y;
	}
	else
	{
		if (*(int*)(*node).item == *(int*)(*(*(*node).parent).right).item)
		{
			/* x was on the right of its parent */
			(*(*node).parent).right = y;
		}
		else
		{
			/* x must have been on the left */
			(*(*node).parent).left = y;
		}
	}

	/* Finally, put x on y's right */
	y->right = node;
	(*node).parent = y;
}

vector<red_black_node> findNode(red_black_node rootNode, void* nodeToFind)
{
	vector<red_black_node> pathToNode = vector<red_black_node>();
	pathToNode.push_back(rootNode);

	red_black_node currentNode = rootNode;

	while (*(int*)nodeToFind != *(int*)currentNode.item)
	{
		if (*(int*)nodeToFind < *(int*)currentNode.item)
		{
			currentNode = *currentNode.left;
		}
		else
		{
			currentNode = *currentNode.right;
		}

		pathToNode.push_back(currentNode);
	}

	return pathToNode;
}

void printPathToANode(vector<red_black_node> pathToNode)
{
	printf("The path to the node is:\n");

	for (size_t i = 0; i < pathToNode.size(); i++)
	{
		if (i == 0)
		{
			printf("Root node - ");
		}

		string colourOfNode = pathToNode[i].colourOfNode == nodeColour::red ? "Red" : "Black";
		printf("%s node with value of %d\n", colourOfNode.c_str(), *(int*)pathToNode[i].item);
	}
}

bool doesNodeExist(red_black_node rootNode, void* nodeToFind)
{
	bool doesNodeExistReturnValue = false;

	if (_isTreeEmpty)
	{
		printf("Tree has no nodes\n");
	}
	else
	{
		red_black_node currentNode = rootNode;

		int a = *(int*)nodeToFind;
		int b = *(int*)currentNode.item;

		if (*(int*)nodeToFind == *(int*)currentNode.item)
		{
			doesNodeExistReturnValue = true;
		}
		else if (*(int*)nodeToFind < *(int*)currentNode.item)
		{
			if (currentNode.left)
			{
				doesNodeExistReturnValue = doesNodeExist(*currentNode.left, nodeToFind);
			}
		}
		else if (*(int*)nodeToFind > *(int*)currentNode.item)
		{
			if (currentNode.right)
			{
				doesNodeExistReturnValue = doesNodeExist(*currentNode.right, nodeToFind);
			}
		}
	}

	return doesNodeExistReturnValue;
}

void printTree(red_black_node treeToPrint)
{
	if (_isTreeEmpty)
	{
		printf("Tree has no nodes\n");
	}
	else
	{
		red_black_node currentNode = treeToPrint;

		string colourOfNode = currentNode.colourOfNode == nodeColour::red ? "Red" : "Black";
		int parentNodeValue = currentNode.parent ? *(int*)(*currentNode.parent).item : -1;
		int nodeValue = *(int*)currentNode.item;

		if (nodeValue > 0)
		{
			printf("%s node with value of %d. Parent value has value %d\n", colourOfNode.c_str(), nodeValue, parentNodeValue);
		}

		if (currentNode.left && *(int*)currentNode.left->item > 0)
		{
			printf("Left node - ");
			printTree(*currentNode.left);
		}

		if (currentNode.right && *(int*)currentNode.right->item > 0)
		{
			printf("Right node - ");
			printTree(*currentNode.right);
		}
	}
}

void deleteTree(red_black_node* rootNode)
{
	if (_isTreeEmpty)
	{
		return;
	}

	red_black_node* currentNode = rootNode;

	if (currentNode->left)
	{
		deleteTree(currentNode->left);
	}

	if (currentNode->right)
	{
		deleteTree(currentNode->right);
	}
	
	if (!currentNode->left && !currentNode->right)
	{
		delete currentNode->item;
		delete currentNode;
	}
}

int main()
{
	bool exit = false;

	while (!exit)
	{
		printf("Select option\n");
		printf("Press 1 to insert node\n");
		printf("Press 2 to find the path to a given node\n");
		printf("Press 3 to check if node exists\n");
		printf("Press 4 to print the tree\n");
		printf("Press 5 to exit\n");

		int optionSelected = 0;

		scanf_s("%d", &optionSelected);

		switch (optionSelected)
		{
			case 1:
			{
				//insert node - nodes must be unique

				if (_isTreeEmpty)
				{
					int* newNumber = new int;

					printf("Input number to assign to node\n");
					scanf_s("%d", newNumber);

					_rootNode = new red_black_node;

					_rootNode->item = newNumber;
					_rootNode->parent = NULL;
					_rootNode->left = NULL;
					_rootNode->right = NULL;
					_rootNode->colourOfNode = nodeColour::black;

					_isTreeEmpty = false;

					break;
				}

				while (true)
				{
					red_black_node* newNode = new red_black_node;
					newNode->parent = NULL;
					newNode->left = NULL;
					newNode->right = NULL;

					int* newNumber = new int;

					printf("Input number to assign to node\n");
					scanf_s("%d", newNumber);

					newNode->item = newNumber;

					if (!doesNodeExist(*_rootNode, newNumber))
					{
						insertNode(newNode);
						break;
					}
					else
					{
						printf("Nodes must have a unique number, please try again\n");
						delete newNode;
						delete newNumber;
					}
				}

				break;
			}
			case 2:
			{
				//find the path to a given node
				while (true)
				{
					int nodeNumberInputted = 0;

					printf("Input number of node to find\n");
					scanf_s("%d", &nodeNumberInputted);

					if (doesNodeExist(*_rootNode, &nodeNumberInputted))
					{
						printPathToANode(findNode(*_rootNode, &nodeNumberInputted));
						break;
					}
					else
					{
						printf("Node not found, please try again\n");
					}
				}

				break;
			}
			case 3:
			{
				//check if node exists
				int nodeNumberInputted = 0;

				printf("Input number of node to find\n");
				scanf_s("%d", &nodeNumberInputted);

				if (doesNodeExist(*_rootNode, &nodeNumberInputted))
				{
					printf("Node exists\n");
				}
				else
				{
					printf("Node does not exist\n");
				}

				break;
			}
			case 4:
			{
				printTree(*_rootNode);
				break;
			}
			case 5:
			{
				exit = true;
				break;
			}
			default:
			{
				printf("Unrecognised command, please try again\n");
				break;
			}
		}

		printf("\n");
	}

	deleteTree(_rootNode);
}