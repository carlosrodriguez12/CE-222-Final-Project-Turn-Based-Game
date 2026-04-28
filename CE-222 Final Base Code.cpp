#include<iostream>
#include<string>
#include<cstdlib> // for random number generator
#include<random>
using namespace std;



class Character { // base character class, to be inherited from
protected:
protected:
	int health;
	int attack;
	int defense;
	int strengthBonus = 0; // how much extra attack is active
	int strengthTurns = 0; // how many turns left;
	string type; // light, med, etc
public:
	Character(string t, int h, int a, int d) : type(t), health(h), attack(a), defense(d) {}
	virtual ~Character() {}

	virtual void attackTarget(Character& target) { // call this when attacking
		
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<int> dist(-5, 10);
		int random = dist(gen); // a more maluable way to choose a random number
		
		if (rand() % 5 == 0)//has a 20% chance of missing the attack
		{
			cout << "Attack has missed!" << endl; 
			return;
		}	
		int damage = (attack + strengthBonus) - target.defense + random;
		if (damage < 1)
			damage = 1;
		target.health -= damage; // new health is health - damage done
		cout << "Attack did " << damage << " damage!" << endl;
	}

	void heal() {
		health += 15;
		cout << "You healed 15 HP!" << endl;
	}

	void strength() {
		strengthBonus += 5;
		strengthTurns += 4;
		cout << "You gained +5 attack for 3 turns!" << endl;
	}

	void updateEffects() 
	{
		if (strengthTurns > 0) 
		{
			strengthTurns--;
			if (strengthTurns == 0) 
			{
				strengthBonus = 0;
				cout << "Strength boost wore off!" << endl;
			}
		}
	}

	bool isAlive() {
		return health > 0; // is alive if health is above 0 
	}

	int getHealth() {
		return health;
	}

	string getType() {
		return type;
	}
};





// character types, could be whatever
class Light : public Character { // Light class, low health, high attack, low defense
public:
	Light() : Character("Light", 80, 18, 5) {}
};

class Medium : public Character { // Medium class, med health, med attack, med defense
public:
	Medium() : Character("Medium", 100, 14, 8) {}
};

class Heavy : public Character { // Heavy class, high health, low attack, high defense
public:
	Heavy() : Character("Heavy", 125, 11, 12) {}
};






int main() {

	Character* player;
	Character* enemy;

	int choice;
	int strengthPotAmount = 3;
	int healthPotAmount = 5;

	cout << "Choose your character type: " << endl;
	cout << "1. Light" << endl;
	cout << "2. Medium" << endl;
	cout << "3. Heavy" << endl;
	cout << "Choice: ";
	cin >> choice;

	if (choice == 1)
		player = new Light();
	else if (choice == 2)
		player = new Medium();
	else
		player = new Heavy();

	enemy = new Heavy(); // temp enemy for testing

	cout << "\nYou chose: " << player->getType() << endl;
	cout << "\nEnemy chose: " << enemy->getType() << endl;

	while (player->isAlive() && enemy->isAlive()) { // game loop, while both players are alive...

		int action;

		cout << "\nChoose an action:" << endl;
		cout << "1. Attack" << endl;
		cout << "2. Use Potion" << endl;
		cout << "3. Quit" << endl;
		cout << "Choice: ";
		cin >> action;

		// Players turn
		if (action == 1) {
			cout << "\nPlayer Attacks!" << endl;
			player->attackTarget(*enemy);
			cout << "Enemy HP: " << enemy->getHealth() << endl;
		}
		else if (action == 2) {
			int potionSelect;

			cout << "\nSelect Potion Type to use: " << endl;
			cout << "1. Heal" << "(" << healthPotAmount << " potions remaining)" << endl;
			cout << "2. Strength" << "(" << strengthPotAmount << " potions remaining)" << endl;
			cout << "Choice: ";
			cin >> potionSelect;
			
			if (potionSelect == 1)
			{
				if (healthPotAmount > 0)
				{ 
					player->heal();
					healthPotAmount--;
				}
				
				else
					cout << "Out of health potions, you lose your turn." << endl;
			}

			else if (potionSelect == 2)
			{
				if (strengthPotAmount > 0)
				{
					player->strength();
					strengthPotAmount--;
				}

				else
					cout << "Out of strength potions, you lose your turn." << endl;
			}
			
			else
				cout << "Invalid choice, you lose your turn." << endl;
		}
		else if (action == 3) {
			cout << "Exiting game..." << endl;
			break;
		}
		else
			cout << "Invalid choice, you lose your turn." << endl;

		if (!enemy->isAlive()) // if is not alive
			break;

		cout << "\nEnemy Attacks!" << endl;
		enemy->attackTarget(*player);
		cout << "Player HP: " << player->getHealth() << endl;

		player->updateEffects();
		enemy->updateEffects();
	}

	if (player->isAlive())
		cout << "\nYou Win!" << endl;
	else
		cout << "\nYou Lose!" << endl;

	delete player;
	delete enemy;


	return 0;
}
