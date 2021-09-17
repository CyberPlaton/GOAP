#include "SmartObject.h"


bool SmartObjectCmp::loadDefinition(tinyxml2::XMLElement* element)
{
	using namespace tinyxml2;

	if (!element) return false;


	// Get agent presence option
	XMLElement* presence = element->FirstChildElement("NeedAgentPresence");
	std::string value = presence->GetText();
	needAgentPresence = value.compare("false") == 0 ? false : true;


	// Get whether to destroy after usage
	XMLElement* usage = element->FirstChildElement("DestroyOnUse");
	value = usage->GetText();
	destroyOnUse = value.compare("false") == 0 ? false : true;


	// Get Fulfillments
	XMLElement* elem = element->FirstChildElement("Fulfillments");
	XMLElement* kid = elem->FirstChildElement("Need");
	while (kid)
	{
		std::string name = kid->Attribute("name");
		double amount = std::stod(kid->Attribute("amount"));


		needsFulfillmentMap[name] = amount;


		kid = kid->NextSiblingElement("Need");
	}


	// Get Actions
	elem = element->FirstChildElement("AvailableActions");
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