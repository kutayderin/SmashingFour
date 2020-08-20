// SmashingFour.cpp : Bu dosya 'main' iþlevi içeriyor. Program yürütme orada baþlayýp biter.

#include <iostream>
#include <Vector>
#include<algorithm>
#include <time.h>
#include<string>
#include <iomanip>

using namespace std;

class Unit;

//--------------------------------------------------------------- Takýmlar

vector<Unit*> team_1;
vector<Unit*> team_2;

//--------------------------------------------------------------------------


//#pragma region Unit Types

class Unit {
protected:
	int teamNo = 0;
	string name = "Unit";
	double HP = 0;
	double maxHP = 0;
	double Attack = 0;
	double Defence = 0;
	double Chance = 0;
	bool specialAbility = false;
	bool Alive = true;
	bool isplayed = false;

	int getRandom(int min, int max) {
		return (min)+(rand() % (max - min));
	}
	bool hasChance() {
		return (rand() % 100) < Chance;
	}

public:

	int randomNumber = 0;   // Vector Sort fonksiyonu için public eriþim yapýldý...

	Unit(int teamNo) :teamNo(teamNo) {
		randomNumber = getRandom(0, 4);
	}
	Unit(double hp, double attack, double defence, double chance) :maxHP(hp), HP(hp), Attack(attack), Defence(defence), Chance(chance) {
		randomNumber = getRandom(0, 4);
	}
	bool isAlive() {
		return Alive;
	}
	int getHP() {
		return HP;
	}

	void healUnitByPercentage(double percentHP) {// Mevcut canýna oranla heal
		HP += HP * percentHP / 100.0;
		if (HP > maxHP)HP = maxHP;
	}
	void healUnit(double hp) {
		HP += hp;
		if (HP > maxHP)HP = maxHP;
	}
	void setAlive(double hpPercentage) {// 0 ile 100 arasýnda oran verilir, 50 verilirse caný maxHP'nin yarýsý olur.
		HP = maxHP * hpPercentage / 100.0;
		Alive = true;
	}
	int giveDamage(double attackpoint) {
		double actualDMG = (attackpoint * (100 / (100 + Defence)));
		HP -= actualDMG;
		if (HP < 0) {
			HP = 0;
			Alive = false;
			onDeath();
			cout << "[Dead]     Unfortunately unit " << name << " gave its last breath" << endl;
		}
		return actualDMG;
	}
	string getName() {
		return name;
	}
	bool isPlayedBefore() {
		return isplayed;
	}
	void setPlayed(bool isPlayed) {
		this->isplayed = isPlayed;
	}
	double getAttackPoint() { return Attack; }
	double getDefencePoint() { return Defence; }

	virtual void printInfo() {
		cout << name << " (HP:" << HP << " Attack:" << Attack << " Defence:" << Defence << ")" << endl;
	}
	virtual bool attack(Unit* enemy) {
		int dmg = enemy->defend(this);
		cout << "[Attack]   " << name << " gave " << dmg << " damage point to " << enemy->getName() << "(HP:" << enemy->getHP() << ")" << endl;
		return false;
	}
	virtual int defend(Unit* enemy) {
		int dmg = giveDamage(enemy->getAttackPoint());
		return dmg;
	}
	virtual void onDeath() {}

};


class Warior :public Unit {   //Ayný rakibe 2 kere vurabilme þansý
protected:
	bool attackedTwice = false;
public:
	int thisisWar = 0;
	Warior(double hp, double attack, double defence, double chance) :Unit(hp, attack, defence, chance) {}
	Warior(int teamNo) :Unit(teamNo) {
		HP = getRandom(100, 200);
		Attack = getRandom(30, 50);
		Defence = getRandom(20, 30);
		Chance = getRandom(12, 16);// 12,16
		name = "Warior";
		maxHP = HP;
	}
	bool attack(Unit* enemy) {
		int dmg = enemy->defend(this);
		cout << "[Attack]   " << name << " gave " << dmg << " damage point to " << enemy->getName() << "(HP:" << enemy->getHP() << ")" << endl;

		if (hasChance() && !enemy->isAlive()) {

			dmg = enemy->defend(this);
			cout << "[Luck]     Warior used its special ability and hit " << enemy->getName() << "(HP:" << enemy->getHP() << ") again with " << dmg << " damage point!" << endl;

			return true;
		}
		return false;
	}
	void onDeath() {
		//cout << "Warior Class";
	}
};
class Defender :public Unit { //Defanstan sonra geri saldýrý þansý

public:
	Defender(double hp, double attack, double defence, double chance) :Unit(hp, attack, defence, chance) {}
	Defender(int teamNo) :Unit(teamNo) {
		HP = getRandom(300, 600);
		maxHP = HP;
		Attack = getRandom(15, 35);
		Defence = getRandom(50, 70);
		Chance = getRandom(10, 13);//10-13
		name = "Defender";
	}
	int defend(Unit* enemy) {
		int damage = giveDamage(enemy->getAttackPoint());
		if (hasChance()) {
			enemy->defend(this);
			cout << "[Luck]     Defender used its special ability and striked back to " << enemy->getName() << "(HP:" << enemy->getHP() << ") with " << getAttackPoint() << " damage point!" << endl;
		}
		return damage;
	}
};
class Wizard :public Unit {   //Çoklu düþman hasarý þansý, Her bir düþman için ayný chance

public:
	Wizard(double hp, double attack, double defence, double chance) :Unit(hp, attack, defence, chance) {}
	Wizard(int teamNo) :Unit(teamNo) {
		HP = getRandom(90, 200);
		maxHP = HP;
		Attack = getRandom(10, 30);
		Defence = getRandom(15, 25);
		Chance = getRandom(12, 16);
		name = "Wizard";
	}
	bool attack(Unit* enemy) {
		int dmg = enemy->defend(this);
		cout << "[Attack]   " << name << " gave " << dmg << " damage point to " << enemy->getName() << "(HP:" << enemy->getHP() << ")" << endl;
		if (teamNo == 0) {
			for (auto& u : team_2) {
				if (enemy != u && hasChance() && !u->isAlive()) {
					dmg = enemy->defend(this);
					cout << "[Luck]     Wizard used its special ability and brought magic effect on " << u->getName() << "(HP:" << u->getHP() << ") with " << dmg << " damage point!" << endl;
					//cout << "[Attack]    " << name << " gave -" << Attack << " damage point to " << enemy->getName() << "(HP:" << enemy->getHP() << ")" << endl;
				}
			}
		}
		else {
			for (auto& u : team_1) {
				if (enemy != u && hasChance() && !u->isAlive()) {
					dmg = enemy->defend(this);
					cout << "[Luck]     Wizard used its special ability and brought magic effect on " << u->getName() << "(HP:" << u->getHP() << ") with " << dmg << " damage point!" << endl;
					//cout << "[Attack]    " << name << " gave -" << Attack << " damage point to " << enemy->getName() << "(HP:" << enemy->getHP() << ")" << endl;
				}
			}
		}


		return false;
	}
};
class Healer :public Unit {   // Seçilen teammate'in mevcut hpsini %20 arttýrýr

public:
	Healer(double hp, double attack, double defence, double chance) :Unit(hp, attack, defence, chance) {}
	Healer(int teamNo) :Unit(teamNo) {
		HP = getRandom(100, 250);
		maxHP = HP;
		Attack = getRandom(10, 30);
		Defence = getRandom(15, 30);
		Chance = getRandom(15, 23);
		name = "Healer";
	}
	bool attack(Unit* enemy) {
		int dmg = enemy->defend(this);
		cout << "[Attack]   " << name << " gave " << dmg << " damage point to " << enemy->getName() << "(HP:" << enemy->getHP() << ")" << endl;
		if (hasChance()) {
			int random_teammate = rand() % 4;
			if (teamNo == 0) {
				while (!(team_1[random_teammate]->isAlive()) || team_1[random_teammate] == this)random_teammate = rand() % 4;
				team_1[random_teammate]->healUnitByPercentage(20);
				cout << "[Luck]     Healer used its special ability and healed %20 of " << team_1[random_teammate]->getName() << "(HP:" << team_1[random_teammate]->getHP() << ")'s hp! " << endl;
				return true;
			}if (teamNo == 1) {
				while (!(team_2[random_teammate]->isAlive()) || team_2[random_teammate] == this)random_teammate = rand() % 4;
				team_2[random_teammate]->healUnitByPercentage(20);
				cout << "[Luck]     Healer used its special ability and healed %20 of " << team_2[random_teammate]->getName() << "(HP:" << team_2[random_teammate]->getHP() << ")'s hp! " << endl;
				return true;
			}

		}
		return false;
	}
};

//#pragma endregion


//#pragma region Arena Types

//Arena
class Casual {
protected:
	double chance = 0;
	double value = 0;
	bool hasChance() {
		return (rand() % 100) < chance;
	}

public:
	virtual void specialAbility() {}
	virtual string getName() { return "Casual"; }
};

//Arena
class FireSpin :public Casual {
public:
	FireSpin() {
		chance = 10;//10
		value = 20;     //20hp hasar alýr

	}
	void specialAbility() {
		// Þansý baðlý olarak unitlerden can çal-> 20dmg %10chance
		for (auto& t1 : team_1)
			if (t1->isAlive() && hasChance()) {
				t1->giveDamage(value);

				cout << endl;
				cout << "[Arena]    " << getName() << ", burned the  " << t1->getName() << "(HP:" << t1->getHP() << ") from Team1!" << endl;
				cout << endl;
			}
		for (auto& t2 : team_2)
			if (t2->isAlive() && hasChance()) {
				t2->giveDamage(value);
				cout << endl;
				cout << "[Arena]    " << getName() << ", burned the  " << t2->getName() << "(HP:" << t2->getHP() << ") from Team2!" << endl;
				cout << endl;
			}
	}
	string getName() { return "Fire Spin"; }
};

//Arena
class Valhalla :public Casual {
public:
	Valhalla() {
		chance = 5; // 5
		value = 10;     // %10 hp ile ölen unitler geri gelir
	}
	void specialAbility() {
		for (auto& t1 : team_1)
			if (!t1->isAlive() && hasChance()) {
				t1->setAlive(value);
				cout << endl;
				cout << "[Arena]    " << getName() << ", bring back the " << t1->getName() << "(HP:" << t1->getHP() << ") from Team1 which was dead!" << endl;
				cout << endl;
				break;
			}

		for (auto& t2 : team_2)
			if (!t2->isAlive() && hasChance()) {
				t2->setAlive(value);
				cout << endl;
				cout << "[Arena]    " << getName() << ", bring back the " << t2->getName() << "(HP:" << t2->getHP() << ") from Team2 which was dead!" << endl;
				cout << endl;
				break;
			}
	}
	string getName() { return "Valhalla"; }
};

//Arena
class Aurora :public Casual {
public:
	Aurora() {
		chance = 10;    //10
		value = 10;     //10 hp verilir
	}
	void specialAbility() {
		for (auto& t1 : team_1)
			if (t1->isAlive() && hasChance()) {
				t1->healUnit(value);
				cout << endl;
				cout << "[Arena]    " << getName() << ", healed the  " << t1->getName() << "(HP:" << t1->getHP() << ") from Team1!" << endl;
				cout << endl;
			}
		for (auto& t2 : team_2)
			if (t2->isAlive() && hasChance()) {
				t2->healUnit(value);
				cout << endl;
				cout << "[Arena]    " << getName() << ", healed the  " << t2->getName() << "(HP:" << t2->getHP() << ") from Team2!" << endl;
				cout << endl;
			}
	}
	string getName() { return "Aurora"; }
};

//#pragma endregion

//Oyunun bitiþini kontrol eder
int checkGameEnd() {    //-> Eðer fonksiyon 0 verirse: oyun devam etmekte | 1 verirse: Takým 1 kazanýr | 2 verirse: Takým 2 kazanýr
	int countDead = 0;
	for (auto& unitInTeam : team_1)if (!unitInTeam->isAlive())countDead++;  // 1.Takýmdaki ölüleri sayar
	if (countDead == 4)return 2;                                            // 4 unitin 4'üde ölümü bakar
	countDead = 0;
	for (auto& unitInTeam : team_2)if (!unitInTeam->isAlive())countDead++;
	if (countDead == 4)return 1;
	return 0;
}

//Saldýrýlabilicek potansiyel düþmanlar listelenir; Örneðin; teamNo eðer 0 verilirse team2'deki saldýrýlabilicek unitler listelenir
void printAttackList(int teamNo) {
	string msg = "";
	for (int i = 0; i < 4; i++) {
		if (teamNo == 1)if (team_1[i]->isAlive())msg += " " + to_string((i + 1)) + "_" + team_1[i]->getName();
		if (teamNo == 0)if (team_2[i]->isAlive())msg += " " + to_string((i + 1)) + "_" + team_2[i]->getName();
	}
	cout << "[Input]    Type unit number that you want to attack: " + msg << endl;

}

//Unitler canlarý yazdýrýlýr veya ölüyse dead durumlarý yazýlýr
void printAllUnit() {
	cout << endl;
	cout << "     [Team 1]                   [Team 2]" << endl;
	for (int i = 0; i < 4; i++) {

		if (team_1[i]->isAlive() && team_1[i]->getName() == "Defender") {
			cout << team_1[i]->getName() << " (HP :" << setw(4) << to_string(team_1[i]->getHP()) << ")        ";
		}
		else if (team_1[i]->isAlive() && !(team_1[i]->getName() == "Defender")) {
			cout << team_1[i]->getName() << "   (HP :" << setw(4) << to_string(team_1[i]->getHP()) << ")        ";
		}
		else if (!(team_1[i]->isAlive()) && team_1[i]->getName() == "Defender") {
			cout << team_1[i]->getName() << " (  Dead  )        ";
		}
		else cout << team_1[i]->getName() << "   (  Dead  )        ";

		if (team_2[i]->isAlive() && team_2[i]->getName() == "Defender") {
			cout << team_2[i]->getName() << " (HP :" << setw(4) << to_string(team_2[i]->getHP()) << ")\n";
		}
		else if (team_2[i]->isAlive() && !(team_2[i]->getName() == "Defender")) {
			cout << team_2[i]->getName() << "   (HP :" << setw(4) << to_string(team_2[i]->getHP()) << ")\n";
		}
		else if (!(team_2[i]->isAlive()) && team_2[i]->getName() == "Defender") {
			cout << team_2[i]->getName() << " (  Dead  )\n";
		}
		else cout << team_2[i]->getName() << "   (  Dead  )\n";

	}
	cout << endl;
}

//Kullanýcýdan uygun rakam almaya çalýþýr; girdiði rakam uygun deðil
int getIntegerFromUser(int teamNo) {
	char c;
	int i;
	while (true) {
		cin >> c;
		if (c >= 48 && c <= 57) {// Consoldan girilen birimin rakam olup olmadýðýný kontrol eder; 48='0', 57='9' eþittir; ASCII rakam sistemine göre
			i = c - 48;     //Rakam girilmiþ ise i hesaplanýr
		}
		else {// Rakam girilmemiþ ise
			cout << "[Error]   '" << c << "' is not a number!" << endl;
			continue;
		}
		if (i >= 5 || i <= 0)cout << "[Error]   The number must be between 1 and 4, try again!" << endl;    //Girilen rakam 1 ile 4 arasýnda olmalýdýr çünkü5. bulunmamaktadýr
		else {
			if (teamNo == 0) {
				if (!(team_2[i - 1]->isAlive()))cout << "[Error]  The unit " << i << " is already dead, choose other units!" << endl;   // girilen numaraya karþýlýk olarak hayatta bir unit bulunuyor mu bakýlýr; Bulunmazsa döngü baþa döner; bulunursa sayý return edilir
				else return i - 1;
			}
			else {
				if (!(team_1[i - 1]->isAlive()))cout << "[Error]  The unit " << i << " is already dead, choose other units!" << endl;
				else return i - 1;
			}
		}
	}
}


// Tahmini kalan raundu belirleyen fonksiyon; deðiþiklik yapýlýrsa silinebilir
int findLeftRounds() {
	int total = 0;
	for (int i = 0; i < 8; i++) {
		//if (curntRound <= i) {
		if (i % 2 == 0) {
			if (team_1[(i / 2)]->isAlive())total++;
		}
		else {
			if (team_2[(i / 2)]->isAlive())total++;
		}
		//}
	}
	return total;
}


// Vectordeki Sort( Sýralama ) algoritmasýný kullanabilmek için gerekli; Sýralama þartý belirlenir
struct unitOrderer
{
	bool operator()(const Unit* lx, const Unit* rx) const {
		return lx->randomNumber < rx->randomNumber;
	}
};


int main() {
	//Random Seed; farklý sayýlar gelmesi için kullanýlýr
	srand(time(0));


	//  Rastgele Harita Belirlenir
	unique_ptr<Casual> arena = nullptr;
	int arena_random = rand() % 4;
	// arena_random'a göre bir harita oluþturulur
	if (arena_random == 0)arena = unique_ptr<Casual>(new Casual());
	else if (arena_random == 1)arena = unique_ptr<FireSpin>(new FireSpin());
	else if (arena_random == 2)arena = unique_ptr<Valhalla>(new Valhalla());
	else arena = unique_ptr<Aurora>(new Aurora());
	cout << "[Info] Arena that you will play is " << arena->getName() << endl;



	//  karakterler Belirlenir
	// Belirleen karakterler, pointer þeklinde bir vector'e atýlýr; Vectorler bir liste gibidir, iþlem kolaylýðý saðlar
	unique_ptr<Warior> w1(new Warior(0));
	unique_ptr<Defender> d1(new Defender(0));
	unique_ptr<Wizard> wz1(new Wizard(0));
	unique_ptr<Healer> h1(new Healer(0));
	team_1.push_back(h1.get());
	team_1.push_back(w1.get());
	team_1.push_back(wz1.get());
	team_1.push_back(d1.get());
	unique_ptr<Warior> w2(new Warior(1));
	unique_ptr<Defender> d2(new Defender(1));
	unique_ptr<Wizard> wz2(new Wizard(1));
	unique_ptr<Healer> h2(new Healer(1));
	team_2.push_back(h2.get());
	team_2.push_back(wz2.get());
	team_2.push_back(d2.get());
	team_2.push_back(w2.get());



	//  Rasgele oluþturulan karakterler randomNumber'ýn büyüklüðüne göre sýralanýr; sort algoritmasý ile
	sort(team_1.begin(), team_1.end(), unitOrderer());
	sort(team_2.begin(), team_2.end(), unitOrderer());

	int winner = 0;         // Kazanan takým deðiþkeni; eðer winner, 0'a eþit ise kanan henüz yoktur
	int round_counter = 0;  //Round Sayacý
   // int teamNo = 0;


	int team1_unit = 0;
	int team2_unit = 0;

	while (winner == 0) {   //Kazanan olmadýðý sürece
		round_counter++;
		cout << endl << endl << endl;

		cout << "=================== ROUND " << round_counter << " ===================" << endl;
		printAllUnit();     //Her Round baþýnda unit durumlarýný ekrana çýkartýr
		cout << endl;
		int selected_unit = 0;
		for (int teamNo = 0; teamNo < 2; teamNo++) {
			if (teamNo == 0) {  // TEAM1

				while (!team_1[team1_unit]->isAlive()) { // Ölen Unit varsa atlar
					team1_unit = (team1_unit + 1) % 4;
				}

				cout << endl;
				cout << "[Info]     1. team turn:" << endl;  // Burda "turn_counter << "/"<<roundLeft" hatalý olabilir bu yüzden silinebilir; Bunlar tahmini kalan turn sýrasýný söyler.
				cout << "[Info]     Your hero:  ";  //Bundan sonra heronun ismi ekrana çýkarýlýr
				team_1[team1_unit]->printInfo();   //Heronun bilgileri print edilir
				printAttackList(teamNo);                    //Saldýrabiliceði düþman unitler ekrana yazdýrýlýr
				cout << endl;
				int c = getIntegerFromUser(teamNo);         //Kullanýcýdan uygun bir sayý alýnýr; 1,4 arasýnda ve karþý takýmýn yaþayan bir unitin numarasý girmesi zorunlu tutulur bu fonksiyonda
				cout << endl;
				team_1[team1_unit]->attack(team_2[c]); // uygun rakam geldiðinde sýrasý gelen takým1 uniti, takým2 nin seçilen unitine saldýrýr, takým2'nin c'ninci sayýsýna(kullanýcýdan alýnan) saldýrýr



				team1_unit = (team1_unit + 1) % 4;
				winner = checkGameEnd();
				if (winner != 0)break;
			}
			else {              // TEAM2

				while (!team_2[team2_unit]->isAlive()) { // Ölen Unit varsa atlar
					team2_unit = (team2_unit + 1) % 4;
				}

				cout << endl;
				cout << "[Info]     2. team turn:" << endl;  // Burda "turn_counter << "/"<<roundLeft" hatalý olabilir bu yüzden silinebilir; Bunlar tahmini kalan turn sýrasýný söyler.
				cout << "[Info]     Your hero:  ";  //Bundan sonra heronun ismi ekrana çýkarýlýr
				team_2[team2_unit]->printInfo();   //Heronun bilgileri print edilir
				printAttackList(teamNo);                    //Saldýrabiliceði düþman unitler ekrana yazdýrýlýr
				int c = getIntegerFromUser(teamNo);         //Kullanýcýdan uygun bir sayý alýnýr; 1,4 arasýnda ve karþý takýmýn yaþayan bir unitin numarasý girmesi zorunlu tutulur bu fonksiyonda

				team_2[team2_unit]->attack(team_1[c]); // uygun rakam geldiðinde sýrasý gelen takým1 uniti, takým2 nin seçilen unitine saldýrýr, takým2'nin c'ninci sayýsýna(kullanýcýdan alýnan) saldýrýr



				team2_unit = (team2_unit + 1) % 4;
				winner = checkGameEnd();
				if (winner != 0)break;
			}

		}

		arena->specialAbility();
		cout << endl << "[Round]    End of " << round_counter << ". round!" << endl;
		winner = checkGameEnd();
	}

	winner = checkGameEnd();
	cout << endl;
	cout << endl;
	cout << endl;
	cout << endl;
	if (winner == 1)cout << "[WINNER]    The winner team is TEAM1  !!" << endl;
	else cout << "[WINNER]    The winner team is TEAM2  !!" << endl;
	cout << endl;
	cout << endl;
}
