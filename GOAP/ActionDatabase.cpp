#include "ActionDatabase.h"

ActionDatabase* ActionDatabase::g_ActionDatabase = nullptr;

bool ActionDatabase::loadDefinitions(const std::string& filepath)
{
	using namespace tinyxml2;

	tinyxml2::XMLDocument doc;
	XMLError result = doc.LoadFile(filepath.c_str());
	if (result != XMLError::XML_SUCCESS)
	{
		return false;
	}

	XMLNode* root = doc.FirstChild();
	if (root == nullptr) return false;

	XMLElement* element = root->FirstChildElement("Action");
	
	while (element)
	{
		std::string actionPath = element->GetText();

		ActionDefinition* def = new ActionDefinition(actionPath);

		actionPrefabs.emplace(std::make_pair( def->id, def ));

		element = element->NextSiblingElement("Action");
	}


	return true;
}