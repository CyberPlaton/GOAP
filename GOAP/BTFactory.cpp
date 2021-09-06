#include "BTFactory.h"

bool BTFactory::exportBehaviorTree(BehaviorTree* tree)
{
	using namespace tinyxml2;
	using namespace std;


	BTNode* root = tree->m_Root;


	XMLDocument doc;
	XMLNode* xml_root = doc.NewElement("BehaviorTree");
	doc.InsertFirstChild(xml_root);



	// Performing Depth First Search Algorithm on Tree.
	std::queue<BTNode*> node_queue;
	node_queue.push(root);

	XMLElement* xml_elem = nullptr;
	XMLElement* xml_parent_elem = nullptr;

	std::map<std::string, XMLElement*> parent_map; // { node_name, parent_element }


	while (!node_queue.empty())
	{
		BTNode* n = node_queue.front(); // Get the oldest node in queue
		node_queue.pop(); // Remove the oldest node in queue


		// Store current xml element as parent for next layer
		xml_elem = doc.NewElement("Node");


		// Emplace into current level parent the data from current node
		if (parent_map.find(n->name()) != parent_map.end())
		{
			xml_parent_elem = parent_map[n->name()];

			xml_elem->SetAttribute("name", n->name().c_str());
			xml_elem->SetAttribute("type", n->type().c_str());
			xml_elem->SetAttribute("parent", n->parent()->name().c_str());
			n->exportToXML(xml_elem); // Set node specific attributes for xml element
			xml_parent_elem->InsertEndChild(xml_elem);
		}
		else
		{
			xml_elem->SetAttribute("name", n->name().c_str());
			xml_elem->SetAttribute("type", n->type().c_str());
			xml_elem->SetAttribute("parent", "none");
			xml_root->InsertEndChild(xml_elem);
		}


		for (auto& kid : n->children())
		{
			// Set current xml node as parent for its children
			parent_map.emplace(std::make_pair(kid.second->name(), xml_elem));

			node_queue.push(kid.second);
		}


	}



	if (doc.SaveFile((root->name() + ".xml").c_str()) == XMLError::XML_SUCCESS)
	{
		return true;
	}

	return false;
}

/*
bool BTFactory::exportBehaviorTree(BehaviorTree* tree)
{
	using namespace tinyxml2;
	using namespace std;

	BTNode* root = tree->m_Root;

	XMLDocument doc;

	XMLNode* xml_root = doc.NewElement("BehaviorTree");
	doc.InsertFirstChild(xml_root);


	XMLElement* xml_child_node = nullptr;
	XMLElement* xml_parent_node = nullptr;

	// Performing Depth First Search Algorithm on Tree.
	std::queue<BTNode*> node_queue;
	node_queue.push(root);


	// Insert Root XML node
	XMLElement* elem = doc.NewElement("Node");
	elem->SetAttribute("name", root->name().c_str());
	elem->SetAttribute("type", root->type().c_str());
	elem->SetAttribute("parent", "none");
	xml_root->InsertFirstChild(elem);
	bool root_skipped = false;


	while (!node_queue.empty())
	{
		BTNode* n = node_queue.front(); // Get the oldest node in queue
		node_queue.pop(); // Remove the oldest node in queue


		// Insert the parent xml element
		if (root_skipped)
		{
			xml_parent_node = doc.NewElement("Node");
			xml_parent_node->SetAttribute("name", n->name().c_str());
			xml_parent_node->SetAttribute("type", n->type().c_str());
			xml_parent_node->SetAttribute("parent", n->parent()->name().c_str());
		}
		else
		{
			xml_parent_node = elem;
		}

		root_skipped = true; // Root is always the first node from queue.


		cout << "<Parent name=" << n->name() <<", type="<< n->type() << ">" << endl;


		for (auto& kid : n->children())
		{
			// Do stuff.
			cout << "\t<Child name=" << kid.second->name() << ", type=" << kid.second->type() << ">" << endl;


			// Insert all children elements as children for parent xml element
			xml_child_node = doc.NewElement("Node");
			xml_child_node->SetAttribute("name", kid.second->name().c_str());
			xml_child_node->SetAttribute("type", kid.second->type().c_str());
			xml_child_node->SetAttribute("parent", kid.second->parent()->name().c_str());

			xml_parent_node->InsertEndChild(xml_child_node);


			node_queue.push(kid.second);
		}

		root_skipped = true;
	}



	if (doc.SaveFile((root->name() + ".xml").c_str()) == XMLError::XML_SUCCESS)
	{
		return true;
	}

	return false;
}
*/