#include <fstream>
#include "data.h"
using namespace std;
string command, object, itemHere, secondItemHere;
int x = 0, y = 0, dragonX = 3, dragonY = 6;
bool actionTaken = false, lastChance = false;
string generateRoom() {
	int item = room[x][y].item;
	int secondItem = room[x][y].secondItem;
	if (item != -1 && items[item].status != 1)
		itemHere = items[item].name;
	else itemHere = "";
	if (secondItem != -1 && items[secondItem].status != 1)
		secondItemHere = items[secondItem].name;
	else secondItemHere = "";
	string output = room[x][y].desc + "\nExits: " + room[x][y].exits;
	if(!itemHere.empty()) 
		output += "\nYou see: " + itemHere + items[item].fullName;
	if (!secondItemHere.empty())
	    output += ", " + secondItemHere;
	if(room[x][y].interactable != -1){
	    if (itemHere.empty())
	        output += "\nYou see: ";
	    else output += ", ";
	    output += interactable[room[x][y].interactable].name;
	    if (interactable[room[x][y].interactable].direction == 0)
	        output += interactable[room[x][y].interactable].name == "lever" ? ", turned off" : " north";
	    else if (interactable[room[x][y].interactable].direction == 1)
	        output += interactable[room[x][y].interactable].name == "lever" ? " on" : " east";
	    else if (interactable[room[x][y].interactable].direction == 2)
	        output += " south";
	    else output += " west";
	}
	return(output);
}
void go() {
	if (object == "north" || object == "east" || object == "south" || object == "west") {
		if (room[x][y].exits.find(object) != string::npos) {
			actionTaken = true;
			if (object == "north")
				y++;
			if (object == "south")
				y--;
			if (object == "east")
				x++;
			if (object == "west")
				x--;
			if (x == 6 && y == 5 && items[15].status != 1) {
			    cout << "There is a maze that direction, which you wander through and get lost, ending back up where you were\n";
			    x--;
			}
			cout << generateRoom() << '\n';
		} else cout << "You can't go that direction\n";
	} else cout << "Not a valid direction\n";
}
int findIndex(string item) {
	for(int i = 0; i < ITEMS; i++) {
		if (item == items[i].name)
			return i;
	}
	return -1;
}
void take() {
	if (object == itemHere || object == secondItemHere) {
		if (items[room[x][y].item].status == 0) {
			items[room[x][y].item].status = 1;
			room[x][y].item = -1;
			cout << "You took the " << object << '\n';
			actionTaken = true;
		}
		else cout << "You can't take the " << itemHere << '\n';
	} else cout << "There is no " << object << " here\n";
}
void examine() {
	if(object == "room")
		cout << generateRoom() << '\n';
	else {
		bool has_item = false;
		for(int i = 0; i < ITEMS; i++) {
			if (items[i].name == object && (items[i].status == 1 || itemHere == object || secondItemHere == object)) {
				cout << items[i].examine << '\n';
				has_item = true;
			}
		}
		if (!has_item) {
			cout << "You learn nothing new\n";
			return;
		}
	}
	actionTaken = true;
}
void use() {
    if (object == "statue" || object == "lever"){
        bool lever = object == "lever";
        if (room[x][y].interactable >= 0){
            if (itemHere.empty()){
                interactable[room[x][y].interactable].direction == interactable[room[x][y].interactable].maxDirection ? interactable[room[x][y].interactable].direction = 0 : interactable[room[x][y].interactable].direction++;
                cout << (lever ? "You flip the lever\n" : "You push the statue, turning it clockwise\n");
                actionTaken = true;
                checkMap(lever, lever ? "You hear a statue turning near you\n" :  "You hear a rumbling in the distance\n");
            }
            else cout << "The " << itemHere << " prevents the " << object << " from moving\n";
        } else cout << "There is no " << object << " here\n";
        return;
    }
	bool hasItem = false;
	string itemObject;
	int itemUsed = findIndex(object);
	if (itemUsed != -1 && items[itemUsed].status == 1) {
		hasItem = true;
		cout << "On what? ";
		cin >> itemObject;
		if (itemObject == "on")
			cin >> itemObject;
		int itemChange = findIndex(itemObject);
		if (itemChange != -1) {
		    if (items[itemChange].useTarget == object) {
		        itemUsed = findIndex(itemObject);
		        itemObject = object;
		        itemChange = findIndex(itemObject);
		    }
			if (items[itemUsed].useTarget == itemObject && (items[itemChange].status == 1 || itemObject == itemHere || itemObject == secondItemHere)) {
				room[x][y].exits += items[itemUsed].exitAdded;
				cout << items[itemUsed].useResult << '\n';
				items[itemChange] = newItems[itemChange];
				items[itemUsed].status = 0;
				if (room[x][y].item == itemChange && itemObject == object)
				    room[x][y].item = -1;
				cout << generateRoom() << '\n';
				actionTaken = true;
				checkMap(false, "The room darkens, but the map in the center starts glowing\n");
			}
			else if(items[itemChange].status != 1 && itemObject != itemHere && itemObject != secondItemHere)
				cout << "You don't have a " << itemObject << '\n';
			else cout << "You can't use the " << object << " on the " << itemObject << '\n';
		}
		else if (object == "dragonslayer"  && itemObject == "dragon" && dragonX == x && dragonY == y) {
		    cout << "You stab the dragon in the heart, and it unleashes one last roar and a burst of flame before it falls down, dead.\nYOU WIN!!!";
			exit(0);
		} else cout << "You don't have a " << itemObject << '\n';
	} else cout << "You don't have a " << object << '\n';
}
void inv() {
	bool has_anything = false;
	cout << "You have: ";
	for(int i = 0; i < ITEMS; i++) {
		if (items[i].status == 1) {
			cout << items[i].name << items[i].fullName << ", ";
			has_anything = true;
		}
	}
	if(!has_anything) {
		cout << "nothing";
	}
	cout << '\n';
	actionTaken = true;
}
void save() {
	ofstream saveData("Save File.txt");
	if (!saveData.is_open()){
	    cout << "File not opened\n";
	    return;
	}
	saveData << x << ' ' << y << ' ' << dragonX << ' ' << dragonY;
	for (int i = 0; i < ITEMS; i++) {
		int num = 0;
		if (items[i].examine == newItems[i].examine)
			num = 1;
		if (items[i].status == 1)
			num += 2;
		saveData << ' ' << num;
	}
	for (int i = 0; i < 7; i++)
	    for (int j = 0; j < 7; j++)
	        saveData << '\n' << room[i][j].item;
	if (items[0].name == "")
	    saveData << ' ' << 1;
	else saveData << ' ' << 0;
	for (int i = 0; i < 8; i++)
	    saveData << ' ' << interactable[i].direction;
	saveData.close();
}
void load() {
    ifstream saveData("Save File.txt");
    	if (!saveData.is_open()){
	    cout << "Save file not found\n";
	    return;
	}
    if(!(saveData >> x >> y >> dragonX >> dragonY)){
        cout << "Corrupted file!";
        return;
    }
    for (int i = 0; i < ITEMS; i++){
        int itemStatus;
        saveData >> itemStatus; 
	    if (itemStatus % 2)
	        items[i] = newItems[i];
	    else items[i] = oldItems[i];
	    if (itemStatus >= 2)
	        items[i].status = 1;
	    else items[i].status = oldItems[i].status;
	}
	for (int i = 0; i < 7; i++)
	    for (int j = 0; j < 7; j++)
	        saveData >> room[i][j].item;
	bool exitOpened;
	saveData >> exitOpened;
	if(exitOpened)
	    room[0][1].exits = "south, north";
	for (int i = 0; i < 8; i++)
	    saveData >> interactable[i].direction;
	saveData.close();
	cout << generateRoom() << '\n';
}
string help(){
    return 	"Write go (direction) to go a direction, take (item) to take an item, examine (thing in your inventory or the room) to look at something in more detail, and inv to view your inventory. Write use (item) to use something, and then write what you want to use it on. You can also save and load your game by writing save, and load, though make sure that the game ends naturally in order to save. There are three ways to do that: win, the dragon eats you, or you write quit to exit the game\n";
}
void useCommand() {
	if(command == "go" || command == "move" || command == "walk" || command == "run")
		go();
	else if (command == "take" || command == "get")
		take();
	else if (command == "inv")
		inv();
	else if (command == "examine" || command == "look")
		examine();
	else if (command == "use")
		use();
	else if (command == "save")
		save();
	else if (command == "load")
	    load();
	else if (command == "help")
	    cout << help();
	else if (command == "quit")
	     exit(0);
	else cout << "I don't understand\n";
}
void getCommand() {
	cin >> command;
	if (command != "inv" && command != "save" && command != "load" && command != "quit"&&command !="help")
		cin >> object;
	cin.ignore(1000, '\n');	
	useCommand();
}
void moveDragon() {
	actionTaken = false;
	int distance = abs(dragonX - x) + abs(dragonY - y);
	if (distance == 0) {
	    if (items[16].status == 1 && !lastChance) {
	        cout << "The dragon approaches you with a roar a opens its jaws wide!\n";
	        lastChance = true;
	        return;
	    }
		cout << "The dragon devours you. You died.";
		exit(0);
	}
	bool moveX = rand() % 2;
	if (rand() % (distance + 1) == 0) {
		if ((dragonX != x && moveX) || dragonY == y)
			x > dragonX ? dragonX++ : dragonX--;
		else  y > dragonY ? dragonY++ : dragonY--;
	}
	else if (moveX)
		rand() % 2 ? dragonX++ : dragonX--;
	else rand() % 2 ? dragonY++ : dragonY--;
	if (dragonX < 0)
		dragonX++;
	if (dragonX > 6)
		dragonX--;
	if (dragonY < 0)
		dragonY++;
	if (dragonY > 6)
		dragonY--;
	distance = abs(dragonX - x) + abs(dragonY - y);
	if (distance <= 2) {
		if (distance == 0) {
			cout << "The dragon flies into the room. RUN!!!\n";
			return;
		}
		cout << "You hear the dragon growl to the ";
		if (dragonY > y)
			cout << "north";
		if (dragonY < y)
			cout << "south";
		if (dragonX > x)
			cout << "east";
		if (dragonX < x)
			cout << "west";
		if (distance == 1)
			cout << ". It's close!";
		cout << '\n';
	}
}
int main()
{
    saveOriginal();
    srand(time(0));
	cout << "You enter the dragon's lair. The entrance behind you caves in! You are trapped\nYou hear a distant roar. The dragon is awake. He will come for you\n";
	cout << generateRoom() << '\n';
	while(true) {
		getCommand();
		if (actionTaken)
			moveDragon();
	}
}