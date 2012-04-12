#include "CK2Character.h"
#include "..\log.h"



CK2Character::CK2Character()
{
	name			= "";
	religion		= "";
	culture		= "";
	dynasty		= NULL;
	birthDate	= (string)"1.1.1";
	dead			= true;
	deathDate	= (string)"1.1.1";
	fatherNum	= -1;
	father		= NULL;
	motherNum	= -1;
	mother		= NULL;
	children.clear();
	female		= false;

	memset(stats, 0, sizeof(stats));
}


void CK2Character::init(Object* obj, map<int, CK2Dynasty*>& dynasties)
{
	name			= obj->getLeaf("birth_name");
	religion		= obj->getLeaf("religion");
	culture		= obj->getLeaf("culture");
	dynasty		= dynasties[ atoi( obj->getLeaf("dynasty").c_str() ) ];
	birthDate	= obj->getLeaf("birth_date");

	vector<Object*> fatherObj = obj->getValue("father");
	if (fatherObj.size() > 0)
	{
		fatherNum = atoi( fatherObj[0]->getLeaf().c_str() );
	}
	else
	{
		fatherNum = -1;
	}
	vector<Object*> motherObj = obj->getValue("mother");
	if (motherObj.size() > 0)
	{
		motherNum = atoi( motherObj[0]->getLeaf().c_str() );
	}
	else
	{
		motherNum = -1;
	}

	vector<Object*> deathObj = obj->getValue("death_date");
	if (deathObj.size() > 0)
	{
		dead			= true;
		deathDate	= deathObj[0]->getLeaf();
	}
	else
	{
		dead			= false;
		deathDate	= (string)"1.1.1";
	}

	vector<Object*> femaleObj = obj->getValue("female");
	if (femaleObj.size() > 0)
	{
		female = ( femaleObj[0]->getLeaf() == "yes" );
	}
	else
	{
		female = false;
	}

	vector<Object*> attributesObj = obj->getValue("attributes");
	if (attributesObj.size() > 0)
	{
		vector<string> attributeTokens = attributesObj[0]->getTokens();
		stats[DIPLOMACY]		= atoi( attributeTokens[0].c_str() );
		stats[MARTIAL]			= atoi( attributeTokens[1].c_str() );
		stats[STEWARDSHIP]	= atoi( attributeTokens[2].c_str() );
		stats[INTRIGUE]		= atoi( attributeTokens[3].c_str() );
		stats[LEARNING]		= atoi( attributeTokens[4].c_str() );
	}
	else
	{
		stats[DIPLOMACY]		= 0;
		stats[MARTIAL]			= 0;
		stats[STEWARDSHIP]	= 0;
		stats[INTRIGUE]		= 0;
		stats[LEARNING]		= 0;
	}

}


string CK2Character::getName()
{
	return name;
}


CK2Dynasty* CK2Character::getDynasty()
{
	return dynasty;
}


date CK2Character::getBirthDate()
{
	return birthDate;
}


void CK2Character::setParents(map<int, CK2Character*>& characters)
{
	if (fatherNum != -1)
	{
		father = characters[fatherNum];
		father->addChild(this);
	}

	if (motherNum != -1)
	{
		mother = characters[motherNum];
		mother->addChild(this);
	}
}


void CK2Character::addChild(CK2Character* newChild)
{
	bool inserted = false;
	for (list<CK2Character*>::iterator i = children.begin(); i != children.end(); i++)
	{
		if ((*i)->getBirthDate() > newChild->getBirthDate())
		{
			children.insert(i, newChild);
			inserted = true;
		}
	}
	if (!inserted)
	{
		children.push_back(newChild);
	}
}


bool CK2Character::isDead()
{
	return dead;
}


date CK2Character::getDeathDate()
{
	return deathDate;
}


bool CK2Character::isFemale()
{
	return female;
}


int* CK2Character::getStats()
{
	return stats;
}


CK2Character* CK2Character::getFather()
{
	return father;
}


CK2Character* CK2Character::getPrimogenitureHeir(string genderLaw)
{
	for (list<CK2Character*>::iterator i = children.begin(); i != children.end(); i++)
	{
		if (	( !(*i)->isDead() ) &&
				( !(*i)->isFemale() || (genderLaw == "true_cognatic") )
			)
		{
			return *i;
		}
		else
		{
			if ( !(*i)->isFemale() || (genderLaw == "true_cognatic") || (genderLaw == "cognatic") )
			{
				CK2Character* possibleHeir = (*i)->getPrimogenitureHeir(genderLaw);
				if (possibleHeir != NULL)
				{
					return possibleHeir;
				}
			}
		}
	}

	return NULL;
}