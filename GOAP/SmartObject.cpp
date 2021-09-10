#include "SmartObject.h"


bool SmartObject::loadDefinition(const std::string& path)
{
	using namespace tinyxml2;

	tinyxml2::XMLDocument doc;
	XMLError result = doc.LoadFile(path.c_str());
	if (result != XMLError::XML_SUCCESS)
	{
		return false;
	}

	XMLNode* root = doc.FirstChild();
	if (root == nullptr) return false;


	// Get Fulfillments
	XMLElement* elem = root->FirstChildElement("Fulfillments");
	XMLElement* kid = elem->FirstChildElement("Need");
	while (kid)
	{
		std::string name = kid->Attribute("name");
		double amount = std::stod(kid->Attribute("amount"));


		needsFulfillmentMap[name] = amount;


		kid = kid->NextSiblingElement("Need");
	}


	// Get Actions
	elem = root->FirstChildElement("AvailableActions");
	kid = elem->FirstChildElement("Action");
	while (kid)
	{
		std::string name = kid->Attribute("name");
		std::string aName = kid->Attribute("aName");

		actionsMap[name] = aName;

		kid = kid->NextSiblingElement("Action");
	}

	return true;
}